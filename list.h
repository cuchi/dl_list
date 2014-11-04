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

typedef struct _iterator _iterator;
struct _iterator {
    _node* p;
    int index;
};


typedef struct list list;
struct list {
    _node* head;
    _node* tail;
    int t_size;
    int current_size;
    void* (*copy_f)(void*);
    void (*free_f)(void*);
    _iterator* i;
};

_node* _node_new(void* data, int t_size, _node* prev, _node* next);
void _node_free(_node* n, void (*free_f)(void*));
_node* _node_at(list* l, int index);
void _node_unlink(_node* n);

void _iterator_reset(_iterator* i);
_node* _iterator_mv_to(list* l, int index);
_node* _iterator_mv_next(_iterator* i);
_node* _iterator_mv_prev(_iterator* i);

list* list_new(int t_size, void (*free_f)(void*), void* (*copy_f)(void*));
void list_free(list* l);
int list_is_empty(list* l);
list* list_copy(list* l);
void list_r_append(list* l, void* data);
void list_l_append(list* l, void* data);
int list_insert(list* l, void* data, int index);
void* list_r_pop(list* l);
void* list_l_pop(list* l);
void* list_remove(list* l, int index);
void* list_get_copy(list* l, int index);
void* list_get_ref(list* l, int index);
void list_foreach(list* l, void (*func)(void*));
void list_mergesort(list* l, int (*cmp)(void*, void*));
void _list_mergesort(list* l, int pos_a, int pos_b, int (*cmp)(void*, void*));
void _list_merge(list* l, int pos_a, int pos_b, int (*cmp)(void*, void*));
list* list_filter(list* l, int (*filter_f)(void*));
int list_linear_search(list* l, void* data, int (*cmp)(void*, void*));

#endif // LIST_H
