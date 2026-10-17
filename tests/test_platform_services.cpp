#include "../src/platform/platform_services.h"
#include <cassert>
#include <string>
#include <vector>

class TestClock : public acunity360::IClock {
public:
    std::uint64_t milliseconds() const { return 1234u; }
};
class TestInput : public acunity360::IInput {
public:
    bool pressed(acunity360::InputButton button) const {
        return button == acunity360::InputButton::Confirm;
    }
};
class TestStorage : public acunity360::IStorage {
public:
    bool readAll(const std::string& path, std::vector<std::uint8_t>& out) {
        if (path != "save.dat" || !exists) return false;
        out = data;
        return true;
    }
    bool writeAll(const std::string& path, const std::vector<std::uint8_t>& bytes) {
        if (path != "save.dat") return false;
        data = bytes;
        exists = true;
        return true;
    }
    bool exists = false;
    std::vector<std::uint8_t> data;
};
class TestLogger : public acunity360::ILogger {
public:
    void info(const char*) {}
    void warning(const char*) {}
    void error(const char*) {}
};

int main() {
    acunity360::PlatformServices services;
    assert(!services.complete());

    TestClock clock;
    TestInput input;
    TestStorage storage;
    TestLogger logger;
    services.clock = &clock;
    services.input = &input;
    services.storage = &storage;
    services.logger = &logger;
    assert(services.complete());
    assert(services.clock->milliseconds() == 1234u);
    assert(services.input->pressed(acunity360::InputButton::Confirm));
    assert(!services.input->pressed(acunity360::InputButton::Jump));

    std::vector<std::uint8_t> payload;
    payload.push_back(1u);
    payload.push_back(2u);
    assert(services.storage->writeAll("save.dat", payload));
    std::vector<std::uint8_t> restored;
    assert(services.storage->readAll("save.dat", restored));
    assert(restored.size() == 2u && restored[0] == 1u && restored[1] == 2u);
    assert(!services.storage->writeAll("wrong.dat", payload));
    return 0;
}
