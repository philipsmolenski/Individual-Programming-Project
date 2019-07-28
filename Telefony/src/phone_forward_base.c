/** @file
 * Implementacja interfejsu klasy przechowywującej
 * operacje na bazach.
 *
 * @author Philip Smolenski-Jensen
 */

#include <stdio.h>
#include <string.h>
#include "phone_forward.h"
#include "phone_forward_base.h"

Head * newHead() {
	Head *h = (Head*)malloc(sizeof(Head));

	if (h == NULL)
		return NULL;

	h->base = (Base*)malloc(sizeof(Base) * MAX_BASE_SIZE);

	if (h->base == NULL) {
		free(h);
		return NULL;
	}

	for (int i = 0; i < MAX_BASE_SIZE; i++) {
		(h->base + i)->pf = NULL;
		(h->base + i)->name = NULL;
	}

	h->recent = NONE;

	return h;
}

void clearAll(Head *h) {
	for (int i = 0; i < MAX_BASE_SIZE; i++) 
		if ((h->base + i)->pf != NULL) {
			phfwdDelete((h->base + i)->pf);
			free((h->base + i)->name);
		}

	free(h->base);
	free(h);
}

bool newBase(Head *h, char const *name) {
	for (int i = 0; i < MAX_BASE_SIZE; i++) {
		if ((h->base + i)->pf != NULL 
				&& strcmp((h->base + i)->name, name) == 0) {
			h->recent = i;

			return true;
		}
	}

	for (int i = 0; i < MAX_BASE_SIZE; i ++) {
		if ((h->base + i)->pf == NULL) {
			(h->base + i)->pf = phfwdNew();
			
			if ((h->base + i)->pf == NULL)
				return false;

			int n = strlen(name) + 1;
			(h->base + i)->name = (char*)malloc(sizeof(char) * n);

			if ((h->base + i)->name == NULL)
				return false; 

			strcpy((h->base + i)->name, name);
			h->recent = i;

			return true;
		}
	}
	return false;
}

bool delBase(Head *h, char const *name) {
	for (int i = 0; i < MAX_BASE_SIZE; i++) {
		if ((h->base + i)->pf != NULL 
				&& strcmp((h->base + i)->name, name) == 0) {
			
			phfwdDelete((h->base + i)->pf);
			free((h->base + i)->name);
			(h->base + i)->pf = NULL;
			(h->base + i)->name = NULL;

			if (h->recent == i)
				h->recent = NONE;
			return true;
		}
	}
	return false;
}