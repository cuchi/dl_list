#include <stdio.h>
#include <time.h>
#include "list.h"

void print_integer(void* n) {
    printf("%d\n", *(int*) n);
}

void add_one(void* n) {
    (*(int*) n)++;
}

int main() {
    srand(time(NULL));

    // Creating our new list of integers:
    list* l = list_new(sizeof(int), NULL, NULL);

    // Let's add some stuff in it.
    int i, j, k;
    for (i = 1; i <= 20; i++) {
        j = rand() % i;
        k = rand() % 50;
        printf("Inserting %d in [%d].\n", k, j);
        list_insert(l, &k, j);
    }

    // We can call any function to manipulate
    printf("Adding one to every element...\n");
    list_foreach(l, add_one);

    // and show the data:
    list_foreach(l, print_integer);

    for (i = 20; i >= 1; i--) {
        j = rand() % i;
        int* p = list_remove(l, j);
        memcpy(&k, p, sizeof(int));
        free(p);
        printf("Removed %d in [%d]\n", k, j);
    }
    return 0;
}
