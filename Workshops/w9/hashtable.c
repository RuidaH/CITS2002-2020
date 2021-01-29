#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define    HASHTABLE_SIZE        997

//  --------------------------------------------------------------------

//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT
//
//  see:  https://en.cppreference.com/w/c/types/integer

uint32_t hash_string(char *string) {
    
    uint32_t hash = 0;
    
    while(*string != '\0') {
        hash = hash * 33 + *string;
        ++string;
    }
    
    return hash;
    
}


HASHTABLE *new_hashtable(void) {
    
    HASHTABLE *new = malloc(HASHTABLE_SIZE * sizeof(LISTITEM *));
    
    CHECK_ALLOC(new);
    return new;
    
}

void add_hashtable(HASHTABLE *hashtable, char *string) {
    
    uint32_t h = hash_string(string) % HASHTABLE_SIZE;
    
    hashtable[h] = add_list(hashtable[h], string);
    
}

bool find_hashtable(HASHTABLE *hashtable, char *string) {
    
    uint32_t h = hash_string(string) % HASHTABLE_SIZE;
    
    return find_list(hashtable[h], string);
    
}



