#include "Declaration.h"



// Scan multiple input tar files
void Many_searches() {

    for ( int i = 0; i < number; i++ ) {
        
        // Pass the tarfile name to function
        One_search( g_argv[i + 1] );
        
    }
    
}



void One_search( char tarfile[] ) {
            
        char newdirname[ MAXFILELEN ];
        
        // Create a temporary directory for an input tar file
        strcpy( newdirname, TEMPLATE );
        mkdtemp( newdirname );
        
        char slash = '/';
        char dirpath[ MAXPATHLEN ];
        
        // Give a slash to the newdirname
        strcpy( dirpath, newdirname );
        strncat( dirpath, &slash, 1 );
    
        // Store the location of the temp dirs (for deleting part)
        temp_dirs = realloc( temp_dirs, ( ntemps + 1 ) * sizeof( char * ) );
        temp_dirs[ntemps] = strdup( newdirname );
        ntemps++;
    
        path_len = strlen(newdirname);
        
        int status;
        int newpid = fork();
        
        // Fork a process to call the tar command
        switch (newpid) {
                
            case -1: {
                printf("Cannot fork!\n");
                exit(EXIT_FAILURE);
                break;
            }
            
            case 0: {
                // Expand the tarfile into the directory
                execl( "/usr/bin/tar", "/usr/bin/tar", "-xf", tarfile, "-C", dirpath,  (char*)NULL );
                
                exit(EXIT_FAILURE);
                break;
            }
            
            default : break;
                
        }
            
        while ( (newpid = wait( &status )) <= 0 ) {
            perror("Error on wait()");
            exit(EXIT_FAILURE);
        }
        
        find_files( newdirname );
    
}




// Check whether they have the same relative path or not
bool Is_same_path ( char *path1, char *path2 ) {
    
    int index1 = 0;
    int index2 = 0;
    
    // Do we need to add more characters here to make sure it's right
    // May be we can use path_len to get to the right place
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



// Copy the file from source path to destination path
void copy( char *source_path, char *destination_path ) {

    FILE  *src   = fopen( source_path, "rb" );
    FILE  *dest  = fopen( destination_path, "wb" );
    
    if ( dest != NULL && src != NULL ) {
        
        char   buffer[BUFSIZ];
        size_t got, wrote;
        
        while( ( got = fread(buffer, 1, sizeof buffer, src) ) > 0 ) {
            
            wrote = fwrite( buffer, 1, got, dest );
            
            if ( wrote != got ) {
                perror("Error on write()");
                exit(EXIT_FAILURE);
            }
            
        }
        
    }
    
    if ( src != NULL ) {
        fclose(src);
    }
    
    if ( dest != NULL ) {
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
        exit(EXIT_FAILURE);
    }
    
    
}



// Delete everything in the basepath
void deleteAllRecursively( char *basepath ) {
    
    size_t  path_len;
    char *  path;
    DIR  *  dir;
    
    struct stat     stat_path, stat_entry;
    struct dirent   *dp;
    
    // Listing all the properties of the path first
    stat( basepath, &stat_path );
    
    // Path is not a directory
    if ( S_ISDIR ( stat_path.st_mode ) == 0 ) {
        perror( "S_ISDIR() Error" );
        exit(EXIT_FAILURE);
    }
    
    // Cannot open the directory
    if ( (dir = opendir(basepath) ) == NULL ) {
        perror( "Opendir() Error" );
        exit(EXIT_FAILURE);
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

                deleteAllRecursively( path );

            }
            // If it's not directory, just delete it
            else {
                
                // Return 0 if it succeed
                if ( unlink( path ) != 0 ) {
                    perror( "Unlink() error" );
                    exit(EXIT_FAILURE);
                }
                
            }
            
            free( path );
            
        }
        
    }
    
    if ( rmdir( basepath ) != 0 ) {
        perror( "Rmdir() error" );
        exit(EXIT_FAILURE);
    }
    
    closedir(dir);
    
}



// Use tar command to put everything in one dir into the output tar file
void tar_all ( char *dirname ) {
    
    // Need to fork a process here
    int status;
    int pid = fork();
    
    switch (pid) {
            
        case -1: {
            printf("Cannot fork!\n");
            exit(EXIT_FAILURE);
            break;
        }
        
        case 0: {
            
            execl( "/usr/bin/tar", "/usr/bin/tar", "-cf", g_argv[g_argc - 1], "-C", dirname, ".",  (char*)NULL );
            
            exit(EXIT_FAILURE);
            break;
            
        }
        
        default : break;
            
    }
        
    while ( (pid = wait( &status )) <= 0 ) {
        perror("Error on wait()");
        exit(EXIT_FAILURE);
    }
    
    
}
