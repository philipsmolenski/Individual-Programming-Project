#include "marathon.h"
#include <stdio.h>

Tree tree;

Tree init() {
    tree = initTree();
    
    return tree;
}

//Zwalnia całą pamięć zajmowaną przez program.
void clearAll() {
    clearTree(&tree, 0);
}

//implementacja operacji z zadania.
void addUser(int parentUserId, int userId) {
    int c = add(parentUserId, userId, &tree);
    
    if (c == 2) {        //gdy nie powiodła się alokacja pamięci
        clearAll();
        exit(1);
    }
    
    if (c == 1)          //gdy user już jest w drzewie lub nie ma jego rodzica 
        fprintf(stderr, "ERROR\n");
    
    else
        printf("OK\n");
}

void delUser(int userId) {
    if (userId == 0)   
        fprintf(stderr, "ERROR\n");
   
    else {
        int c = del(userId, &tree);
        
        if (c == 1)      //gdy chcemy usunąć nieistniejący wierzchołek.
            fprintf(stderr, "ERROR\n");
        
        else
            printf("OK\n");
    }
}

void addMovie(int userId, int movieRating) {
    if (tree.tab[userId] == NULL) //gdy chcemy dodać film do nieistniej osoby
        fprintf(stderr, "ERROR\n");
    
    else {
        List *l = &(tree.tab[userId]->movies);
        int c = insert_sort(l, movieRating);
        
        if (c == 2) {   //gdy nie uda się alokacja pamięci
            clearAll();
            exit(1);
        }
        
        if (c == 1)   //gdy chcemy dodać istniejący już film
            fprintf(stderr, "ERROR\n");
        
        else
            printf("OK\n");
    }
}

void delMovie(int userId, int movieRating) {
    if (tree.tab[userId] == NULL) {  //gdy nie ma użytkownika
        fprintf(stderr, "ERROR\n");
        
        return;
    }
    
    List *l = &(tree.tab[userId]->movies);
    int c = delElem(l, movieRating);
    
    if (c == 1)      //gdy chcemu usunąć nieistniejący film
        fprintf(stderr, "ERROR\n");
    
    else
        printf("OK\n");
}


//Wyliczone rekurencyjnie listy od dzieci łączymy z listą wynikową używając
//merge1, a następnie łączymy z filmami użytkownika używając merge2.
List marathonlist(int userId, int k) {
    NodeList *l = &(tree.tab[userId]->children);
    List lst = initList();
    
    if (lst.beg == NULL || lst.end == NULL) { //nieudana próba alokacji
        clearAll();
        exit(1);
    }
    
    for (NodeElem *i = l->beg->next; i != l->end; i = i->next) {
        List lst2 = marathonlist(i->node->id, k);
        List lst3 = merge1(&lst, &lst2, k);
        
        if (lst3.beg == NULL || lst3.end == NULL) { //nieudana próba 
            clearAll();                            //alokacji w mergu
            exit(1);
        }
        
        clearList(&lst);
        clearList(&lst2);
        lst = lst3;
    }
    
    List lst2 = merge2(&(tree.tab[userId]->movies), &lst, k);
    
    if (lst2.beg == NULL || lst2.end == NULL) { //nieudana próba 
        clearAll();                             //alokacji w mergu2
        exit(1);
    }
    
    clearList(&lst);
    
    return lst2;
}

 
void marathon(int userId, int k) {
    if (tree.tab[userId] == NULL) {
        fprintf(stderr, "ERROR\n");
        
        return;
    }
    
    List lst = marathonlist(userId, k);
    
    if (lst.beg == NULL || lst.beg->next == lst.end)
        printf("NONE\n");
    
    else {
        for (Elem *i = lst.beg->next; i != lst.end; i = i->next) {
            printf("%d",i->val);
            
            if (i->next != lst.end)
                printf(" ");
        }
        
        printf("\n");
    }
    
    clearList(&lst);
}
