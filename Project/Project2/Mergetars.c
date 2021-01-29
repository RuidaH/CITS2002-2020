#include "Declaration.h"

// Example command : ./Mergetars a1.tar ./Test/a.tar a2.tar ./Test/Final.tar

int main(int argc, char *argv[]) {
    
    g_argc = argc;
    g_argv = argv;
    
    if ( argc < 2 ) {
        
        printf("Usuage :  %s input_tarfile1 [input_tarfile2 ...] output_tarfile\n", argv[0]);
        
        exit(EXIT_FAILURE);
        
    }
    else {
        
        // Number of input tar files
        number = argc - 2;
        
        process_flow ();
        
        exit(EXIT_SUCCESS);
        
    }
    
    return 0;
}


/**
 What this function does:
  1. Get all the info of all the files with latest version and store in the data structure (all_files)
  2. Meanwhile, get all the info of directory and store them in another structure (all_dirs)
  3. Create the whole directory structure in an empty dir (for the output tar file)
  4. Copy all the files to the right location
  5. Delete all the temp dir for the input tar files
  6. Use tar command to put everything into the output tar files
  7. Delete the temp dir for output tar file
 */
void process_flow () {
    
    // Create a temp dir for the output tar files
    char dirname[ MAXFILELEN ];
    
    strcpy( dirname, TEMPLATE );
    mkdtemp( dirname );

    
    // Scan multiple input tar files
    // Get all the file and dir info of the latest version
    Many_searches();
    
    
    // Creat the whole directory structure
    for (int i = 0; i < ndirs; i++ ) {
        
        // Creat the new location
        char newdirpath[ MAXFILELEN ];
        
        strcpy( newdirpath, dirname );
        strcat( newdirpath, all_dirs[i].localpath );

        if ( mkdir( newdirpath, 0700 ) != 0 ) {
            perror("Error on mkdir()");
            exit(EXIT_FAILURE);
        }
        
    }
    
    
    // Copy all the files
    for ( int k = 0; k < nfiles; k++ ) {
        
        // Creat the new location
        char newfilepath[ MAXFILELEN ];
        
        strcpy( newfilepath, dirname );
        strcat( newfilepath, all_files[k].localpath );

        copy( all_files[k].pathname, newfilepath );

    }

    
    // Delete the temp dirs for the input tar files
    for ( int i = 0; i < ntemps; i++ ) {
        
        deleteAllRecursively( temp_dirs[i] );
        
    }


    // Tar everything inside the temp dir of the output tar files
    tar_all ( dirname );

    
    // Delete the temp dir for the output tar file
    deleteAllRecursively( dirname );
    
    
    // Do we need to consider the type of the tar file tar / tgz ? (Yes)
    // One more question. Do we need to delete the input tar files? (Not sure)
    // Still have one question, do we need to change the modification time of the dir? (NO)
    
}

