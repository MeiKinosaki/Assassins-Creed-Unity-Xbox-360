#include "../src/gameplay/gameplay_event_queue.h"
#include <cassert>

int main() {
    acunity360::GameplayEventQueue queue(3u);
    acunity360::GameplayEvent out = {
        acunity360::GameplayEventType::ObjectiveProgress, 0u, 0
    };

    assert(queue.empty());
    assert(queue.size() == 0u);
    assert(!queue.tryDequeue(out));

    acunity360::GameplayEventQueue zero_capacity(0u);
    assert(zero_capacity.capacity() == 0u);
    assert(zero_capacity.empty());
    assert(zero_capacity.full());
    assert(!zero_capacity.enqueue(first));
    assert(!zero_capacity.tryDequeue(out));

    const acunity360::GameplayEvent first = {
        acunity360::GameplayEventType::ItemCollected, 42u, 3
    };
    const acunity360::GameplayEvent second = {
        acunity360::GameplayEventType::ObjectiveProgress, 7u, 1
    };
    const acunity360::GameplayEvent third = {
        acunity360::GameplayEventType::InteractionCompleted, 9u, -1
    };

    assert(queue.enqueue(first));
    assert(queue.enqueue(second));
    assert(queue.enqueue(third));
    assert(queue.size() == 3u);
    assert(queue.full());
    assert(!queue.enqueue(first));

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
