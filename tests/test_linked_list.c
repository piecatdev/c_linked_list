/* tests/test_linked_list.c
 *
 * Unit test suite for the singly-linked list library.
 * Uses only the C standard library (<assert.h>, <stdio.h>) — no external deps.
 *
 * Each test function:
 *   - builds a local list from scratch
 *   - asserts the expected postcondition
 *   - frees all memory before returning
 *
 * Compile and run via:
 *   make test
 */

#include <assert.h>
#include <stdio.h>
#include "../linked_list.h"

/* Print a PASS message after each successful test. */
#define PASS(name) printf("  PASS  %s\n", (name))

/* ── new_node ────────────────────────────────────────────────────────────── */

static void test_new_node(void) {
    Node *n = new_node(7);
    assert(n != NULL);
    assert(n->value == 7);
    assert(n->next  == NULL);
    free(n);
    PASS("test_new_node");
}

/* ── insert_front ────────────────────────────────────────────────────────── */

static void test_insert_front(void) {
    Node *head = NULL;

    head = insert_front(head, 10);
    assert(head->value      == 10);
    assert(head->next       == NULL);

    head = insert_front(head, 5);
    assert(head->value      == 5);
    assert(head->next->value == 10);
    assert(list_length(head) == 2);

    list_free(&head);
    PASS("test_insert_front");
}

/* ── insert_back ─────────────────────────────────────────────────────────── */

static void test_insert_back(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    assert(list_length(head) == 3);

    Node *cur = head;
    assert(cur->value == 1); cur = cur->next;
    assert(cur->value == 2); cur = cur->next;
    assert(cur->value == 3);
    assert(cur->next  == NULL);

    list_free(&head);
    PASS("test_insert_back");
}

/* ── insert_at ───────────────────────────────────────────────────────────── */

static void test_insert_at_middle(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 3);

    head = insert_at(head, 2, 1);   /* [1, 2, 3] */
    assert(list_length(head)              == 3);
    assert(head->value                    == 1);
    assert(head->next->value              == 2);
    assert(head->next->next->value        == 3);
    assert(head->next->next->next         == NULL);

    list_free(&head);
    PASS("test_insert_at_middle");
}

static void test_insert_at_zero(void) {
    Node *head = NULL;
    head = insert_at(head, 99, 0);   /* behaves like insert_front on empty list */
    assert(head->value == 99);
    assert(head->next  == NULL);

    list_free(&head);
    PASS("test_insert_at_zero");
}

static void test_insert_at_beyond_end(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_at(head, 2, 100);   /* clamps to tail */

    assert(list_length(head)       == 2);
    assert(head->next->value       == 2);

    list_free(&head);
    PASS("test_insert_at_beyond_end");
}

/* ── pop_front ───────────────────────────────────────────────────────────── */

static void test_pop_front(void) {
    Node *head = NULL;
    head = insert_back(head, 10);
    head = insert_back(head, 20);
    head = insert_back(head, 30);

    int val = 0;
    int ret = pop_front(&head, &val);
    assert(ret        == 1);
    assert(val        == 10);
    assert(head->value == 20);
    assert(list_length(head) == 2);

    list_free(&head);
    PASS("test_pop_front");
}

static void test_pop_front_empty(void) {
    Node *head = NULL;
    int   val  = 0;
    int   ret  = pop_front(&head, &val);
    assert(ret  == 0);
    assert(head == NULL);
    PASS("test_pop_front_empty");
}

/* ── pop_back ────────────────────────────────────────────────────────────── */

static void test_pop_back(void) {
    Node *head = NULL;
    head = insert_back(head, 10);
    head = insert_back(head, 20);
    head = insert_back(head, 30);

    int val = 0;
    int ret = pop_back(&head, &val);
    assert(ret              == 1);
    assert(val              == 30);
    assert(list_length(head) == 2);

    list_free(&head);
    PASS("test_pop_back");
}

static void test_pop_back_single(void) {
    Node *head = insert_back(NULL, 42);
    int   val  = 0;
    int   ret  = pop_back(&head, &val);
    assert(ret  == 1);
    assert(val  == 42);
    assert(head == NULL);
    PASS("test_pop_back_single");
}

static void test_pop_back_empty(void) {
    Node *head = NULL;
    int   val  = 0;
    int   ret  = pop_back(&head, &val);
    assert(ret  == 0);
    assert(head == NULL);
    PASS("test_pop_back_empty");
}

/* ── remove_value ────────────────────────────────────────────────────────── */

static void test_remove_value_middle(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    int ret = remove_value(&head, 2);
    assert(ret              == 1);
    assert(list_length(head) == 2);
    assert(head->value       == 1);
    assert(head->next->value == 3);

    list_free(&head);
    PASS("test_remove_value_middle");
}

static void test_remove_value_head(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    int ret = remove_value(&head, 1);
    assert(ret        == 1);
    assert(head->value == 2);
    assert(list_length(head) == 2);

    list_free(&head);
    PASS("test_remove_value_head");
}

static void test_remove_value_tail(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    int ret = remove_value(&head, 3);
    assert(ret              == 1);
    assert(list_length(head) == 2);
    assert(head->next->value == 2);

    list_free(&head);
    PASS("test_remove_value_tail");
}

static void test_remove_value_not_found(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);

    int ret = remove_value(&head, 99);
    assert(ret              == 0);
    assert(list_length(head) == 2);

    list_free(&head);
    PASS("test_remove_value_not_found");
}

static void test_remove_value_empty(void) {
    Node *head = NULL;
    int   ret  = remove_value(&head, 1);
    assert(ret  == 0);
    assert(head == NULL);
    PASS("test_remove_value_empty");
}

/* ── list_search ─────────────────────────────────────────────────────────── */

static void test_list_search_found(void) {
    Node *head = NULL;
    head = insert_back(head, 10);
    head = insert_back(head, 20);
    head = insert_back(head, 30);

    Node *found = list_search(head, 20);
    assert(found         != NULL);
    assert(found->value  == 20);

    list_free(&head);
    PASS("test_list_search_found");
}

static void test_list_search_not_found(void) {
    Node *head = NULL;
    head = insert_back(head, 10);
    head = insert_back(head, 20);

    assert(list_search(head, 99) == NULL);

    list_free(&head);
    PASS("test_list_search_not_found");
}

/* ── list_contains ───────────────────────────────────────────────────────── */

static void test_list_contains(void) {
    Node *head = NULL;
    head = insert_back(head, 5);
    head = insert_back(head, 10);
    head = insert_back(head, 15);

    assert(list_contains(head, 10) == 1);
    assert(list_contains(head, 99) == 0);

    list_free(&head);
    PASS("test_list_contains");
}

/* ── list_length ─────────────────────────────────────────────────────────── */

static void test_list_length(void) {
    assert(list_length(NULL) == 0);

    Node *head = insert_back(NULL, 1);
    assert(list_length(head) == 1);

    head = insert_back(head, 2);
    head = insert_back(head, 3);
    assert(list_length(head) == 3);

    list_free(&head);
    PASS("test_list_length");
}

/* ── list_reverse ────────────────────────────────────────────────────────── */

static void test_list_reverse(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    head = list_reverse(head);

    assert(head->value                    == 3);
    assert(head->next->value              == 2);
    assert(head->next->next->value        == 1);
    assert(head->next->next->next         == NULL);

    list_free(&head);
    PASS("test_list_reverse");
}

static void test_list_reverse_single(void) {
    Node *head = insert_back(NULL, 42);
    head = list_reverse(head);
    assert(head->value == 42);
    assert(head->next  == NULL);

    list_free(&head);
    PASS("test_list_reverse_single");
}

static void test_list_reverse_empty(void) {
    assert(list_reverse(NULL) == NULL);
    PASS("test_list_reverse_empty");
}

/* ── list_free ───────────────────────────────────────────────────────────── */

static void test_list_free(void) {
    Node *head = NULL;
    head = insert_back(head, 1);
    head = insert_back(head, 2);
    head = insert_back(head, 3);

    list_free(&head);
    assert(head == NULL);
    PASS("test_list_free");
}

/* ── main ────────────────────────────────────────────────────────────────── */

int main(void) {
    printf("Running linked list tests...\n");

    test_new_node();

    test_insert_front();
    test_insert_back();
    test_insert_at_middle();
    test_insert_at_zero();
    test_insert_at_beyond_end();

    test_pop_front();
    test_pop_front_empty();
    test_pop_back();
    test_pop_back_single();
    test_pop_back_empty();

    test_remove_value_middle();
    test_remove_value_head();
    test_remove_value_tail();
    test_remove_value_not_found();
    test_remove_value_empty();

    test_list_search_found();
    test_list_search_not_found();
    test_list_contains();
    test_list_length();

    test_list_reverse();
    test_list_reverse_single();
    test_list_reverse_empty();

    test_list_free();

    printf("All tests passed.\n");
    return 0;
}
