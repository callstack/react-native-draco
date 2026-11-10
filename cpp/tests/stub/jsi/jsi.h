#pragma once
// Minimal host-only JSI stub so cpp/draco-helpers.h and cpp/draco-state.h
// compile without React Native. Signatures follow the real jsi/jsi.h where the
// real API has the same member; setNativeState exists in real JSI too.
#include <exception>
#include <memory>
#include <string>
#include <utility>

namespace facebook::jsi {

class NativeState {
 public:
  virtual ~NativeState() = default;
};

class Runtime {};

class Object {
 public:
  Object() = default;
  explicit Object(Runtime &) {}

  bool hasNativeState(Runtime &) const { return state_ != nullptr; }

  std::shared_ptr<NativeState> getNativeState(Runtime &) const {
    return state_;
  }

  void setNativeState(Runtime &, std::shared_ptr<NativeState> state) const {
    state_ = std::move(state);
  }

 private:
  mutable std::shared_ptr<NativeState> state_;
};

class JSIException : public std::exception {
 public:
  JSIException() = default;
  explicit JSIException(std::string what) : what_(std::move(what)) {}
  const char *what() const noexcept override { return what_.c_str(); }

 protected:
  std::string what_;
};

class JSError : public JSIException {
 public:
  JSError(Runtime &, std::string message)
      : JSIException(message), message_(std::move(message)) {}
  JSError(Runtime &rt, const char *message) : JSError(rt, std::string(message)) {}

  const std::string &getMessage() const { return message_; }

 private:
  std::string message_;
};

}  // namespace facebook::jsi

namespace facebook::react {
namespace jsi = facebook::jsi;
}
