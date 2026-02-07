/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value){
    if (tree == NULL) { // no node here
        return false;
    }

    if (tree->key == key) { // key match
        *value = &tree->content; // return stored data
        return true;
    }

    if (key < tree->key) { // go left side
        return bst_search(tree->left, key, value);
    } else { // go right sid
        return bst_search(tree->right, key, value);
    }
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
bool bst_insert(bst_node_t **tree, char key, bst_node_content_t value){
    if (*tree == NULL) { // insert new node here
        bst_node_t *new = malloc(sizeof(bst_node_t));
        if (!new) { // alloc fail
            return false;
        }

        new->key = key;
        new->content = value; // store content
        new->left = NULL;
        new->right = NULL;

        *tree = new; // link new node
        return true;
    }

    if (key < (*tree)->key) { // recurse left
        return bst_insert(&(*tree)->left, key, value);

    } else if (key > (*tree)->key) { // recurse right
        return bst_insert(&(*tree)->right, key, value);

    } else {
        // updte existing
        if ((*tree)->content.value != NULL) {
            free((*tree)->content.value); // remove old val
        }
        (*tree)->content = value; // set new data
        return true;
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree){
    if (*tree == NULL) { // nothing here
        return;
    }

    if ((*tree)->right != NULL) { // still go right
        bst_replace_by_rightmost(target, &(*tree)->right);

    } else {
        bst_node_t *rightmost = *tree; // found last right

        if (target->content.value != NULL) {
            free(target->content.value); // drop old data
        }

        target->key = rightmost->key; // copy key
        target->content = rightmost->content; // copy valu

        *tree = rightmost->left; // shift subtree
        free(rightmost); // free node
    }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
    if (*tree == NULL) { // nothing to del
        return;
    }

    if (key < (*tree)->key) { // go left
        bst_delete(&(*tree)->left, key);

    } else if (key > (*tree)->key) { // go right
        bst_delete(&(*tree)->right, key);

    } else {
        bst_node_t *helper = *tree; // node to remove

        if (helper->left == NULL && helper->right == NULL) { // leaf
            if (helper->content.value) free(helper->content.value);
            free(helper);
            *tree = NULL;

        } else if (helper->left == NULL) { // only right child
            *tree = helper->right;
            if (helper->content.value) free(helper->content.value);
            free(helper);

        } else if (helper->right == NULL) { // only left child
            *tree = helper->left;
            if (helper->content.value) free(helper->content.value);
            free(helper);

        } else {
            // both subtrees exist
            bst_replace_by_rightmost(helper, &helper->left);
        }
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree){
    if (tree == NULL || *tree == NULL) { // done
        return;
    }

    bst_dispose(&(*tree)->left); // clear left
    bst_dispose(&(*tree)->right); // clear right

    if ((*tree)->content.value != NULL) {
        free((*tree)->content.value); // free data
    }

    free(*tree); // fre node
    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree == NULL) { // no node
        return;
    }
    bst_add_node_to_items(tree, items); // visit
    bst_preorder(tree->left, items); // left
    bst_preorder(tree->right, items); // rigt
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree == NULL) { // empty
        return;
    }
    bst_inorder(tree->left,  items); // left
    bst_add_node_to_items(tree, items); // visit
    bst_inorder(tree->right, items); // right
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
    if (tree == NULL) { // nothing
        return;
    }
    bst_postorder(tree->left, items); // left
    bst_postorder(tree->right, items); // right
    bst_add_node_to_items(tree, items); // visit last
}
