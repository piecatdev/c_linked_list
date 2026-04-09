/*
 * linked_list.h
 *
 * Public interface for a singly-linked list of integers.
 *
 * Include this header in any translation unit that needs the list:
 *
 *   #include "linked_list.h"
 *
 *   Node *head = NULL;
 *   head = insert_back(head, 42);
 *   list_free(&head);
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* ── Node type ───────────────────────────────────────────────────────────── */

/**
 * A single node in the singly-linked list.
 *
 * @value  Integer payload stored in this node.
 * @next   Pointer to the following node, or NULL if this is the tail.
 */
typedef struct Node {
    int         value;
    struct Node *next;
} Node;

/* ── Allocation ──────────────────────────────────────────────────────────── */

/**
 * new_node — allocate and initialise a single node on the heap.
 *
 * @param value  Integer to store in the node.
 * @return       Pointer to the new node, or NULL on allocation failure
 *               (an error message is printed to stderr).
 */
Node *new_node(int value);

/* ── Insertion ───────────────────────────────────────────────────────────── */

/**
 * insert_front — prepend value to the list.
 *
 * @param head   Current head (may be NULL for an empty list).
 * @param value  Value to insert.
 * @return       The new head of the list.
 * Complexity: O(1).
 */
Node *insert_front(Node *head, int value);

/**
 * insert_back — append value at the tail of the list (recursive).
 *
 * @param head   Current head (may be NULL for an empty list).
 * @param value  Value to insert.
 * @return       The head of the list (unchanged unless list was empty).
 * Complexity: O(n).
 */
Node *insert_back(Node *head, int value);

/**
 * insert_at — insert value at a 0-based index.
 *
 * - index == 0  → same as insert_front.
 * - index >= length → appends at the tail.
 *
 * @param head   Current head (may be NULL).
 * @param value  Value to insert.
 * @param index  0-based position for the new node.
 * @return       The (possibly new) head of the list.
 * Complexity: O(n).
 */
Node *insert_at(Node *head, int value, int index);

/* ── Removal ─────────────────────────────────────────────────────────────── */

/**
 * pop_front — remove the head node and write its value into *out.
 *
 * Usage:  pop_front(&head, &val);
 *
 * @param head  Double pointer to the head; updated to head->next on success.
 * @param out   Receives the removed value.
 * @return      1 on success, 0 if the list is empty.
 * Complexity: O(1).
 */
int pop_front(Node **head, int *out);

/**
 * pop_back — remove the tail node and write its value into *out.
 *
 * @param head  Double pointer to the head.
 * @param out   Receives the removed value.
 * @return      1 on success, 0 if the list is empty.
 * Complexity: O(n).
 */
int pop_back(Node **head, int *out);

/**
 * remove_value — delete the first node whose value equals value.
 *
 * @param head   Double pointer to the head.
 * @param value  Value to search for and remove.
 * @return       1 if a node was removed, 0 if value was not found.
 * Complexity: O(n).
 */
int remove_value(Node **head, int value);

/* ── Search & query ──────────────────────────────────────────────────────── */

/**
 * list_search — return a pointer to the first node matching value.
 *
 * @param head   Head of the list.
 * @param value  Value to search for.
 * @return       Pointer to the matching node, or NULL if not found.
 * Complexity: O(n).
 */
Node *list_search(Node *head, int value);

/**
 * list_contains — check whether value is present in the list.
 *
 * @return  1 if found, 0 otherwise.
 * Complexity: O(n).
 */
int list_contains(Node *head, int value);

/**
 * list_length — count the number of nodes (recursive).
 *
 * @return  Number of nodes; 0 for an empty list.
 * Complexity: O(n).
 */
int list_length(Node *head);

/* ── Transformation ──────────────────────────────────────────────────────── */

/**
 * list_reverse — reverse the list in-place and return the new head.
 *
 * Uses three pointers; no extra allocations.
 * Complexity: O(n) time, O(1) space.
 */
Node *list_reverse(Node *head);

/* ── Utilities ───────────────────────────────────────────────────────────── */

/**
 * list_print — print every node to stdout.
 *
 * Format:  value (addr) -> value (addr) -> NULL
 */
void list_print(Node *head);

/**
 * list_free — release every node and set *head to NULL.
 *
 * Always call this when the list is no longer needed to prevent memory leaks.
 * Setting *head = NULL guards against accidental use-after-free.
 */
void list_free(Node **head);

#endif /* LINKED_LIST_H */
