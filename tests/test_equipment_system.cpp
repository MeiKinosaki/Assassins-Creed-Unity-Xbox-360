#include "../src/gameplay/equipment_system.h"
#include <cassert>

int main() {
    acunity360::EquipmentSystem equipment;
    assert(equipment.current().main_hand == 0u);
    assert(!equipment.equip(acunity360::EquipmentSlot::MainHand, 0u));
    assert(equipment.equip(acunity360::EquipmentSlot::MainHand, 101u));
    assert(equipment.equip(acunity360::EquipmentSlot::Tool, 202u));
    assert(equipment.equip(acunity360::EquipmentSlot::Outfit, 303u));
    assert(equipment.current().main_hand == 101u);
    assert(equipment.current().tool == 202u);
    assert(equipment.current().outfit == 303u);
    assert(equipment.equip(acunity360::EquipmentSlot::MainHand, 404u));
    assert(equipment.current().main_hand == 404u);
    assert(equipment.unequip(acunity360::EquipmentSlot::Tool));
    assert(equipment.current().tool == 0u);

    acunity360::EquipmentLoadout saved;
    saved.main_hand = 11u;
    saved.tool = 22u;
    saved.outfit = 33u;
    assert(equipment.restore(saved));
    assert(equipment.current().main_hand == 11u);
    assert(equipment.current().tool == 22u);
    assert(equipment.current().outfit == 33u);
    return 0;
}
