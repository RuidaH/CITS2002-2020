//
//  q1.c
//  
//
//  Created by 何瑞达 on 13/8/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argcount, char *argvalue[])
{
    if ( argcount != 2 ) 
    {
        printf("You are putting the wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        char card_num = **argvalue[1];
        int s1 = 0;
        int s2 = 0;

        for ( int i = strlen(card_num) - 1; i >= 0; i = i - 2)
        {
            s1 = s1 + atoi(card_num[i]);
        }

        for ( int k = strlen(card_num) - 2; k >= 0; k = k - 2)
        {
            int temp = atoi(card_num[k]) * 2;
            s2 = s2 + temp;
        }

        if ( (s1 + s2) % 10 == 0)
        {
            printf("The original number is a credit card number\n");
        }
        else
        {
            printf("The original number is not a credit card number\n");
        }
        exit(EXIT_SUCCESS); 
    }

    return 0;

}
