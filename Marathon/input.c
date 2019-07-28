#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"

//obcina tablicę do inteksów z przedziału [pocz, kon)
char *cut(int pocz, int kon, char *tab) {
    char *result;
    result = (char*)malloc(sizeof(char) * (kon - pocz));
    
    if (result == NULL) {
        clearAll();
        exit(1);
    }
    
    for (int i = pocz; i < kon; i++) {
        result[i - pocz] = tab[i];
    }
    
    return result;
}

//odczytuje liczę z tablicy charów
long long int getInt(char *tab, int length) {
    long long int result = 0;
    
    if (length == 0) 
        return -1;
    
    for (int i = 0; i < length; i++) {
        char c = tab[i];
        int n = c - 48; 
        
        if(i == 12)
            return -1;

        if (n < 0 || n > 9)
            return -1;
        
        result = 10 * result + n;
    }
    
    return result;
}

//porównuje pierwsze length znaków w dwóch stringach
int equal(char *tab1, char *tab2, int length) {
    int result = 1;
    
    for (int i = 0; i < length; i++) {
        if (tab1[i] != tab2[i])
            result = 0;
    }
    
    return result;
}

//bierze tablicę z linią wczytanego tekstu wykonuje odpowiednią 
//operację a pliku marathon lub wypisuje "ERROR"
int process(char *tab) {
    if (equal(tab, "addUser ", 8)) {
        int space = 0;
        int end = 0;
        
        for (int i = 8; ; i++) {
            if (i == 29 || tab[i] == '\n') 
                return 1;
            
            else if (tab[i] == ' ') {
                space = i;
                break;
            }
        }
        
        for (int i = space + 1; ; i++) {
            if (i == 29)
                return 1;
            
            else if (tab[i] == '\n') {
                end = i;
                break;
            }
        }
        
        char *fstNumber = cut(8, space, tab);
        char *sndNumber = cut(space + 1, end, tab);
        long long int parentUserId = getInt(fstNumber, space - 8);
        long long int userId = getInt(sndNumber, end - space - 1);
        free(fstNumber);
        free(sndNumber);
        
        if (userId > MAX_TREE_SIZE - 1 || userId < 0 ||
                 parentUserId < 0 || parentUserId > MAX_TREE_SIZE - 1) 
            return 1;
        
        else 
            addUser((int)parentUserId, (int)userId);
    }
    
    else if (equal(tab, "delUser ", 8)) {
        int end = 0;
        
        for (int i = 8; ; i++) {
            if (i == 29)
                return 1;
            
            else if (tab[i] == '\n') {
                end = i;
                break;
            }
        }
        
        char *number = cut(8, end, tab);
        long long int userId = getInt(number, end - 8);
        free(number);
        
        if (userId > MAX_TREE_SIZE - 1 || userId < 0) 
            return 1;
        
        else 
            delUser((int)userId);
    }
    
    else {
        if (equal(tab, "addMovie ", 9)) {
            int space = 0;
            int end = 0;
            
            for (int i = 9; ; i++) {
                if (i == 29 || tab[i] == '\n') 
                    return 1;
                
                else if (tab[i] == ' ') {
                    space = i;
                    break;
                }
            }
            
            for (int i = space + 1; ; i++) {
                if (i == 29)
                    return 1;
                
                else if (tab[i] == '\n') {
                    end = i;
                    break;
                }
            }
            
            char *fstNumber = cut(9, space, tab);
            char *sndNumber = cut(space + 1, end, tab);
            long long int userId = getInt(fstNumber, space - 9);
            long long int movieRating = getInt(sndNumber, end - space - 1);
            free(fstNumber);
            free(sndNumber);
            
            if (userId > MAX_TREE_SIZE - 1 || userId < 0 ||
                    movieRating > MAX_K || movieRating < 0)
                return 1;
            
            else
                addMovie((int)userId, (int)movieRating);  
        }
        
        else if (equal(tab, "delMovie ", 9)) {
            int space = 0;
            int end = 0;
            
            for (int i = 9; ; i++) {
                if (i == 29 || tab[i] == '\n') 
                    return 1;
                
                else if (tab[i] == ' ') {
                    space = i;
                    break;
                }
            }
            
            for (int i = space + 1; ; i++) {
                if (i == 29)
                    return 1;
                
                else if (tab[i] == '\n') {
                    end = i;
                    break;
                }
            }
            
            char *fstNumber = cut(9, space, tab);
            char *sndNumber = cut(space + 1, end, tab);
            long long int userId = getInt(fstNumber, space - 9);
            long long int movieRating = getInt(sndNumber, end - space - 1);
            free(fstNumber);
            free(sndNumber);
            
            if (userId > MAX_TREE_SIZE - 1 || userId < 0 ||
                    movieRating > MAX_K || movieRating < 0)
                return 1;
            
            else
                delMovie((int)userId, (int)movieRating);
        }
        
        else if (equal(tab, "marathon ", 9)) {
            int space = 0;
            int end = 0;
            
            for (int i = 10; ; i++) {
                if (i == 29 || tab[i] == '\n') 
                    return 1;
                
                else if (tab[i] == ' ') {
                    space = i;
                    break;
                }
            }
            
            for (int i = space + 1; ; i++) {
                if (i == 29)
                    return 1;
                
                else if (tab[i] == '\n') {
                    end = i;
                    break;
                }
            }
            
            char *fstNumber = cut(9, space, tab);
            char *sndNumber = cut(space + 1, end, tab);
            long long int userId = getInt(fstNumber, space - 9);
            long long int k = getInt(sndNumber, end - space - 1);
            free(fstNumber);
            free(sndNumber);
            
            if (k > MAX_K || k < 0 ||
                userId > MAX_TREE_SIZE - 1 || userId < 0)
                return 1;
            
            else
                marathon((int)userId, (int)k);
        }
        
        else 
            return 1;     
    }

    return 0;
}

//wczytuje resztę błędnego wiersza
int goOn() {
    while (1) {
        char c = getchar();
        
        if (c == '\n')
            return 0;
        
        if (c == EOF)
            return 1;
    }
}

//przetwarza linię z wejścia, zwraca 1 gdy napotkaa koniec pliku.
int processLine() {
    char *tab = (char*)malloc(sizeof(char) * 30);
    
    if (tab == NULL) { 
        clearAll();
        exit(1);
    }
    
    int empty = 1;
    
    for (int i = 0; ; i++) {
        if (i == 29) {           
            if (empty == 0) 
                fprintf(stderr, "ERROR\n");

            int returncode = goOn();
            free(tab);
            
            return returncode;
        }
        
        else {
            char c = getchar();
            tab[i] = c;
            
            if (c == 10) {
                if (empty == 1) {
                    free(tab);
                    
                    return 0;
                }
                
                for (int j = i + 1; j < 10; j++)
                    tab[j] = 'x';

                int error = process(tab);
                
                if (error == 1)
                    fprintf(stderr, "ERROR\n");
                
                free(tab);
                
                return 0;
            }
            
            if (i == 0 && c == '#') {
                free(tab);
                goOn();
                
                return 0;
            }   
            
            if (c == EOF) {
                free(tab);
                
                if (i != 0) 
                    fprintf(stderr, "ERROR\n");
                
                return 1;
            }
            
            if (c != 32)
                empty = 0;
        }   
    }
}
