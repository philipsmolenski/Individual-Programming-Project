#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_SIZE 65536

//Typ drzewa, w którym, dzieci reprezentowane są przez listę nodów
typedef struct NodeElem NodeElem;

typedef struct NodeList {
	NodeElem *beg, *end;
} NodeList;

//Typ reprezentujący wierzchołek w drzewie
typedef struct Node {
	List movies;
	NodeList children;
	int id;
	NodeElem *wsk;
} Node;

//Typ reprezentujący drzewo w postaci tablicy wskaźników
typedef struct Tree {
	Node *tab[MAX_TREE_SIZE];
} Tree;

//Elementy listy przechowującej wierzchołki.
typedef struct NodeElem {
	Node *node;
	struct NodeElem *prev, *next;
} NodeElem;

//Tworzy nowy wierzchołek, gdy zabraknie pamięci zwraca NULL.
Node *newNode(int nid);

Tree initTree();

//Zwraca end dla pustej listy.
NodeElem *getNodeFirst(NodeList *l);

//Zwraca end dla pustej listy.
NodeElem *getNodeLast(NodeList *l);

//Dodanie wierzchołka o numerze nr jako dziecko wierzchołka nod
//do drzewa t. Gdy wierzchołek jest już dodany lub ojciec nie istnieje
//zwracane jest 1. Gdy nie uda się zaalokować pamięci funkcja zwraca 2.
int add(int nod, int nr, Tree *t);

//Usuwanie wierzchołka z drzewa. Jeżeli usuwanego wierzchołka nie było 
//funkcja zwraca 1.
int del(int nod, Tree *t);

//Gdy zabraknie pamięci zwracana jest lista, 
//w której beg == NULL lub end == NULL.
NodeList initNodeList();

//Wstawia wierzchołek node po wierzchołku nod. 
//Gdy zabraknie pamięci zwraca NULL.
NodeElem *insertNode(NodeElem *nod, Node *node);

//Wstawia listę l w miejsce elementu nod. (nod != beg, nod != end).
void insertNodeList(NodeElem *nod, NodeList *l);

//Usuwa elementy listy wierzchołków l i zwalnia pamięć zajmowaną przez nie.
void clearNodeList(NodeList *l);

//Zwalania pamięć zajmowaną przez poddrzewo zakorzenione w t->tab[nr].
void clearTree (Tree *t, int nr);
