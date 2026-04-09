# c-linked-list

A singly-linked list library written in C99.  
13 operations, a runnable demo, and a 24-test unit suite — zero external dependencies.

---

## Features

- [x] **13 operations** — prepend, append, insert at index, pop front/back, remove by value, search, membership test, length, in-place reverse, pretty-print, safe memory release
- [x] **Recursive algorithms** — `insert_back` and `list_length` are implemented recursively, mapping directly onto the mathematical definition of a list
- [x] **O(1) space reversal** — `list_reverse` relinks pointers in-place using three local variables, no auxiliary memory needed
- [x] **Memory-safe** — `list_free` frees every node *and* nulls the caller's pointer to prevent use-after-free bugs
- [x] **24 unit tests** — pure `<assert.h>`, no external frameworks
- [x] **CI via GitHub Actions** — build + test on every push

---

## Project layout

```
c-linked-list/
├── linked_list.h          # Public interface — struct definition and all declarations
├── linked_list.c          # Implementation of all 13 operations
├── demo.c                 # Interactive walkthrough of every function
├── tests/
│   └── test_linked_list.c # 24 unit tests (assert-based, no external deps)
├── Makefile               # Targets: all · demo · test · clean
├── .github/workflows/
│   └── ci.yml             # GitHub Actions: build + test on ubuntu-latest
└── LICENSE
```

---

## Build & Run

**Prerequisites:** GCC ≥ 7 and GNU Make.

```bash
# Ubuntu / Debian
sudo apt-get install gcc make

# macOS (Homebrew)
brew install gcc make
```

```bash
git clone https://github.com/piecatdev/c-linked-list.git
cd c-linked-list
```

**Run the interactive demo**

```bash
make demo
./demo
```

Expected output:

```
insert_back:     30 (0x...) -> 60 (0x...) -> 90 (0x...) -> NULL
insert_front:    1 (0x...)  -> 5 (0x...)  -> 30 (0x...) -> 60 (0x...) -> 90 (0x...) -> NULL
insert_at(2,20): 1 (0x...)  -> 5 (0x...)  -> 20 (0x...) -> 30 (0x...) -> 60 (0x...) -> 90 (0x...) -> NULL
pop_front --> 1
after pop_front: 5 (0x...)  -> 20 (0x...) -> 30 (0x...) -> 60 (0x...) -> 90 (0x...) -> NULL
pop_back  --> 90
after pop_back:  5 (0x...)  -> 20 (0x...) -> 30 (0x...) -> 60 (0x...) -> NULL
remove(30):      5 (0x...)  -> 20 (0x...) -> 60 (0x...) -> NULL
search(20):      found (value=20)
contains(99):    no
length:          3
reversed:        60 (0x...) -> 20 (0x...) -> 5 (0x...)  -> NULL
after free:      NULL
```

**Run the test suite**

```bash
make test
```

Expected output:

```
Running linked list tests...
  PASS  test_new_node
  PASS  test_insert_front
  PASS  test_insert_back
  PASS  test_insert_at_middle
  PASS  test_insert_at_zero
  PASS  test_insert_at_beyond_end
  PASS  test_pop_front
  PASS  test_pop_front_empty
  PASS  test_pop_back
  PASS  test_pop_back_single
  PASS  test_pop_back_empty
  PASS  test_remove_value_middle
  PASS  test_remove_value_head
  PASS  test_remove_value_tail
  PASS  test_remove_value_not_found
  PASS  test_remove_value_empty
  PASS  test_list_search_found
  PASS  test_list_search_not_found
  PASS  test_list_contains
  PASS  test_list_length
  PASS  test_list_reverse
  PASS  test_list_reverse_single
  PASS  test_list_reverse_empty
  PASS  test_list_free
All tests passed.
```

**Clean build artefacts**

```bash
make clean
```

---

## API reference

| Function | Signature | Returns | Complexity |
|---|---|---|---|
| `new_node` | `Node *new_node(int value)` | New node or `NULL` on failure | O(1) |
| `insert_front` | `Node *insert_front(Node *head, int value)` | New head | O(1) |
| `insert_back` | `Node *insert_back(Node *head, int value)` | Head (unchanged) | O(n) |
| `insert_at` | `Node *insert_at(Node *head, int value, int index)` | New head if index == 0 | O(n) |
| `pop_front` | `int pop_front(Node **head, int *out)` | 1 success · 0 empty | O(1) |
| `pop_back` | `int pop_back(Node **head, int *out)` | 1 success · 0 empty | O(n) |
| `remove_value` | `int remove_value(Node **head, int value)` | 1 removed · 0 not found | O(n) |
| `list_search` | `Node *list_search(Node *head, int value)` | Node ptr or `NULL` | O(n) |
| `list_contains` | `int list_contains(Node *head, int value)` | 1 or 0 | O(n) |
| `list_length` | `int list_length(Node *head)` | Node count | O(n) |
| `list_reverse` | `Node *list_reverse(Node *head)` | New head | O(n) time · O(1) space |
| `list_print` | `void list_print(Node *head)` | — | O(n) |
| `list_free` | `void list_free(Node **head)` | — sets `*head = NULL` | O(n) |

> `insert_at` with `index == 0` behaves like `insert_front`.  
> `insert_at` with `index >= length` appends at the tail.

---

## Usage example

```c
#include "linked_list.h"

int main(void) {
    Node *list = NULL;

    /* build: 5 -> 10 -> 20 */
    list = insert_back(list, 10);
    list = insert_back(list, 20);
    list = insert_front(list, 5);
    list_print(list);           /* 5 -> 10 -> 20 -> NULL */

    /* insert 15 between 10 and 20 */
    list = insert_at(list, 15, 2);
    list_print(list);           /* 5 -> 10 -> 15 -> 20 -> NULL */

    /* reverse in-place */
    list = list_reverse(list);
    list_print(list);           /* 20 -> 15 -> 10 -> 5 -> NULL */

    /* search */
    Node *found = list_search(list, 15);
    if (found) printf("found: %d\n", found->value);

    /* always free when done */
    list_free(&list);           /* list == NULL after this */
    return 0;
}
```

---

## Implementation notes

### Recursive `insert_back` and `list_length`

Both functions follow the same structural recursion pattern:

```
f(NULL)  = base case
f(node)  = combine(node, f(node->next))
```

This mirrors the mathematical definition of a linked list — either empty, or a head node followed by a smaller list. The recursive version reads like the definition itself.

### Three-pointer reversal in `list_reverse`

In-place reversal requires tracking three positions simultaneously: the node already reversed (`prev`), the node being relinked (`current`), and the remainder of the list (`next`) — saved before the pointer is overwritten. One pass, zero allocations.

```
Initial:  prev=NULL  current=A  (list: A→B→C→NULL)

step 1:  save next=B, set A→NULL,  prev=A,  current=B
step 2:  save next=C, set B→A,     prev=B,  current=C
step 3:  save next=NULL, set C→B,  prev=C,  current=NULL  ← new head
```

---

## License

Released under the [MIT License](LICENSE).
