#include "shell.h"
#include "set.h"
#include "gaps.h"

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    uint32_t i, j, k, temp, gap;
    for (i = 0; i < GAPS; i++) {
        gap = gaps[i];

        for (k = gap; k < length; k++) {
            j = k;
            temp = arr[move(stats, k)];
            //while ((j >= gap) && (temp < arr[j - gap]))
            while ((j >= gap) && (cmp(stats, arr[j - gap], temp) > 0)) {
                arr[move(stats, j)] = arr[j - gap];
                j -= gap;
            }
            arr[move(stats, j)] = temp;
        }
    }

    //for (i = 0; i < length; i++)
    //{
    //	printf("%d\n", arr[i]);
    //}
}
