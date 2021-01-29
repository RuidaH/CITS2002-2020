#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

typedef struct _l {
    
    char        *string;
    struct _l   *next;
    
} LISTITEM;

extern LISTITEM *new_list(void);

extern LISTITEM *add_list(LISTITEM *list, char *newstring);

extern bool     find_list(LISTITEM *list, char *wanted);

extern void     print_list(LISTITEM *list);



