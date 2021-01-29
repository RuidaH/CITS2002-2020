//
//  q2.c
//  
//
//  Created by 何瑞达 on 13/8/20.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argcount, char *argvalue[])
{
    if (argcount != 4) 
    {
        printf("The number of the arguments is wrong!\n");
        exit(EXIT_FAILURE);
    }
    else 
    {
        int num1 = atoi( argvalue[1] );
        int num2 = atoi( argvalue[2] );
        int num3 = atoi( argvalue[3] );
        int temp;

        if ( num1 > num2 )
        {
            if ( num1 > num3 )
            {
                temp = num1;
            } 
            else
            {
                temp = num3;
            }
        }
        else
        {
            if ( num2 > num3 )
            {
                temp = num2;
            }
            else
            {
                temp = num3;
            }
        }
        
        printf("The maximum of 3 values is %i\n", temp);
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
}
