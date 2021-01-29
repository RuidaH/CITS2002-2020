#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


void copyfile( char *source_path, char *destination_path ) {
    
//    char  *source;
//    char  *destination;
//    int   ch = "/";
//
//    // Get their file name from the file path
//
//    source = strrchr( source_path, ch );
//    memmove( source, source + 1, strlen(source) );
//    source++;
//
//    destination = strrchr( destination_path, ch );
//    memmove( destination, destination + 1, strlen(destination) );
//    destination++;

    FILE  *src   = fopen( source_path, "rb" );
    FILE  *dest  = fopen( destination_path, "wb" );
    
    if ( dest != NULL && src != NULL ) {
        
        char   buffer[BUFSIZ];
        size_t got, wrote;
        
        while( ( got = fread(buffer, 1, sizeof buffer, src) ) > 0 ) {
            
            wrote = fwrite( buffer, 1, got, dest );
            
            if ( wrote != got ) {
                perror("Error on write()");
            }
            
        }
        
    }
    
    if ( src != NULL ) {
        fclose(src);
    }
    
    if (dest != NULL ) {
        fclose(dest);
    }
    
    // Need to change the modification time
    struct timeval time_struct[2];
    struct stat    stat_buffer;
    struct stat    *ptr = &stat_buffer;
    
    stat( source_path, &stat_buffer );

    time_struct[0].tv_sec  = ptr->st_mtime;
    time_struct[0].tv_usec = 0;
    time_struct[1].tv_sec  = ptr->st_mtime;
    time_struct[1].tv_usec = 0;

    if ( utimes( destination_path, time_struct ) != 0 ) {
        perror("Error on utimes()");
    }
    
}

int main(int argc, char *argv[]) {
    
    // Assume there are two arguments
    copyfile( argv[1], argv[2] );
    
}
