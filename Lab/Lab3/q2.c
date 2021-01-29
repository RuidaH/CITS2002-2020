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

bool is_safe( char s[] ) {
    
    int uppercase = 0;
    int lowercase = 0;
    int digits = 0;
    
    int length = strlen(s);
    
    for ( int i = 0; i < length; i++ ) {
        
        if ( isupper(s[i]) ) {
            uppercase ++;
        }
        else if ( islower(s[i]) ) {
            lowercase ++;
        }
        else if ( isdigit(s[i]) ) {
            digits ++;
        }
        
    }
    
    bool flag = false;
    
    if ( lowercase >= 2 && uppercase >= 2 && digits >= 2 ) {
        flag = true;
    }

    return flag;
}

int main( int argc, char *argv[] ) {
    
    if ( argc < 2 ) {
        
        fprintf( stderr, "Usage : %s argument \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        bool answer = is_safe( argv[1] );
        
        // printf( "Is this password safe?  %d \n", answer );
        
        printf( "Is the password safe ? %s \n", answer ? "true" : "false" );
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
