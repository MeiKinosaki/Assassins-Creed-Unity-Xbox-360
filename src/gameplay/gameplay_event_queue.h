#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

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
        : capacity_(capacity), storage_(capacity), head_(0u), size_(0u) {}

    bool enqueue(const GameplayEvent& event) {
        if (size_ >= capacity_) return false;
        const std::size_t tail = (head_ + size_) % capacity_;
        storage_[tail] = event;
        ++size_;
        return true;
    }

    bool tryDequeue(GameplayEvent& out_event) {
        if (size_ == 0u) return false;
        out_event = storage_[head_];
        head_ = (head_ + 1u) % capacity_;
        --size_;
        return true;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool full() const { return size_ >= capacity_; }
    bool empty() const { return size_ == 0u; }

    void clear() {
        head_ = 0u;
        size_ = 0u;
    }

private:
    std::size_t capacity_;
    std::vector<GameplayEvent> storage_;
    std::size_t head_;
    std::size_t size_;
};

} // namespace acunity360
