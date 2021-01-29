/**
 * C program to list contents of a directory recursively.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void deleteFilesRecursively(char *path);


// Require one extra parameter in command line
int main(int argc, char *argv[]) {

    deleteFilesRecursively(argv[1]);

    return 0;
}


// You can just pass the path of the directory to it
void deleteFilesRecursively(char *basepath) {
    
    size_t  path_len;
    char *  path;
    DIR  *  dir;
    
    struct stat     stat_path, stat_entry;
    struct dirent   *dp;
    
    // Since what I pass is the diretory of the tar file
    //      I need to get the properties of that directory first
    
    // Listing all the properties of the path first
    stat( basepath, &stat_path );
    
    // Path is not a directory
    if ( S_ISDIR ( stat_path.st_mode ) == 0 ) {
        perror( "S_ISDIR() Error" );
    }
    
    // Cannot open the directory
    if ( (dir = opendir(basepath) ) == NULL ) {
        perror( "Opendir() Error" );
    }
    
    // Get the length of the basepath
    path_len = strlen(basepath);

    while ((dp = readdir(dir)) != NULL) {
        
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            
            // Allocate new moemory to the path for the file in the directory
            path = calloc( path_len + strlen( dp->d_name ) + 1, sizeof(char) );
            
            // Construct new path from our base path
            strcpy(path, basepath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            
            // Listing all the perperties of the file in the directory
            stat( path, &stat_entry );
            
            if ( S_ISDIR ( stat_entry.st_mode ) ) {

                deleteFilesRecursively( path );

            }
            // If it's not directory, just delete it
            else {
                
                // Return 0 if it succeed
                if ( unlink( path ) == 0 ) {
                    printf( "Remove the file : %s\n", path );
                }
                else {
                    perror( "Unlink() error" );
                }
                
            }
            
            free( path );
            
        }
        
    }
    
    if ( rmdir(basepath) == 0 ) {
        printf( "Remove the directory : %s\n", basepath );
    }
    else {
        perror( "Rmdir() error" );
    }
    
    closedir(dir);
    
}
