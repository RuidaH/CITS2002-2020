#include <stdio.h>
#include <stdlib.h>

typedef struct _s {
    int         value;
    struct _s   *next;
}  STACKITEM;

STACKITEM    *stack = NULL;

void push_item(int newvalue)
{
    STACKITEM  *new = malloc( sizeof(STACKITEM) );  

    if(new == NULL) {     // check for insufficient memory   
        perror("push_item");
        exit(EXIT_FAILURE);
    }

    new->value   = newvalue;
    new->next    = stack;
    stack        = new;
}

int pop_item(void)
{
    STACKITEM  *old;
    int        oldvalue;

    if(stack == NULL) {
        fprintf(stderr, "attempt to pop from an empty stack\n");  
        exit(EXIT_FAILURE);
    }

    oldvalue     = stack->value;
    old          = stack;
    stack        = stack->next;
    free(old);

    return oldvalue;
}

void print_stack(void)
{
    STACKITEM  *thisitem = stack;

    while(thisitem != NULL) {
        printf("%d", thisitem->value);

        thisitem = thisitem->next;

        if(thisitem != NULL) {
            printf(" -> ");
	}
    }
    if(stack != NULL) {
	printf("\n");
    }
}

int main(int argc, char *argv[])
{
    for(int i=1 ; i<=10 ; ++i) {
	push_item(i);
    }
    print_stack();

    pop_item();	print_stack();

    pop_item();	print_stack();

    pop_item();	print_stack();

    return 0;
}
