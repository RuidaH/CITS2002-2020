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


void replace( char oldword[], char newword[], char* whole_sentence[] ) {

    int len1 = strlen(*whole_sentence);
    int len2 = strlen(newword);
    int len3 = strlen(oldword);
    
    int len = len1;
    
    char temp[ 2 * len1 ];
    int temp_index = 0;
    int flag;
    
    char whole_sentence1[ len1 ];
    
    for (int i = 0; i < len1; i++) {
        whole_sentence1[i] = *whole_sentence[i];
    }
    
    
    for (int i = 0; i < len1; i++) {
        
        if (whole_sentence1[i] == oldword[0]) {
            
            flag = i;
            
            // Check if the whole oldword matches the characters in the whole sentence
            for (int k = 0; k < len3; k++) {
                if (oldword[k] == whole_sentence1[i]) {
                    i++;
                }
            }
            
            if (i == flag + len3) {
                
                for (int a = 0; a < len2; a++) {
                    temp[temp_index] = newword[a];
                    temp_index++;
                }
                
                len = len - len3 + len2;
                
            }
            else {
                temp[temp_index] = whole_sentence1[i];
                temp_index++;
            }
            
        }
        else {
            
            temp[temp_index] = whole_sentence1[i];
            temp_index++;
            
        }
    }
    
    // Reallocate memory of whole_sentence
    whole_sentence = (char *)realloc(whole_sentence, len);
    
    for (int i = 0; i < len; i++) {
        *whole_sentence[i] = temp[i];
    }

}



int main( int argc, char *argv[] ) {
    
    if ( argc < 4 ) {
        
        fprintf( stderr, "Usage : %s oldword newword whole_sentence \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        replace( argv[1], argv[2], &argv[3] );
        
        printf("The new whole sentence is %s\n", argv[3]);
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
