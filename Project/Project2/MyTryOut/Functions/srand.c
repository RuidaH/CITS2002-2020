//
//  srand.c
//  
//
//  Created by 何瑞达 on 2/10/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    
    char *str;
    char *str1;
    
    // Initial memory allocation
    str = (char *)malloc(sizeof(char)*25);
    
    strcpy(str, "HelloDevil");
    
    printf("The string is %s\n", str);
    printf("The address is %p", str);
    
    // Another way to allocate the memory
    str1 = (char *)calloc(30, sizeof(char));
    strcpy(str1, "HelloDevil.com");
    
    printf("\nThe string is %s\n", str1);
    printf("The address is %p\n", str1);
    
    return 0;
    
}
