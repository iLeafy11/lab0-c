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
    if (q) {
        q->head = q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *tmp = q->head;
    while (q->head) {
        q->head = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }
    free(q);
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
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = strdup(s);
    if (!newh->value) {
        free(newh);
        return false;
    }
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;

    q->size++;
    return true;
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
    if (!q)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    newt->value = strdup(s);
    if (!newt->value) {
        free(newt);
        return false;
    }
    newt->next = NULL;
    if (!q->tail)
        q->head = newt;
    else
        q->tail->next = newt;

    q->tail = newt;
    q->size++;
    return true;
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
    if (!q || !q->head)
        return false;

    if (sp && bufsize) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
size_t q_size(queue_t *q)
{
    return (!q || !q->head) ? 0 : q->size;
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
    if (!q || !q->head)
        return;

    if (!q->head->next)
        return;

    list_ele_t *curr = q->head;
    list_ele_t *next = NULL;
    list_ele_t *prev = NULL;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->tail = q->head;
    q->head = prev;
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void split(list_ele_t *src, list_ele_t **front, list_ele_t **back)
{
    list_ele_t *slow = src;
    list_ele_t *fast = src->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    *front = src;
    *back = slow->next;
    slow->next = NULL;
}

void merge_list(list_ele_t **head, list_ele_t *l, list_ele_t *r)
{
    list_ele_t *list = NULL;
    list_ele_t **indirect = &list;
    while (l && r) {
        if (strcmp(l->value, r->value) < 0) {
            *indirect = l;
            l = l->next;
        } else {
            *indirect = r;
            r = r->next;
        }

        indirect = &(*indirect)->next;
    }
    if (l)
        *indirect = l;
    if (r)
        *indirect = r;

    *head = list;
}

void merge_sort(list_ele_t **head)
{
    if (!*head || !(*head)->next)
        return;

    list_ele_t *left = NULL;
    list_ele_t *right = NULL;
    split(*head, &left, &right);
    merge_sort(&left);
    merge_sort(&right);
    merge_list(head, left, right);
}

void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);
    list_ele_t *walk = q->head;
    while (walk)
        walk = walk->next;

    q->tail = walk;
}
