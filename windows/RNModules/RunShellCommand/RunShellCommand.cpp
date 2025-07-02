#include "pch.h"

#include "RunShellCommand.h"

#include <sstream>
#include <future>
#include <thread>
#include <vector>
#include <stdexcept>

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace {

    // Helper function to run a command and return its output
    std::string RunCommandSync(const std::string& command) {
        std::string fullCommand = "cmd /C " + command;

        SECURITY_ATTRIBUTES saAttr{};
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = nullptr;

        HANDLE hReadPipe, hWritePipe;
        if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
            throw std::runtime_error("Failed to create pipe");
        }

        if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hReadPipe);
            CloseHandle(hWritePipe);
            throw std::runtime_error("Failed to set pipe handle information");
        }

        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(STARTUPINFOA);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = hWritePipe;
        si.hStdError = hWritePipe;
        si.hStdInput = nullptr;

        DWORD creationFlags = CREATE_NO_WINDOW;

        std::vector<char> cmdLine(fullCommand.begin(), fullCommand.end());
        cmdLine.push_back('\0');

        BOOL success = CreateProcessA(
            nullptr,
            cmdLine.data(),
            nullptr,
            nullptr,
            TRUE,
            creationFlags,
            nullptr,
            nullptr,
            &si,
            &pi
        );

        CloseHandle(hWritePipe); // Parent doesn't need to write

        if (!success) {
            CloseHandle(hReadPipe);
            throw std::runtime_error("Failed to launch process");
        }

        std::ostringstream output;
        char buffer[128];
        DWORD bytesRead;
        BOOL readSuccess;

        while ((readSuccess = ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) && bytesRead != 0) {
            buffer[bytesRead] = '\0';
            output << buffer;
        }

        CloseHandle(hReadPipe);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return output.str();
    }

} // namespace

void RunShellCommand::runAsync(std::string command, ReactPromise<std::string>&& result) noexcept {
    std::thread([command = std::move(command), result = std::move(result)]() mutable {
        try {
            std::string output = RunCommandSync(command);
            result.Resolve(output);
        }
        catch (const std::exception& ex) {
            std::string errorMessage = std::string("run_async_error: ") + ex.what();
            result.Reject(errorMessage.c_str());
        }
        }).detach();
}

std::string RunShellCommand::runSync(std::string command) noexcept {
    try {
        return RunCommandSync(command);
    }
    catch (...) {
        return ""; // Return empty on failure
    }
}
