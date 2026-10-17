#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace acunity360 {

enum class InputButton : std::uint8_t {
    Confirm = 0,
    Cancel = 1,
    Pause = 2,
    Attack = 3,
    Interact = 4,
    Jump = 5
};

// Platform-specific Xbox 360 implementations must be supplied separately.
class IClock {
public:
    virtual ~IClock() {}
    virtual std::uint64_t milliseconds() const = 0;
};

class IInput {
public:
    virtual ~IInput() {}
    virtual bool pressed(InputButton button) const = 0;
};

class IStorage {
public:
    virtual ~IStorage() {}
    virtual bool readAll(const std::string& path, std::vector<std::uint8_t>& out) = 0;
    virtual bool writeAll(const std::string& path, const std::vector<std::uint8_t>& data) = 0;
};

class ILogger {
public:
    virtual ~ILogger() {}
    virtual void info(const char* message) = 0;
    virtual void warning(const char* message) = 0;
    virtual void error(const char* message) = 0;
};

struct PlatformServices {
    IClock* clock;
    IInput* input;
    IStorage* storage;
    ILogger* logger;

    PlatformServices() : clock(0), input(0), storage(0), logger(0) {}

    bool complete() const {
        return clock != 0 && input != 0 && storage != 0 && logger != 0;
    }
};

} // namespace acunity360
