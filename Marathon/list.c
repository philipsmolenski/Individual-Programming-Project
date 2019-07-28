#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//Tworzy listę dwukierunkową. Jeżeli nie można zaalokować pamięci na beg 
//lub end to zwracana jest lista z NULLem na co najmniej jednym z tych 
//wskaźników.
List initList() {
    List result;
    result.beg = (Elem*)malloc(sizeof(Elem));
    result.end = (Elem*)malloc(sizeof(Elem));
    
    if (result.beg == NULL || result.end == NULL)
        return result;
    
    result.beg->prev = result.beg;
    result.beg->next = result.end;
    result.end->prev = result.beg;
    result.end->next = result.end;
    
    return result;
}

// wstawia element o wartości val po elemencie elem oraz zwraca wskaźnik
// na otrzymany element. Zwraca NULL gdy nie uda się zaalokować pamięci.
Elem *insert(Elem *elem, int val) {
    Elem *resultPtr = (Elem*)malloc(sizeof(Elem));
    
    if (resultPtr == NULL)
        return NULL;
    
    resultPtr->val = val;
    Elem *nex = elem->next;
    elem->next = resultPtr;
    resultPtr->next = nex;
    nex->prev = resultPtr;
    resultPtr->prev = elem;
    
    return resultPtr;
}

//Wstawia element o wartości v do posorowanej listy l. Zwraca 1 gdy element
//o wartości v jest już na liście oraz 2, gdy zabraknie pamięci. 
int insert_sort(List *l, int v) {
    Elem *ptr = l->beg;
    
    while (1) {
        if (ptr->next == l->end || ptr->next->val < v) {
            Elem *x = insert(ptr, v);
            
            if (x == NULL)
                return 2;
            
            return 0;
        }

        else if (ptr->next->val > v)
            ptr = ptr->next;
        
        else
            return 1;
    }
}

//Usuwa elementy listy l i zwalnia pamięć zajmowaną przez jej elementy.
void clearList(List *l) {
    for (Elem *i = l->beg; i != l->end;) {
        Elem *next = i->next;
        free(i);
        i = next;
    }
    
    free(l->end);
    l->beg = l->end = NULL;
}

//Łączy posortowane listy list1 i list2 w posortowaną listę lis o długości
//co najwyżej k. Jeśli nie uda się zaalokować pamięci zwraca listę, w której
//beg == NULL lub end == NULL.
List merge1(List *list1, List *list2, int k) {
    List lis = initList();
    
    if (lis.beg == NULL || lis.end == NULL)
        return lis;
    
    Elem *ptr1 = list1->beg->next;
    Elem *ptr2 = list2->beg->next;
    Elem *resultPtr = lis.beg;
    
    while (k > 0) {
        if (ptr1 == list1->end) {
            if (ptr2 == list2->end)
                return lis;
            
            else {
                resultPtr = insert(resultPtr, ptr2->val);
                
                if (resultPtr == NULL) {
                    clearList(&lis);
                    return lis;
                }
                
                ptr2 = ptr2->next;
                k--;
            }
        }
        
        else if (ptr2 == list2->end) {
            resultPtr = insert(resultPtr, ptr1->val);
            
            if (resultPtr == NULL) {
                clearList(&lis);
                return lis;
            }
            
            ptr1 = ptr1->next;
            k--;
        }
        
        else {
            if (ptr1->val > ptr2->val) {
                resultPtr = insert(resultPtr, ptr1->val);
                
                if (resultPtr == NULL) {
                    clearList(&lis);
                    return lis;
                }
                
                ptr1 = ptr1->next;
                k--;
            }
            
            else if (ptr2->val > ptr1->val) {
                resultPtr = insert(resultPtr, ptr2->val);
                
                if (resultPtr == NULL) {
                    clearList(&lis);
                    return lis;
                }
                
                ptr2 = ptr2->next;
                k--;
            }
            
            else {
                resultPtr = insert(resultPtr, ptr2->val);
                
                if (resultPtr == NULL) {
                    clearList(&lis);
                    return lis;
                }
                
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
                k--;
            }
        }
    }
    return lis;
}

//Łączy dwie posortowane listy  filmów prioritylist oraz list2 w jedną 
//posortowaną listę długości co najwyżej k w taki sposób, by każdy film z 
//listy l2 na liście wynikowej był lepszy od najlepszego filmu z listy
//prioritylist.
List merge2(List *priorityList, List *list2, int k) {
    List lis = initList();
    
    if (lis.beg == NULL || lis.end == NULL)
        return lis;
    
    Elem *resultPtr = lis.beg; //wskaźnik na listę wynikową
    Elem *priorityPtr = priorityList->beg->next;
    Elem *ptr2 = list2->beg->next;
    int czy = 0;
    
    while (k > 0) {
        if (priorityPtr == priorityList->end) {
            if (ptr2 == list2->end || czy)
                return lis;
            
            else {
                resultPtr = insert(resultPtr, ptr2->val);
                
                if (resultPtr == NULL) {
                    clearList(&lis);
                    return lis;
                }
                
                ptr2 = ptr2->next;
                k--; 
            }
        }
        
        else if (ptr2 == list2->end) {
            resultPtr = insert(resultPtr, priorityPtr->val);
            
            if (resultPtr == NULL) {
                clearList(&lis);
                return lis;
            }
            
            priorityPtr = priorityPtr->next;
            k--;
        }
        
        else if (czy) {
            resultPtr = insert(resultPtr, priorityPtr->val);
            
            if (resultPtr == NULL) {
                clearList(&lis);
                return lis;
            }
            
            priorityPtr = priorityPtr->next;
            k--;
        }
        
        else if (priorityPtr->val >= ptr2->val) {
            czy = 1;
            resultPtr = insert(resultPtr, priorityPtr->val);
            if (resultPtr == NULL) {
                clearList(&lis);
                return lis;
            }
            
            priorityPtr = priorityPtr->next;
            k--;
        }
        
        else {
            resultPtr = insert(resultPtr, ptr2->val);
            
            if (resultPtr == NULL) {
                clearList(&lis);
                return lis;
            }
            
            ptr2 = ptr2->next;            
            k--;
        }
    }
    
    return lis;
}


//Usuwa element o wartości v z listy l. Jeżeli na liście nie ma elementu o 
//wartości v funkcja zwraca 1.
int delElem(List *l, int v) {
    for (Elem *i = l->beg->next; i != l->end; i = i->next) {
        if (i->val == v) {
            i->next->prev = i->prev;
            i->prev->next = i->next;
            free(i);    
            
            return 0;
        }
    }
    
    return 1;
}

