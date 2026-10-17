#pragma once

#include <cstddef>
#include <cstdint>
#include <queue>

namespace acunity360 {

enum class GameplayEventType : std::uint8_t {
    ObjectiveProgress = 0,
    ItemCollected = 1,
    InteractionCompleted = 2
};

struct GameplayEvent {
    GameplayEventType type;
    std::uint32_t subject_id;
    std::int32_t value;
};

// Single-threaded FIFO queue intended to be drained by the game loop.
// It deliberately has no dependency on Xbox-specific APIs.
class GameplayEventQueue {
public:
    void enqueue(const GameplayEvent& event) {
        events_.push(event);
    }

    bool tryDequeue(GameplayEvent& out_event) {
        if (events_.empty()) return false;
        out_event = events_.front();
        events_.pop();
        return true;
    }

    std::size_t size() const { return events_.size(); }
    bool empty() const { return events_.empty(); }

    void clear() {
        while (!events_.empty()) events_.pop();
    }

private:
    std::queue<GameplayEvent> events_;
};

} // namespace acunity360
