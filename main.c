#include <stdio.h>
#include <time.h>
#include "list.h"

void print_integer(void* n) {
    printf("%d\n", *(int*) n);
}

int cmp_int(void* a, void* b) {
    return (*(int*) a) - (*(int*) b);
}

int main() {
    list* l = list_new(sizeof(int), NULL, NULL);
    int i, j;
    int t = 1000000;
    for (i = 0; i < t; i++) {
        j = rand();
        list_l_append(l, &j);
    }
    printf("Sorting 1 million integers (MergeSort)...\n");
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
    list_mergesort(l, cmp_int);
    gettimeofday(&t1, 0);
    printf("Done!\n");
    double time = (t1.tv_sec - t0.tv_sec) + 0.000001 * (t1.tv_usec - t0.tv_usec);
    printf("Time: %lf.\n", time);
    return 0;
}
