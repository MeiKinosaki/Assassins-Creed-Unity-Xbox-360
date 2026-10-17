#include "../src/gameplay/gameplay_event_queue.h"
#include <cassert>

int main() {
    acunity360::GameplayEventQueue queue;
    acunity360::GameplayEvent out = {
        acunity360::GameplayEventType::ObjectiveProgress, 0u, 0
    };

    assert(queue.empty());
    assert(queue.size() == 0u);
    assert(!queue.tryDequeue(out));

    const acunity360::GameplayEvent first = {
        acunity360::GameplayEventType::ItemCollected, 42u, 3
    };
    const acunity360::GameplayEvent second = {
        acunity360::GameplayEventType::ObjectiveProgress, 7u, 1
    };
    const acunity360::GameplayEvent third = {
        acunity360::GameplayEventType::InteractionCompleted, 9u, -1
    };

    queue.enqueue(first);
    queue.enqueue(second);
    queue.enqueue(third);
    assert(queue.size() == 3u);

    assert(queue.tryDequeue(out));
    assert(out.type == first.type && out.subject_id == 42u && out.value == 3);
    assert(queue.tryDequeue(out));
    assert(out.type == second.type && out.subject_id == 7u && out.value == 1);
    assert(queue.tryDequeue(out));
    assert(out.type == third.type && out.subject_id == 9u && out.value == -1);
    assert(queue.empty());
    assert(!queue.tryDequeue(out));

    queue.enqueue(first);
    queue.enqueue(second);
    queue.clear();
    assert(queue.empty());
    assert(queue.size() == 0u);
    return 0;
}
