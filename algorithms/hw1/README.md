# IAL – Abstract Data Type Implementations (c202, c204, c206)

This repository contains three independent assignments from the course  
**IAL – Algorithms (FIT VUT Brno)**.  
Each task implements a different abstract data type or algorithm, and some tasks build on each other.

## Contents
- [c202 – Stack of Characters](#c202--stack-of-characters)
- [c204 – Infix → Postfix Conversion & Expression Evaluation](#c204--infix--postfix-conversion--expression-evaluation)
- [c206 – Doubly Linked List](#c206--doubly-linked-list)
- [Compilation](#compilation)
- [Notes](#notes)

---

## c202 – Stack of Characters

File: **`c202.c`**

Implements an abstract data type **Stack**, stored in a dynamically allocated array.  
The stack uses an integer `topIndex` to track the top element:

- `topIndex == -1` → empty stack  
- `topIndex == 0` → one element  
- `topIndex == STACK_SIZE - 1` → full stack  

### Implemented operations
- `Stack_Init` – initialize the stack  
- `Stack_IsEmpty` – check if empty  
- `Stack_IsFull` – check if full  
- `Stack_Top` – read the top element  
- `Stack_Pop` – remove the top element  
- `Stack_Push` – push a new element  
- `Stack_Dispose` – free allocated memory  

This stack is later used in **c204** for expression conversion and evaluation.

---

## c204 – Infix → Postfix Conversion & Expression Evaluation

File: **`c204.c`**

This task implements:

1. **Conversion of an infix mathematical expression to postfix (RPN)**  
2. **Evaluation of the resulting postfix expression**, including variables

The implementation uses the stack from **c202**.

### Helper functions
- `untilLeftPar`  
  Removes operators from the stack until a left parenthesis is found.

- `doOperation`  
  Handles operator precedence (`+ - * /`) and moves operators to the postfix output.

### Main conversion function
- `infix2postfix`  
  - Reads an infix expression  
  - Produces a postfix expression ending with `=`  
  - Supports digits, letters, parentheses, and binary operators  
  - Allocates memory for the output string  

### Integer handling on a char-based stack
Because the stack stores only `char`, integers are pushed/popped byte‑by‑byte:

- `expr_value_push` – splits a 4‑byte integer into 4 chars  
- `expr_value_pop` – reconstructs the integer from 4 chars  

### Expression evaluation
- `eval`  
  - Converts infix → postfix  
  - Evaluates the postfix expression  
  - Uses provided variable values  
  - Returns the final integer result  

---

## c206 – Doubly Linked List

File: **`c206.c`**

Implements an abstract data type **DLList** — a doubly linked list with an **active element**.

Each list node contains:
- `data` – integer value  
- `previousElement`  
- `nextElement`  

The list structure stores:
- `firstElement`  
- `lastElement`  
- `activeElement`  
- `currentLength`  

### Implemented operations

#### Initialization & disposal
- `DLL_Init`  
- `DLL_Dispose`

#### Insertion
- `DLL_InsertFirst`  
- `DLL_InsertLast`  
- `DLL_InsertAfter`  
- `DLL_InsertBefore`

#### Deletion
- `DLL_DeleteFirst`  
- `DLL_DeleteLast`  
- `DLL_DeleteAfter`  
- `DLL_DeleteBefore`

#### Activity control
- `DLL_First`  
- `DLL_Last`  
- `DLL_Next`  
- `DLL_Previous`  
- `DLL_IsActive`

#### Value access
- `DLL_GetFirst`  
- `DLL_GetLast`  
- `DLL_GetValue`  
- `DLL_SetValue`

---

## Compilation

Each task can be compiled independently:

```bash
gcc c202.c -o c202
gcc c204.c c202.c -o c204
gcc c206.c -o c206
