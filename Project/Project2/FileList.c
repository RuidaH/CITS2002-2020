#include "Declaration.h"



// Add the latest files or directories in the corresponding data structure
void add_to_structures ( struct dirent *dp, int index, char *path ) {
    
    struct stat stat_buffer;
    struct stat *pointer = &stat_buffer;
    
    char *localpath = NULL;
    
    if (stat( path, &stat_buffer ) != 0) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    
    // If it's a directory
    if ( S_ISDIR ( stat_buffer.st_mode ) ) {
        
        strcpy( all_dirs[index].dirname, dp->d_name );
        strcpy( all_dirs[index].pathname, path );

        localpath = path + path_len;
        strcpy( all_dirs[index].localpath, localpath );

    }
    // If it's a regular file
    else {
        
        strcpy( all_files[index].filename, dp->d_name );
        strcpy( all_files[index].pathname, path );

        all_files[index].last_modified_time = pointer->st_mtime;
        all_files[index].f_size = pointer->st_size;

        localpath = path + path_len;
        strcpy( all_files[index].localpath, localpath );

    }
    
}



// Check the file or directory before adding them into the data structure
void checking ( struct dirent *dp, char *path ) {
    
    enum { NOT_CHECK = 0, IS_FILE_TRUE = 1, IS_FILE_FLASE = 2, IS_DIR_TRUE = 3, IS_DIR_FALSE = 4 };
    
    int           file_status = NOT_CHECK;
    int           dir_status  = NOT_CHECK;
    
    struct stat   stat_buffer;
    struct stat   *pointer = &stat_buffer;

    if ( stat( path, &stat_buffer ) != 0 ) {
        perror("Error on stat()");
        exit(EXIT_FAILURE);
    }
    else if ( S_ISREG ( pointer->st_mode ) ) {
        
        file_status = IS_FILE_TRUE;
        
        for ( int i = 0; i < nfiles; i++ ) {
            
            // If you find the same name in the file data structure
            if (  Is_same_path ( path, all_files[i].pathname ) ) {
                
                file_status = IS_FILE_FLASE;
                
                // New file is late than the old one
                // Replace the old one with new one
                if ( difftime( pointer->st_mtime, all_files[i].last_modified_time ) > 0 ) {
                    
                    add_to_structures( dp, i, path );
                    
                }
                // Having the same moified time, compare file size
                // The size of new file is greater than the old one, get replaced
                else if ( difftime( pointer->st_mtime, all_files[i].last_modified_time ) == 0 && pointer->st_size > all_files[i].f_size ) {
                    
                    add_to_structures( dp, i, path );
                    
                }
                // Having the same size, just get the latest version file
                else if ( pointer->st_size == all_files[i].f_size ) {
                    
                    // replce old file with new file
                    // remove old file
                    add_to_structures( dp, i, path );
                    
                }
                
                break;

            }
            
        }
        
    }
    else if ( S_ISDIR ( pointer->st_mode ) ) {
        
        dir_status = IS_DIR_TRUE;
        
        for ( int k = 0; k < ndirs; k++ ) {
            
            // If there is already the same directroy in the structure, stop looping
            if ( Is_same_path ( path, all_dirs[k].pathname ) ) {
                
                // No need to get it repalced, it's just a directory
                dir_status = IS_DIR_FALSE;
                break;
                
            }
            
        }

    }
    
    
    if ( file_status == IS_FILE_TRUE ) {
        
        all_files = realloc( all_files, ( nfiles + 1 ) * sizeof( FILE_INFO ) );

        // A brand new file, so add it into all_files
        add_to_structures( dp, nfiles, path );
        
        nfiles++;
        
    }
        
    if ( dir_status == IS_DIR_TRUE ) {
        
        all_dirs = realloc( all_dirs, ( ndirs + 1 ) * sizeof( DIR_INFO ) );

        // A brand new directory, so add it into all_dirs
        add_to_structures( dp, ndirs, path );
        
        ndirs++;
        
    }
    
}



// A recursive function to iterate through all the files and directories
void find_files( char *pathname ) {
    
    char path[ MAXPATHLEN ];
    
    DIR            *dirp;
    struct dirent  *dp;
    
    dirp = opendir( pathname );
    
    // Do I need to use perro function here?
    if ( !dirp ) {
        return;
    }
    
    while ((dp = readdir(dirp)) != NULL) {
        
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            
            // Construct new path from our base path
            strcpy( path, pathname );
            strcat( path, "/" );
            strcat( path, dp->d_name );
            
            checking ( dp, path );

            find_files( path );
            
        }
        
    }
    
    closedir( dirp );
        
}
