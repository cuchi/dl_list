#include "list.h"

_node* _node_new(void* data, int t_size, _node* prev, _node* next) {
    if (data == NULL) {
        return NULL;
    } else {
        _node* n = (_node*) malloc(sizeof(_node));
        n->data = malloc(t_size);
        memcpy(n->data, data, t_size);
        n->prev = prev;
        n->next = next;
        return n;
    }
}

void _node_free(_node* n, void (*free_f)(void*)) {
    if (free_f == NULL) {
        free(n->data);
    } else {
        free_f(n->data);
    }
    free(n);
}

void _node_unlink(_node* n) {
    if (n->prev != NULL) {
        n->prev->next = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    }
}

_node* _node_at(list* l, int index) {
    if (index > l->current_size - 1 || index < 0) {
        return NULL;
    } else if (index < l->current_size / 2) {
        _node* n = l->head;
        int i = 0;
        while (i < index) {
            n = n->next;
            i++;
        }
        return n;
    } else {
        _node* n = l->tail;
        int i = l->current_size - 1;
        while (i > index) {
            n = n->prev;
            i--;
        }
        return n;
    }
}

list* list_new(int t_size, void (*free_f)(void*), void* (*copy_f)(void*)) {
    list* l = (list*) malloc(sizeof(list));
    l->t_size = t_size;
    l->head = NULL;
    l->tail = NULL;
    l->current_size = 0;
    l->free_f = free_f;
    l->copy_f = copy_f;
    return l;
}

void list_free(list* l) {
    _node* n = l->head;
    _node* aux;
    while (n != NULL) {
        aux = n;
        n = n->next;
        _node_free(aux, l->free_f);
    }
    free(l);
}

list* list_copy(list* l) {
    list* l_copy = list_new(l->t_size, l->free_f, l->copy_f);
    _node* n = l->head;
    if (l->copy_f == NULL || l->free_f == NULL) {
        while (n != NULL) {
            list_r_append(l_copy, n->data);
        }
    } else {
        while (n != NULL) {
            void* data_copy = l->copy_f(n->data);
            list_r_append(l_copy, data_copy);
            l->free_f(data_copy);
        }
    }
    return l_copy;
}

int list_is_empty(list* l) {
    return l->current_size == 0;
}

void list_r_append(list* l, void* data) {
    l->tail = _node_new(data, l->t_size, l->tail, NULL);
    if (l->tail->prev == NULL) {
        l->head = l->tail;
    } else {
        l->tail->prev->next = l->tail;
    }
    l->current_size++;
}

void list_l_append(list* l, void* data) {
    l->head = _node_new(data, l->t_size, NULL, l->head);
    if (l->head->next == NULL) {
        l->tail = l->head;
    } else {
        l->head->next->prev = l->head;
    }
    l->current_size++;
}

void* list_r_pop(list* l) {
    if (!list_is_empty(l)) {
        void* data = malloc(l->t_size);
        memcpy(data, l->tail->data, l->t_size);
        _node* n = l->tail;
        l->tail = l->tail->prev;
        _node_free(n, l->free_f);
        if (l->tail == NULL) {
            l->head = NULL;
        } else {
            l->tail->next = NULL;
        }
        l->current_size--;
        return data;
    } else {
        return NULL;
    }
}

void* list_l_pop(list* l) {
    if (!list_is_empty(l)) {
        void* data = malloc(l->t_size);
        memcpy(data, l->head->data, l->t_size);
        _node* n = l->head;
        l->head = l->head->next;
        _node_free(n, l->free_f);
        if (l->head == NULL) {
            l->tail = NULL;
        } else {
            l->head->prev = NULL;
        }
        l->current_size--;
        return data;
    } else {
        return NULL;
    }
}

int list_insert(list* l, void* data, int index) {
    if (index == 0) {
        list_l_append(l, data);
        return 1;
    } else if (index == l->current_size) {
        list_r_append(l, data);
        return 1;
    } else {
        _node* n = _node_at(l, index);
        if (n != NULL) {
            n->prev = _node_new(data, l->t_size, n->prev, n);
            n->prev->prev->next = n->prev;
            l->current_size++;
            return 1;
        } else {
            return 0;
        }
    }
}

void* list_remove(list* l, int index) {
    if (index >= 0 && index < l->current_size) {
        _node* n = _node_at(l, index);
        if (n == l->head) {
            l->head = n->next;
        }
        if (n == l->tail) {
            l->tail = n->prev;
        }
        _node_unlink(n);
        void* data = malloc(l->t_size);
        memcpy(data, n->data, l->t_size);
        _node_free(n, l->free_f);
        return data;
    } else {
        return NULL;
    }
}

void* list_get_copy(list* l, int index) {
    _node* n = _node_at(l, index);
    if (n != NULL) {
        void* data = malloc(l->t_size);
        memcpy(data, n->data, l->t_size);
        return data;
    } else {
        return NULL;
    }
}

void* list_get_ref(list* l, int index) {
    _node* n = _node_at(l, index);
    if (n != NULL) {
        return n->data;
    } else {
        return NULL;
    }
}

void list_foreach(list* l, void (*func)(void*)) {
    _node* n = l->head;
    while (n != NULL) {
        func(n->data);
        n = n->next;
    }
}
