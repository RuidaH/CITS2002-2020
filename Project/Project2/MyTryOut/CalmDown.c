//
//  CalmDown.c
//  
//
//  Created by 何瑞达 on 28/9/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>

char *progname;

void file_attributes(char *filename) {
    
    struct stat     stat_buffer;
    
    // stat : listing all the properties of a file
    // 0 --> success
    // -1 -> failure
    if (stat(filename, &stat_buffer) != 0) {
        perror( progname );
        exit(EXIT_FAILURE);
    }
    // If I can find that file, check if it's a regular file
    else if ( S_ISREG( stat_buffer.st_mode ) ) {
        
        printf( "%s is a regular file\n", filename );
        printf( "is %i bytes long\n", (int)stat_buffer.st_size );
        
        printf( "and was last modified on %i\n", (int)stat_buffer.st_mtime);
        printf( "which was %s\n", ctime( &stat_buffer.st_mtime ) );
        
    }
    // Check if it's a directory or not
    else if ( S_ISDIR( stat_buffer.st_mode ) ) {
        
        printf( "%s is a directory\n", filename );
        printf( "is %i bytes long\n", (int)stat_buffer.st_size );
        
        printf( "and was last modified on %i\n", (int)stat_buffer.st_mtime);
        printf( "which was %s\n", ctime( &stat_buffer.st_mtime ) );
        
        
    }
    
}


int main(int argc, char *argv[]) {
    
    if( argc != 2 ) {
        
        printf("Usuage :  %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
        
    }
    else {
        
        progname = argv[1];

        file_attributes(argv[1]);
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
    
}