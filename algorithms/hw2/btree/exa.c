/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je
 * ukládán průběžně do stromu (klíč vždy lowercase). V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání
 * operace insert) vrací funkce false.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

// 1) Pomocná funkcia – transformuje JEDEN znak
char input_transform(char c) {
    if (c >= 'A' && c <= 'Z') {
        c += 32; // make lowercase
    } else if (c == ' ' || (c >= 'a' && c <= 'z')) {
        // ok chars stay same
    }
    else {
        c = '_'; // everything else go under score :)
    }
    return c; // return changed letter
}


bool letter_count(bst_node_t **tree, char* input) {

    //input_transform(input); // not used now, per char handled
    bst_init(tree); // init empty tree

    while (*input) { // loop thru string
        char letter = input_transform(*input); // normalize char

        bst_node_content_t* val=NULL;
        if(bst_search(*tree, letter, &val)){ // found exist
            int* count=(int*)val->value;
            (*count)++; // increment freq
            input++; // move next char
            continue; // skip insert
        }

        bst_node_content_t new_val;
        int *count = malloc(sizeof(int)); // alloc new counter
        if (!count) {
            bst_dispose(tree); // cleanup all
            return false; // alloc fail
        }
        *count = 1; // first occurrence

        new_val.value=count; // store ptr
        new_val.type=INTEGER; // set type

        if(!bst_insert(tree, letter, new_val)){ // insert new node
            bst_dispose(tree); // free whole tree
            free(count);
            return false;
        }
        input++; // move on
    }

    return true; // all good
}
