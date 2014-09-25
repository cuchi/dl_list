#include <stdio.h>
#include <time.h>
#include "list.h"

void print_integer(void* n) {
    printf("%d\n", *(int*) n);
}

void add_one(void* n) {
    (*(int*) n)++;
}

int cmp_int(void* a, void* b) {
    return (*(int*) a) - (*(int*) b);
}

int main() {
    srand(time(NULL));

    // Creating our new list of integers:
    list* l = list_new(sizeof(int), NULL, NULL);

    // Let's add some stuff in it.
    int i, j, k;
    // Playing with randomness:
    for (i = 1; i <= 50; i++) {
        j = rand() % i;
        k = rand() % 500;
        printf("Inserting %d in [%d].\n", k, j);
        list_insert(l, &k, j);
    }

    printf("Playing with the quicksort:\n");
    list_quicksort(l, cmp_int);
    list_foreach(l, print_integer);
    return 0;
}
