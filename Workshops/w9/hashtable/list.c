#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

//  ---------------------------------------------------------------------

LIST *new_list(void)
{
    return NULL;
}

LIST *add_list(LIST *list, char *newstring)
{
    if(list == NULL) {                          // append to empty list   
        list          = malloc( sizeof(LIST) );
        CHECK_ALLOC(list);
        list->string  =  strdup(newstring);
        list->next    =  NULL;
    }
    else {                                      // append to existing list
        LIST *p = list;

        while(p->next != NULL) {                // walk to end of list  
	    if(strcmp(p->string, newstring) == 0) { // already exists!
		return list;
	    }
            p  =  p->next;
        }
        p->next     = malloc( sizeof(LIST) );
        CHECK_ALLOC(p->next);
        p           =  p->next;                  // append after last item
        p->string   =  strdup(newstring);
        p->next     =  NULL;
    }
    return list;
}

void print_list(LIST *list)
{
    while(list != NULL) {
	printf("%s", list->string);
	if(list->next != NULL) {
	    printf(" -> ");
        }
	list	= list->next;
    }
    if(list != NULL) {
	printf("\n");
    }
}

bool find_list(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->string, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}
