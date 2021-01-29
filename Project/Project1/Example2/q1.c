//
//  q1.c
//  
//
//  Created by 何瑞达 on 18/8/20.
//

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES   10

int main() {
    
    int process_still_sleeping[MAX_PROCESSES] = {0};
    int num_sleeping_process = 0;
    
    int x[MAX_PROCESSES] = {0, 0, 4, 5, 9, 2, 6, 4, 7, 0};
    
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        
        if ( x[i] > 0 ) {
            
            int temp = x[i];
            
            for (int k = 0; k < MAX_PROCESSES; k++) {
                
                if (process_still_sleeping[k] == 0) {
                    process_still_sleeping[k] = temp;
                    num_sleeping_process++;
                    
                    printf("---- process_still_sleeping[%d] =  %d ----\n", k, process_still_sleeping[k]);
                    
                    break;
                    
                    printf("---- process_still_sleeping[%d] =  %d ----\n", k, process_still_sleeping[k]);
                }
                // When the element is not 0 and temp is greater than it
                else if (temp > process_still_sleeping[k]) {
                    continue;
                }
                // When the element is not 0 and temp is smaller than it
                else {
                    
                    for (int p = num_sleeping_process; p > k; p--) {
                        process_still_sleeping[p] = process_still_sleeping[p - 1];
                    }
                    process_still_sleeping[k] = temp;
                    num_sleeping_process++;
                    
                    printf("---- process_still_sleeping[%d] =  %d ----\n", k, process_still_sleeping[k]);
                    
                    break;
                    
                    
                }
            }
        }
        
    }


    for (int p = 0; p < MAX_PROCESSES; p++) {
        printf("%d\n", process_still_sleeping[p]);
    }
    
    printf("%d\n", num_sleeping_process);
    
}
