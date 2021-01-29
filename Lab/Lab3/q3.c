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

int my_strcmp( char s1[], char s2[] ) {

    
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int i = 0;
    
    while ( i < len1 && i <= len2 ) {
        
        if ( s1[i] > s2[i] ) {
            return 1;
        }
        else if ( s1[i] < s2[i] ) {
            return -1;
        }
        i++;

    }
    
    // This indicate one string is longer
    //    when the rest of parts are equal
    if ( len1 != len2 ) {
        return ((len1 > len2) ? 1 : -1);
    }
    
    return 0;
    
}

int main( int argc, char *argv[] ) {
    
    if ( argc < 3 ) {
        
        fprintf( stderr, "Usage : %s argument1, argument2 \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        int answer = my_strcmp( argv[1], argv[2] );
        
        printf("1  : s1 is greater than s2.\n");
        printf("0  : s1 is equal to s2.\n");
        printf("-1 : s1 is smaller than s2.\n");
        
        printf( "Is the first string greater than the second string?  %d \n", answer );
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
