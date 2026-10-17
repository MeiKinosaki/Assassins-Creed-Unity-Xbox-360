#include "../src/save/gameplay_save_adapter.h"
#include <cassert>

int main() {
    acunity360::GameplaySystems original(4u);
    assert(original.inventory().add(20u, 3u));
    assert(original.inventory().add(21u, 1u));
    assert(original.objectiveTracker().configure(77u, 8u));
    assert(original.objectiveTracker().advance(77u, 5u));
    assert(original.equipment().equip(acunity360::EquipmentSlot::MainHand, 501u));
    assert(original.equipment().equip(acunity360::EquipmentSlot::Tool, 502u));
    assert(original.equipment().equip(acunity360::EquipmentSlot::Outfit, 503u));

    acunity360::SaveSnapshot snapshot;
    assert(acunity360::GameplaySaveAdapter::capture(original, snapshot));
    assert(snapshot.items.size() == 2u);
    assert(snapshot.objective_progress == 5u);

    std::vector<std::uint8_t> bytes;
    assert(acunity360::SaveDataCodec::encode(snapshot, bytes));
    acunity360::SaveSnapshot decoded;
    assert(acunity360::SaveDataCodec::decode(bytes, decoded));

    acunity360::GameplaySystems restored(4u);
    assert(acunity360::GameplaySaveAdapter::restoreIntoFreshGame(decoded, restored));
    assert(restored.inventory().count(20u) == 3u);
    assert(restored.inventory().count(21u) == 1u);
    assert(restored.objectiveTracker().configured());
    assert(restored.objectiveTracker().current().current == 5u);
    assert(restored.objectiveTracker().current().required == 8u);
    assert(restored.equipment().current().main_hand == 501u);
    assert(restored.equipment().current().tool == 502u);
    assert(restored.equipment().current().outfit == 503u);

    assert(!acunity360::GameplaySaveAdapter::restoreIntoFreshGame(decoded, restored));

    acunity360::SaveSnapshot invalid;
    invalid.items.push_back(acunity360::SaveItem{5u, 1u});
    invalid.items.push_back(acunity360::SaveItem{5u, 2u});
    acunity360::GameplaySystems target(4u);
    assert(!acunity360::GameplaySaveAdapter::restoreIntoFreshGame(invalid, target));
    assert(target.inventory().occupiedSlots() == 0u);
    return 0;
}
