/* linked_list.c
 *
 * Implementation of the singly-linked list interface declared in linked_list.h.
 * See that header for the full API documentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/* ── Allocation ──────────────────────────────────────────────────────────── */

/* Allocate a new node on the heap, initialise its fields, and return it.
 * Returns NULL (and prints to stderr) when malloc fails.              */
Node *new_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }
    node->value = value;
    node->next  = NULL;
    return node;
}

/* ── Insertion ───────────────────────────────────────────────────────────── */

/* Prepend VALUE to the list whose head is HEAD.
 * The new node becomes the new head; the old list follows unchanged.
 * Time complexity: O(1).                                              */
Node *insert_front(Node *head, int value) {
    Node *node = new_node(value);
    if (node == NULL) return head;   /* leave list untouched on alloc failure */
    node->next = head;
    return node;
}

/* Append VALUE at the tail of the list (recursive).
 * Base case: HEAD is NULL → we are past the last node, create a new one.
 * Recursive case: walk forward and re-attach head->next on the way back.
 * Time complexity: O(n).                                              */
Node *insert_back(Node *head, int value) {
    if (head == NULL)
        return new_node(value);
    head->next = insert_back(head->next, value);
    return head;
}

/* Insert VALUE at position INDEX (0-based).
 * If INDEX is 0 the behaviour is identical to insert_front.
 * If INDEX is >= list length the node is appended at the tail.
 * Time complexity: O(n).                                              */
Node *insert_at(Node *head, int value, int index) {
    if (index <= 0 || head == NULL)
        return insert_front(head, value);

    /* Walk until we reach the node just before the desired position. */
    Node *current = head;
    for (int i = 0; i < index - 1 && current->next != NULL; i++)
        current = current->next;

    Node *node = new_node(value);
    if (node == NULL) return head;

    node->next    = current->next;   /* new node points to the rest */
    current->next = node;            /* predecessor points to new node */
    return head;
}

/* ── Removal ─────────────────────────────────────────────────────────────── */

/* Remove the head node and write its value into *OUT.
 * Returns 1 on success, 0 if the list is empty.
 * Caller must use the double-pointer form: pop_front(&head, &val).   */
int pop_front(Node **head, int *out) {
    if (*head == NULL) return 0;

    Node *temp = *head;
    *out  = temp->value;
    *head = temp->next;
    free(temp);
    return 1;
}

/* Remove the tail node and write its value into *OUT.
 * Returns 1 on success, 0 if the list is empty.                      */
int pop_back(Node **head, int *out) {
    if (*head == NULL) return 0;

    /* Special case: only one node in the list. */
    if ((*head)->next == NULL) {
        *out  = (*head)->value;
        free(*head);
        *head = NULL;
        return 1;
    }

    /* Walk until current->next is the last node. */
    Node *current = *head;
    while (current->next->next != NULL)
        current = current->next;

    *out          = current->next->value;
    free(current->next);
    current->next = NULL;
    return 1;
}

/* Delete the first node whose value equals VALUE.
 * Returns 1 if a node was removed, 0 if VALUE was not found.         */
int remove_value(Node **head, int value) {
    if (*head == NULL) return 0;

    /* The head itself matches. */
    if ((*head)->value == value) {
        Node *temp = *head;
        *head = temp->next;
        free(temp);
        return 1;
    }

    /* Search for the predecessor of the node to remove. */
    Node *current = *head;
    while (current->next != NULL && current->next->value != value)
        current = current->next;

    if (current->next == NULL) return 0;   /* value not found */

    Node *to_delete = current->next;
    current->next   = to_delete->next;
    free(to_delete);
    return 1;
}

/* ── Search & query ──────────────────────────────────────────────────────── */

/* Return a pointer to the first node whose value equals VALUE,
 * or NULL if no such node exists.
 * Time complexity: O(n).                                              */
Node *list_search(Node *head, int value) {
    while (head != NULL) {
        if (head->value == value) return head;
        head = head->next;
    }
    return NULL;
}

/* Return 1 if VALUE is present in the list, 0 otherwise.             */
int list_contains(Node *head, int value) {
    return list_search(head, value) != NULL;
}

/* Return the number of nodes in the list (recursive).
 * Base case: empty list → 0.
 * Recursive case: 1 + length of the remaining list.                  */
int list_length(Node *head) {
    if (head == NULL) return 0;
    return 1 + list_length(head->next);
}

/* ── Transformation ──────────────────────────────────────────────────────── */

/* Reverse the list in-place and return the new head.
 * Uses three pointers to relink nodes without extra allocations.
 * Time complexity: O(n), space complexity: O(1).
 *
 * Step-by-step with [A → B → C → NULL]:
 *   iter 1: prev=NULL  → A→NULL,  prev=A,  current=B
 *   iter 2: prev=A     → B→A,     prev=B,  current=C
 *   iter 3: prev=B     → C→B,     prev=C,  current=NULL  ← new head  */
Node *list_reverse(Node *head) {
    Node *prev    = NULL;
    Node *current = head;

    while (current != NULL) {
        Node *next    = current->next;  /* save the rest before relinking */
        current->next = prev;           /* point backward */
        prev          = current;
        current       = next;
    }
    return prev;   /* prev is the new head when current fell off the end */
}

/* ── Utilities ───────────────────────────────────────────────────────────── */

/* Print every node in the format:  value (addr) -> … -> NULL          */
void list_print(Node *head) {
    while (head != NULL) {
        printf("%d (%p) -> ", head->value, (void *)&head->value);
        head = head->next;
    }
    puts("NULL");
}

/* Free every node in the list and set *HEAD to NULL.
 * Always call this when the list is no longer needed to avoid leaks.  */
void list_free(Node **head) {
    Node *current = *head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
