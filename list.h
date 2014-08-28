#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

typedef struct _node _node;
struct _node {
    _node* next;
    _node* prev;
    void* data;
};

typedef struct list list;
struct list {
    _node* head;
    _node* tail;
    int t_size;
    int current_size;
    void* (*copy_f)(void*);
    void (*free_f)(void*);
};

_node* _node_new(void* data, int t_size, _node* prev, _node* next);
void _node_free(_node* n, void (*free_f)(void*));
_node* _node_at(list* l, int index);

list* list_new(int t_size, void (*free_f)(void*), void* (*copy_f)(void*));
void list_free(list* l);
int list_is_empty(list* l);
list* list_copy(list* l);
void list_r_append(list* l, void* data);
void list_l_append(list* l, void* data);
int list_insert(list* l, void* data, int index);
void* list_r_pop(list* l);
void* list_l_pop(list* l);
void* list_get_copy(list* l, int index);
void* list_get_ref(list* l, int index);
void list_foreach(list* l, void (*func)(void*));

#endif // LIST_H
