#include <stdio.h>
#include "list.h"

void print_integer(void* n) {
    printf("%d\n", *(int*) n);
}

void add_one(void* n) {
    (*(int*) n)++;
}

int main() {
    // Creating our new list of integers:
    list* l = list_new(sizeof(int), NULL, NULL);
    // Let's add some stuff in it.
    int i, j;
    for (i = 1; i <= 10; i++) {
        j = i * i;
        list_r_append(l, &j);
    }
    // We can call any function to manipulate
    // and show the data:
    list_foreach(l, add_one);
    list_foreach(l, print_integer);
    return 0;
}
