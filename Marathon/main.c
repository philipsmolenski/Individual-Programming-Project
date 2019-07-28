#include <stdio.h>
#include <stdlib.h>
#include "input.h"

int main () {

	init();
	
	for (int i = 0; ; i++) {
		int end = processLine();
		
		if (end == 1) //koniec pliku
			break;
	}
	
	clearAll();
	
	return 0;
}
