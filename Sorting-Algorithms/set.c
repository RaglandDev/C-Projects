#include "set.h"

// NOTE: "Set" is a uint32 type, not mathematical set

// Return empty Set (0)
Set set_empty(void) {
    return 0;
}

// Return set where every possible member is included
Set set_universal(void) {
    return 511; // (1 1111 1111)
}

// Returns set with bit x set to 1
Set set_insert(Set s, uint8_t x) {
    return s | (1 << x);
}

// Returns set with bit x set to 0
Set set_remove(Set s, uint8_t x) {
    return s & (~1 << x);
}

// Returns true if x is in the Set, else false
bool set_member(Set s, uint8_t x) {
    Set temp = s & (1 << x);
    if (temp != 0) {
        return true;
    } else {
        return false;
    }
}

// Returns Set that includes all elements in both Sets
Set set_union(Set s, Set t) {
    return s | t;
}

// Returns Set that includes elements that only in both sets
Set set_intersect(Set s, Set t) {
    return s & t;
}

// Returns Set of elements that are in s and not t
Set set_difference(Set s, Set t) {
    return s & ~t;
}

// Returns complement of a Set
Set set_complement(Set s) {
    return ~s;
}
