#include "../src/gameplay/objective_tracker.h"
#include <cassert>

int main() {
    acunity360::ObjectiveTracker tracker;
    assert(!tracker.configured());
    assert(!tracker.advance(1u, 1u));
    assert(!tracker.configure(0u, 3u));
    assert(!tracker.configure(1u, 0u));
    assert(tracker.configure(12u, 5u));
    assert(tracker.advance(12u, 2u));
    assert(tracker.current().current == 2u);
    assert(!tracker.advance(13u, 1u));
    assert(!tracker.advance(12u, 0u));
    assert(tracker.advance(12u, 10u));
    assert(tracker.current().current == 5u);
    assert(tracker.current().completed);
    assert(!tracker.advance(12u, 1u));
    return 0;
}
