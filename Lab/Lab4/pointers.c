//
//  pointers.c
//  
//
//  Created by 何瑞达 on 27/9/20.
//

#include "Declaration.h"


/*
 Concatenate strings
    1. The strcat() append a copy of the null-terminated string s2 to the end of the null-terminated string s1, then add a terminating '\0'
    2. The string s1 must have sufficient space to hold the result.
 */
char *strcat_p(char *s1, char *s2) {
    
    // Need a copy because we need to know the starting position
    char *copy = s1;
    
    while ( *s1 != '\0' ) {
        s1++;
    }
    while ( *s2 != '\0' ) {
        *s1 = *s2;
        s1++;
        s2++;
    }
    
    *s1 = '\0';
    
    return copy;
    
}

/*
 Compare the strings
    1. The function lexicographically compare the null-terminated strings s1 and s2
    2. return a integer greater than 0 --> s1 is greater than s2
                        equal to 0 ------> s1 is equal to s2
                        less than 0 -----> s1 is less than s2
 */
int strcmp_p(char *s1, char *s2) {
    
    int i = 0;
    
    while (*s1 == *s2 && *s2 != '\0') {
        ++s1;
        ++s2;
    }
    
    // You don't need to turn it to int, it will do that automatically
    //      when you do the arithmetic
    return (*s1) - (*s2);
    
}


// Compare the first n characte of s1 and s2
int strncmp_p(char *s1, char *s2, int n) {
    
    while (*s1 == *s2 && *s2 != '\0' && n > 0) {
        ++s1;
        ++s2;
        --n;
    }
    
    // You don't need to turn it to int, it will do that automatically
    //      when you do the arithmetic
    return (*s1) - (*s2);
    
}


/*
 Locate character in string
    1. locate the first occurrence of c (converted to a char) in the string
    2. The terminating null character is considered to be the part of the string
    3. return a pointer to the located character, or NULL if the character does not appear in the string
 */
char *strchr_p(char *s, int c) {
    
    while ( *s != c && *s != '\0' ) {
        s++;
    }
    
    if (*s != '\0') {
        return s;
    }
    else {
        return NULL;
    }
    
}


// It locates the last occurrence of c
char *strrchr_p(char *s, int c) {

    char * found = NULL;
    
    while (*s != '\0') {
        
        if (*s == c) {
            found = s;
        }
        
        s++;
        
    }
    
    return found;
    
}


