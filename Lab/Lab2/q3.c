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
    if (argcount < 2) 
    {
        printf("You must input arguments\n");
        exit(EXIT_FAILURE);
    }
    else 
    {
        int temp = 0;
        
        for ( int i = 1; i < argcount; i++ )
        {
            if ( temp < atoi(argvalue[i]) )
            {
                temp = atoi(argvalue[i]);
            }
        }

        printf("The maximum of %i values is %i\n", argcount - 1, temp);
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
}
