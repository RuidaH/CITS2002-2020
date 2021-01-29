#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include 'list.h'
#include 'support.h'

// -----------------------------------------------------

int main(int agrc, char* argv[]) {
    
    if ( argc > 1 ) {
        
        int NWORDS = atoi(argv[1]);
        long t0, t1;
        
        LISTITEM *list = new_list();
        
// ADD ITEMS
        srand(COMMON_SEED);
        
        t0  = microseconds();
        for (int i = 0; i < NWORDS; ++i) {
            list = add_list(list, random_word());
        }
        t1 = microseconds();
        
        printf("%s-add\t%i\t%lius\n", argv[0], NWORDS, (t1 - t0)/1000);
        
// FIND THE SAME ITEMS
        srand(COMMON_SEED);
        
        t0  = microseconds();
        for (int i = 0; i < NWORDS; ++i) {
            find_list(list, random_word());
        }
        t1 = microseconds();
        
        printf("%s-find\t%i\t%lius\n", argv[0], NWORDS, (t1 - t0)/1000);
        
     }
    
    return 0;
    
}
