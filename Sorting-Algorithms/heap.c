#include "heap.h"
#include "set.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left, right;
    left = 2 * first;
    right = left + 1;
    //if ((right <= last) && (A[right - 1] > A[left - 1]))
    if ((right <= last) && (cmp(stats, A[right - 1], A[left - 1]) > 0)) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while ((mother <= (floor(last / 2))) && (!found)) {
        //if (A[mother - 1] < A[great - 1])
        if (cmp(stats, A[great - 1], A[mother - 1]) > 0) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t father;
    for (father = floor(last / 2); father > (first - 1); father -= 1) {
        fix_heap(stats, A, father, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //printf("%d\n", A[0]);
    uint32_t first, last, leaf;
    first = 1;
    last = n;
    build_heap(stats, A, first, last);

    for (leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
}
