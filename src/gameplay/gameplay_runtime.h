#pragma once

#include "gameplay_systems.h"
#include "../platform/platform_services.h"

#include <cstddef>
#include <cstdint>

namespace acunity360 {

enum class RuntimeState : std::uint8_t {
    Uninitialized = 0,
    Running = 1,
    Paused = 2,
    Stopped = 3
};

// Portable orchestration only; this is not an Xbox 360 platform backend.
class GameplayRuntime {
public:
    explicit GameplayRuntime(std::uint32_t inventory_capacity,
                             std::size_t max_events_per_tick = 64u)
        : systems_(inventory_capacity), services_(), state_(RuntimeState::Uninitialized),
          last_tick_ms_(0u), elapsed_ms_(0u), frame_count_(0u),
          max_events_per_tick_(max_events_per_tick), pause_was_pressed_(false) {}

    bool initialize(const PlatformServices& services) {
        if (state_ != RuntimeState::Uninitialized || !services.complete()) {
            return false;
        }
        services_ = services;
        last_tick_ms_ = services_.clock->milliseconds();
        state_ = RuntimeState::Running;
        services_.logger->info("Gameplay runtime initialized");
        return true;
    }

    bool tick() {
        if (state_ != RuntimeState::Running && state_ != RuntimeState::Paused) {
            return false;
        }

        const std::uint64_t now = services_.clock->milliseconds();
        if (now >= last_tick_ms_) {
            elapsed_ms_ += now - last_tick_ms_;
        }
        last_tick_ms_ = now;

        const bool pause_pressed = services_.input->pressed(InputButton::Pause);
        if (pause_pressed && !pause_was_pressed_) {
            state_ = state_ == RuntimeState::Running
                ? RuntimeState::Paused : RuntimeState::Running;
            services_.logger->info(state_ == RuntimeState::Paused
                ? "Gameplay paused" : "Gameplay resumed");
        }
        pause_was_pressed_ = pause_pressed;

        if (state_ == RuntimeState::Running) {
            systems_.processEvents(max_events_per_tick_);
            ++frame_count_;
        }
        return true;
    }

    void stop() {
        if (state_ == RuntimeState::Stopped) {
            return;
        }
        state_ = RuntimeState::Stopped;
        if (services_.logger != 0) {
            services_.logger->info("Gameplay runtime stopped");
        }
    }

    RuntimeState state() const { return state_; }
    std::uint64_t elapsedMilliseconds() const { return elapsed_ms_; }
    std::uint64_t frameCount() const { return frame_count_; }
    GameplaySystems& systems() { return systems_; }
    const GameplaySystems& systems() const { return systems_; }

private:
    GameplaySystems systems_;
    PlatformServices services_;
    RuntimeState state_;
    std::uint64_t last_tick_ms_;
    std::uint64_t elapsed_ms_;
    std::uint64_t frame_count_;
    std::size_t max_events_per_tick_;
    bool pause_was_pressed_;
};

} // namespace acunity360
