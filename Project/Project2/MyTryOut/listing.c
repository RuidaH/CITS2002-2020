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

void listFilesRecursively(char *path);


int main()
{
    // Directory path to list files
    char path[100];

    // Input path from user
    printf("Enter path to list files: ");
    scanf("%s", path);

    listFilesRecursively(path);

    return 0;
}


/**
 * Lists all files and sub-directories recursively
 * considering path as base path.
 */
void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            struct stat stat_buffer;
            struct stat *pointer = &stat_buffer;
            
            printf("%s\n", dp->d_name);
            
            stat(dp->d_name, &stat_buffer);
            
            if ( S_ISDIR ( stat_buffer.st_mode ) ) {

                printf( "\nThe name is %s\n", dp->d_name );
                printf( "%s is a directory\n", path );
                printf( "The last modified time is %s\n", ctime(&pointer->st_mtime) );


            }
            else if (S_ISREG ( stat_buffer.st_mode ) ) {

                printf( "\nThe name if %s\n", dp->d_name );
                printf( "%s is a regular file\n", path );
                printf( "The last modified time is %s\n", ctime(&pointer->st_mtime) );

            }

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            


            listFilesRecursively(path);
        }
    }

    closedir(dir);
}
