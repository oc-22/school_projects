/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE; // global size for table (can be smaller later)

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    for (unsigned i=0; i<HT_SIZE; i++) {
        (*table)[i] = NULL; // every bucket empty
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    int seek_idx = get_hash(key); // compute index
    ht_item_t* seek = (*table)[seek_idx]; // first item in chain
    while (seek!=NULL) { // walk list
        if (strcmp(key, seek->key)==0) { // key match
            return seek; // found yey
        }
        seek = seek->next; // go next in synonmys
    }
    return NULL; // not found
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 * V případě úspěchu funkce tuto skutečnost indikuje návratovou hodnotou
 * true, v opačném případě (např. při selhání alokace) vrací funkce false.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
bool ht_insert(ht_table_t *table, char *key, float value) {
    int new_idx = get_hash(key); // index for insert
    ht_item_t* seek = ht_search(table, key);
    if (seek!=NULL) { // already exists
        seek->value=value; // overwrite val
        return true;
    }

    ht_item_t* new = malloc(sizeof(ht_item_t)); // create new item
    if (new==NULL) {
        return false; // no memory
    }
    new->value = value;
    new->key= key;
    new->next=(*table)[new_idx]; // insert at front
    (*table)[new_idx] = new; // new head
    return true;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t* seek = ht_search(table, key);
    if (!seek) {
        return NULL; // no such
    }
    return &seek->value; // return ptr to float
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int del_idx = get_hash(key);

    ht_item_t* curr = (*table)[del_idx];
    ht_item_t* prev = NULL;
    if (curr==NULL) {
        return; // empty bucket
    }
    while (curr!=NULL) { // walk chain
        if (strcmp(curr->key, key)==0) { // match found
            if (prev!=NULL) {
                prev->next=curr->next; // unlink mid
            }else{
                (*table)[del_idx] = curr->next; // deleting head
            }
            free(curr); // free item
            return;
        }
        prev=curr;
        curr = curr->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    for (unsigned idx=0; idx<HT_SIZE; idx++) {
        ht_item_t* curr =(*table)[idx]; // grab bucket
        while (curr!=NULL) {
            ht_item_t* next = curr->next; // save next
            free(curr); // free item
            curr=next; // move on
        }
        (*table)[idx] = NULL; // reset bucket
    }
}
