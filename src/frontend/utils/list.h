#ifndef __LIST_H__
#define __LIST_H__

#include "clist/clist.h"

typedef CList list;

#define list_new(type) (CList_init(sizeof(type)))

void *list_add(list *l, void *element) {
    return l->add(l, element);
}

void *list_insert(list *l, void *element, int index) {
    return l->insert(l, element, index);
}

void *list_replace(list *l, void *element, int index) {
    void *res = l->at(l, index);
    l->replace(l, element, index);
    return res;
}

#define list_replace_cast(l, element, index, type) ((type *) list_replace(l, element, index))

void *list_remove(list *l, int index) {
    void *res = l->at(l, index);
    l->remove(l, index);
    return res;
}

#define list_remove_cast(l, index, type) ((type *) list_remove(l, index))

void *list_get(list *l, int index) {
    return l->at(l, index);
}

#define list_get_cast(l, index, type) ((type *) list_get(l, index))

int list_size(list *l) {
    return l->count(l);
}

int list_swap(list *l, int first_index, int second_index) {
    return l->swap(l, first_index, second_index);
}

void list_clear(list *l) {
    l->clear(l);
}

void list_free(list *l) {
    l->free(l);
}

#endif