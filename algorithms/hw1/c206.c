/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->activeElement=NULL;
    list->firstElement=NULL;
    list->lastElement=NULL;
    list->currentLength=0;
    //solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr tmp;      //temporary element that we are gonna be freeing and relinking

    while (list->firstElement!=NULL) {
        tmp=list->firstElement;
        list->firstElement=tmp->nextElement;        //relink the list
        free(tmp);  //free the element
    }

    //set values as if the list was first initialized
    list->lastElement=NULL;
    list->firstElement=NULL;
    list->currentLength=0;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
    DLLElementPtr tmp = malloc(sizeof(struct DLLElement));  //allocate heap memory for the new element
    if (tmp==NULL) {    //allocation check
        DLL_Error();
        return;
    }
    tmp->data=data;     //set data

    //linking
    tmp->previousElement=NULL;
    tmp->nextElement=list->firstElement;

    if (list->firstElement!=NULL) { //if the wasnt empty relink the element
        list->firstElement->previousElement=tmp;
    }
    else{
        list->lastElement=tmp;      //if it was empty its also the last element
    }
    list->firstElement=tmp;     //the insertion itself
    list->currentLength++;      //increment the lenght
    //solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, long data ) {
	//solved = false; /* V případě řešení, smažte tento řádek! */
	DLLElementPtr tmp = malloc(sizeof(struct DLLElement));
    if (tmp==NULL) {
        DLL_Error();
        return;
    }
    tmp->data=data;
    tmp->nextElement=NULL;
    tmp->previousElement=list->lastElement; //link the element to the last place on the list

    if (list->lastElement!=NULL) {  //if the list wasnt empty make the previous last element previous
        list->lastElement->nextElement=tmp;
    }
    else{
        list->firstElement=tmp;     //if its empty its also the first element
    }
    list->lastElement=tmp;  //insertion
    list->currentLength++;  //incrementation
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement=list->firstElement;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement=list->lastElement;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
    if (list->firstElement==NULL) { //check if the list is not empty
        DLL_Error();
        return;
    }
    *dataPtr=list->firstElement->data;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
    if (list->lastElement==NULL) {
        DLL_Error();
        return;
    }
    *dataPtr=list->lastElement->data;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    if (list->firstElement==NULL) { //chceck if the list was not empty
        return;
    }
    DLLElementPtr tmp=list->firstElement;   //set the temporary element to first one, the tmp is gonna be deleted later
    if (list->firstElement==list->activeElement) {  //if it was active, take activity away
        list->activeElement=NULL;
    }
    list->firstElement=tmp->nextElement;    //re-set the first element of the list
    if (list->firstElement!=NULL) {
        list->firstElement->previousElement=NULL;
    }
    else{
        list->lastElement=NULL;
    }
    free(tmp);   //free the element itself and decrement the lenght of the list
    list->currentLength--;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    if (list->lastElement==NULL) {
        return;
    }
    DLLElementPtr tmp=list->lastElement;
    if (list->lastElement==list->activeElement) {
        list->activeElement=NULL;
    }
    list->lastElement=tmp->previousElement; //relink the last element
    if (list->lastElement!=NULL) {
        list->lastElement->nextElement=NULL;
    }
    else{
        list->firstElement=NULL;
    }
    free(tmp);
    list->currentLength--;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    if (list->activeElement==list->lastElement || list->activeElement==NULL) {  //check if some element actually is active
        return;
    }

    DLLElementPtr tmp = list->activeElement->nextElement;   //set the tmp to the next element after the active one
    list->activeElement->nextElement=tmp->nextElement;  //relink the list
    if (tmp==list->lastElement) {       //check the "active was one before the last" edge case
        list->lastElement=list->activeElement;
    }
    else{
        tmp->nextElement->previousElement=list->activeElement;  //relink the previous pointer of the element after the new one
    }
    free(tmp);
    list->currentLength--;

	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if (list->activeElement==NULL || list->activeElement==list->firstElement) {
        return;
    }
    DLLElementPtr tmp = list->activeElement->previousElement;   //set the tmp to the element to be freed
    list->activeElement->previousElement=tmp->previousElement;  //relinking
    if (tmp==list->firstElement) {  //reset the first element if thats the case
        list->firstElement=list->activeElement;
    }
    else{
        tmp->previousElement->nextElement=list->activeElement;  //relink
    }
    free(tmp);
    list->currentLength--;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, long data ) {
    if (list->activeElement==NULL) {        //check if we actually have some active element
        return;
    }
    DLLElementPtr tmp = malloc(sizeof(struct DLLElement));  //allocate the new element
    if (tmp==NULL) {    //malloc check
        DLL_Error();
        return;
    }
    tmp->data=data; //set the data

    //relink
    tmp->nextElement=list->activeElement->nextElement;
    tmp->previousElement=list->activeElement;
    list->activeElement->nextElement=tmp;

    if (list->activeElement==list->lastElement) {   //check last element edge case
        list->lastElement=tmp;
    }
    else{
        tmp->nextElement->previousElement=tmp;  //just relink the next elements previous pointer
    }
    list->currentLength++;  //increment length
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, long data ) {
    if (list->activeElement==NULL) {    //check if we have an active element
        return;
    }
    DLLElementPtr tmp = malloc(sizeof(struct DLLElement));
    if (tmp==NULL) {    //malloc check
        DLL_Error();
        return;
    }

    //relink
    tmp->data=data;
    tmp->nextElement=list->activeElement;
    tmp->previousElement=list->activeElement->previousElement;

    if (list->activeElement->previousElement!=NULL) {   //if its( the new elements space ) not the first element, relink in a regular fashion
        list->activeElement->previousElement->nextElement=tmp;
    }
    else{
        list->firstElement=tmp; //if it is, set it as the first one too
    }
    list->activeElement->previousElement=tmp;
    list->currentLength++;
	// solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
    if (list->activeElement==NULL) {    //check if we have an active element
        DLL_Error();
        return;
    }
    *dataPtr=list->activeElement->data; //return the active ones data

	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
    if (list->activeElement==NULL) { //check if we have an active element
        return;
    }
    list->activeElement->data=data; //set the active ones data
    return;
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if (list->activeElement==NULL) {    //check if we have an active element
        return;
    }
    if (list->activeElement==list->lastElement) {   //check if its not the last one, if it is, remove activity from the list altogether
        list->activeElement=NULL;
        return;
    }
    list->activeElement=list->activeElement->nextElement;   //set the new activity
	//solved = false; /* V případě řešení, smažte tento řádek! */
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if (list->activeElement==NULL) {    //check if we have an active element
        return;
    }
    if (list->activeElement==list->firstElement) {  //check if its not the first one, if it is, remove activity from the list altogether
        list->activeElement=NULL;
        return;
    }
    list->activeElement=list->activeElement->previousElement;   //set the new activity
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	//solved = false; /* V případě řešení, smažte tento řádek! */
	return list->activeElement==NULL?false:true;    //check the active elements presence
}

/* Konec c206.c */
