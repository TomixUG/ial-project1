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
	// List is empty, put everything to NULL
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;

	list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	// loop over every element and free it
	DLLElementPtr element = list->firstElement;
	while(element != NULL){
		// free every element
		DLLElementPtr nextEl = element->nextElement;
		free(element);
		element = nextEl;
	}

	// set pointers to NULL
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
	list->currentLength = 0;
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
	// try to allocate newElement
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
	if(newElement == NULL){
		DLL_Error();
		return;
	}

	// fill it with data
	newElement->data = data;
	newElement->nextElement = list->firstElement; // the next element is the previously first one
	newElement->previousElement = NULL;

	// if the first element is NULL
	if(list->firstElement == NULL){
		// the list is empty
		list->lastElement = newElement; // it's the only element, so it's last and first
	} else {
		list->firstElement->previousElement = newElement;
	}

	list->firstElement = newElement;

	// increment list length
	list->currentLength++;
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
	// try to allocate newElement
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
	if(newElement == NULL){
		DLL_Error();
		return;
	}

	// fill it with data
	newElement->data = data;
	newElement->nextElement = NULL; // the next element is the previously first one
	newElement->previousElement = list->lastElement;

	if(list->lastElement == NULL){
		// the list is empty
		list->firstElement = newElement;
	} else {
		list->lastElement->nextElement = newElement;
	}

	list->lastElement = newElement; // ukazatel na konec

	// increment list length
	list->currentLength++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	// check if list is empty
	if(list->firstElement == NULL){
		DLL_Error();
		return;
	}

	// return data field of the first element
	*dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	// check if list is empty
	if(list->lastElement == NULL){
		DLL_Error();
		return;
	}

	// return data field of the last element
	*dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	// check if there the list is empty
	if(list->firstElement == NULL){
		return;
	}

	DLLElementPtr firstElem = list->firstElement;

	if(list->firstElement == list->activeElement){
		// the first element was active, make in inactive
		list->activeElement = NULL;
	}

	// check if the list had only one entry
	if(list->firstElement == list->lastElement){
		list->firstElement = NULL;
		list->lastElement = NULL;
	} else {
		firstElem->nextElement->previousElement = NULL;
		list->firstElement = list->firstElement->nextElement; // update firstElement
	}

	// free the element
	free(firstElem);

	// decrement list length
	list->currentLength--;
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	// check if there the list is empty
	if(list->firstElement == NULL){
		return;
	}

	DLLElementPtr lastElem = list->lastElement;

	if(list->lastElement == list->activeElement){
		// the last element was active, make in inactive
		list->activeElement = NULL;
	}

	// check if the list had only one entry
	if(list->firstElement == list->lastElement){
		list->firstElement = NULL;
		list->lastElement = NULL;
	} else {
		lastElem->previousElement->nextElement = NULL;
		list->lastElement = list->lastElement->previousElement; // update firstElement
	}

	// free the element
	free(lastElem);

	// decrement list length
	list->currentLength--;
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	// if the list isn't active or the active is the last one
	if(list->activeElement == NULL || list->activeElement->nextElement == NULL) {
		return;
	}

	// FIXME: PROBABLY WRONG

	DLLElementPtr toDelete = list->activeElement->nextElement;

	if(toDelete->nextElement == NULL) {
		// if toDelete is the last element
		list->activeElement->nextElement = NULL; // the active will now be the last one
		list->lastElement = list->activeElement; // last is now the active one
	} else {
		// toDelete is not the last one
		list->activeElement->nextElement = toDelete->nextElement;
		toDelete->nextElement->previousElement = list->activeElement;
	}

	free(toDelete);

	// decrement list length
	list->currentLength--;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	// if the list isn't active or the active is the last one
	if(list->activeElement == NULL || list->activeElement->previousElement == NULL) {
		return;
	}

	// FIXME: PROBABLY WRONG

	DLLElementPtr toDelete = list->activeElement->previousElement;

	if(toDelete->previousElement == NULL) {
		// if toDelete is the first element
		list->activeElement->previousElement = NULL; // the active will now be the last one
		list->firstElement = list->activeElement; // last is now the active one
	} else {
		// toDelete is not the last one
		list->activeElement->previousElement = toDelete->previousElement;
		toDelete->previousElement->nextElement = list->activeElement;
	}

	free(toDelete);

	// decrement list length
	list->currentLength--;
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
	// if there is no active element
	if(list->activeElement == NULL){
		return;
	}

	// try to allocate new element
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
	if(newElement == NULL){
		DLL_Error();
		return;
	}

	newElement->data = data;
	newElement->nextElement = list->activeElement->nextElement;
	newElement->previousElement = list->activeElement;
	list->activeElement->nextElement = newElement;

	if(list->activeElement == list->lastElement) {
		// if the activeElement is the last element
		list->lastElement = newElement;
	} else {
		// activeElement is not the last one
		newElement->nextElement->previousElement = newElement;
	}

	// increment list length
	list->currentLength++;
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
	// if there is no active element
	if(list->activeElement == NULL){
		return;
	}

	// try to allocate new element
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement));
	if(newElement == NULL){
		DLL_Error();
		return;
	}

	newElement->data = data;
	newElement->nextElement = list->activeElement;
	newElement->previousElement = list->activeElement->previousElement;
	list->activeElement->previousElement = newElement;

	if(list->activeElement == list->firstElement) {
		// if the activeElement is the first element
		list->firstElement = newElement;
	} else {
		// activeElement is not the first one
		newElement->nextElement->previousElement = newElement;
	}

	// increment list length
	list->currentLength++;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	// check if there is active element
	if(list->activeElement == NULL){
		DLL_Error();
		return;
	}

	// returns activeElement data through the pointer
	*dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	// check if there is active element
	if(list->activeElement == NULL){
		return;
	}

	list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement == NULL){
		return;
	}

	// move activity to the next one
	list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list->activeElement == NULL){
		return;
	}

	// move activity to the next one
	list->activeElement = list->activeElement->previousElement;
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
	return list->activeElement != NULL;
}

/* Konec c206.c */
