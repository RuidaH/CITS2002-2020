//
//  q1.c
//  
//
//  Created by 何瑞达 on 18/8/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define N   10

void random_num_generation( int size, int s[] ) {

    srand(time(NULL));
    
    int temp = 0;
    int max_index = 0;

    for ( int i = 0; i < size; i++ ) {
        // Get random numbers from 0 to 49
        s[i] = rand() % 50;
        
        if (temp < s[i]) {
            temp = s[i];
            max_index = i;
        }
    }
    
    printf("The largest value of these random numbers is : %d\n", temp);
    
    // Putting the largest value in the index 0, and push all other values down in the array
    for (int j = max_index; j > 0; j--) {
        s[j] = s[j - 1];
    }
    
    s[0] = temp;

}



int main( int argc, char *argv[] ) {
    
    if ( argc < 1 ) {
        
        fprintf( stderr, "Usage : %s argument1 \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        int s[ N ];
        
        random_num_generation( N, s );
        
        for ( int i = 0; i < N; i++ ) {
            printf("%d  ", s[i] );
        }
        printf("\n");
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
