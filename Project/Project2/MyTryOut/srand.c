//
//  srand.c
//  
//
//  Created by 何瑞达 on 2/10/20.
//

#include <stdio.h>
#include <stdlib.h>

int main() {
    
    srand(4);
    
    for (int i = 0; i < 20; i++) {
        printf("%d\n", rand());
    }
    
    return 0;
    
}
