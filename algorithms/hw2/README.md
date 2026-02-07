# Data Structures Project – Binary Search Trees & Hash Table

This repository is divided into two independent modules:

btree/       # Binary Search Tree (iterative + recursive) + letter_count
hashtable/   # Hash table with chaining

Code

Each module is self‑contained and implements a classic data structure used in the IAL course (FIT VUT Brno).

---

#Folder: `btree/`

This folder contains:

- **btree.h** – public interface shared by both BST implementations  
- **btree_iterative.c** – iterative binary search tree  
- **btree_recursive.c** – recursive binary search tree  
- **bst_helpers.c** – printing and traversal helpers  
- **letter_count.c** – character frequency counter using the BST  

##Binary Search Tree (BST)

Both the iterative and recursive versions implement the same API:

### Core operations
- `bst_init` — initialize an empty tree  
- `bst_insert` — insert or update a node  
- `bst_search` — search for a key  
- `bst_delete` — delete a node  
- `bst_dispose` — free the entire tree  

### Traversals
- `bst_preorder`  
- `bst_inorder`  
- `bst_postorder`  

Traversal results are stored in a dynamic array (`bst_items_t`).

### Helper utilities
Provided in `bst_helpers.c`:
- `bst_print_node`
- `bst_print_node_content`
- `bst_add_node_to_items`

### Replacement helper
- `bst_replace_by_rightmost`  
  Used during deletion when a node has two children.

---

##Letter Frequency Counter (`letter_count.c`)

This module uses the BST to compute character frequencies in a string.

### Character rules
- `A–Z` → converted to lowercase  
- `a–z` and `' '` → counted directly  
- all other characters → grouped under `'_'`  

### Example

Input:
"abBccc_ 123 *"

### Notes
- Values are stored as dynamically allocated integers (`INTEGER` type).  
- On allocation failure, the function frees the tree and returns `false`.  
- The BST may become unbalanced (worst case: a linked list).  

---

#Folder: `hashtable/`

This folder contains:

- **hashtable.h** – public interface  
- **hashtable.c** – hash table implementation with chaining  

## Hash Table with Explicit Chaining

### Supported operations
- `ht_init` — initialize table  
- `ht_search` — find an item  
- `ht_insert` — insert or update  
- `ht_get` — return pointer to value  
- `ht_delete` — remove a single item  
- `ht_delete_all` — clear entire table  

### Hash function
A simple additive hash:

###Collision handling

Collisions are resolved using linked lists.
New items are inserted at the beginning of the chain for efficiency.
