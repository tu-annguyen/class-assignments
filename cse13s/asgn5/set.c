#include "set.h"

Set set_empty(void) {
    Set s = (uint32_t) 0;
    return s;
}

bool set_member(Set s, uint8_t x) {
    uint32_t mask = 1 << x;

    s = s & mask;

    if (s != 0) {
        return true;
    }
    return false;
}

Set set_insert(Set s, uint8_t x) {
    uint32_t mask = 1 << x;

    s = s | mask;

    return s;
}

Set set_remove(Set s, uint8_t x) {
    uint32_t mask = ~(1 << x);

    s = s & mask;

    return s;
}

Set set_intersect(Set s, Set t) {
    Set u = s & t;
    return u;
}

Set set_union(Set s, Set t) {
    Set u = s | t;
    return u;
}

Set set_complement(Set s) {
    Set u = ~s;
    return u;
}

Set set_difference(Set s, Set t) {
    Set u = s & ~t;
    return u;
}
