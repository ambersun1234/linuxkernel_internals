#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct __node {                   
    int value;
    struct __node *next;
    struct __node *previous;
} node_t;

static inline void list_add_node_t(node_t **list, node_t *node_t) {
    node_t->next = *list;
    *list = node_t;
}

static inline void list_concat(node_t **left, node_t *right) {
    while (*left)
        left = &((*left)->next);
    *left = right;
}

static bool list_is_ordered(node_t *list) {
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

static void list_display(node_t *list) {
    printf("%s IN ORDER : ", list_is_ordered(list) ? "   " : "NOT");
    while (list) {
        // printf("%d %p\n", list->value, list);
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

void quicksort_iterative(node_t *list, node_t *tail, int count) {
    #define MAX_LEVELS 100
    int i = 0;
    node_t *tl, *tr;
    node_t *beg[MAX_LEVELS];
    node_t *end[MAX_LEVELS];

    beg[0] = list;
    end[0] = tail;

    while(i >= 0) {
        tl = beg[i];
        tr = end[i];

        if (tl != tr && tl && tr) {
            int pivot = tl->value;
            if (i == count - 1) return;
            while(tl != tr && tl && tr) {
                while (tr->value >= pivot && tl != tr) tr = tr->previous;
                if (tl != tr) {
                    tl->value = tr->value;
                    tl = tl->next;
                }

                while (tl->value <= pivot && tl != tr) tl = tl->next;
                if (tl != tr) {
                    tr->value = tl->value;
                    tr = tr->previous;
                }
            }
            tl->value = pivot;
            beg[i + 1] = tl->next;
            end[i + 1] = end[i];
            end[i] = tl;
            i++;
        }
        else i--;
    }
    return;
}

void quicksort(node_t **list)
{
    if (!*list)
        return;

    node_t *pivot = *list;
    int value = pivot->value;
    node_t *p = pivot->next;
    pivot->next = NULL;

    node_t *left = NULL, *right = NULL;
    while (p) {
        node_t *n = p;
        p = p->next;
        list_add_node_t(n->value > value ? &right: &left, n);
    }

    quicksort(&left);
    quicksort(&right);

    node_t *result = NULL;
    list_concat(&result, left);
    list_concat(&result, pivot); list_concat(&result, right);
    *list = result;
}

static void list_free(node_t **list) {
    while (*list) {
        node_t *tmp = (*list)->next;
        // printf("########## %d %p %p\n", (*list)->value, *list, (*list)->next);
        free(*list);
        *list = tmp;
    }
}

static node_t* list_make_node_t(node_t *list, int value) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->value = value;
    newNode->next = list;
    if (list != NULL) list->previous = newNode;

    return newNode;
}

int main(int argc, char **argv) {
    srand((uintptr_t)&main);

    size_t count = 20;

    node_t *list = NULL;
    while (count--) {
        list = list_make_node_t(list, random() % 1024);
    }

    // get tail
    node_t *tail = list;
    while (tail->next != NULL) tail = tail->next;

    list_display(list);
    // quicksort(&list);
    quicksort_iterative(list, tail, count);
    list_display(list);


    if (!list_is_ordered(list))
        return EXIT_FAILURE;

    list_free(&list);
    return EXIT_SUCCESS;
}
