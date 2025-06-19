#include "physics/Collider.h"
#include <cassert>

int main() {
    AABB a({0,0,0}, {1,1,1});
    AABB b({0.5f,0.5f,0.5f}, {1.5f,1.5f,1.5f});
    assert(a.intersects(b));
    auto overlap = a.getOverlap(b);
    assert(overlap.x > 0 && overlap.y > 0 && overlap.z > 0);
    return 0;
}