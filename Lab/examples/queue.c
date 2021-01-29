#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _e {
    char                *data;
    unsigned int        length;
    struct _e           *next;
} ELEMENT;

typedef struct {
    ELEMENT             *head;
    ELEMENT             *tail;
    int                 count;
} QUEUE;


// --------------------------------------------------------------------

static QUEUE *new_queue(void)
{
    return calloc(1, sizeof(QUEUE));
}

static void print_element(ELEMENT *e)
{
    printf("%s (%u bytes)\n", e->data, e->length);
}

static void print_queue(QUEUE *q)
{
    ELEMENT     *e;

    printf("queue contents:\n");
    e   = q->head;
    while(e != NULL) {
        print_element(e);
        e       = e->next;
    }
}

// --------------------------------------------------------------------

static void add_to_queue(QUEUE *q, char *data, unsigned int length)
{
    ELEMENT     *newe;

    newe                = calloc(1, sizeof(ELEMENT));   // allocate space
    newe->data          = malloc(length);
    memcpy(newe->data, data, length);
    newe->length        = length;       // copy element's data fields
    newe->next          = NULL;         // nothing follows this element

    if(q->head == NULL) {               // is the queue empty?
        q->head         = newe;
        q->tail         = newe;
    }
    else {                              // no, just append to queue's tail
        q->tail->next   = newe;
        q->tail         = newe;
    }
    q->count++;                         // increment length of queue

    printf("  added: "); print_element(newe);
}


static void remove_from_queue(QUEUE *q)
{
    ELEMENT     *olde;

    if(q->head == NULL)
        return;                         // more correctly, a problem!

    olde        = q->head;              // remember element to be removed

    q->head     = q->head->next;        // advance head pointer
    if(q->tail == olde) {               // was this the only item on queue?
        q->head = NULL;
        q->tail = NULL;
    }
    q->count--;                         // decrement length of queue

//  AT THIS POINT WE COULD DO SOMETHING USEFUL WITH THIS olde ELEMENT
    printf("deleted: "); print_element(olde);

    free(olde->data);                   // deallocate storage of contents
    free(olde);                         // deallocate storage of element
}


// --------------------------------------------------------------------


static char *fruit[] = { "apple", "pineapple", "banana" , "tomato", "plum" };
#define NFRUITS         (sizeof(fruit) / sizeof(fruit[0]))

#define DEFAULT_MAX     10

int main(int argc, char *argv[])
{
    extern void srand(unsigned int seed);

    int         max = DEFAULT_MAX;
    char        *f;
    QUEUE       *q;

    if(argc > 1) {
        max     = atoi(argv[1]);
    }
    srand(time(NULL));			// seed the random number generator

    q   = new_queue();                  // allocation/initialize a new queue
    for(int n=0 ; n<max ; ++n) {	// add N random fruits to the queue
        f = fruit[rand()%NFRUITS];
        add_to_queue(q, f, strlen(f)+1);	// add 1 for null-byte
    }

    printf("\n");
    print_queue(q);
    printf("\n");

    for(int n=0 ; n<max ; ++n) {	// remove N fruits from queue
        remove_from_queue(q);
    }

    return 0;
}
