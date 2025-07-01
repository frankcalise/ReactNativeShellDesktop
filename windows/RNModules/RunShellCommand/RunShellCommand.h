#pragma once

#include <pch.h>

#include "../codegen/NativeRunShellCommandSpec.g.h"

#include <winrt/Microsoft.ReactNative.h>
#include <windows.h>
#include <string>


REACT_TURBO_MODULE(RunShellCommand)
struct RunShellCommand {
	using ModuleSpec = RNModulesCodegen::RunShellCommandSpec;

	// Spec methods
	REACT_METHOD(runAsync)
	void runAsync(std::string command, ::React::ReactPromise<std::string>&& result) noexcept;

	REACT_SYNC_METHOD(runSync)
	std::string runSync(std::string command) noexcept;

};