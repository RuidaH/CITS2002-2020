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



int main() {
    
    int x = 5;
    
    int* p = &x;
    *p = 6;
    
    // Pointer to pointer
    int** q = &p;
    
    int*** r = &q;
    
    printf("%d\n", *p);   // x
    printf("%p\n", *q);   // Address of p
    printf("%d\n", **q);  // x
    printf("%p\n", **r);  // Address of p
    printf("%d\n", ***r); // x
    
    ***r = 10;            // x = 10
    printf("x = %d\n", x);
    
    **q = *p + 2;         // **q and *p are x
    printf("x = %d\n", x);
    
    return 0;
}
