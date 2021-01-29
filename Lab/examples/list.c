#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if      defined(__linux__)
extern	char	*strdup(const char *str);
#endif

typedef struct _l {
     char        *string;
     struct _l   *next;
} LISTITEM;

LISTITEM   *list  =  NULL;

static void append_item(char *newstring)
{
    if(list == NULL) {           // append to an empty list   
        list = malloc( sizeof(LISTITEM) );
        if(list == NULL) {
            perror("append_item");
            exit(EXIT_FAILURE);
        }
        list->string  =  strdup(newstring);
        list->next    =  NULL;
    }
    else {                       // append to an existing list
        LISTITEM *p = list;

        while(p->next != NULL) { // walk to the end of the list  
            p  =  p->next;
        }
        p->next = malloc( sizeof(LISTITEM) );
        if(p->next == NULL) {
            perror("append_item");
            exit(EXIT_FAILURE);
        }
        p          =  p->next;   // append after the last item
        p->string  =  strdup(newstring);
        p->next    =  NULL;
    }
}

void print_list(void)
{
    LISTITEM	*p = list;

    while(p != NULL) {
	printf("%s", p->string);
	if(p->next != NULL)
	    printf(" -> ");
	p	= p->next;
    }
    if(list != NULL) {
	printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc > 1) {
	int	n = atoi(argv[1]);

	for(int i=0 ; i<n ; ++i) {
	    char num[10];

	    sprintf(num, "%i", i);
	    append_item(num);
	}
	print_list();
    }
    return 0;
}
