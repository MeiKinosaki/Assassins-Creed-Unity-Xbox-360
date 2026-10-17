#pragma once

#include <cstdint>

namespace acunity360 {

enum class EquipmentSlot : std::uint8_t {
    MainHand = 0,
    Tool = 1,
    Outfit = 2
};

struct EquipmentLoadout {
    std::uint32_t main_hand;
    std::uint32_t tool;
    std::uint32_t outfit;

    EquipmentLoadout() : main_hand(0u), tool(0u), outfit(0u) {}
};

class EquipmentSystem {
public:
    EquipmentSystem() : loadout_() {}

    bool equip(EquipmentSlot slot, std::uint32_t item_id) {
        if (item_id == 0u) return false;
        switch (slot) {
        case EquipmentSlot::MainHand: loadout_.main_hand = item_id; return true;
        case EquipmentSlot::Tool: loadout_.tool = item_id; return true;
        case EquipmentSlot::Outfit: loadout_.outfit = item_id; return true;
        }
        return false;
    }

    bool unequip(EquipmentSlot slot) {
        switch (slot) {
        case EquipmentSlot::MainHand: loadout_.main_hand = 0u; return true;
        case EquipmentSlot::Tool: loadout_.tool = 0u; return true;
        case EquipmentSlot::Outfit: loadout_.outfit = 0u; return true;
        }
        return false;
    }

    const EquipmentLoadout& current() const { return loadout_; }

    bool restore(const EquipmentLoadout& loadout) {
        loadout_ = loadout;
        return true;
    }

private:
    EquipmentLoadout loadout_;
};

} // namespace acunity360
