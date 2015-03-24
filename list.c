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
    } else {
        int d0, di, dn;
        d0 = index;
        dn = l->current_size - index - 1;
        if (l->i->index == -1) {
            if (d0 < dn) {
                l->i->index = 0;
                l->i->p = l->head;
            } else {
                l->i->index = l->current_size - 1;
                l->i->p = l->tail;
            }
        }
        di = index - l->i->index;
        di = di >= 0 ? di : -di;
        if (d0 < di && d0 <= dn) {
            _node* n = l->head;
            int i = 0;
            while (i < index) {
                n = n->next;
                i++;
            }
            return n;
        } else if (dn < di && dn <= d0) {
            _node* n = l->tail;
            int i = l->current_size - 1;
            while (i > index) {
                n = n->prev;
                i--;
            }
            return n;
        } else {
            return _iterator_mv_to(l, index);
        }
    }
}

void _iterator_reset(_iterator* i) {
    i->index = -1;
    i->p = NULL;
}

_node* _iterator_mv_to(list* l, int index) {
    if (l->i->p == NULL) {
        if (l->current_size / 2 > index) {
            l->i->index = 0;
            l->i->p = l->head;
        } else {
            l->i->index = l->current_size - 1;
            l->i->p = l->tail;
        }
    }
    while (l->i->index < index) {
        if (_iterator_mv_next(l->i) == NULL) {
            return NULL;
        }
    }
    while (l->i->index > index) {
        if (_iterator_mv_prev(l->i) == NULL) {
            return NULL;
        }
    }
    return l->i->p;
}

_node* _iterator_mv_next(_iterator* i) {
    if (i->p == NULL) {
        return NULL;
    } else {
        if (i->p->next != NULL) {
            i->index++;
            i->p = i->p->next;
            return i->p;
        } else {
            _iterator_reset(i);
            return NULL;
        }
    }
}

_node* _iterator_mv_prev(_iterator* i) {
    if (i->p == NULL) {
        return NULL;
    } else {
        if (i->p->prev != NULL) {
            i->index--;
            i->p = i->p->prev;
            return i->p;
        } else {
            _iterator_reset(i);
            return NULL;
        }
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
    l->i = (_iterator*) malloc(sizeof(_iterator));
    _iterator_reset(l->i);
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
    return list_remove(l, l->current_size - 1);
}

void* list_l_pop(list* l) {
    return list_remove(l, 0);
}

int list_insert(list* l, void* data, int index) {
    if (index > l->current_size && index < 0) {
        return 0;
    } else {
        if (index == 0) {
            list_l_append(l, data);
        } else if (index == l->current_size) {
            list_r_append(l, data);
        } else {
            _node* n = _node_at(l, index);
            n->prev = _node_new(data, l->t_size, n->prev, n);
            n->prev->prev->next = n->prev;
            l->current_size++;
        }
        if (l->i->index >= index) {
            l->i->index++;
        }
        _iterator_mv_to(l, index);
        return 1;
    }
}

void* list_remove(list* l, int index) {
    _node* n = _node_at(l, index);
    if (n == NULL) {
        return NULL;
    } else {
        if (n == l->head) {
            l->head = n->next;
        }
        if (n == l->tail) {
            l->tail = n->prev;
        }
        _iterator_mv_to(l, index + 1);
        _node_unlink(n);
        void* data = malloc(l->t_size);
        memcpy(data, n->data, l->t_size);
        _node_free(n, l->free_f);
        if (l->i->p != NULL) {
            l->i->index--;
        }
        return data;
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

void list_swap(list* l, int _a, int _b) {
    _node* a = _node_at(l, _a);
    _node* b = _node_at(l, _b);
    if (a && b) {
        void* aux;
        aux = a->data;
        a->data = b->data;
        b->data = aux;
    }
}

void list_mergesort(list* l, int (*cmp)(void*, void*)) {
    _list_mergesort(l, 0, l->current_size - 1, cmp);
}

void _list_mergesort(list* l, int pos_a, int pos_b, int (*cmp)(void*, void*)) {
    if (pos_b > pos_a) {
        int m = (pos_b + pos_a + 1) / 2;
        _list_mergesort(l, pos_a, m - 1, cmp);
        _list_mergesort(l, m, pos_b, cmp);
        _list_merge(l, pos_a, pos_b, cmp);
    }
}

void _list_merge(list* l, int pos_a, int pos_b, int (*cmp)(void*, void*)) {
    int mid = (pos_b + pos_a + 1) / 2;
    _node* a = _node_at(l, pos_a);
    _node* b = _node_at(l, mid);
    if(_iterator_mv_to(l, pos_b + 1) == NULL) {
        _iterator_reset(l->i);
    }
    int i = pos_a;
    int j = mid;
    if (pos_b - pos_a == 1) {
        if (cmp(a->data, b->data) > 0) {
            void* aux = a->data;
            a->data = b->data;
            b->data = aux;
        }
        return;
    }
    while (i < mid && j <= pos_b) {
        if (cmp(a->data, b->data) > 0) {;
            _node* aux = b;
            b = b->next;
            if (j == pos_b) {
                if (b == NULL) {
                    l->tail = aux->prev;
                }
            }
            j++;
            _node_unlink(aux);
            aux->prev = a->prev;
            aux->next = a;
            a->prev = aux;
            if (aux->prev != NULL) {
                aux->prev->next = aux;
            } else {
                l->head = aux;
            }
        } else {
            i++;
            a = a->next;
        }
    }

}

list* list_filter(list* l, int (*filter_f)(void*)) {
    list* nl = list_new(l->t_size, l->free_f, l->copy_f);
    _node* p = l->head;
    while (p != NULL) {
        if (filter_f(p->data)) {
            list_r_append(nl, p->data);
        }
        p = p->next;
    }
    return nl;
}

int list_linear_search(list* l, void* data, int (*cmp)(void*, void*)) {
    if (list_is_empty(l)) {
        return -1;
    } else {
        _node* p = l->head;
        int i = 0;
        while (p != NULL && cmp(data, p->data) != 0) {
            p = p->next;
            i++;
        }
        if (p == NULL) {
            return -1;
        } else {
            return i;
        }
    }
}
