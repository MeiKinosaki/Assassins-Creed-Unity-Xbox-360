#pragma once

#include "gameplay_event_queue.h"
#include "inventory_system.h"
#include "objective_tracker.h"

namespace acunity360 {

// Thin orchestration layer; engine/input/animation hooks are intentionally external.
class GameplaySystems {
public:
    explicit GameplaySystems(std::uint32_t inventory_capacity)
        : inventory_(inventory_capacity) {}

    InventorySystem& inventory() { return inventory_; }
    const InventorySystem& inventory() const { return inventory_; }

    ObjectiveTracker& objectiveTracker() { return objectives_; }
    const ObjectiveTracker& objectiveTracker() const { return objectives_; }

    void postEvent(const GameplayEvent& event) { events_.enqueue(event); }

    // Processes all queued events in FIFO order. ItemCollected expects a positive
    // quantity in value; ObjectiveProgress expects a positive progress amount.
    std::size_t processEvents() {
        std::size_t processed = 0;
        GameplayEvent event = { GameplayEventType::ObjectiveProgress, 0u, 0 };
        while (events_.tryDequeue(event)) {
            if (event.value > 0) {
                const std::uint32_t amount = static_cast<std::uint32_t>(event.value);
                switch (event.type) {
                case GameplayEventType::ItemCollected:
                    inventory_.add(event.subject_id, amount);
                    break;
                case GameplayEventType::ObjectiveProgress:
                    objectives_.advance(event.subject_id, amount);
                    break;
                case GameplayEventType::InteractionCompleted:
                    // Interaction is surfaced as an event; mission-specific
                    // interpretation belongs to a higher-level mission adapter.
                    break;
                }
            }
            ++processed;
        }
        return processed;
    }

    std::size_t pendingEvents() const { return events_.size(); }

private:
    InventorySystem inventory_;
    ObjectiveTracker objectives_;
    GameplayEventQueue events_;
};

} // namespace acunity360
