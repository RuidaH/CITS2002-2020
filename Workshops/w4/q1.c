//
//  q1.c
//  
//
//  Created by 何瑞达 on 13/8/20.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argcount, char *argvalue[])
{
    if ( argcount != 2 ) 
    {
        printf("You are putting the wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int year = atoi( argvalue[1] );

        if ( (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0) )
        {
            printf("It is a leap year\n");
        }
        else
        {
            printf("It is not a leap year\n");
        }

        exit(EXIT_SUCCESS); 
    }

    return 0;

}
