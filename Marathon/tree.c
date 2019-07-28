#include "tree.h"

//Tworzy nowy wierzchołek, gdy zabraknie pamięci zwraca NULL.
Node *newNode(int nid) {
    Node *nod = (Node*)malloc(sizeof(Node));
    
    if (nod == NULL)
        return NULL;
    
    nod->id = nid;
    NodeList children = initNodeList();
    
    if (children.beg == NULL || children.end == NULL)
        return NULL;
    
    nod->children = children;
    List movies = initList();
    
    if (movies.beg == NULL || movies.end == NULL)
        return NULL;
    
    nod->movies = movies;
    
    return nod;
}

Tree initTree() {
    Tree t;
    t.tab[0] = newNode(0);
    
    for (int i = 1; i < MAX_TREE_SIZE; i++)
        t.tab[i] = NULL;
    return t;
}

//Zwraca end dla pustej listy.
NodeElem *getNodeFirst(NodeList *l) {
    return l->beg->next;
}

//Zwraca end dla pustej listy.
NodeElem *getNodeLast(NodeList *l) {
    return l->end->prev;
}

//Dodanie wierzchołka o numerze nr jako dziecko wierzchołka nod
//do drzewa t. Gdy wierzchołek jest już dodany lub ojciec nie istnieje
//zwracane jest 1. Gdy nie uda się zaalokować pamięci funkcja zwraca 2.
int add(int nod, int nr, Tree *t) {
    if (t->tab[nr] != NULL || t->tab[nod] == NULL)
        return 1;
    
    Node *node = newNode(nr);
    
    if (node == NULL)
        return 2;
    
    t->tab[nr] = node;
    Node *ptr = t->tab[nod];
    NodeElem *ost = getNodeLast(&(ptr->children));
    NodeElem *wsk = insertNode(ost, node);
    
    if (wsk == NULL)
        return 2;
    
    node->wsk = wsk;
    
    return 0;
}

//Usuwanie wierzchołka z drzewa. Jeżeli usuwanego wierzchołka nie było 
//funkcja zwraca 1.
int del(int nod, Tree *t) {
    if (t->tab[nod] == NULL)
        return 1;
    
    else { 
        Node *node = t->tab[nod];
        insertNodeList(node->wsk, &(node->children));
        clearList(&(node->movies));
        free(node);
        t->tab[nod] = NULL;
        
        return 0;
    }
}

//Gdy zabraknie pamięci zwracana jest lista, 
//w której beg == NULL lub end == NULL.
NodeList initNodeList() {
    NodeList result;
    result.beg = (NodeElem*)malloc(sizeof(NodeElem));
    result.end = (NodeElem*)malloc(sizeof(NodeElem));
    
    if (result.beg == NULL || result.end == NULL)
        return result;
    
    result.beg->prev = result.beg;
    result.beg->next = result.end;
    result.end->prev = result.beg;
    result.end->next = result.end;
    
    return result;
}

//Wstawia wierzchołek node po wierzchołku nod. 
//Gdy zabraknie pamięci zwraca NULL.
NodeElem *insertNode(NodeElem *nod, Node *node) {
    NodeElem *resultPtr = (NodeElem*)malloc(sizeof(NodeElem));
    
    if (resultPtr == NULL)
        return NULL;
    
    resultPtr->node = node;
    NodeElem *nex = nod->next;
    nod->next = resultPtr;
    resultPtr->next = nex;
    nex->prev = resultPtr;
    resultPtr->prev = nod;
    
    return resultPtr;
}


//Wstawia listę l w miejsce elementu nod. (nod != beg, nod != end).
void insertNodeList(NodeElem *nod, NodeList *l) {
    NodeElem *prev = nod->prev;
    NodeElem *next = nod->next;
    free(nod);
    
    if (l->beg->next == l->end) {
        prev->next = next;
        next->prev = prev;
    }
    
    else {
        NodeElem *fst = getNodeFirst(l);
        NodeElem *lst = getNodeLast(l);
        prev->next = fst;
        fst->prev = prev;
        lst->next = next;
        next->prev = lst;
    }
    
    free(l->beg);
    free(l->end);
}

//Usuwa elementy listy wierzchołków l i zwalnia pamięć zajmowaną przez nie.
void clearNodeList(NodeList *l) {
    for (NodeElem *i = l->beg; i != l->end;) {
        NodeElem *next = i->next;
        free(i);
        i = next;
    }
    
    free(l->end);
    l->beg = l->end = NULL;
}

//Zwalania pamięć zajmowaną przez poddrzewo zakorzenione w t->tab[nr].
void clearTree (Tree *t, int nr) {
    Node *nod = t->tab[nr];
    NodeList *lst = &(nod->children);
    
    for (NodeElem *i = lst->beg->next; i != lst->end; i = i->next) {
        clearTree (t, i->node->id);
    }
    
    clearList(&(nod->movies));
    clearNodeList(&(nod->children));
    free(t->tab[nr]);
    t->tab[nr] = NULL;

}