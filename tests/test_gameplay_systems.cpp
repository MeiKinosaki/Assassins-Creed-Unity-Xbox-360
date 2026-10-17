#include "../src/gameplay/gameplay_systems.h"
#include <cassert>

int main() {
    acunity360::GameplaySystems game(4);
    assert(game.objectiveTracker().configure(100u, 3u));

    game.postEvent(acunity360::GameplayEvent{
        acunity360::GameplayEventType::ItemCollected, 55u, 2
    });
    game.postEvent(acunity360::GameplayEvent{
        acunity360::GameplayEventType::ObjectiveProgress, 100u, 1
    });
    game.postEvent(acunity360::GameplayEvent{
        acunity360::GameplayEventType::ObjectiveProgress, 100u, 2
    });

    assert(game.pendingEvents() == 3u);
    assert(game.processEvents() == 3u);
    assert(game.pendingEvents() == 0u);
    assert(game.inventory().count(55u) == 2u);
    assert(game.objectiveTracker().current().completed);
    assert(game.processEvents() == 0u);
    return 0;
}
