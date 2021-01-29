//
//  stringtest.c
//  
//
//  Created by 何瑞达 on 27/9/20.
//

#include "Declaration.h"

int main() {
    
    char s1[100] = "I want to break free!";
    char s2[100] = "Don't stop fighting!";
    
    char *p1 = s1;
    char *p2 = s2;
    
    char *pointer = strcat_p(p1, p2);
    
    printf("The result of strcat_p is %s\n", pointer);
    
    
    
    
    char s3[100] = "This is the string";
    char s4[100] = "This is the ztring";
    
    char *p3 = s3;
    char *p4 = s4;
    
    char s5[100] = "This is the string";
    char s6[100] = "This is the str";
    
    char *p5 = s5;
    char *p6 = s6;

    printf("The comparasion between s3 and s4 is %d\n", strcmp_p(p3, p4));
    printf("The comparasion between s5 and s6 is %d\n", strcmp_p(p5, p6));
    
    int length = 4;
    printf("The comparasion between s3 and s4 of the first %d characters is %d\n", length, strncmp_p(p3, p4, length));
    
    
    
    
    
    int find = 'i';
    
    char *p7 = s6;
    
    printf("The first occurrence of %c in the string s6 is %p\n", find, strchr_p(p7, find));
    
    
    printf("The last occurrence of %c in the string s6 is %p\n", find, strrchr_p(p7, find));
    
}
