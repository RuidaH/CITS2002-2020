//
//  q1.c
//  
//
//  Created by 何瑞达 on 18/8/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define NLOOPS      3

void printValues(int n, int values[]) {
    
    for (int i = 0; i < n; i++) {
        printf("%d  ", values[i]);
    }
    printf("\n");
}

void superloop(int nloops, int lower_bounds[], int upper_bounds[]) {
    
    int     values[nloops];
    int     n = 0;
    
    values[n] = lower_bounds[n];
    while (n >= 0) {
        if (values[n] <= upper_bounds[n]) {
            if (n == nloops - 1) {
                printValues(nloops, values);
                values[n]++;
            }
            else {
                n++;
                values[n] = lower_bounds[n];
            }
        }
        else {
            --n;
            values[n]++;
        }
    }
}

int main() {
    
    int lower_bounds[NLOOPS] = {1, 1, 1};
    int upper_bounds[NLOOPS] = {10, 10, 10};
    
    superloop(NLOOPS, lower_bounds, upper_bounds);
    
    return 0;
}





