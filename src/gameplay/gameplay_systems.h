#pragma once

#include "gameplay_event_queue.h"
#include "inventory_system.h"
#include "objective_tracker.h"
#include "equipment_system.h"

namespace acunity360 {

class GameplaySystems {
public:
    explicit GameplaySystems(std::uint32_t inventory_capacity)
        : inventory_(inventory_capacity), objectives_(), equipment_(), events_() {}

    InventorySystem& inventory() { return inventory_; }
    const InventorySystem& inventory() const { return inventory_; }
    ObjectiveTracker& objectiveTracker() { return objectives_; }
    const ObjectiveTracker& objectiveTracker() const { return objectives_; }
    EquipmentSystem& equipment() { return equipment_; }
    const EquipmentSystem& equipment() const { return equipment_; }

    void postEvent(const GameplayEvent& event) { events_.enqueue(event); }

    std::size_t processEvents() {
        return processEvents(static_cast<std::size_t>(-1));
    }

    // Bounded processing prevents a single frame from draining an unbounded queue.
    std::size_t processEvents(std::size_t max_events) {
        std::size_t processed = 0;
        GameplayEvent event = { GameplayEventType::ObjectiveProgress, 0u, 0 };
        while (processed < max_events && events_.tryDequeue(event)) {
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
    EquipmentSystem equipment_;
    GameplayEventQueue events_;
};

} // namespace acunity360
