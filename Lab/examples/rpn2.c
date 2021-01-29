#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

//  ----------------------------------------------------------------

int evaluate_RPN(FILE *fp)
{
    char  line[BUFSIZ];

    while( fgets(line, sizeof line, fp) != NULL ) {
        if(line[0] == '#') {
            continue;
	}
        if(isdigit(line[0]) || line[0] == '-') {
            push_item( atoi(line) );
	}
        else if(line[0] == 'a') {
            push_item( pop_item() + pop_item() );
	}
        else if(line[0] == 'd') {
            int	val1 = pop_item();
            int	val2 = pop_item();

            push_item( val2 / val1 );
        }
    }
    return pop_item();
}

int main(int argc, char *argv[])
{
    printf("%d\n", evaluate_RPN( stdin ) );
    return 0;
}
