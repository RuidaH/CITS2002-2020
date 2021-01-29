#include "Declaration.h"

void read_directories_and_files (char *dirname) {
    
    char fullpath[MAXPATHLEN];
    
    DIR             *dirp;
    struct dirent   *dp;
    
    dirp = opendir( dirname );
    
    // Read all the files and subdirectories of the current directory
    while ( ( dp = readdir(dirp) ) != NULL ) {
        
        struct stat stat_buffer;
        struct stat *pointer = &stat_buffer;
        
        // Store the absolute path of a file into variable fullpath
        sprintf( fullpath, "%s/%s", dirname, dp->d_name );
        
        if ( stat(fullpath, pointer) != 0 ) {
            perror( progname );
        }
        else if ( S_ISDIR ( pointer->st_mode ) ) {
            
            printf( "The name is %s\n", dp->d_name);
            printf( "%s is a directory\n", fullpath );
            printf( "The last modification time is %s \n", ctime(stat_buffer.st_mtime));
            
            // Do I use recursive function or fork a child process again
            if (strncmp(dp->d_name, ".", 1) != 0 && strncmp(dp->d_name, "..", 2) != 0) {
                printf("---Did you get in?\n");
                char *new_dirname = dp->d_name;
                read_directories_and_files (new_dirname);

            }
            
            // For now, there is segment fault in recursive function, let's try another method first.
            
        }
        else if ( S_ISREG ( pointer->st_mode ) ) {
            
            printf( "%s is a regular file\n", fullpath );
            printf( "The last modification time is %s \n", ctime(stat_buffer.st_mtime));
            
        }
        else {
            printf( "%s is unknown!\n", fullpath);
        }
        
    }
    
    closedir( dirp );
    
}
