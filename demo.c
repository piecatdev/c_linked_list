/* demo.c
 *
 * Interactive walkthrough of every linked-list operation.
 * Compile and run:
 *
 *   make demo
 *   ./demo
 */

#include <stdio.h>
#include "linked_list.h"

int main(void) {
    Node *head = NULL;
    int   val;

    /* insert_back — order must be preserved */
    head = insert_back(head, 30);
    head = insert_back(head, 60);
    head = insert_back(head, 90);
    printf("insert_back:     ");
    list_print(head);   /* expected: 30 -> 60 -> 90 -> NULL */

    /* insert_front — new elements go to the front */
    head = insert_front(head, 5);
    head = insert_front(head, 1);
    printf("insert_front:    ");
    list_print(head);   /* expected: 1 -> 5 -> 30 -> 60 -> 90 -> NULL */

    /* insert_at — insert 20 at index 2 */
    head = insert_at(head, 20, 2);
    printf("insert_at(2,20): ");
    list_print(head);   /* expected: 1 -> 5 -> 20 -> 30 -> 60 -> 90 -> NULL */

    /* pop_front */
    pop_front(&head, &val);
    printf("pop_front --> %d\n", val);   /* expected: 1 */
    printf("after pop_front: ");
    list_print(head);   /* expected: 5 -> 20 -> 30 -> 60 -> 90 -> NULL */

    /* pop_back */
    pop_back(&head, &val);
    printf("pop_back  --> %d\n", val);   /* expected: 90 */
    printf("after pop_back:  ");
    list_print(head);   /* expected: 5 -> 20 -> 30 -> 60 -> NULL */

    /* remove_value */
    remove_value(&head, 30);
    printf("remove(30):      ");
    list_print(head);   /* expected: 5 -> 20 -> 60 -> NULL */

    /* list_search / list_contains */
    Node *found = list_search(head, 20);
    printf("search(20):      %s (value=%d)\n",
           found ? "found" : "not found",
           found ? found->value : -1);   /* expected: found, value=20 */

    printf("contains(99):    %s\n",
           list_contains(head, 99) ? "yes" : "no");   /* expected: no */

    /* list_length */
    printf("length:          %d\n", list_length(head));   /* expected: 3 */

    /* list_reverse */
    head = list_reverse(head);
    printf("reversed:        ");
    list_print(head);   /* expected: 60 -> 20 -> 5 -> NULL */

    /* list_free */
    list_free(&head);
    printf("after free:      ");
    list_print(head);   /* expected: NULL */

    return 0;
}
