//
//  goodchessmove.c
//  
//
//  Created by 何瑞达 on 30/9/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// This is the child program that we're gonna call multiple time
// We don't care about the computation and the result
int main(int argc, char *argv[]) {
    
    if (argc == 3) {
        
        printf("\thello from program %s, pid = %i \n", argv[0], getpid());
        sleep(3);
        exit(EXIT_SUCCESS);
        
    }
    exit(EXIT_FAILURE);
    
}



