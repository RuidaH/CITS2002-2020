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

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>

#define MAXPATHLEN          50

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

void print_file(char *filename) {
    
    FILE *fp;
    char line[BUFSIZ];
    
    fp = fopen(filename, "r");
    
    if (fp == NULL) {
        perror( progname );
        exit(EXIT_FAILURE);
    }
    
    while ( fgets( line, sizeof line, fp ) != NULL ) {
        printf( "%s", line );
    }
    
    fclose(fp);
    
}

void list_directory(char *dirname) {
    
    DIR             *dirp;
    struct dirent   *dp;
    
    dirp = opendir( dirname );
    
    if (dirp == NULL) {
        perror( progname );
        exit(EXIT_FAILURE);
    }
    
    while ( (dp = readdir( dirp )) != NULL ) {
        printf( "%s\n", dp->d_name );
    }
    
    closedir(dirp);
    
    
}



// This is only for one directory
void list_all_files_in_one_directory(char *dirname) {
    
    char fullpath[MAXPATHLEN];
    
    DIR            *dirp;
    struct dirent  *dp;
    
    dirp = opendir( dirname );
    
    // readdir reads the files and subdirectories of the current directory
    while ( (dp = readdir(dirp)) != NULL) {
        
        struct stat stat_buffer;
        struct stat *pointer = &stat_buffer;
        
        // The path is stored in fullpath
        sprintf(fullpath, "%s/%s", dirname, dp->d_name);
        
        // May be I can write its subdirectories in a text file
        
        if (stat(fullpath, pointer) != 0) {
            perror( progname );
        }
        else if ( S_ISDIR ( pointer->st_mode ) ) {
            printf( "%s is a directory\n", fullpath );
            printf( "The last modification time is %s \n", ctime(&stat_buffer.st_mtime));
        }
        else if ( S_ISREG ( pointer->st_mode ) ) {
            printf( "%s is a regular file\n", fullpath );
            printf( "The last modification time is %s \n", ctime(&stat_buffer.st_mtime));
        }
        else {
            printf( "%s is unknown!\n", fullpath);
        }
        
    }
    closedir(dirp);
    
}




int main(int argc, char *argv[]) {
    
    if( argc != 2 ) {
        
        printf("Usuage :  %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
        
    }
    else {
        
        progname = argv[1];
        
        // print_file(argv[1]);
        // list_directory(argv[1]);
        // file_attributes(argv[1]);
        list_all_files_in_one_directory(argv[1]);
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
    
}
