//
//  fork.c
//  
//
//  Created by 何瑞达 on 30/9/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void function( void ) {
    
    int pid;
    
    switch ( pid = fork() ) {
            
        case -1:
            printf("Fork() failed\n");
            exit(EXIT_FAILURE);
            break;
        
        case 0:         // New child process
            
            printf("c : child's pid = %i \n", getpid());

            for (int t = 0; t < 3; t++) {
                printf("    tick\n");
                sleep(1);
            }
            
            exit(EXIT_SUCCESS);
            break;
        
        default: {            // Original parent process
            
            int child, status;
            
            printf("parent waiting\n");
            // Wait() returns the child ID
            // Wait() doesn't return until the child terminates
            child = wait( &status );
            
            // Use WEXITSTATUS to know the exit status
            printf("process pid = %i terminated with exit status = %i \n", child, WEXITSTATUS(status));
            
            exit(EXIT_SUCCESS);
            
            break;
            
        }
            
    }
    
    
}

int main() {
    
    function();
    
}

