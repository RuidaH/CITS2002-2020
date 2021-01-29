#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// Try to type command like this
// ./creat_dir_structure ./dir1 ./dir1/dir2 ./dir1/dir3
int main(int argc, char *argv[]) {
    
    for (int i = 1; i < argc; i++ ) {
        
        if ( mkdir(argv[i], 0700) != 0 ) {
            perror("Error on mkdir()");
        }
        
    }
    
    // Do we need to change the modification time of the directory?
    
}
