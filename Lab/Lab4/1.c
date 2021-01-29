//
//  1.c
//  
//
//  Created by 何瑞达 on 27/9/20.
//

#include <stdio.h>

void money(int total, int *dollors, int *cents) {
    
    *cents   = total % 100;
    *dollors = total - *cents;

}

int main(){
    
    int total = 1334;
    int dollors;
    int cents;
    
    money(total, &dollors, &cents);
    
    printf("Dollors are %d\n", dollors);
    printf("Cents are %d\n", cents);
    
}
