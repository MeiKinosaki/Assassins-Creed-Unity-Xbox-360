#include "../src/gameplay/inventory_system.h"
#include <cassert>

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
    assert(inventory.occupiedSlots() == 1);
    assert(!inventory.remove(99, 1));
    return 0;
}
