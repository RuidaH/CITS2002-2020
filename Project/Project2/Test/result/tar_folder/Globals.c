 #include "Declaration.h"

char**           g_argv     = NULL;
char**           temp_dirs  = NULL;
int              g_argc     = 0;
int              number     = 0;
int              nfiles     = 0;
int              ndirs      = 0;
int              ntemps     = 0;
int              path_len   = 0;
DIR_INFO*        all_dirs   = NULL;
FILE_INFO*       all_files  = NULL;

