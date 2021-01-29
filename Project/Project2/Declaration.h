//  CITS2002 Project 2 2020
//  Names:             Ruida He, Ron Zatuchny
//  Student numbers:   22762872, 22984076

#ifndef Declaration_h
#define Declaration_h


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


// Declare global preprocessor constants
#define _POSIX_C_SOURCE     200809L
#define MAXPATHLEN          1024
#define MAXFILELEN          200
#define MAXSTRUCTLEN        500
#define TEMPLATE            "./TEMP-XXXXXX"


// Declare global functions
extern void             Many_searches();
extern void             process_flow ();
extern void             tar_all ( char * );
extern void             One_search( char* );
extern void             find_files( char* );
extern void             copy( char*, char* );
extern bool             Is_same_path( char*, char* );
extern void             deleteAllRecursively( char * );
extern void             checking( struct dirent *, char* );
extern void             add_to_all_files( struct dirent *, int, char* );


// Self-defined structure
typedef struct {
    
    char     filename[ MAXFILELEN ];
    char     pathname[ MAXPATHLEN ];
    char     localpath[ MAXPATHLEN ];
    time_t   last_modified_time;
    long int f_size;
    
} FILE_INFO;

typedef struct {
    
    char     dirname[ MAXFILELEN ];
    char     pathname[ MAXPATHLEN ];
    char     localpath[ MAXPATHLEN ];
    
} DIR_INFO;



// Declare global variables
extern char**           g_argv;
extern char**           temp_dirs;
extern int              g_argc;
extern int              number;
extern int              nfiles;
extern int              ntemps;
extern int              ndirs;
extern int              path_len;
extern FILE_INFO*       all_files;
extern DIR_INFO*        all_dirs;



#endif /* Declaration_h */


