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


void replace( char oldword[], char newword[], char whole_sentence[] ) {

    int len1 = strlen(whole_sentence);
    int len2 = strlen(newword);
    int len3 = strlen(oldword);
    
    int len = len1;
    
    char temp[ 2 * len1 ];
    int temp_index = 0;
    bool flag;
    
    for (int i = 0; i < len1; i++) {
        
        if (whole_sentence[i] == oldword[0]) {
            
            flag = true;
            
            // Check if the whole oldword matches the characters in the whole sentence
            for (int k = 0; k < len3; k++) {
                if (oldword[k] != whole_sentence[i + k]) {
                    flag = false;
                }
            }
            
            // If they matched
            if (flag == true) {
                // Append the new word array to the temp
                for (int a = 0; a < len2; a++) {
                    temp[temp_index] = newword[a];
                    temp_index++;
                }
                // Adjusting the length of the final result.
                len = len - len3 + len2;
                
                i = i + len3 - 1;
                
            }
            // If they the whole old word doesn't match, only copy the first word
            else {
                temp[temp_index] = whole_sentence[i];
                temp_index++;
            }
        }
        // If they are not matched at all
        else {
            temp[temp_index] = whole_sentence[i];
            temp_index++;
        }
    }
    temp[temp_index] = '\0';
    
    char result[ len + 1 ];
    
    for (int i = 0; i < (len + 1); i++ ){
        result[i] = temp[i];
    }

    printf("The final result is : %s\n", result);

}



int main( int argc, char *argv[] ) {
    
    if ( argc < 4 ) {
        
        fprintf( stderr, "Usage : %s oldword newword whole_sentence \n", argv[0] );
        exit( EXIT_FAILURE );
        
    }
    else {
        
        replace( argv[1], argv[2], argv[3] );
        
        exit( EXIT_SUCCESS );
        
    }
    
    return 0;
}
