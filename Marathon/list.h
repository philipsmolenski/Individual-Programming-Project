#pragma once

//typ listy dwukierunkowej
typedef struct Elem {
	int val;
	struct Elem *prev, *next;
} Elem;


typedef struct List {
	Elem *beg, *end;
} List;

//Zwraca pierwszy element listy (end w przypadku pustej).
Elem *getFirst(List *l);

//Zwraca ostatni element listy (beg w przypadku pustej).
Elem *getLast(List *l);

//Tworzy listę dwukierunkową. Jeżeli nie można zaalokować pamięci na beg 
//lub end to zwracana jest lista z NULLem na co najmniej jednym z tych 
//wskaźników.
List initList();

// wstawia element o wartości val po elemencie elem oraz zwraca wskaźnik
// na otrzymany element. Zwraca NULL gdy nie uda się zaalokować pamięci.
Elem *insert(Elem *nod, int val);

//Wstawia element o wartości v do posorowanej listy l. Zwraca 1 gdy element
//o wartości v jest już na liście oraz 2, gdy zabraknie pamięci. 
int insert_sort(List *l, int v);

//Usuwa elementy listy l i zwalnia pamięć zajmowaną przez jej elementy.
void clearList(List *l);

//Łączy posortowane listy list1 i list2 w posortowaną listę lis o długości
//co najwyżej k. Jeśli nie uda się zaalokować pamięci zwraca listę, w której
//beg == NULL lub end == NULL.
List merge1(List *list1, List *list2, int k);

//Łączy dwie posortowane listy  filmów prioritylist oraz list2 w jedną 
//posortowaną listę długości co najwyżej k w taki sposób, by każdy film z 
//listy l2 na liście wynikowej był lepszy od najlepszego filmu z listy
//prioritylist.
List merge2(List *Prioritylist, List *list2, int k);

//Usuwa element o wartości v z listy l. Jeżeli na liście nie ma elementu o 
//wartości v funkcja zwraca 1.
int delElem(List *l, int v);