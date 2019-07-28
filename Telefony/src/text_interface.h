/** @file
 * Interfejs klasy udostępniającej interfejs tekstowy.
 *
 * @author Philip Smolenski-Jensen
 */

 #ifndef __TEXT_INTERFACE_H__
#define __TEXT_INTERFACE_H__

#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#include "phone_forward_base.h"

/// Enumerator określający wynik przetwarzania operacji.
enum ProcessOperationResult {
    GO_ON,
    END, 
    ERROR
};

/// Numer pierwszego białego znaku w kodzie ASCII.
#define FIRST_WHITE_SPACE 9

/// Numer ostatniego białego znaku w kodzie ASCII (nie licząc spacji)
#define LAST_WHITE_SPACE 13

/// Element listy.
typedef struct Elem Elem;

/// Struktura listy
typedef struct List List;

/// Struktura służąca do wczytywania wejścia.
typedef struct Reader Reader;

/** @brief Tworzy strukturę wczytującą.
 * Tworzy strukturę służącą do przetwarzania wejścia. 
 * @return Wskaźnik na utworzoną strukturę lub NULL jeśli nie udało się
 *         zaalokować pamięci.
 */
Reader * newReader();

/** @brief Usuwa strukturę wczytującą.
 * Usuwa strukturę służącą do przetwarzania wejścia. 
 * @param[in] r - wskaźnik na usuwaną strukturę.
 */
void clearReader(Reader *r);

/// Wypisuje błąd spowodowany brakiem pamięci.
void printMemoryError();

/** @brief Wczytuje i przetwarza operację.
 * Wczytuje znaki tak długo, aż wczytane wartości pozwolą na wykonanie
 * jednej z określonych w treści zadania operacji lub aż wystąpi błąd.
 * Następnie, jeśli dane wczytano pomyślnie, wykonuje odpowiednią operację.
 * @param[in] r - Wskaźnik na strukturę wczytującą dane.
 * @param[in] h - Wskaźnik na centralę, na której wykonana zostanie operacja.
 * @return Wartość @p GO_ON, jeżeli operację wykonano i wczytano pomyślnie, a 
 *         proces wczytywania jest kontynuowany.
 *         Wartość @p END, gdy proces wczytywania zakończył się bez błędu 
 *         (możliwe, że po uprzednim pomyślnym przetworzeniu operacji).
 *         Wartość @p ERROR, gdy wystąpi któryś z opisanych w treści zadania
 *         błędów lub nie uda się zaalokować pamięci.
 */
int processOperation (Reader *r, Head *h);

 #endif /* __TEXT_INTERFACE_H__ */