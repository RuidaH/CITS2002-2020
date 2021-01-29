#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "support.h"

//  --------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if(argc > 1) {
	int	NWORDS          = atoi(argv[1]);
	long	t0, t1;

        HASHTABLE *hashtable    = new_hashtable();

//  ADD RANDOM WORDS TO OUR HASHTABLE
	srand(COMMON_SEED);

	t0	= microseconds();
	for(int i=0 ; i<NWORDS ; ++i) {
	    add_hashtable(hashtable, random_word() );
	}
	t1	= microseconds();

	printf("%s-add\t%i\t%lims\n", argv[0], NWORDS, (t1 - t0)/1000);

//  FIND THE SAME WORDS
	srand(COMMON_SEED);

	t0	= microseconds();
	for(int i=0 ; i<NWORDS ; ++i) {
	    find_hashtable(hashtable, random_word() );
	}
	t1	= microseconds();

	printf("%s-find\t%i\t%lims\n", argv[0], NWORDS, (t1 - t0)/1000);
    }
    return 0;
}
