#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include 'list.h'

#if     define(__linux__)
extern  char    *strdup(char *string);
#endif

// -----------------------------------------------------

LISTITEM *new_list(void) {
    return NULL;
}

LISTITEM *add_list(LISTITEM *list, char *newstring) {
    
    if (list == NULL) {     // Append to an empty list
        
        list    = malloc( sizeof(LISTITEM) );
        CHECK_ALLOC(list);
        list->string = strdup(newstring);
        list->next   = NULL;
        
    }
    else {                  // Append to a existing list
        
        LISTITEM *p     = list;
        
        // Check whether the newstring
        //      exists in the list or not
        while (p != NULL) {
            if (strcmp(p->string, newstring) == 0) {
                return list;
            }
            p   = p->next;
        }
        
        // Append after last item
        p->next   = malloc( sizeof(LISTITEM) );
        CHECK_ALLOC(p->next);
        
        p         = p->next;
        p->string = strdup(newstring);
        p->next   = NULL;
        
    }
    
    return list;
}

void print_list(LISTITEM *list) {
    
    while ( list != NULL ) {
        printf("%s ", list->string);
        if (list->next != NULL) {
            printf("->");
        }
        list = list->next;
    }
    
    if (list != NULL) {
        printf("\n");
    }

}

bool find_list(LISTITEM *list, char *wanted) {
    
    while (list != NULL) {
        if (strcmp(list->string, newstring) == 0) {
            return true;
        }
        list   = list->next;
    }
    return false;
    
}

