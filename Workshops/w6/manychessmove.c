//
//  goodchessmove.c
//  
//
//  Created by 何瑞达 on 30/9/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// A fake library-Function
// Telling the user the computer the N cores
int numberOfCores(void) { return 4; }

// Start one of the child process off
// Return the process ID
int start_onemove(int N, char gamestate[], char goodmove[]) {
    
    printf("./goodchessmove %s, %s-%i\n", gamestate, goodmove, N);
    
    int pid     = fork();
    
    switch (pid) {
        case -1 :
            printf("Cannot fork!\n");
            exit(EXIT_FAILURE);
            break;
        
        case 0: {
            char filename[100];
            
            // It will gernerate the filename each time
            sprintf(filename,"%s-%i", goodmove, N);
            
            printf("Child pid = %i \n", getpid());
            // ...
            // Go out and find this program directly
            // Now you are actually running the goodchessmove function
            execl("./goodchessmove",
                  "goodchessmove",   // argv[0]
                  gamestate,         // argv[1]
                  filename,          // argv[2]
                  NULL);
            exit(EXIT_SUCCESS);
            break;
                  
        }
            
        // Default : we will actually give the child ID to parent
        // Any other numbers will just signify the parent
        //      --> This is the process ID of the child
        default :
            break;
            
    }
    
    return pid;
    
}



void manychessmoves(int N, char gamestate[], char goodmove[]) {
    
    // These variables keep track of how much work we done
    //   and how much work we still need to do
    int ncores      = numberOfCores();
    int nstarted    = 0;            // how many processes have I started?
    int nrunning    = 0;            // how many processes are running?
    int nfinished   = 0;            // how many processes did I finished?
    
    int pid, status;
    
    // While the given number of child processes haven't finished, keep looping
    while (nfinished < N) {
        
        // Keep the child running until the cores are full or they all finished
        while (nrunning < ncores && nstarted < N) {
            
            pid = start_onemove(nstarted, gamestate, goodmove);
            
            if (pid < 0) {
                break;
            }
            
            printf("Parent waits for pid = %i\n", pid);
            ++nstarted;
            ++nrunning;
            
        }
        
        if ((pid = wait( &status )) > 0) {
            
            printf("pid = %i has terminated with status = %i \n\n", pid, status);
            ++nfinished;
            --nrunning;
            
        }
        
    }
    
    
    
    
    
}


// Check the argument in the main function and pass it to another function
int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        
        // Error ...
        exit(EXIT_FAILURE);
        
    }
    
    int ninstances      = atoi(argv[1]);
    
    // gamestate is argv[2]
    // goodmove  is argv[3]
    
    manychessmoves(ninstances, argv[1], argv[2]);
    
    exit(EXIT_SUCCESS);
    
}



