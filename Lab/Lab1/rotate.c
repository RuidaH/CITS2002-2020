//
//  rotate.c
//  
//
//  Created by 何瑞达 on 6/8/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Compile this program with:
//      cc -std=c99 -Wall -Werror -o rotate rotate.c

// #define ROT 3

//  The rotate function returns the character ROT positions further along the
//  alphabetic character sequence from c, or c if c is not lower-case

char rotate(char c, int ROT)
{
        // Check if c is lower-case or not
        if (islower(c))
        {
                // The ciphered character is ROT positions beyond c,
                // allowing for wrap-around
                return ('a' + (c - 'a' + ROT) % 26);
        }
        else if (isupper(c))
        {
            return ('A' + (c - 'A' + ROT) % 26);
        }
        else
        {
                return c;
        }
}

//  Execution of the whole program begins at the main function

int main(int argcount, char *argvalue[])
{
    
        int ROT1 = atoi(argvalue[1]);
        
        // Exit with an error if the number of arguments (including
        // the name of the executable) is not precisely 2 ---> 3
        if(argcount < 3)
        {
                fprintf(stderr, "%s: program expected at least 2 argument, but instead received %d\n",
                    argvalue[0], argcount-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            for(int k = 2; k < argcount; k++)
            {
                    // Define a variable for a later loop
                    int i;
                    // Calculate the length of the first argument
                    int length = strlen(argvalue[k]);

                    // Loop for every character in the text
                    for(i = 0; i < length; i++)
                    {
                            // Determine and print the ciphered character
                            printf("The plaintext is: %c and the position is %d \t", argvalue[k][i], i);
                            printf("And the corresponding cipher text is %c and the position is %d \n", rotate(argvalue[k][i], ROT1), (i + ROT1) % 26);
                    }

                    // Print one final new-line character
                    printf("\n");

            }
            
            // Exit indicating success
            exit(EXIT_SUCCESS);
        }
        return 0;
}
