#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool Is_same_path ( char *path1, char *path2 ) {
    
    int index1 = 0;
    int index2 = 0;
    
    while ( !( path1[index1] == 'P' && path1[index1 + 1] == '-') ) {
        index1++;
    }
    
    while ( !( path2[index2] == 'P' && path2[index2 + 1] == '-') ) {
        index2++;
    }
    
    index1 = index1 + 8;
    index2 = index2 + 8;
    
    while ( path1[index1] != '\0' && path2[index2] != '\0' ) {
        
        if ( path1[index1] != path2[index2] ) {
            return false;
        }
        
        index1++;
        index2++;
        
    }
    
    // If one of them hasn't gone to the end of the string
    if ( path1[index1] != '\0' || path2[index2] != '\0' ) {
        return false;
    }
    
    return true;
    
}

 
int main(int argc, char *argv[]) {
   
    if ( Is_same_path(argv[1], argv[2]) ) {
        printf("They are the same\n");
    }
    else {
        printf("They are different\n");
    }
}
