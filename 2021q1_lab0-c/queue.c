#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL) {
        return NULL;
    } else {
        q->head = NULL;
        q->q_size = 0;
        q->last = q->head;
        return q;
    }
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q != NULL) {
        list_ele_t *current = q->head, *next;
        while (current != NULL) {
            next = current->next;
            free(current->value);
            free(current);
            current = next;
        }
        free(q);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (newh == NULL) {
        return false;
    } else {
        newh->value = strdup(s);
        if (newh->value == NULL) {
            free(newh);
            return false;
        }
        newh->next = q->head;  // newh->next will definitely be head

        if (q->head == NULL) {
            // queue empty
            q->last = newh;
        }
        q->head = newh;
        q->q_size++;
        return true;
    }
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }

    list_ele_t *newh;

    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    } else {
        newh->value = strdup(s);
        if (newh->value == NULL) {
            free(newh);
            return false;
        }
        newh->next = NULL;

        if (q->last == NULL) {
            q->head = q->last = newh;
        } else {
            q->last->next = newh;
            q->last = newh;
        }
        q->q_size++;

        return true;
    }
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (q == NULL || q->head == NULL) {
        return false;
    } else {
        if (sp != NULL) {
            strncpy(sp, q->head->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        }
        list_ele_t *temp = q->head;
        q->head = q->head->next;
        q->q_size--;
        free(temp->value);
        free(temp);
        return true;
    }
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q == NULL ? 0 : q->q_size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if (q == NULL) {
        return;
    }

    list_ele_t *previous = NULL, *current = q->head, *next = q->head, *temp;
    temp = q->head;
    q->head = q->last;
    q->last = temp;

    while (next != NULL) {
        // store
        next = current->next;

        // reverse
        current->next = previous;

        // move on
        previous = current;
        current = next;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *q1, list_ele_t *q2)
{
    list_ele_t *thead = NULL, *tcur = NULL;

    if (strcasecmp(q1->value, q2->value) < 0) {
        thead = q1;
        q1 = q1->next;
    } else {
        thead = q2;
        q2 = q2->next;
    }
    tcur = thead;

    while (q1 && q2) {
        if (strcasecmp(q1->value, q2->value) < 0) {
            tcur->next = q1;
            q1 = q1->next;
        } else {
            tcur->next = q2;
            q2 = q2->next;
        }
        tcur = tcur->next;
    }

    if (q1) {
        tcur->next = q1;
    }
    if (q2) {
        tcur->next = q2;
    }

    return thead;
}

list_ele_t *mergeSort(list_ele_t *q)
{
    if (!q || !q->next) {
        return q;
    }
    list_ele_t *q1 = q->next, *q2 = q;

    while (q1 && q1->next) {
        q2 = q2->next;
        q1 = q1->next->next;
    }

    q1 = q2->next;
    q2->next = NULL;

    list_ele_t *t1 = mergeSort(q);
    list_ele_t *t2 = mergeSort(q1);
    return merge(t1, t2);
}

void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    q->head = mergeSort(q->head);

    // update q->last
    list_ele_t *current = q->head;
    while (current != NULL) {
        q->last = current;
        current = current->next;
    }
}
