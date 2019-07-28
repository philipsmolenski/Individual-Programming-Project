/** @file
 * Interfejs klasy przechowującej operacje na bazach.
 *
 * @author Philip Smolenski-Jensen
 */

 #ifndef __PHONE_FORWARD_BASE_H__
#define __PHONE_FORWARD_BASE_H__

#include <stdio.h>
#include <string.h>
#include "phone_forward.h"

/// Maksymalna liczba baz w centrali.
#define MAX_BASE_SIZE 1000

/// Wartość którą przyjmuje @p recent, gdy nie ma ustawionej aktualnej bazy.
#define NONE -1

/** @brief Struktura bazy.
* Struktura przechowująca bazy. Jedna baza składa się z
* identyfikatora w postaci Stringa składającego się z liter
* i cyfr (identyfikator zaczyna się literą) oraz drzewa przekierowań
* stworzonego w pliku phone_forward.
*/
typedef struct Base {
	/// Drzewo przekierowań bazy.
	struct PhoneForward *pf;
	/// Identyfikator bazy.
	char *name;
}Base;

/** @brief Struktura przechowująca wiele baz (Centrala).
* Struktura służąca do obsługiwania wielu baz jednocześnie
* Składa się z tablicy baz (rozmiaru MAX_BASE_SIZE) oraz liczby,
* informującej o numerze aktualnie obsługiwanej bazy (liczba ta
* jest równa NONE, gdy żadna baza nie jest aktualnie obsługiwana).
*/
typedef struct Head {
	/// Tablica zawierająca dostępne bazy.
	Base *base;
	/// Numer aktualnie obsługiwanej bazy.
	int recent;
}Head;

/** @brief Tworzy centralę.
* Tworzy centralę nie zawierającą żadnych baz.
* @return Wskaźnik na utworzoną strukturę lub NULL, gdy alokacja
* pamięci nie powiodła się.
*/
Head * newHead();

/** @brief Usuwa centralę.
* Usuwa centralę wskazywaną przez @p h i zwalnia pamięć.
* @param[in] h - wskaźnik na usuwaną centralę.
*/
void clearAll(Head *h);

/** @brief Tworzy bazę.
* Tworzy bazę o identyfikatorze @p name i dodaje ją do centrali @p h.
* @param[in] name - Wskaźnik na identyfikator powstającej bazy
* @param[in] h - Wskaźnik na centralę, do której dodana zostanie baza.
* @return Wartość @p true jeżeli pomyślnie dodano przekierowanie.
* 		  Wartość @p false jeśli wystąpił błąd (centrala jest już pełna,
* 					 lub nie udało się zaalokować pamięci).
*/
bool newBase(Head *h, char const *name);

/** @brief Usuwa bazę.
* Usuwa bazę o identyfikatorze @p name z centrali @p h.
* @param[in] name - Wskaźnik na identyfikator usuwanej bazy.
* @param[in] h - Wskaźnik na centralę, z której usuwana jest baza.
* @return Wartość @p true, jeśli pomyślnie usunięto bazę.
* 		  Wartość @p false, gdy baza o podanym identyfikatorze nie istnieje.
*/
bool delBase(Head *h, char const *name);

#endif /* __PHONE_FORWARD_BASE_H__ */