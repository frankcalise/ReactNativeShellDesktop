
/*
 * This file is auto-generated from a NativeModule spec file in js.
 *
 * This is a C++ Spec class that should be used with MakeTurboModuleProvider to register native modules
 * in a way that also verifies at compile time that the native module matches the interface required
 * by the TurboModule JS spec.
 */
#pragma once
// clang-format off


#include <NativeModules.h>
#include <tuple>

namespace RNModulesCodegen {

struct RunShellCommandSpec : winrt::Microsoft::ReactNative::TurboModuleSpec {
  static constexpr auto methods = std::tuple{
      Method<void(std::string, Promise<std::string>) noexcept>{0, L"runAsync"},
      SyncMethod<std::string(std::string) noexcept>{1, L"runSync"},
  };

  template <class TModule>
  static constexpr void ValidateModule() noexcept {
    constexpr auto methodCheckResults = CheckMethods<TModule, RunShellCommandSpec>();

    REACT_SHOW_METHOD_SPEC_ERRORS(
          0,
          "runAsync",
          "    REACT_METHOD(runAsync) void runAsync(std::string command, ::React::ReactPromise<std::string> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(runAsync) static void runAsync(std::string command, ::React::ReactPromise<std::string> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          1,
          "runSync",
          "    REACT_SYNC_METHOD(runSync) std::string runSync(std::string command) noexcept { /* implementation */ }\n"
          "    REACT_SYNC_METHOD(runSync) static std::string runSync(std::string command) noexcept { /* implementation */ }\n");
  }
};

} // namespace RNModulesCodegen
