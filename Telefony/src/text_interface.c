/** @file
 * Implementacja interfejsu klasy udostępniającej
 * interfejs tekstowy.
 *
 * @author Philip Smolenski-Jensen
 */

#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#include "text_interface.h"
#include "phone_forward_base.h"
#include "ctype.h"

/// Znak oddzielający komentarze od poleceń.
#define COMMENT_CHAR '$'

/** @brief Element listy.
 * Struktura stanowiąca element listy. Składa się ze
 * wskaźników na elementy poprzedni i następny oraz znaku.
 */
typedef struct Elem {
    /// Znak przechowywany przez element.
    char ch;
    /// Wskaźnik na następny element.
    struct Elem *next;
    /// Wskaźnik na poprzedni element.
    struct Elem *prev;
}Elem;

/** @brief Struktura listy
 * Struktura listy przechowująca znaki wczytane z wejścia.
 * Składa się ze wskaźników na pierwszy i ostatni element.
 */
typedef struct List {
    /// Wskaźnik na początek listy.
    Elem *beg;
    /// Wskaźnik na koniec listy.
    Elem *end;
}List;

/** @brief Struktura służąca do wczytywania wejścia.
 * Struktura przetwarzająca znaki wczytane ze standardowego wejścia.
 * zawiera wskaźnik na listę przechowywującą znaki oraz dwie wartości 
 * liczbowe: liczbę znaków znajdujących się aktualnie w liście oraz
 * liczę znaków dotychczas wczytanych.
 */
typedef struct Reader {
    /// Wskaźnik na listę przechowującą znaki.
    List *list;
    /// Liczba znaków przechowywanych przez listę.
    int size;
    /// Liczba dotychczas wczytanych znaków z wejścia.
    int read;
}Reader;


/// Enumerator określający wynik wczytywania komentarzy i białych znaków.
enum ProcessIrrelevantResult {
	OK, 
	GOOD_EOF, 
	ERROR_EOF, 
	SYNTAX_ERROR, 
	MEM_ERROR
};

/** @brief Tworzy listę.
* Tworzy nową listę zawierającą tylko pierwszy i ostatni element 
* (które są puste). 
* @return Wskaźnik na utworzoną listę lub NULL gdy nie uda się 
* 		  zaalokować pamięci.
*/
List * newList() {
	List *l = (List*)malloc(sizeof(List));

	if (l == NULL) 
		return NULL;

	l->beg = (Elem*)malloc(sizeof(Elem));
	l->end = (Elem*)malloc(sizeof(Elem));

	if (l->beg == NULL || l->end == NULL) {
		free(l->beg);
		free(l->end);
		free(l);
		return NULL; 
	}

	l->beg->next = l->end;
	l->end->prev = l->beg;

	return l;
}

/** @brief Usuwa listę.
* Usuwa listę oraz zwalnia całą pamięć przez nią zajmowaną. 
* @param[in] l = Wskaźnik na usuwaną listę.
*/
void clearList (List *l) {
	for (Elem *i = l->beg; i != l->end; ) {
		Elem *next = i->next;
		free(i);
		i = next;
	}

	free(l->end);
	free(l);
}

Reader * newReader() {
	Reader *r = (Reader*)malloc(sizeof(Reader));

	if (r == NULL)
		return NULL;

	r->list = newList();

	if (r->list == NULL) {
		free(r);
		return NULL;
	}

	r->read = 0;
	r->size = 0;

	return r;
}

void clearReader(Reader *r) {
	clearList(r->list);
	free(r);
}

/** @brief Wczytuje znak.
* Wczytuje znak Readerem @p r zwiększając przy tym liczbę 
* wczytanych liter.
* @param[in] r - Wskaźnik na strukturę, która przetworzy wczytany znak.
* @return wczytany znak.
*/
char readChar(Reader *r) {
	char c = getchar();
	r->read++;
	return c;
} 

/** @brief Dodaje znak do struktury wczytującej
* Dodaje znak @p c do listy przechowującej litery, znajdującej się
* w strukturze @p r.
* @param[in] r - Wskaźnik na strukturę wczytującą.
* @param[in] c - dodawany znak.
* return Wartość @p true, jeżeli znak dodano pomyślnie.
* 		 Wartość @p false, jeżeli nie udało się zaalokować pamięci.
*/
bool insertChar(Reader *r, char c) {
	List *l = r->list;
	Elem *e = (Elem*)malloc(sizeof(Elem));

	if (e == NULL)
		return false;

	e->ch = c;
	Elem *last = l->end->prev;
	e->next = l->end;
	e->prev = last;
	last->next = e;
	l->end->prev = e;
	r->size++;

	return true;
}

/** @brief Usuwa znaki ze struktury wczytującej.
* Usuwa znaki z końca listy należącej do struktury wczytującej.
* @param[in] r - Wskaźnik na strukturę wczytującą.
* @param[in] remain - Liczba liter w liście, które pozostaną w liście po 
* 					  wykonaniu operacji.
*/
void removeLetters(Reader *r, int remain) {
	if (r->size <= remain)
		return;

	List *l = r->list;
	while (r->size > remain) {
		Elem *last = l->end->prev;
		Elem *prev = last->prev;
		prev->next = l->end;
		l->end->prev = prev;
		free(last);
		r->size--;
	}
}

/** @brief Konwertuje napis do Stringa
* Znaki znajdujące się w liście struktury wczytującej @p r wpisuje
* do tablicy znaków i zwraca utworzony napis.
* @param[in] r - Wskaźnik na strukturę wczytującą, która przechowuje znaki.
* @return Tablica charów reprezentująca napis przechowywany w 
*   	  strukturze wczytującej.
*/
char * toString (Reader *r) {
	int n = r->size;
	List *l = r->list;
	char *result = (char*)malloc(sizeof(char) * (n + 1));

	if (result == NULL)
		return NULL;

	Elem *e = l->beg;
	
	for (int i = 0; i < n; i++) {
		e = e->next;
		result[i] = e->ch;
	}
	result[n] = '\0';

	return result;
}

/** @brief Sprawdza czy znak cyfrą
* Sprawdza, czy znak @p c jest cyfrą.
* @param[in] c - znak, o którym chcemy dowiedzieć się, czy jest cyfrą.
* @return Wartość @p true jeśli @p c jest cyfrą.
*		  Wartość @p false w przeciwnym przypadku.
*/
bool isDigit(char c) {
	return c >= '0' && c < '0' + ALPHABET_SIZE;
}

/** @brief Sprawdza czy znak cyfrą lub literą.
* Sprawdza, czy znak @p c jest cyfrą lub literą.
* @param[in] c - znak, o którym chcemy dowiedzieć się, czy jest cyfrą
* lub literą.
* @return Wartość @p true jeśli @p c jest cyfrą lub literą.
*		  Wartość @p false w przeciwnym przypadku.
*/
bool isLetter(char c) {
	return isalpha(c) || (isDigit(c) && c != ':' && c != ';');
}

// 0 brak EOFa, jest ok
// 1 EOF podczas czytania spacji
// 2 EOF w komentarzu
// 3 sekwencja $X, X != $ podczas wczytywania białych znaków
// 4 zabrakło pamięci w trakcie wczytywania znaku

/** @brief Wczytuje komentarze i białe znaki.
* Wczytuje wczystkie białe znaki i komentarze, które mogą pojawić się 
* między Stringami zawierającymi parametry do wywołania funkcji. 
* Wczytuje również pierwszy znak kolejnej operacji i umieszcza go w liście 
* struktury @p r oraz sprawdza czy podczas wczytywania 
* wystąpił błąd, a jeśli tak to jakiego jest on typu.
* @param[in] r - wskaźnik na strukturę wczytującą.
* @param[in] comment - przyjmuje wartość @p true jeżeli funkcja 
* 			 uruchamiana jest wewnątrz komentarza, zaś wartość @p false
* 			 w przeciwnym przypadku.
* @return Wartość @p ok, jeżeli pomyślnie wczytano komentarze i można kontynuować
* 		  wczytywanie.
* 		  Wartość @p GOOD_EOF, jeżeli struktura wczytująca natrafi na EOF podczas
* 		  wczytywania białych znaków.
*  		  Wartość @p ERROR_EOF, jeżeli struktura wczyta na EOF w obrębie komentarza.
*		  Wartość @p SYNTAX_ERROR, gdy w trakcie wczytywania białych znaków natrafimy
*		  na sekwencję $X, gdzie X jest znakiem różnym of $ i EOF.
*		  Wartość @p MEM_ERROR, gdy nie uda się zaalokować pamięci podczas wczytywania.
*			
*/
int readIrrelevant(Reader *r, bool comment) {
	if (comment) {
		bool last = false;
		while (1) {
			char c = readChar(r);

			if (c == EOF)
				return ERROR_EOF;

			if (last && c == COMMENT_CHAR)
				return readIrrelevant(r, false);

			if (c == COMMENT_CHAR)
				last = true;
			else 
				last = false;
		}
	}

	else {
		while (1) {
			char c = readChar(r);

			if (c == EOF)
				return GOOD_EOF;

			if (!isspace(c)) {
				if (c != COMMENT_CHAR) {
					bool b = insertChar(r, c);

					if (b == false)
						return MEM_ERROR;

					return OK;
				}

				char d = readChar(r);

				if (d == COMMENT_CHAR)
					return readIrrelevant(r, true);

				if (d == EOF)
					return ERROR_EOF;

				return SYNTAX_ERROR;
			}
		}
	}
}

/** @brief Wypisuje błąd składniowy.
* Wypisuje błąd składniowy, który pojawia się po wczytaniu
* znaku o numerze @p n.
* @param[in] n - numer znaku, który wywołał błąd.
*/
void printSyntaxError(int n) {
	fprintf(stderr, "%s %d\n", "ERROR", n);
}

/// Wypisuje błąd spowodowany nagłym końcem pliku.
void printErrorEOF() {
	fprintf(stderr, "%s\n", "ERROR EOF");
}

/** @brief Wypisuje błąd wykonania.
* Wypisuje błąd wykonania operacji @p operator, 
* który pojawia się po wczytaniu znaku o numerze @p n.
* @param[in] n - numer znaku, który wywołał błąd.
* @param[in] operator - wskaźnik na napis reprezentujacy operator.
*/
void printOperatorError(char const *operator, int n) {
	fprintf(stderr, "%s %s %d\n", "ERROR", operator, n);
}

void printMemoryError() {
	fprintf(stderr, "%s\n", "zabrakło pamięci przy wczytywaniu");
}

/** @brief Wypisuje błąd wczytywania komentarzy.
* Jeżeli operacje readIrrelevant zakończy się błędem, wypisuje
* odpowiedni komunikat na wyjście diagnostyczne.
* @param[in] r - Wskaźnik na strukturę wczytującą znaki.
* @param[in] n - kod, którym zakończyło się wykonywanie operacji readIrrelevnat.
* @param[in] middle - przyjmuje wartość @p false, jeżeli funkcja readIrrelevant
* 			 została wywołana przed lub po wczytaniu komponentów niezbędnych do
*			 wykonania operacji oraz wartość @p false jeżeli została wywołana 
*			 między tymi Stringami.
*/
void printError (Reader *r, int n, bool middle) {
	// EOF w czasie wczytywania spacji.
	if (n == GOOD_EOF) 
		if (middle)
			printErrorEOF();

	// EOF w czasie wczytywania komentarzy.
	if (n == ERROR_EOF) 
		printErrorEOF();

	// Sekwencja $X w czasie wczytywania spacji.
	if (n == SYNTAX_ERROR)
		printSyntaxError(r->read);

	// Gdy nie uda się zaalokować pamięci przy wczytywaniu.
	if (n == MEM_ERROR)
		printMemoryError();
}


/** @brief Sprawdza czy znak spełnie określoną własność.
* Sprawdza, czy znak @p c jest cyfrą gdy @p num ma wartość false,
* lub sprawdcza czy znak @p c jest literą w przeciwnym przypadku.
* @param[in] c - znak, o którym chcemy dowiedzieć się, czy jest cyfrą/literą.
* @param[in] num - jeżeli ma wartość @p true to sprawczamy czy @p c jest 
* 			 cyfrą. W przeciwnym przypadku sprawdzamy czy jest literą.
* @return Wartość @p true jeśli @p c spełnia opisaną wyżej własność.
*		  Wartość @p false w przeciwnym przypadku.
*/
bool isOK (char c, bool num) {
	if (num)
		return isDigit(c);
	return isLetter(c);
}

/** @brief Wczytuje liczbę lub identyfikator.
* W zależności od wartości parametru @p num wczytuje liczbę lub 
* identyfikator używane potem do wywołania opereacji na bazach.
* @param[in] r - Wskaźnik na strukturę wczytującą.
* @param[in] num - przyjmuje wartość @p true, jeżeli chcemy wczytać liczbę,
*			lub wartość @p false w przeciwnym przypadku.
* @return Wskaźnik na wczytaną liczbę/słowo lub NULL, jeśli nie
*		 uda się zaalokować pamięci.
*/
char * readWord(Reader *r, bool num) {
	while (1) {
		char d = readChar(r);

		if (isOK(d, num)) {
			bool b = insertChar(r, d);

			if(!b) {
				printMemoryError();
				return NULL;
			}
		}
		else {
			char *word = toString(r);

			if (word == NULL) {
				printMemoryError();
				return NULL;
			}

			removeLetters(r, 0);
			bool b = insertChar(r, d); 

			if (!b) {
				printMemoryError();
				free(word);
				return NULL;
			}

			return word;
		}
	}
}

/** @brief Zwraca wynik wczytywania komentarza
* Wczytuje komentarz i białe znaki i zwraca wartość informującą, czy wczytywanie
* przebiegło pomyślnie. Wartości zwracane są dopasowane tak, aby funkcję można
* było wygodnie używać wewnątrz funkcji processComment.
* @param[in] r - Wskaźnik na strukturę wczytującą znaki.
* @param[in] middle - przyjmuje wartość @p true jeśli komentarz wczytywany jest
* 					  między słowami kluczowymi pewnej operacji, zaś wartość  
*					  @p false w przeciwnym przypadku.
* @param[in] comment - przyjmuje wartość @p true jeśli wczytywany jest komentraz,
* 					   lub @p false jeśli wczytywane są białe znaki.
* @return Wartość @p GO_ON jeżeli pomyślnie wczytano komentarz i można dalej
* 		 wczytywać dane.
* 		 Wartość @p END gdy pomyślnie wczytano dane struktura wczytująca 
* 		 napotkała znak EOF.
*		 Wartość @p ERROR gdy wystąpi błąd wczytywania lub nie uda się 
*		 zaalokować pamięci.
*/
int processCommentHelper (Reader *r, bool middle, bool comment) {
	int n = readIrrelevant(r, comment);
	printError(r, n, middle);

	if (n == OK)
		return GO_ON;

	if (middle)
		return ERROR;

	if (n == GOOD_EOF)
		return END;

	return ERROR;
}

/** @brief Przetwarza komentarz.
* Za pomocą wcześniejszych operacji wczytuje komentarz i wykrywa ewentualne
* błędy wypisując stosowny komunikat na wyjście diagnostyczne.
* @param[in] r - Wskaźnik na strukturę wczytującą znaki.
* @param[in] middle - przyjmuje wartość @p false, jeżeli funkcja readIrrelevant
* 			 została wywołana przed lub po wczytaniu komponentów niezbędnych do
*			 wykonania operacji oraz wartość @p false jeżeli została wywołana 
*			 między tymi Stringami.
* @return Wartość @p GO_ON jeżeli pomyślnie wczytano komentarz i można dalej
* 		 wczytywać dane.
* 		 Wartość @p END gdy pomyślnie wczytano dane struktura wczytująca 
* 		 napotkała znak EOF.
*		 Wartość @p ERROR gdy wystąpi błąd wczytywania lub nie uda się 
*		 zaalokować pamięci.
*/
int processComment (Reader *r, bool middle) {
	if (r->size == 0)
		insertChar(r, ' ');

	char c = r->list->beg->next->ch;

	if (isspace(c)) {
		removeLetters(r, 0);
		return processCommentHelper(r, middle, false);
	}

	if (c == COMMENT_CHAR) {
		removeLetters(r, 0);

		char d = readChar(r);

		if (d != COMMENT_CHAR) {
			printSyntaxError(r->read);
			return ERROR;
		}

		return processCommentHelper(r, middle, true);
	}

	if (c == EOF) {
		if (middle)
			return ERROR;

		return GOOD_EOF;
	}

	return GO_ON;
}

/** @brief Wczytuje pozostałe znaki operatora.
* Gdy wczytana zostaje pierwsza litera operatora "NEW" lub "DEL"
* funkcja sprawdza, czy następne znaki odpowiadają kolejnym literom
* wczytywanego operatora i wypisuje odpowiedni komunikat w razie błędu.
* @param[in] r - Wskaźnik na strukturę wczytującą znaki.
* @param[in] operator - Wskaźnik na stringa będącego resztą nazwy operatora.
* @return Wartość @p true gdy następne wczytane znaki są resztą nazwy operatora,
*		  Wartość @p fasle w przeciwnym wypadku.
*/
bool readOperator (Reader *r, char const *operator) {
	int len = strlen(operator);

	for (int i = 0; i < len; i++) {
		char c = readChar(r);

		if (c == EOF) {
			printErrorEOF();
			return false;
		}

		if (c != operator[i]) {
			printSyntaxError(r->read);
			return false;
		}
	}

	return true;
}

int processOperation (Reader *r, Head *h) {
	// Najpierw wczytujemy komentarze.
	int x = processComment(r, false);

	if (x != GO_ON)
		return x;

	char c = r->list->beg->next->ch;

	// Gdy operacja zaczyna się słowem "NEW".
	if (c == 'N') {
		int entrySize = r->read;

		// Są to kolejne litery operatora "NEW".
		bool bo = readOperator(r, "EW");

		if (!bo)
			return ERROR;

		char c = getchar();
		if (!isspace(c) && c != COMMENT_CHAR) {
			printSyntaxError(r->read + 1);
			return ERROR;
		}
		ungetc(c, stdin);


		removeLetters(r, 0);
		int x = processComment(r, true);

		if (x != OK)
			return x;

		c = r->list->beg->next->ch;

		if (isDigit(c) || !isLetter(c)) {
			printSyntaxError(r->read);
			return ERROR;
		}

		char *name = readWord(r, false);

		if (name == NULL)
			return ERROR;

		if (strcmp(name, "NEW") == 0 || strcmp(name, "DEL") == 0) {
			printSyntaxError(r->read);
			free(name);
			return ERROR;
		}

		bool b = newBase(h, name);
		free (name);

		if (!b) {
			printOperatorError("NEW", entrySize);
			return ERROR;
		}

		return GO_ON;
	}

	// Gdy operacja zaczyna się słowem "DEL".
	if (c == 'D') {
		int entrySize = r->read;
		
		bool bo = readOperator(r, "EL");

		if (!bo)
			return ERROR;

		char c = getchar();
		if (!isspace(c) && c != COMMENT_CHAR) {
			printSyntaxError(r->read + 1);
			return ERROR;
		}
		ungetc(c, stdin);

		removeLetters(r, 0);
		int x = processComment(r, true);

		if (x != OK)
			return x;

		c = r->list->beg->next->ch;

		if (!isLetter(c)) {
			printSyntaxError(r->read);
			return ERROR;
		}

		// Operacje DEL number.
		if (isDigit(c)) {
			char *number = readWord(r, true);

			if (number == NULL)
				return ERROR;

			int k = h->recent;

			if (k == NONE) {
				printOperatorError("DEL", entrySize);
				free(number);
				return ERROR;
			}

			phfwdRemove((h->base + k)->pf, number);
			free(number);

			return GO_ON;
		}

		// Operacja DEL identyfikator.
		else {
			char *name = readWord(r, false);

			if (name == NULL)
				return ERROR;

			if (strcmp(name, "NEW") == 0 || strcmp(name, "DEL") == 0) {
				printSyntaxError(r->read);
				free(name);
				return ERROR;
			}

			bool b = delBase(h, name);
			free (name);

			if (!b) {
				printOperatorError("DEL", entrySize);
				return ERROR;
			}

			return GO_ON;
		}
	}

	// Opereacje zaczynające się od podania liczby.
	if (isDigit(c)) {
		char *fstNum = readWord(r, true);

		if (fstNum == NULL)
			return ERROR;

		int x = processComment(r, true);

		if (x != OK){
			free(fstNum);
			return x;
		}

		c = r->list->beg->next->ch;

		if (c != '?' && c != '>') {
			printSyntaxError(r->read);
			free (fstNum);
			return ERROR;
		}

		// Operacja typu numer ?.
		if (c == '?') {

			int k = h->recent;

			if (k == NONE) {
				printOperatorError("?", r->read);
				free(fstNum);
				return ERROR;
			}

			else {
				struct PhoneNumbers const *pnum; 
				pnum = phfwdGet((h->base + k)->pf, fstNum);
				free(fstNum);

				if (pnum == NULL) {
					printOperatorError("?", r->read);
					return ERROR;
				}

				printf("%s\n", phnumGet(pnum, 0));

				phnumDelete(pnum);
				removeLetters(r, 0);

				return GO_ON;
			}
		}

		// Operacja typu numer > numer.
		else {
			int entrySize = r->read; 
			removeLetters(r, 0);

			int x = processComment(r, true);

			if (x != OK){
				free(fstNum);
				return x;
			}

			char c = r->list->beg->next->ch;

			if (!isDigit(c)) {
				printSyntaxError(r->read);
				free(fstNum);
				return ERROR;
			}
			
			char *sndNum = readWord(r, true);

			if (sndNum == NULL) {
				free(fstNum);
				return ERROR;
			}
			
			int k = h-> recent;

			if (k == NONE) {
				printOperatorError(">", entrySize);
				free(fstNum);
				free(sndNum);
				return ERROR;
			}

			bool b = phfwdAdd((h->base + k)->pf , fstNum, sndNum);
			free(fstNum);
			free(sndNum);

			if (!b) {
				printOperatorError(">", entrySize);
				return ERROR;
			}

			return GO_ON;
		}
	}

	// Operacja typu ?numer.
	if (c == '?') {
		int entrySize = r->read;
		removeLetters(r, 0);
		int x = processComment(r, true);

		if (x != OK)
			return x;

		c = r->list->beg->next->ch;

		if (!isDigit(c)) {
			printSyntaxError(r->read);
			return ERROR;
		}

		char *num = readWord(r, true);

		if (num == NULL)
			return ERROR;

		int k = h->recent;

		if (k == NONE) {
			printOperatorError("?", entrySize);
			free(num);
			return ERROR;
		}

		struct PhoneNumbers const * pnum;
		pnum = phfwdReverse((h->base + k)->pf, num);
		free(num);

		if (pnum == NULL) {
			printOperatorError("?", entrySize);
			return ERROR;
		}

		char const *numer;
			
		size_t idx = 0;
		while ((numer = phnumGet(pnum, idx++)) != NULL)
     	printf("%s\n", numer);
     	phnumDelete(pnum);

     	return GO_ON;
	}

	//operacja typu @numer.
	if (c == '@') {
		int entrySize = r->read;
		removeLetters(r, 0);
		int x = processComment(r, true);

		if (x != OK)
			return x;

		c = r->list->beg->next->ch;

		if (!isDigit(c)) {
			printSyntaxError(r->read);
			return ERROR;
		}

		char *set = readWord(r, true);

		if (set == NULL)
			return ERROR;

		int k = h->recent;

		if (k == NONE) {
			printOperatorError("@", entrySize);
			free(set);
			return ERROR;
		}

		int len = strlen(set) - ALPHABET_SIZE;

		if (len < 0)
			len = 0;

		size_t result = phfwdNonTrivialCount((h->base + k)->pf, set, len);
		free (set);
		printf("%lu\n", result);

		return GO_ON;
	}

	else {
		printSyntaxError(r->read);
		return ERROR;
	}

}