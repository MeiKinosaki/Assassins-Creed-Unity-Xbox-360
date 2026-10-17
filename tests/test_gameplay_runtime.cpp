#include "../src/gameplay/gameplay_runtime.h"
#include <cassert>
#include <string>
#include <vector>

namespace {
class FakeClock : public acunity360::IClock {
public:
    std::uint64_t now = 0u;
    std::uint64_t milliseconds() const override { return now; }
};
class FakeInput : public acunity360::IInput {
public:
    bool pause = false;
    bool pressed(acunity360::InputButton button) const override {
        return button == acunity360::InputButton::Pause && pause;
    }
};
class FakeStorage : public acunity360::IStorage {
public:
    bool readAll(const std::string&, std::vector<std::uint8_t>&) override { return false; }
    bool writeAll(const std::string&, const std::vector<std::uint8_t>&) override { return false; }
};
class FakeLogger : public acunity360::ILogger {
public:
    unsigned messages = 0u;
    void info(const char*) override { ++messages; }
    void warning(const char*) override { ++messages; }
    void error(const char*) override { ++messages; }
};
}

int main() {
    FakeClock clock;
    FakeInput input;
    FakeStorage storage;
    FakeLogger logger;
    acunity360::PlatformServices services;
    services.clock = &clock;
    services.input = &input;
    services.storage = &storage;
    services.logger = &logger;

    acunity360::GameplayRuntime runtime(8u);
    assert(runtime.state() == acunity360::RuntimeState::Uninitialized);
    assert(!runtime.tick());
    assert(!runtime.initialize(acunity360::PlatformServices()));
    assert(runtime.initialize(services));
    assert(runtime.state() == acunity360::RuntimeState::Running);

    assert(runtime.systems().objectiveTracker().configure(7u, 1u));
    runtime.systems().postEvent(acunity360::GameplayEvent{
        acunity360::GameplayEventType::ObjectiveProgress, 7u, 1
    });
    clock.now = 16u;
    assert(runtime.tick());
    assert(runtime.frameCount() == 1u);
    assert(runtime.elapsedMilliseconds() == 16u);
    assert(runtime.systems().objectiveTracker().current().completed);

    input.pause = true;
    clock.now = 32u;
    assert(runtime.tick());
    assert(runtime.state() == acunity360::RuntimeState::Paused);
    assert(runtime.frameCount() == 1u);

    clock.now = 48u;
    assert(runtime.tick());
    assert(runtime.state() == acunity360::RuntimeState::Paused);

    input.pause = false;
    assert(runtime.tick());
    input.pause = true;
    clock.now = 64u;
    assert(runtime.tick());
    assert(runtime.state() == acunity360::RuntimeState::Running);
    assert(runtime.frameCount() == 2u);
    assert(runtime.elapsedMilliseconds() == 64u);

    runtime.stop();
    assert(runtime.state() == acunity360::RuntimeState::Stopped);
    assert(!runtime.tick());
    assert(logger.messages >= 3u);
    return 0;
}
