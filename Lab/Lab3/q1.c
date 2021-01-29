//
//  q1.c
//  
//
//  Created by 何瑞达 on 18/8/20.
//

#include <stdio.h>
#include <stdlib.h>

int my_Strlen( char s[] ) {
    
    int i = 0;
    
    while ( s[i] != '\0') {
        i = i + 1;
    }
    
    return i;
}

int main( int argc, char *argv[] ) {
    
    if ( argc < 2 ) {
        
        fprintf( stderr, "Usage : %s argument \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        int answer = my_Strlen( argv[1] );
        
        printf( "The answer is %d\n", answer );
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
