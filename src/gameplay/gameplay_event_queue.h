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
    explicit GameplayEventQueue(std::size_t capacity = 1024u)
        : capacity_(capacity) {}

    bool enqueue(const GameplayEvent& event) {
        if (events_.size() >= capacity_) return false;
        events_.push(event);
        return true;
    }

    bool tryDequeue(GameplayEvent& out_event) {
        if (events_.empty()) return false;
        out_event = events_.front();
        events_.pop();
        return true;
    }

    std::size_t size() const { return events_.size(); }
    std::size_t capacity() const { return capacity_; }
    bool full() const { return events_.size() >= capacity_; }
    bool empty() const { return events_.empty(); }

    void clear() {
        while (!events_.empty()) events_.pop();
    }

private:
    std::size_t capacity_;
    std::queue<GameplayEvent> events_;
};

} // namespace acunity360
