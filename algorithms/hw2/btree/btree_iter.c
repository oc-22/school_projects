/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree){
    *tree = NULL; // reset root ptr
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
    while(tree){ // iterate nodes
        if(tree->key == key){ // found match
            *value = &tree->content; // return content ptr
            return true;
        }
        tree = tree->key > key ? tree->left : tree->right; // go next sid
    }
    return false; // not found
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel. V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání
 * alokace) vrací funkce false.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
bool bst_insert(bst_node_t **tree, char key, bst_node_content_t value){
    bst_node_t* prev = NULL; // parent ptr
    bst_node_t* curr = *tree; // current node

    while (curr!=NULL) { // find place
        if (curr->key==key) { // update existing
            if (curr->content.value!=NULL) {
                free(curr->content.value); // free old val
            }
            curr->content=value; // set new data
            return true;
        }
        prev= curr; // store parent
        if (key<curr->key) {
            curr=curr->left; // go left
        }else{
            curr=curr->right; // go right
        }
    }

    bst_node_t* new= malloc(sizeof(bst_node_t)); // alloc node
    if (new==NULL) {
        return false; // alloc fail
    }
    new->key=key;
    new->content=value; // store content
    new->left=NULL;
    new->right=NULL;

    if (prev==NULL) { // tree was empty
        *tree= new;
    }else if (key<prev->key) {
        prev->left=new; // link left
    }else{
        prev->right=new; // link right
    }

    return true;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree){
    bst_node_t* rightmost = *tree; // start here
    bst_node_t* prev = NULL; // parent ptr

    while (rightmost->right!=NULL) { // move right
        prev = rightmost;
        rightmost = rightmost->right;
    }

    if (target->content.value!=NULL) {
        free(target->content.value); // free old data
    }

    target->key = rightmost->key; // copy key
    target->content = rightmost->content; // copy value

    if (prev==NULL) {
        *tree=rightmost->left; // shift subtree
    }else{
        prev->right=rightmost->left; // unlink rm node
    }

    free(rightmost); // free node
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key){
    bst_node_t* curr = *tree; // search node
    bst_node_t* prev = NULL; // track parent

    while (curr!=NULL && curr->key!=key) { // find key
        prev = curr;
        if (key<curr->key) {
            curr=curr->left; // go left
        }else{
            curr=curr->right; // go right
        }
    }

    if (curr==NULL) {
        return; // nothing to del
    }

    if (curr->left==NULL && curr->right==NULL) { // leaf node
        if (prev==NULL) {
            *tree=NULL; // root removed
        }else if(prev->left==curr) {
            prev->left=NULL;
        }else {
            prev->right=NULL;
        }
        if (curr->content.value!=NULL) {
            free(curr->content.value);
        }
        free(curr);
        return;
    }

    if ((curr->left==NULL && curr->right!=NULL) ||
        (curr->left!=NULL && curr->right==NULL)) { // one child
        bst_node_t* link = curr->left!=NULL ? curr->left : curr->right;

        if (prev==NULL) {
            *tree=link; // replace root
        }else if(prev->right==curr){
            prev->right=link;
        }else{
            prev->left=link;
        }

        if (curr->content.value!=NULL) {
            free(curr->content.value);
        }
        free(curr);
        return;
    }

    bst_replace_by_rightmost(curr, &curr->left); // two childs
    return;
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree){
    if (tree==NULL || *tree==NULL) {
        return; // nothing
    }

    stack_bst_t stack;
    stack_bst_init(&stack); // init stack

    stack_bst_push(&stack, *tree); // push root
    while (stack_bst_empty(&stack)==false) {
        bst_node_t* curr = stack_bst_pop(&stack);

        if (curr->left!=NULL) {
            stack_bst_push(&stack, curr->left); // push left
        }
        if (curr->right!=NULL) {
            stack_bst_push(&stack, curr->right); // push right
        }

        if (curr->content.value!=NULL) {
            free(curr->content.value); // free val
        }

        free(curr); // free node
    }

    *tree = NULL; // reset
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items){
    bst_node_t* curr = tree;
    while (curr!=NULL) { // go left chain
        bst_add_node_to_items(curr, items); // visit
        stack_bst_push(to_visit, curr); // push node
        curr=curr->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items){
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_preorder(tree, &stack, items); // start left
    while (stack_bst_empty(&stack)!=true) {
        bst_node_t* curr = stack_bst_pop(&stack); // pop
        bst_leftmost_preorder(curr->right, &stack, items); // go right
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit){
    bst_node_t* curr = tree;
    while (curr!=NULL) { // push path left
        stack_bst_push(to_visit, curr);
        curr=curr->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items){
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_inorder(tree, &stack);
    while (stack_bst_empty(&stack)!=true) {
        bst_node_t* curr = stack_bst_pop(&stack);
        bst_add_node_to_items(curr, items); // visit node
        bst_leftmost_inorder(curr->right, &stack); // right subtree
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
    bst_node_t* curr = tree;
    while (curr!=NULL) { // left chain
        stack_bst_push(to_visit, curr);
        stack_bool_push(first_visit, true); // mark first
        curr=curr->left;
    }
}


/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items){
    stack_bst_t to_visit;
    stack_bool_t first_visit;

    stack_bst_init(&to_visit);
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &to_visit, &first_visit);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t* curr = stack_bst_pop(&to_visit);
        bool first = stack_bool_pop(&first_visit);

        if (curr==NULL) {
            continue; // skip
        }

        if (first==true) { // first time seen
            stack_bst_push(&to_visit,curr);
            stack_bool_push(&first_visit, false);

            bst_leftmost_postorder(curr->right, &to_visit, &first_visit);
        }else{
            bst_add_node_to_items(curr, items); // visit now
        }
    }
}
