/** @file
 * Plik zawierający główną funkcję main. Pozwala na wykonywanie
 * operacji na wielu bazach przekierowań telefonicznych za 
 * pomocą interfejsu tekstowego.
 *
 * @author Philip Smolenski-Jensen
 */


#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#include "text_interface.h"
#include "phone_forward_base.h"

/** @brief Pozwala wykonywać operacje na bazach.
* Umożliwia wykonywanie operacji opisanych w treści zadania 
* przez interfejs tekstowy.
* @return Wartość @p 0, gdy działanie programu zakończy się poprawnie.
* 		  Wartość @p 1, gdy działanie programu zakończy się błędem.
*/
int main() {
	Reader *r = newReader();


	if (r == NULL) {
		printMemoryError();
		return 1;
	}

	Head *h = newHead();

	if (h == NULL) {
		clearReader(r);
		printMemoryError();
		return 1;
	}

	int processOperationReturnCode;

	while (1) {
		int n = processOperation(r, h);
		if (n != GO_ON) {
			processOperationReturnCode = n;
			break;
		}
	}

	clearAll(h);
	clearReader(r);
	int returnCode = processOperationReturnCode - 1;

	return returnCode;
}