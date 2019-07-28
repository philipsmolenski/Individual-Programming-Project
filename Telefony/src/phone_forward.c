/** @file
 * Implementacja interfejsu klasy przechowywującej
 * przekierowania numerów telefonicznych.
 *
 * @author Philip Smolenski-Jensen
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "phone_forward.h"

/** @brief Struktura przechowująca przekierowania numerów telefonów.
 * Przekierowania trzymamy w drzewie prefixowym.
 * Każde przekierowanie zawiera tablicę dzieci, przy czym a jest dzieckiem b
 * gdy pierwszy numer przekierowania w a jest pierwszym numerem
 * przekierowania w b, powiększonym o 1 cyfrę.
 */
struct PhoneForward {
	/// Tablica dzieci przekierowania. 
	struct PhoneForward **children;
	/// Wskaźnik na przekierowywany numer.
	char *fstNum;
	/// Wskażnik na numer, na który przekierowany jest numer.
	char *sndNum;   
};

/** @brief Struktura przechowująca ciąg numerów telefonów.
 * Składa się ona z tablicy stringów(przechowywujących numery) 
 * oraz liczby będącej wielkością tej tablicy.
 */
struct PhoneNumbers {
	/// Tablica numerów.
	char **numbers;
	/// Rozmiar tablicy.
	size_t size;
};

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
struct PhoneForward * phfwdNew(void) {
	struct PhoneForward *pf = (struct PhoneForward*)malloc(sizeof(struct PhoneForward));

	if (pf == NULL)
		return NULL;

	pf->children = NULL;
	pf->fstNum = NULL;
	pf->sndNum = NULL;

	return pf;
}

void phfwdDelete(struct PhoneForward *pf) {
	if (pf == NULL)
		return;

	if (pf->children != NULL) {
		for(int i = 0; i < ALPHABET_SIZE; i++) {
			phfwdDelete(pf->children[i]);
		}
		free(pf->children);
	}

	if (pf->fstNum != NULL)
		free (pf->fstNum);

	if (pf->sndNum != NULL)
		free (pf->sndNum);	 

	free(pf);
	pf = NULL;
}

/** @brief Zwraca liczbę znaków numeru.
* @param[in] num - wskaźnik na numer którego długość wyznaczamy.
* @return Liczba znaków składających się na numer.
*/
size_t size(char const *num) {
	size_t i = 0;
	
	while (num[i] != '\0')
		i++;
	
	return i;
}

/** @brief Sprawdza czy napis jest numerem.
* Napis pusty nie jest trakttowany jak numer.
* @param[in] num - wskaźnik na rozpatrywany napis.
* @return Wartość @p true, jeśli @p num jest numerem.
* 		  Wartość @p false w przeciwnym przypadku.
*/
bool isNumber(char const *num) {
	if (strcmp("", num) == 0)
		return false;

	bool wyn = true;
	int i = 0;
	
	while (num[i] != '\0') {
		int k = num[i];
		if (k < '0' || k >= ALPHABET_SIZE + '0')
			wyn = false;
		i++;
	}
	
	return wyn;
}

/** @brief Sprawdza czy pierwszy z numerów jest prefixem drugiego.
* @param[in] num1 - wskaźnik na pierwszy z numerów.
* @param[in] num2 - wskaźnik na drugi z numerów.
* return Wartość @p true jeśli @p num1 jest prefixem @p num2.
* 		 Wartość @p false w przeciwnym przypadku.
*/
bool isPrefix (char const *num1, char const *num2) {
	bool wyn = true;
	int i = 0;
	
	while (num1[i] != '\0') {
		if (num2[i] == '\0') 
			return false;
		
		if (num2[i] != num1[i])
			wyn = false;
		
		i++;
	}
	
	return wyn;
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {
	if (!isNumber(num1) || !isNumber(num2))
		return false;

	if (strcmp(num1, num2) == 0)
		return false;

	int n = size(num1);
	int i = 0;

	while (i < n) {
		if (pf->children == NULL) {
			pf->children = (struct PhoneForward**)malloc
				(sizeof(struct PhoneForward*) * ALPHABET_SIZE);
			
			if (pf->children == NULL)
				return false;

			for(int j = 0; j < ALPHABET_SIZE; j++) 
				pf->children[j] = NULL;
		}
		
		char c = num1[i];
		int k = c - '0';
		
		if (k < 0 || k > ALPHABET_SIZE - 1)
			return false;

		if (pf->children[k] == NULL) {
			pf->children[k] = (struct PhoneForward*)malloc(sizeof(struct PhoneForward));

			if (pf->children[k] == NULL)
				return false;

			(pf->children[k])->fstNum = NULL;
			(pf->children[k])->sndNum = NULL;
			(pf->children[k])->children = NULL;
		}
		i++;
		pf = pf->children[k];
	}
	
	if (pf->fstNum == NULL) {
		pf->fstNum = (char*)malloc(sizeof(char) * (n + 1));

		if (pf->fstNum == NULL)
			return false;

		strcpy(pf->fstNum, num1);
	}

	if (pf->sndNum != NULL)
		free (pf->sndNum);

	pf->sndNum = (char*)malloc(sizeof(char) * (size(num2) + 1));

	if (pf->sndNum == NULL)
		return false;

	strcpy(pf->sndNum, num2);

	return true;
}

void phfwdRemove(struct PhoneForward *pf, char const *num) {
	if (num == NULL || strcmp(num, " ") == 0)
		return;

	int n = size(num);
	int i = 0;

	while (i < n - 1) {
		if (pf->children == NULL)
			return;

		char c = num[i];
		int k = c - '0';

		if (k < 0 || k > ALPHABET_SIZE - 1)
			return;

		if (pf->children[k] == NULL)
			return;

		pf = pf->children[k];
		i++;
	}
	
	if (pf->children == NULL)
			return;

	char c = num[i];
	int k = c - '0';

	if (k < 0 || k > ALPHABET_SIZE - 1)
		return;

	if (pf->children[k] == NULL)
		return;

	phfwdDelete(pf->children[k]);
	pf->children[k] = NULL;	
}

/** @brief Wyznacza numer na podstawie przekierowania.
* Wyznacza numer, na który przekeierowany zostanie @p number
* gdy jego prefix @p num1 zostanie zamieniony na @p num2. O danych 
* wejściowych zakłada się że @p num1 jest prefixem @p number.
* @param[in] number - wskaźnik na numer, którego przekierowanie mamy wyznaczyć
* @param[in] num1 - wskaźnik na prefix, który zostanie przekierowany na @p num2.
* @param[in] num2 - wskaźnik na numer, na który zostanie przekierowany @p num1.
* return Wskaźnik na powstały w wyniku operacji numer lub NULL, gdy nie uda
* 		 się zaalokować pamięci.
*/
char * redirect(char const *number, char const *num1, char const *num2) {
	int n = size(number);
	int n1 = size(num1);
	int n2 = size(num2);
	int c = n + n2 - n1 + 1;
	
	char *wyn = (char*)malloc(sizeof(char) * c);
	
	if (wyn == NULL)
		return NULL;
	
	strcpy(wyn, num2);
	strcpy(wyn + n2, number + n1);
	
	return wyn;
} 

/** @brief Znajduje przekierowanie z najdłuższym pasującym prefixem.
* Zwraca przekierowanie, którego pierwszy numer jest najdłuższym
* prefixem numeru @p num spośród tych, które znajdują się w drzewie
* przekierowań @p pf (jako pierwsze numery przekierowań)
* @param[in] pf - wskażnik na korzeń dzewa przekierowań,
*				  z którego wybieramy przekierowanie
* @param[in] num - wskaźnik na numer, którego najdłuższy prefix jest poszukiwany.
* @return Wskaźnik na szukane przekierowanie, korzeń drzewa gdy w drzewie nie
* 		  ma żadnego prefixu liczby @p num lub NULL, gdy napis nie reprezentue
*         numeru albo @p pf jest NULLem.
*/
struct PhoneForward * findBest (struct PhoneForward *pf, char const *num) {
	if (pf == NULL)
		return NULL;

	struct PhoneForward *wyn = pf;
	int n = size(num);
	int i = 0;

	while (i < n) {
		if (pf->children == NULL)
			return wyn;

		char c = num[i];
		int k = c - '0';
		
		if (k < 0 || k > ALPHABET_SIZE - 1)
			return NULL;

		if (pf->children[k] == NULL)
			return wyn;

		pf = pf->children[k];

		if (pf->fstNum != NULL)
			wyn = pf;

		i++;
	}

	return wyn;
}

struct PhoneNumbers const * phfwdGet(struct PhoneForward *pf, char const *num) {
	struct PhoneNumbers *ph = (struct PhoneNumbers*)malloc(sizeof(struct PhoneNumbers));
	
	if (ph == NULL)
		return NULL;
	
	ph->numbers = (char**)malloc(sizeof(char*));
	
	if (ph->numbers == NULL) {
		free (ph);
		return NULL;
	}
	
	struct PhoneForward *best = findBest(pf, num); //znajdujemy najlepsze
												   // przekierowanie
	
	if (best == NULL || !isNumber(num)) { // jeżeli napis nie reprezentuje numeru
		ph->size = 0;
		
		return ph;
	} 
	
	if (best->fstNum == NULL) { //jeżeli żaden prefix nie pasuje zwracamy null
		int n = size(num);
		ph->numbers[0] = (char*)malloc(sizeof(char) * (n + 1));
		
		if (ph->numbers[0] == NULL) {
			free (ph->numbers);
			free (ph);
			return NULL;
		}
		
		strcpy(ph->numbers[0], num);
	}
	
	else { //w przeciwnym wypadku wyznaczamy przekierowanie
		char const *num1 = best->fstNum;
		char const *num2 = best->sndNum;
		
		char *wyn = redirect(num, num1, num2);
		
		if (wyn == NULL) {
			free(ph->numbers);
			free(ph);
			return NULL;
		}
		
		int n = size(wyn);
		ph->numbers[0] = (char*)malloc(sizeof(char) * (n + 1));
		
		if (ph->numbers[0] == NULL) {
			free(wyn);
			free(ph->numbers);
			free(ph);
			return NULL;
		}
		
		strcpy(ph->numbers[0], wyn);
		free(wyn);
	}
	
	ph->size = 1;
	
	return ph;
}

/** @brief Znajduje liczbę przekierowań.
* Znajduje liczbę przekierowań na liczbę @p num w drzewie przekierowań @p pf.
* @param[in] pf - wskaźnik na korzeń drzewa przekierowań.
* @param[in] num - wskaźnik na numer, na który przekierowań szukamy.
* @return Liczba przekierowań na numer num.
*/
int findSize(struct PhoneForward *pf, char const *num) {
	if (pf == NULL)
		return 0;

	int n = 0;
	if (pf->sndNum != NULL && isPrefix(pf->sndNum, num))
		n++;

	if (pf->children == NULL)
		return n;

	for (int i = 0; i < ALPHABET_SIZE; i++) 
		n += findSize(pf->children[i], num);

	return n;
}

/** @brief Wypełnia strukturę przekierowaniami na dany numer.
* Wypełnia strukturę @p pnum numerami z drzewa przekierowań @p pf, które 
* zostaną przekierowane na @p num oraz aktualizuje rozmiar struktury.
* Numery w tablicy struktury @p pnum mogą się powtarzać.
* @param[in] pf - wskaźnik na korzeń drzewa przekierowań.
* @param[in] num - wskaźnik na numer, na który zostaną przekierowane elementy
* 		     wrzucane do tablicy.
* @param[in] pnum - struktura do której wpisywane są numery spełniające wyżej
*			 opisane warunki.
* @return Wartość @p true jeżeli wypełnianie struktury się powiodło lub 
* 		  Wartość @p false gdy nie udało się zaalokować pamięci.
*/
bool fill(struct PhoneForward *pf, char const *num, struct PhoneNumbers *pnum) {
	if (pf == NULL)
		return true;

	if (pf->children != NULL) {
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			bool b = fill(pf->children[i], num, pnum);

			if (!b)
				return false;
		}
	}

	if (pf->sndNum != NULL && isPrefix(pf->sndNum, num)) {
		int c = size(num) + size(pf->fstNum) - size(pf->sndNum) + 1;
		pnum->numbers[pnum->size] = (char*)malloc(sizeof(char) * c);
		
		if (pnum->numbers[pnum->size] == NULL)
			return false;
		
		char *redirection = redirect(num, pf->sndNum, pf->fstNum);

		if (redirection == NULL)
			return false;

		strcpy(pnum->numbers[pnum->size], redirection);
		free(redirection);
		pnum->size++;
	}
	
	return true;
}


/** @brief Komparator.
* Funkcja porównuąca dwa napisy, użyta dalej jako parametr funkcji qsort.
* @param[in] a - wskaźnik na wskaźnik reprezentujący pierwszy napis.
* @param[in] b - wskaźnik na wskaźnik reprezentujący drugi napis.
* @return Wartość @p -1, gdy @p a jest przed @p b w porządku leksykograficznym,
*         Wartość @p 1, gdy @p a jest po @p b w porządku leksykograficznym,
*		  Wartość @p 0, gdy napisy są identyczne.
*/
int cmpfunc (const void *a, const void *b) {
	char **xs = (char **)a;
	char **ys = (char **)b;
	char *as = xs[0];
	char *bs = ys[0];
	
	for (; *as == *bs; as++, bs++)
		if (*as == '\0')
			return 0;
	
	return *as - *bs;
}

struct PhoneNumbers const * phfwdReverse(struct PhoneForward *pf, char const *num) {
	int n = findSize(pf, num);
	struct PhoneNumbers *ph = (struct PhoneNumbers*)malloc(sizeof(struct PhoneNumbers));
	
	if (ph == NULL)
		return NULL;
	
	ph->numbers = (char**)malloc(sizeof(char *) * (n + 1));
	
	if (ph->numbers == NULL) {
		free(ph);
		return NULL;
	}
	
	if (!isNumber(num)) { // gdy num nie jest numerem.
		ph->size = 0;
		return ph;
	}
	
	//wypełniamy ph żądanymi numerami.
	ph->numbers[0] = (char*)malloc(sizeof(char) * (size(num) + 1));
	
	if (ph->numbers[0] == NULL) {
		free(ph->numbers);
		free(ph);
		return NULL;
	}
	
	strcpy(ph->numbers[0], (char *)num);
	ph->size = 1;
	bool b = fill(pf, num, ph);
	
	if (b == false) {
		phnumDelete(ph);
		return NULL;
	}
	
	//posortowanie tablicy i usunięcie powtórzeń.
	qsort(ph->numbers, ph->size, sizeof(char*), cmpfunc);
	
	char **result = (char**)malloc(sizeof(char*) * ph->size);
	
	if (result == NULL) {
		phnumDelete(ph);
		return NULL;
	}
	
	int a = ph->size;
	int minus = 0;
	
	for (int i = 0; i < a; i++) {
		if (i > 0 && strcmp(ph->numbers[i], ph->numbers[i - 1]) == 0) {
			minus ++;
			
			continue;
		}
		
		result[i - minus] = (char*)malloc(sizeof(char) * (size(ph->numbers[i]) + 1));
		
		if (result[i - minus] == NULL) {
			phnumDelete(ph);
			
			for (int j = 0; j < i - minus; i++)
				free(result[j]);
			free (result);
			return NULL;
		}
		
		strcpy(result[i - minus], ph->numbers[i]);
	}
	
	for (size_t i = 0; i < ph->size; i++)
		free(ph->numbers[i]);
	
	free(ph->numbers);
	ph->size -= minus;
	ph->numbers = (char**)malloc(sizeof(char*) * ph->size);
	
	if (ph->numbers == NULL) {
		for (size_t i = 0; i < ph->size; i++)
			free(result[i]);
		free(result);
		free(ph);
		return NULL;
	}
	
	for (size_t i = 0; i < ph->size; i++) {
		ph->numbers[i] = (char*)malloc(sizeof(char) * (size(result[i]) + 1));
		
		if (ph->numbers[i] == NULL) {
			for (size_t j = 0; j < ph->size; j++)
				free(result[i]);
			free(result);

			for (size_t j = 0; j < i; j++)
				free(ph->numbers[i]);

			free(ph->numbers);
			free(ph);
			return NULL;
		}
		
		strcpy(ph->numbers[i], result[i]);
	}
	
	for (size_t i = 0; i < ph->size; i++)
		free(result[i]);
	
	free(result);
	
	return ph;
}

char const * phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
	if (pnum == NULL || idx >= pnum->size)
		return NULL;
	
	else
		return pnum->numbers[idx];
}

void phnumDelete(struct PhoneNumbers const *pnum){
	if (pnum == NULL)
		return;
	
	for(size_t i = 0; i < pnum->size; i++)
		free(pnum->numbers[i]);
	
	free(pnum->numbers);
	free((void*)pnum);
}

/** @brief Znajduje cyfry w napisie.
* Dla podanego napisu @p set wyznacza cyfry, które należą do tego napisu.
* @param[in] set - wskaźnik na zbiór, w którym szukamy cyfr.
* @return Wskaźnik na tablicę booli długości ALPHABET_SIZE, w której na 
* 		  i-tym miejscu jest wartość @p true wtedy i tylko wtedy, gdy @p set 
*		  zawiera cyfrę i (lub : dla i = 10, lub ; dla i = 11). 		
*/
bool * getDigits(char const *set) {
	bool *present = (bool*)malloc(sizeof(bool) * ALPHABET_SIZE);

	if (present == NULL)
		return NULL;

	for (size_t i = 0; i < ALPHABET_SIZE; i++)
		present[i] = false;

	for (size_t i = 0; i < size(set); i++) 
		if (set[i] >= '0' && set[i] < '0' + ALPHABET_SIZE)
			present[set[i] - '0'] = true;

	return present;
}

/** @brief Znajduje liczbę wartości @p true.
* Dla tablicy booli długości ALPHABET_SIZE znajduje liczbę 
* wartości @p true.
* @param[in] present - wskaźnik na tablicę booli.
* @return Liczba wartości @p true w tablicy.
*/
size_t digitsNumber (bool const *present) {
	size_t result = 0;

	for (size_t i = 0; i < ALPHABET_SIZE; i++)
		if (present[i])
			result++;

	return result;
}

/** @brief Sprawdza czy liczba składa się z podanych cyfr.
* Sprawdza, czy liczba zawiera tylko takie cyfry i, dla których
* @p present[i] = true
* @param[in] number - wskaźnik na liczbę
* @param[in] present - wskaźnik na tablicę booli.
* @return Wartość @p true, jeśli @p number zawiera tylko cyfry zakodowane
*  		  w tablicy @p present lub wartość @p false w przeciwnym przypadku.
*/
bool isOk (char const *number, bool const *present) {
	size_t n = size(number);

	for (size_t i = 0; i < n; i++) 
		if (!present[number[i] - '0'])
			return false;

	return true;
}

/** @brief Znajduje liczbę prefixów zawierających tylko określone znaki.
* Znajduje w drzewie @p pf liczbę prefixów długości niewiększej niż @p len, 
* na które istnieje przekierowanie i których zapis składa się wyłącznie z cyfr 
* i takich, że present[i] = true.
* @param [in] pf - Wskaźnik na drzewo z przekierowaniami.
* @param [in] present - tablica booli, koduje dozwolone znaki.
* @param [in] len - maksymalna dozwolona długość prefixu.
* @return liczba prefixów spełniających powyższe warunki.
*/
size_t findNonTrivialPrefixNumber (struct PhoneForward *pf, bool *present, size_t len) {
	if (pf == NULL)
		return 0;

	size_t result = 0;

	if (pf->sndNum != NULL && size(pf->sndNum) <= len && isOk(pf->sndNum, present))
		result++;

	if (pf->children == NULL)
		return result;
	
	for (size_t i = 0; i < ALPHABET_SIZE; i++) 
		result += findNonTrivialPrefixNumber(pf->children[i], present, len);

	return result;
}

/** @brief Wypełnia strukturę PhoneNumbers numerami.
* Wypełnia @p pnum prefixami długości nieprzekraczajączej @p len będącymi 
* przekierowaniami z pewnego numeru w drzewie @p pf, które zawierają cyfrę
* i tylko wtedy, gdy @p present[i] = true.
* @param[in] pf - wskaźnik na drzewo przekierowań.
* @param[in] present - tablica booli określająca dozwolone znaki.
* @param [in] len - maksymalna dozwolona długość prefixu.
* @param [in] pnum - struktura, do której wpisujemy dobre prefixy.
* @return Wartość @p true, jeśli udało się wypełnić @p pnum numerami, lub
* 		  wartość @p false, gdy nie udało się zaalokować pamięci.   
*/
bool fillNonTrivial(struct PhoneForward *pf, bool *present,
							 size_t len, struct PhoneNumbers *pnum) {
	if (pf == NULL)
		return true;

	if (pf->children != NULL) {
		for (size_t i = 0; i < ALPHABET_SIZE; i++) {
			bool b = fillNonTrivial(pf->children[i], present, len, pnum);

			if (!b)
				return false;
		}
	}

	if (pf->sndNum != NULL && size(pf->sndNum) <= len && isOk(pf->sndNum, present)) {
		size_t l = size(pf->sndNum) + 1;
		pnum->numbers[pnum->size] = (char*)malloc(sizeof(char) * l);

		if (pnum->numbers[pnum->size] == NULL)
			return false;

		strcpy(pnum->numbers[pnum->size], pf->sndNum);
		pnum->size++;
	}

	return true;
}

/** @brief Potęgowanie.
* Podnosi liczbę @p x do potęgi @p n.
* @param[in] x - liczba potęgowana.
* @param[in] n - wykładnik.
* @return Wynik działania.
*/
size_t power (size_t x, size_t n) {
	if (n == 0)
		return 1;

	if (n % 2 != 0)
		return x * power(x, n - 1);

	size_t a = power(x, n / 2);

	return a * a;
}

size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) {
	if (pf == NULL || set == NULL || len == 0)
		return 0;

	bool *present = getDigits(set);

	if (present == NULL)
		exit(1);

	size_t goodDigits = digitsNumber(present);

	if (goodDigits == 0)
		return 0;

	size_t n = findNonTrivialPrefixNumber(pf, present, len);
	struct PhoneNumbers *pnum = (struct PhoneNumbers*)malloc(sizeof(struct PhoneNumbers));

	if (pnum == NULL) {
		free(present);
		exit(1);
	}

	pnum->numbers = (char**)malloc(sizeof(char*) * n);

	if(pnum->numbers == NULL) {
		free(present);
		free(pnum);
		exit(1);
	}

	pnum->size = 0;

	bool b = fillNonTrivial(pf, present, len, pnum);

	if (!b) {
		phnumDelete(pnum);
		free(present);
		exit(1);
	}

	free(present);
	qsort(pnum->numbers, pnum->size, sizeof(char*), cmpfunc);
	size_t result = 0;

	size_t j = pnum->size;

	for (size_t i = 0; i < pnum->size; i++) { 
		if ((j == pnum->size || !isPrefix(pnum->numbers[j], pnum->numbers[i]))) {
			result += 
			power(goodDigits, (size_t)(len - size(pnum->numbers[i])));
			j = i;
		}
	}

	phnumDelete(pnum);

	return result;
}	
