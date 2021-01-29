#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/*  CITS2002 Project 1 2020
    Name:                Ruida He
    Student number(s):   22762872
 */


//  MAXIMUM NUMBER OF PROCESSES OUR SYSTEM SUPPORTS (PID=1..20)
#define MAX_PROCESSES                       20

//  MAXIMUM NUMBER OF SYSTEM-CALLS EVER MADE BY ANY PROCESS
#define MAX_SYSCALLS_PER_PROCESS            50

//  MAXIMUM NUMBER OF PIPES THAT ANY SINGLE PROCESS CAN HAVE OPEN (0..9)
#define MAX_PIPE_DESCRIPTORS_PER_PROCESS    10

//  TIME TAKEN TO SWITCH ANY PROCESS FROM ONE STATE TO ANOTHER
#define USECS_TO_CHANGE_PROCESS_STATE       5

//  TIME TAKEN TO TRANSFER ONE BYTE TO/FROM A PIPE
#define USECS_PER_BYTE_TRANSFERED           1











//  ---------------------------------------------------------------------

//  YOUR DATA STRUCTURES, VARIABLES, AND FUNCTIONS SHOULD BE ADDED HERE:

int timetaken       = 0;

// Define all states of process
enum { NEW = 0, READY = 1, RUNNING = 2, EXITED = 3, SLEEPING = 4, WAITING = 5, WRITING = 6, READING = 7};

// Define all the system calls
enum { SYS_COMPUTE = 0, SYS_SLEEP = 1, SYS_EXIT = 2, SYS_FORK = 3, SYS_WAIT = 4, SYS_PIPE = 5, SYS_WRITEPIPE = 6, SYS_READPIPE = 7};

// Define the state of the descriptor port
enum { UNUSED = 0, WRITING_END = 1, READING_END = 2};

struct {
    
    int     state;                                  // NEW, READY, ...
    
    int     nextsyscall;
    
    int     syscall_index;                          // Record the current index of system call in array
    
    int     parentPID;                              // Record parent PID for sys_wait () function
    
    int     waking_moment;                          // Record the waking moment if process calls sleep function
    
    bool first_time_to_read;                        // Variables set for writepipe() and readpipe()
    bool first_time_to_write;
    
    struct syscall_struct {
        
        int     syscall;                            // SYS_COMPUTE, SYS_SLEEP ...
        int     usecs;                              // Extra data
        int     back_up_usecs;                      // In case you have other data to store
        
    } syscalls[MAX_SYSCALLS_PER_PROCESS];
    
    /*
        port = 0 (UNOPENED)   -> Haven't been used
        port = 1 (WRITING_END) -> write data in pipe
        port = 2 (READING_END) -> read data from pipe
     */
    struct {
        
        int port;
        int bytes_in_pipe;                         // For writing_end proess
        int remaining_bytes_to_write;              // For writing_end proess to store extra bytes
        int remaining_bytes_to_read;               // For reading_end process to store extra bytes
        int reader;                                // Used only when port is the writing end
                                                   // -1 initially --> no reader yet
    } pipes[MAX_PIPE_DESCRIPTORS_PER_PROCESS];
    
} process[MAX_PROCESSES];





//  ---------------- Creating the queue for READY state processes ---------------------

// Use an array to mimic Queue
// Write something to prevent the case that process > 20

int RQ[MAX_PROCESSES]   = {0};
int RProcessCount       =  0;

bool isEmpty_RQ() {
    
    return (RProcessCount == 0);
    
}

void enqueue_R(int data) {
    
    RQ[RProcessCount] = data;
    RProcessCount++;
    
}

int dequeue_R() {
    
    int temp = RQ[0];
    
    RQ[0] = 0;
    RProcessCount--;
    
    for (int i = 1; i < MAX_PROCESSES; i++) {
        RQ[i - 1] = RQ[i];
    }
    
    return temp;
    
}

void printQueue_R() {
    
    if (RProcessCount != 0) {
        
        printf("\t  RQ: ");
        
        for (int i = 0; i < RProcessCount; i++) {
            printf("%d  ", RQ[i]);
        }
        
        printf("\n");
    }
    
}



//  ---------------- Creating the queue for SLEEPING state processes ------------------
//  ----------------- The SLEEPING queue need to be sorted by size  -------------------


int SQ[MAX_PROCESSES]   = {0};              // Record PID
int SProcessCount       =  0;

bool isEmpty_SQ() {
    
    return (SProcessCount == 0);
    
}

// Need to insert the queue by size of process's waking_moment
// temp --> PID
void insert_S(int temp) {
    
    for (int k = 0; k < MAX_PROCESSES; k++) {
        
        if (SQ[k] == 0) {
            
            SQ[k] = temp;
            SProcessCount++;
            break;
            
        }
        // When the element is not 0 and temp is equal to or smaller than it
        else if (process[temp - 1].waking_moment <= process[SQ[k] - 1].waking_moment) {
            
            for (int p = SProcessCount; p > k; p--) {
                SQ[p] = SQ[p - 1];
            }
            
            SQ[k] = temp;
            SProcessCount++;
            
            break;
            
        }
        
        // When the element is not 0 and temp is greater than it, continue the loop
        
    }
    
}

// Remove the first item in the queue (whose waking_element is the smallest)
int remove_S() {
    
    int temp = SQ[0];
    
    SQ[0] = 0;
    SProcessCount--;
    
    for (int i = 1; i < MAX_PROCESSES; i++) {
        SQ[i - 1] = SQ[i];
    }
    
    return temp;
    
}


void printQueue_S() {
    
    if (SProcessCount != 0) {
        
        printf("\t  SQ: ");
        
        for (int i = 0; i < SProcessCount; i++) {
            printf("%d  ", SQ[i]);
        }
        
        printf("\n");
    }
    
}







// ---------- Creating the blocked queue for both reading and writing processes --------

struct blocked_queue {
    
    int processID;
    int pipe_descriptor;
    
} BQ[MAX_PIPE_DESCRIPTORS_PER_PROCESS];

int BProcessCount       =  0;

bool isEmpty_BQ() {
    
    return (BProcessCount == 0);
    
}

// Data -> PID
void enqueue_B(int data, int descriptor) {
    
    BQ[BProcessCount].processID       = data;
    BQ[BProcessCount].pipe_descriptor = descriptor;
    BProcessCount++;
    
    
    
}


struct blocked_queue dequeue_B() {
    
    struct blocked_queue temp = BQ[0];
    
    BProcessCount--;
    
    for (int i = 1; i < MAX_PIPE_DESCRIPTORS_PER_PROCESS; i++) {
        BQ[i - 1].processID       = BQ[i].processID;
        BQ[i - 1].pipe_descriptor = BQ[i].pipe_descriptor;
    }
    
    return temp;
    
}

void printQueue_B() {
    
    if (BProcessCount != 0) {
        
        printf("\t  BQ: ");
        
        for (int i = 0; i < BProcessCount; i++) {
            printf("%d  ", BQ[i].processID);
        }
        
        printf("\n");
    }
    
}





void init_processes(void) {
    
    for ( int p = 0; p < MAX_PROCESSES; ++p ) {
        
        process[p].state                = NEW;
        process[p].nextsyscall          =  0;
        process[p].syscall_index        =  0;
        process[p].parentPID            = -1;
        process[p].waking_moment        = -1;
        process[p].first_time_to_read   = true;
        process[p].first_time_to_write  = true;
        
        for ( int s = 0; s < MAX_SYSCALLS_PER_PROCESS; ++s ) {
            process[p].syscalls[s].syscall      = SYS_EXIT;
        }
        
        for (int k = 0; k < MAX_PIPE_DESCRIPTORS_PER_PROCESS; k++) {
            process[p].pipes[k].port                     = UNUSED;
            process[p].pipes[k].reader                   = -1;
            process[p].pipes[k].bytes_in_pipe            = 0;
            process[p].pipes[k].remaining_bytes_to_write = 0;
            process[p].pipes[k].remaining_bytes_to_read  = 0;
        }
        
    }
    
    // Initialisation of blocked queue
    for (int p = 0; p < MAX_PIPE_DESCRIPTORS_PER_PROCESS; p++) {
        BQ[p].processID       = -1;
        BQ[p].pipe_descriptor = -1;
    }
    
}










// Compare the the waking moment of the first process in the sleeping queue with timataken.
void timetaken_execution() {
    
    // If the sleeping queue is empty or the timetaken is smaller than the waking_moment of the process
    if ( ( isEmpty_SQ() ) || ( timetaken < process[SQ[0] - 1].waking_moment ) ) {
        
        timetaken += USECS_TO_CHANGE_PROCESS_STATE;
        
    }
    // Otherwise (now it's the time for this process to wake)
    else if ( timetaken >= process[SQ[0] - 1].waking_moment ){
        
        // dequeue the first element from the slepping queue (whose owns the smallest waking_moment
        int PID = remove_S();
        printf("@%0.6d   p%d finishes sleeping, p%d. SLEEPING -> READY\n", timetaken, PID, PID);

        enqueue_R(PID);
        // printQueue_R();
        // printQueue_S();
        
        // Process : SLEEPING -> READY
        timetaken += USECS_TO_CHANGE_PROCESS_STATE;
        
        printf("@%0.6d   \n", timetaken);
        
        process[PID - 1].state = READY;
        // The process has awaked, so change the value back to -1
        process[PID - 1].waking_moment = -1;
        
        // SYS_SLEEP finished, exeute the next operation for the next round
        process[PID - 1].syscall_index++;
        
    }

}





void sys_compute(int PID, int time, int index, int timequantum) {
    
    if ( time <= timequantum ) {
        printf("@%0.6d   p%d: sys_compute(), for %d usec (now completed)\n", timetaken, PID, time );
        timetaken += time;
        
        // SYS_COMPUTE finished, exeute the next operation for the next round
        process[PID - 1].syscall_index++;
    }
    else {
        // Calculate the remaining time for "compute" operation
        process[PID - 1].syscalls[index].usecs = (time - timequantum);
        printf("@%0.6d   p%d: sys_compute(), for %d usec (%d usecs remaining)\n", timetaken, PID, timequantum, process[PID - 1].syscalls[index].usecs );
        timetaken += timequantum;
        
        // Haven't finished the "compute" operation, so index don't plus 1.
    }
    
    printQueue_R();
    
    // Put the current process in the READY queue
    enqueue_R(PID);
    
    printf("@%0.6d   p%d.RUNNING -> READY\n", timetaken, PID);
    printQueue_R();
    
    // Record the timetaken switching the state from RUNNING to READY
    timetaken_execution();
    
    // The state of process : RUNNING -> READY
    process[PID - 1].state = READY;
}






void sys_exit(int PID) {
    
    printf("@%0.6d   p%d: sys_exit(),  p%d.RUNNING -> EXITED\n", timetaken, PID, PID);
    
    process[PID - 1].state = EXITED;
    timetaken_execution();
    
    // Check if there is a waiting process
    int parentPID_recorded = process[PID - 1].parentPID;
    
    // If the exited process has a parent process and its state is WAITING
    if ( parentPID_recorded != -1 && process[parentPID_recorded - 1].state == WAITING ) {
        printf("@%0.6d   p%d has been waiting for p%d, p%d. WAITING -> READY\n", timetaken, parentPID_recorded, PID, parentPID_recorded);
        
        enqueue_R(parentPID_recorded);
        
        timetaken_execution();
    }
    
    printQueue_R();

}






void sys_fork(int PID, int childPID) {
    
    // Record the the parent PID of this child process
    process[childPID - 1].parentPID = PID;
    
    // Push the child process into the queue
    enqueue_R(childPID);
    
    // Child process (childPID) : RUNNING -> READY
    printf("@%0.6d   p%d: fork(),  new childPID = %d,  p%d.NEW -> READY\n", timetaken, PID, childPID, childPID);
    process[childPID - 1].state = READY;
    printQueue_R();
    
    // Push the parent process into the queue
    enqueue_R(PID);
    
    // Child process : NEW -> READY
    timetaken_execution();
    
    // Process (PID) : RUNNING -> READY
    printf("@%0.6d   p%d.RUNNING -> READY\n", timetaken, PID);
    process[PID - 1].state = READY;
    printQueue_R();
    
    process[childPID - 1].state = READY;
    process[PID - 1].state = READY;
    
    // Parent Process : RUNNING -> READY
    timetaken_execution();
    
    
    // Copy the descriptors of parent process that has no reader yet to the child process
    for (int i = 0; i < MAX_PIPE_DESCRIPTORS_PER_PROCESS; i++) {
        
        if ( process[PID - 1].pipes[i].port == 1 && process[PID - 1].pipes[i].reader == -1 ) {
            
            // Make the child process be the reading end
            process[childPID - 1].pipes[i].port = READING_END;
            
            // Record the reading end in the parent process
            process[PID - 1].pipes[i].reader = childPID;
            
        }
        
    }

    
    // SYS_FORK finished, exeute the next operation for the next round
    process[PID - 1].syscall_index++;
    
}






void sys_wait(int childPID, int PID) {
    
    printf("@%0.6d   p%d: sys_wait(), for childPID = %d, p%d. RUNNING -> WAITING\n", timetaken, PID, childPID, PID);
    
    // Parent Process : RUNNING -> WAITING
    timetaken_execution();
    
    process[PID - 1].state = WAITING;
    
    printQueue_R();
    
    // SYS_WAIT finished, exeute the next operation for the next round
    process[PID - 1].syscall_index++;
    
}






void sys_sleep (int PID, int sleeping_time) {
    
    // Need to include the time for the state of process transferring from RUNNING -> SLEEP
    process[PID - 1].waking_moment = sleeping_time + timetaken + USECS_TO_CHANGE_PROCESS_STATE;    
    
    printf("@%0.6d   p%d.sys_sleep()  for %d usecs,  p%d. RUNNING -> SLEEPING\n", timetaken, PID, sleeping_time, PID);
    
    process[PID - 1].state = SLEEPING;
    
    printQueue_R();
    
    insert_S(PID);
    printQueue_S();
    

    // Process : RUNNING -> SLEEPING
    timetaken_execution();
    
}






void sys_pipe(int descriptor, int PID) {
    
    // Set the process calling pipe() be the writing end
    process[PID - 1].pipes[descriptor - 1].port = WRITING_END;
    
    printf("@%0.6d   p%d: sys_pipe(), pipedesc = %d, p%d. RUNNING -> READY\n", timetaken, PID, descriptor, PID);
    
    // Push the process calling sys_pipe() into the queue
    enqueue_R(PID);
    printQueue_R();
    
    // Parent Process : RUNNING -> READY
    timetaken_execution();
    
    process[PID - 1].state = READY;
    
    // SYS_PIPE finished, exeute the next operation for the next round
    process[PID - 1].syscall_index++;

    
}






void sys_readpipe( int PID, int parent, int currentPipe, int current_bytes_in_pipe, int current_remaining_bytes_to_read ) {
    
    // Situation : no bytes in the pipe to be read
    if ( current_bytes_in_pipe == 0) {
        
        // Process : Running -> Reading
        process[PID - 1].state = READING;
        
        printf("@%0.6d   p%d: sys_readpipe(), from empty pipedesc = %d, p%d. RUNNING -> READING\n", timetaken, PID, currentPipe, PID);
        
        printQueue_R();
        
        // Haven't read any bytes from pipe, so push it into the blocked queue
        enqueue_B(PID, currentPipe);
        printQueue_B();
        
        timetaken_execution();
        
    }
    // Situation : The amount of bytes that readpipe() needs > Current bytes in the pipe
    else if ( current_bytes_in_pipe < current_remaining_bytes_to_read ) {
            
        process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read -= current_bytes_in_pipe;
        
        // readpipe() reads all the bytes from the pipe
        process[parent - 1].pipes[currentPipe - 1].bytes_in_pipe = 0;
        
        printf("@%0.6d   p%d: sys_readpipe(), Reading : %d bytes from pipedesc = %d (%d remaining), p%d. RUNNING -> READING\n", timetaken, PID, current_bytes_in_pipe, currentPipe, process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read, PID);
        
        process[PID - 1].state = READING;
        timetaken_execution();
        
        timetaken += (current_bytes_in_pipe * USECS_PER_BYTE_TRANSFERED);
        
        // Put it back to the queue since it still needs to read bytes from pipe.
        enqueue_B(PID, currentPipe);
        printQueue_B();
                    
    }
    // Situation : readpipe() can read all the bytes it needs all in once.
    else if ( current_bytes_in_pipe >= current_remaining_bytes_to_read ) {

        process[parent - 1].pipes[currentPipe - 1].bytes_in_pipe = current_bytes_in_pipe - current_remaining_bytes_to_read;
        
        process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read = 0;
        
        printf("@%0.6d   p%d: sys_readpipe(), Reading : %d bytes from pipedesc = %d (now completed), p%d. RUNNING -> READY\n", timetaken, PID, current_remaining_bytes_to_read, currentPipe, PID);
        
        // Process : Reading -> Ready
        process[PID - 1].state = READY;
        timetaken_execution();
        timetaken += (current_remaining_bytes_to_read * USECS_PER_BYTE_TRANSFERED);
        
        
        // SYS_WRITEPIPE finished, exeute the next operation for the next round
        process[PID - 1].syscall_index++;
        process[PID - 1].first_time_to_read = true;

        
        enqueue_R(PID);
        printQueue_R();
                    
    }
    
    
}






void sys_writepipe( int PID, int currentPipe, int new_current_bytes_in_pipe, int current_remaining_bytes_to_write, int pipesize ) {
    
    // Situation : The data can be written in the pipe all in once
    if ( current_remaining_bytes_to_write <= (pipesize - new_current_bytes_in_pipe) ) {

        process[PID - 1].pipes[currentPipe - 1].bytes_in_pipe += current_remaining_bytes_to_write;
        
        process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_write = 0;
        
        printf("@%0.6d   p%d: sys_writepipe(), Writing : %d bytes to pipedesc = %d, (now completed), p%d. RUNNING -> READY\n", timetaken, PID, current_remaining_bytes_to_write, currentPipe, PID);
        
        // Process : Running -> Ready
        process[PID - 1].state = READY;
        enqueue_R(PID);
        printQueue_R();
        
        timetaken_execution();
        timetaken += (current_remaining_bytes_to_write * USECS_PER_BYTE_TRANSFERED);

        // SYS_WRITEPIPE finished, exeute the next operation for the next round
        process[PID - 1].syscall_index++;
        process[PID - 1].first_time_to_write = true;

        
    }
    // Situation : you cannot write all the bytes in the pipe
    else if ( current_remaining_bytes_to_write > ( pipesize - new_current_bytes_in_pipe ) ) {
        
        process[PID - 1].pipes[currentPipe - 1].bytes_in_pipe = pipesize;
        
        process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_write = current_remaining_bytes_to_write - ( pipesize - new_current_bytes_in_pipe );
        
        printf("@%0.6d   p%d: sys_writepipe(), Writing %d bytes to pipedesc = %d, (%d remaining)\n", timetaken, PID, ( pipesize - new_current_bytes_in_pipe ), currentPipe, process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_write);
        
        printf("@%0.6d   p%d. RUNNING -> WRITING\n", timetaken, PID);
        
        // Process : Running -> Writing
        process[PID - 1].state = WRITING;
        timetaken_execution();
        timetaken += ( ( pipesize - new_current_bytes_in_pipe ) * USECS_PER_BYTE_TRANSFERED );
        
        // There are still some remaining bytes to write, so put the PID back to the blocked queue
        enqueue_B(PID, currentPipe);
        printQueue_B();
        
    }
    
}







// Go through all the system calls stored in the structure
void run_simulcation( int timequantum, int pipesize ) {
    
    int PID         = 1;
    int index       = 0;
    int loop_times  = 0;
    
    // For the blocked queue
    int currentPipe = 0;
    int parent      = 0;
    
    // For the readpipe()
    int current_bytes_in_pipe           = 0;
    int current_remaining_bytes_to_read = 0;
    
    // For the writepipe()
    int current_remaining_bytes_to_write = 0;
    int new_current_bytes_in_pipe        = 0;
    
    // The first process : NEW -> RUNNING
    process[PID - 1].state = RUNNING;
    printf("@%0.6d   BOOT, p%d.RUNNING\n", timetaken, PID);
    
    
    
    do {
        
        if (!isEmpty_RQ() || loop_times == 0) {
        
            
            // When the loop_times is 0 (the process just started), there is no process in the queue.
            if (loop_times != 0) {
                
                PID = dequeue_R();
                
                printf("@%0.6d   p%d.READY -> RUNNING\n", timetaken, PID);
                printQueue_R();
                
                timetaken_execution();
                
            }
            
            index = process[PID - 1].syscall_index;
            
            struct syscall_struct currentSyscall = process[PID - 1].syscalls[index];
        
            
            if ( currentSyscall.syscall == SYS_COMPUTE ) {
                
                sys_compute(PID, currentSyscall.usecs, index, timequantum);
                
            }
            else if ( currentSyscall.syscall == SYS_EXIT ) {
                
                sys_exit(PID);
                
            }
            else if ( currentSyscall.syscall == SYS_FORK ) {
                
                // sys_fork(PID, childPID);
                sys_fork(PID, currentSyscall.usecs);
            
            }
            else if ( currentSyscall.syscall == SYS_SLEEP ) {
                
                sys_sleep (PID, currentSyscall.usecs);
                
            }
            else if ( currentSyscall.syscall == SYS_WAIT ) {
                
                // sys_wait(childPID, PID)
                sys_wait(currentSyscall.usecs, PID);
                
                
            }
            else if ( currentSyscall.syscall == SYS_PIPE ) {
                
                sys_pipe(currentSyscall.usecs, PID);
                
            }
            else if ( currentSyscall.syscall == SYS_READPIPE ) {
                
                currentPipe             = currentSyscall.usecs;
                parent                  = process[PID - 1].parentPID;
                current_bytes_in_pipe   = process[parent - 1].pipes[currentPipe - 1].bytes_in_pipe;

                // The first time I execute readpipe(), put the bytes that were requested in the remaining parts
                if (process[PID - 1].first_time_to_read) {
                    process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read = currentSyscall.back_up_usecs;
                    process[PID - 1].first_time_to_read = false;
                }

                current_remaining_bytes_to_read = process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read;
                
                
                sys_readpipe( PID, parent, currentPipe, current_bytes_in_pipe, current_remaining_bytes_to_read );

                
            }
            else if ( currentSyscall.syscall == SYS_WRITEPIPE ) {
                
                // Store the bytes that needs to be written in the pipe to the remaining part
                if (process[PID - 1].first_time_to_write) {
                    process[PID - 1].pipes[currentSyscall.usecs - 1].remaining_bytes_to_write = currentSyscall.back_up_usecs;
                    process[PID - 1].first_time_to_write = false;
                }
                
                currentPipe                      = currentSyscall.usecs;
                current_remaining_bytes_to_write = process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_write;    // It's all the bytes that needs to be written at first
                new_current_bytes_in_pipe        = process[PID - 1].pipes[currentPipe - 1].bytes_in_pipe;               // It's 0 at first
                

                sys_writepipe( PID, currentPipe, new_current_bytes_in_pipe, current_remaining_bytes_to_write, pipesize );

            }
        
        }

        
        
        // if blocked queue is not empty
        if (!isEmpty_BQ()) {
            
            struct blocked_queue return_B = dequeue_B();
            
            PID         = return_B.processID;
            currentPipe = return_B.pipe_descriptor;
            
            // Process : reading_end
            if ( process[PID - 1].pipes[currentPipe - 1].port == READING_END ) {
                
                parent                          = process[PID - 1].parentPID;
                current_bytes_in_pipe           = process[parent - 1].pipes[currentPipe - 1].bytes_in_pipe;
                current_remaining_bytes_to_read = process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_read;
                
                // If there are bytes in the pipe for process to read
                if (current_bytes_in_pipe != 0) {
                    
                    // Process : Reading -> Ready
                    process[PID - 1].state = READY;
                    
                    printf("@%0.6d   p%d can read from pipedesc = %d, p%d. READING -> READY\n", timetaken, PID, currentPipe, PID);
                    
                    enqueue_R(PID);
                    printQueue_R();
                    
                    timetaken_execution();
                    
                }
                // No bytes in the pipe now, push the process into the blocked queue to wait until there are bytes again.
                else {
                    
                    enqueue_B(PID, currentPipe);
                    
                }
                                              
            }
            // Process : writing_end
            else if ( process[PID - 1].pipes[currentPipe - 1].port == WRITING_END ) {
                
                current_remaining_bytes_to_write = process[PID - 1].pipes[currentPipe - 1].remaining_bytes_to_write;
                new_current_bytes_in_pipe        = process[PID - 1].pipes[currentPipe - 1].bytes_in_pipe;
                
                if ( new_current_bytes_in_pipe < pipesize ) {
                    
                    // Process : Writing -> Ready
                    process[PID - 1].state = READY;
                    
                    printf("@%0.6d   p%d can write to pipedesc = %d, p%d. WRITING -> READY\n", timetaken, PID, currentPipe, PID);
                    
                    enqueue_R(PID);
                    printQueue_R();
                    
                    timetaken_execution();
                    
                }
                // The pipe is still full, the process stay blocked and wait until other process read bytes from the pipe.
                else {
                    
                    enqueue_B(PID, currentPipe);
                    
                }
                
            }
            
        }
        
        

        // When the ready queue is empty, we need to check if there are still sleeping process in the sleeping queue
        if (isEmpty_RQ()) {
            
            // There are some sleeping processes in the sleeping queue
            if (!isEmpty_SQ()) {
                
                PID = remove_S();
                
                timetaken = process[PID - 1].waking_moment;
                printf("@%0.6d   p%d finishes sleeping, p%d. SLEEPING -> READY\n", timetaken, PID, PID);
                
                enqueue_R(PID);
                printQueue_R();
                
                // Process : SLEEPING -> READY
                timetaken += USECS_TO_CHANGE_PROCESS_STATE;
                
                process[PID - 1].state = READY;
                // The process has awaked, so change the value back to -1
                process[PID - 1].waking_moment = -1;
                
                // SYS_SLEEP finished, exeute the next operation for the next round
                process[PID - 1].syscall_index++;
                
            }
            
        }
            
        loop_times ++;
        
    } while (!isEmpty_RQ() || !isEmpty_SQ() || !isEmpty_BQ());    
    
    // Processes terminated
    printf("@%0.6d   HALT (no processes alive) \n", timetaken);
    
}






//  ---------------------------------------------------------------------

//  FUNCTIONS TO VALIDATE FIELDS IN EACH eventfile - NO NEED TO MODIFY
int check_PID(char word[], int lc)
{
    int PID = atoi(word);

    if(PID <= 0 || PID > MAX_PROCESSES) {
        printf("invalid PID '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return PID;
}

int check_microseconds(char word[], int lc)
{
    int usecs = atoi(word);

    if(usecs <= 0) {
        printf("invalid microseconds '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return usecs;
}

int check_descriptor(char word[], int lc)
{
    int pd = atoi(word);

    if(pd < 0 || pd >= MAX_PIPE_DESCRIPTORS_PER_PROCESS) {
        printf("invalid pipe descriptor '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return pd;
}

int check_bytes(char word[], int lc)
{
    int nbytes = atoi(word);

    if(nbytes <= 0) {
        printf("invalid number of bytes '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return nbytes;
}



//  parse_eventfile() READS AND VALIDATES THE FILE'S CONTENTS
//  YOU NEED TO STORE ITS VALUES INTO YOUR OWN DATA-STRUCTURES AND VARIABLES
void parse_eventfile(char program[], char eventfile[])
{
    #define LINELEN                 100
    #define WORDLEN                 20
    #define CHAR_COMMENT            '#'

    //  ATTEMPT TO OPEN OUR EVENTFILE, REPORTING AN ERROR IF WE CAN'T
    FILE *fp    = fopen(eventfile, "r");

    if(fp == NULL) {
        printf("%s: unable to open '%s'\n", program, eventfile);
        exit(EXIT_FAILURE);
    }

    char    line[LINELEN], words[4][WORDLEN];
    int     lc = 0;

    //  READ EACH LINE FROM THE EVENTFILE, UNTIL WE REACH THE END-OF-FILE
    while(fgets(line, sizeof line, fp) != NULL) {
        ++lc;

        //  COMMENT LINES ARE SIMPLY SKIPPED
        if(line[0] == CHAR_COMMENT) {
            continue;
        }

        //  ATTEMPT TO BREAK EACH LINE INTO A NUMBER OF WORDS, USING sscanf()
        int nwords = sscanf(line, "%19s %19s %19s %19s",
                                    words[0], words[1], words[2], words[3]);

        //  WE WILL SIMPLY IGNORE ANY LINE WITHOUT ANY WORDS
        if(nwords <= 0) {
            continue;
        }

        //  ENSURE THAT THIS LINE'S PID IS VALID
        int thisPID = check_PID(words[0], lc);

        //  OTHER VALUES ON (SOME) LINES
        int otherPID, nbytes, usecs, pipedesc;
        
        // next refers to the next system call in process
        int next;

        //  IDENTIFY LINES RECORDING SYSTEM-CALLS AND THEIR OTHER VALUES
        //  THIS FUNCTION ONLY CHECKS INPUT;  YOU WILL NEED TO STORE THE VALUES

        if(nwords == 3 && strcmp(words[1], "compute") == 0) {
            
            usecs   = check_microseconds(words[2], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall = SYS_COMPUTE;
            process[ thisPID - 1 ].syscalls[next].usecs = usecs;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 3 && strcmp(words[1], "sleep") == 0) {
            
            usecs   = check_microseconds(words[2], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall  = SYS_SLEEP;
            process[ thisPID - 1 ].syscalls[next].usecs    = usecs;
            
            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 2 && strcmp(words[1], "exit") == 0) {
            ;
        }
        else if(nwords == 3 && strcmp(words[1], "fork") == 0) {
            
            otherPID = check_PID(words[2], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall = SYS_FORK;
            process[ thisPID - 1 ].syscalls[next].usecs = otherPID;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 3 && strcmp(words[1], "wait") == 0) {
            
            otherPID = check_PID(words[2], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall  = SYS_WAIT;
            process[ thisPID - 1 ].syscalls[next].usecs    = otherPID;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 3 && strcmp(words[1], "pipe") == 0) {
            
            pipedesc = check_descriptor(words[2], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall  = SYS_PIPE;
            process[ thisPID - 1 ].syscalls[next].usecs    = pipedesc;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 4 && strcmp(words[1], "writepipe") == 0) {
            
            pipedesc = check_descriptor(words[2], lc);
            nbytes   = check_bytes(words[3], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall       = SYS_WRITEPIPE;
            process[ thisPID - 1 ].syscalls[next].usecs         = pipedesc;
            process[ thisPID - 1 ].syscalls[next].back_up_usecs = nbytes;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        else if(nwords == 4 && strcmp(words[1], "readpipe") == 0) {
            
            pipedesc = check_descriptor(words[2], lc);
            nbytes   = check_bytes(words[3], lc);
            
            next = process[ thisPID - 1 ].nextsyscall;
            
            process[ thisPID - 1 ].syscalls[next].syscall       = SYS_READPIPE;
            process[ thisPID - 1 ].syscalls[next].usecs         = pipedesc;
            process[ thisPID - 1 ].syscalls[next].back_up_usecs = nbytes;

            ++process[ thisPID - 1 ].nextsyscall;
            
        }
        //  UNRECOGNISED LINE
        else {
            printf("%s: line %i of '%s' is unrecognized\n", program,lc,eventfile);
            exit(EXIT_FAILURE);
        }
        
    }
    
    fclose(fp);

    #undef  LINELEN
    #undef  WORDLEN
    #undef  CHAR_COMMENT
}



//  ---------------------------------------------------------------------

//  CHECK THE COMMAND-LINE ARGUMENTS, CALL parse_eventfile(), RUN SIMULATION
int main(int argc, char *argv[])
{
    
    if ( argc < 4 ) {
        
        printf("Usuage :  %s eventfile timequantum-usecs pipesize-bytes\n", argv[0]);
        exit(EXIT_FAILURE);
        
    }
    else {
        
        if ( argv[2] <= 0 || argv[3] <= 0 ) {
            
            printf("Timequantum or pipesize do not meet the requirement.\n");
            exit(EXIT_FAILURE);
            
        }
        else {
            
            int timequantum     = atoi(argv[2]);
            int pipesize        = atoi(argv[3]);
            
            init_processes();
            
            parse_eventfile( argv[0], argv[1] );
            
            run_simulcation( timequantum, pipesize );
            
            printf("Timetaken %i\n", timetaken);
            
            exit(EXIT_SUCCESS);
            
        }
    }
     
    return 0;
}
