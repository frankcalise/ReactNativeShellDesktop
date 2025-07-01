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
        std::string fullCommand = "cmd /C \"" + command + "\"";
        FILE* pipe = _popen(fullCommand.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("Failed to open pipe");
        }

        char buffer[128];
        std::ostringstream result;

        while (fgets(buffer, sizeof(buffer), pipe)) {
            result << buffer;
        }

        _pclose(pipe);
        return result.str();
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
