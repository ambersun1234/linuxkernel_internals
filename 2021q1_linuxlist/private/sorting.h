#include <stdio.h>
#include <stdlib.h>

void list_insert_sorted(struct listitem *entry, struct list_head *head)
{
    struct listitem *item = NULL;

    if (list_empty(head)) {
        list_add(&entry->list, head);
        return;
    }

    list_for_each_entry (item, head, list) {
        if (cmpint(&entry->i, &item->i) < 0) {
            list_add_tail(&entry->list, &item->list);
            return;
        }
    }

    list_add_tail(&entry->list, head);
}

void list_insertsort(struct list_head *head)
{
    struct list_head list_unsorted;
    struct listitem *item = NULL, *is = NULL;

    INIT_LIST_HEAD(&list_unsorted);
    list_splice_init(head, &list_unsorted);

    list_for_each_entry_safe (item, is, &list_unsorted, list) {
        list_del(&item->list);
        list_insert_sorted(item, head);
    }
}

//----

inline void showlist(struct list_head *head)
{
    struct listitem *itemiterator = NULL, *store = NULL;

    printf("head = %p, head->next = %p, head->prev = %p\n", (void *) (head),
           (void *) (head->next), (void *) (head->prev));

    list_for_each_entry_safe (itemiterator, store, head, list) {
        printf(" %u, item = %p, i = %p, list = %p, next = %p, prev = %p\n",
               *(&itemiterator->i), (void *) (itemiterator),
               (void *) (&itemiterator->i), (void *) (&itemiterator->list),
               (void *) (itemiterator->list.next),
               (void *) (itemiterator->list.prev));
    }
    putchar('\n');
    putchar('\n');
}

void list_merge(struct list_head *left,
                struct list_head *right,
                struct list_head *head)
{
    struct listitem *l_item, *r_item;
    INIT_LIST_HEAD(head);  // re-init list head

    left = left->next;
    right = right->next;

    while (!list_empty(left) && !list_empty(right)) {
        l_item = list_entry(left, struct listitem, list);
        r_item = list_entry(right, struct listitem, list);

        struct listitem *new =
            (struct listitem *) malloc(sizeof(struct listitem));
        struct list_head *temp = NULL;

        if (cmpint(&l_item->i, &r_item->i) < 0) {
            temp = left;
            left = left->next;
            list_del(temp);
            new->i = l_item->i;
        } else {
            temp = right;
            right = right->next;
            list_del(temp);
            new->i = r_item->i;
        }
        list_add_tail(&new->list, head);
    }

    if (!list_empty(left)) {
        list_splice_tail(left->prev, head);
    }
    if (!list_empty(right)) {
        list_splice_tail(right->prev, head);
    }
}

void list_mergesort(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head list_left, list_right;
    struct listitem *itemiterator = NULL, *store = NULL;
    int counter = 0;

    INIT_LIST_HEAD(&list_left);
    INIT_LIST_HEAD(&list_right);
    list_for_each_entry_safe (itemiterator, store, head, list) {
        if (counter & 1) {
            list_move(&itemiterator->list, &list_left);
        } else {
            list_move(&itemiterator->list, &list_right);
        }
        counter += 1;
    }

    // showlist(&list_left);
    // showlist(&list_right);

    // recursive call
    list_mergesort(&list_left);
    list_mergesort(&list_right);

    // merge
    list_merge(&list_left, &list_right, head);
}

//--------------
void list_qsort(struct list_head *head)
{
    struct list_head list_less, list_greater;
    struct listitem *pivot;
    struct listitem *item = NULL, *is = NULL;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    pivot = list_first_entry(head, struct listitem, list);
    list_del(&pivot->list);

    list_for_each_entry_safe (item, is, head, list) {
        if (cmpint(&item->i, &pivot->i) < 0)
            list_move_tail(&item->list, &list_less);
        else
            list_move(&item->list, &list_greater);
    }

    list_qsort(&list_less);
    list_qsort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);
}
