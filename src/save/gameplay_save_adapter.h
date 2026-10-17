#pragma once

#include "../gameplay/gameplay_systems.h"
#include "save_data_codec.h"

namespace acunity360 {

class GameplaySaveAdapter {
public:
    static bool capture(const GameplaySystems& game, SaveSnapshot& out) {
        SaveSnapshot snapshot;
        const ObjectiveTracker& tracker = game.objectiveTracker();
        if (tracker.configured()) {
            const ObjectiveProgress& p = tracker.current();
            snapshot.objective_id = p.objective_id;
            snapshot.objective_progress = p.current;
            snapshot.objective_required = p.required;
        }
        snapshot.equipment = game.equipment().current();

        const std::vector<InventoryStack>& stacks = game.inventory().stacks();
        if (stacks.size() > SaveDataCodec::kMaxItems) return false;
        for (std::size_t i = 0; i < stacks.size(); ++i) {
            SaveItem item = {stacks[i].item_id, stacks[i].quantity};
            snapshot.items.push_back(item);
        }
        std::vector<std::uint8_t> validation;
        if (!SaveDataCodec::encode(snapshot, validation)) return false;
        out = snapshot;
        return true;
    }

    static bool restoreIntoFreshGame(const SaveSnapshot& snapshot, GameplaySystems& game) {
        const EquipmentLoadout& current_equipment = game.equipment().current();
        if (game.inventory().occupiedSlots() != 0u || game.objectiveTracker().configured() ||
            game.pendingEvents() != 0u || current_equipment.main_hand != 0u ||
            current_equipment.tool != 0u || current_equipment.outfit != 0u) return false;
        if (snapshot.items.size() > game.inventory().capacity() ||
            snapshot.items.size() > SaveDataCodec::kMaxItems) return false;
        if ((snapshot.objective_id == 0u) != (snapshot.objective_required == 0u) ||
            snapshot.objective_progress > snapshot.objective_required) return false;

        for (std::size_t i = 0; i < snapshot.items.size(); ++i) {
            if (snapshot.items[i].item_id == 0u || snapshot.items[i].quantity == 0u) return false;
            for (std::size_t j = i + 1u; j < snapshot.items.size(); ++j) {
                if (snapshot.items[i].item_id == snapshot.items[j].item_id) return false;
            }
        }

        for (std::size_t i = 0; i < snapshot.items.size(); ++i) {
            if (!game.inventory().add(snapshot.items[i].item_id, snapshot.items[i].quantity)) return false;
        }
        if (snapshot.objective_id != 0u) {
            if (!game.objectiveTracker().configure(snapshot.objective_id, snapshot.objective_required)) return false;
            if (snapshot.objective_progress > 0u &&
                !game.objectiveTracker().advance(snapshot.objective_id, snapshot.objective_progress)) return false;
        }
        game.equipment().restore(snapshot.equipment);
        return true;
    }
};

} // namespace acunity360
