#include "../src/gameplay/inventory_system.h"
#include <cassert>
#include <cstdint>
#include <limits>

int main() {
    acunity360::InventorySystem inventory(2);
    assert(inventory.add(10, 3));
    assert(inventory.add(10, 2));
    assert(inventory.count(10) == 5);
    assert(inventory.occupiedSlots() == 1);

    assert(!inventory.add(0, 1));
    assert(!inventory.add(11, 0));
    assert(inventory.add(11, 1));
    assert(!inventory.add(12, 1));
    assert(!inventory.canAddNewItem());

    assert(!inventory.remove(10, 6));
    assert(inventory.count(10) == 5);
    assert(inventory.remove(10, 2));
    assert(inventory.count(10) == 3);
    assert(inventory.remove(10, 3));
    assert(inventory.count(10) == 0);
    assert(inventory.canAddNewItem());
    assert(inventory.add(12, 4));
    assert(inventory.count(12) == 4);
    assert(inventory.occupiedSlots() == 2);
    assert(!inventory.remove(99, 1));

    acunity360::InventorySystem overflow_inventory(1u);
    const std::uint32_t max_quantity = (std::numeric_limits<std::uint32_t>::max)();
    assert(overflow_inventory.add(77u, max_quantity));
    assert(!overflow_inventory.add(77u, 1u));
    assert(overflow_inventory.count(77u) == max_quantity);

    acunity360::InventorySystem no_slots(0u);
    assert(!no_slots.canAddNewItem());
    assert(!no_slots.add(1u, 1u));
    return 0;
}
