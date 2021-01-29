
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

/*
 The plan :
 
    process command-line arguments
 
        open + read any provided file, or read from standard input
 
        read each line from the input, until the end of the line
 
            break each line into words (sequences of alphabetic characters)
 
                store each word found (only once)
 
        finish read from input
    
    print all words
 
 */

// char *words[1000];
// Give a pointer to a pointer

// The first pointer points to an array of pointers
//      and each element points to a single character

// The single character is folowed by other characters
//      and eventually termianted by '\0' -> string

// ** -----> is a pointer to an array of strings
//      or a pointer to an array of pointers to a character
char **words    = NULL;
int nwords      = 0;


void print_words(void) {

    for (int w = 0; w < nwords; w++) {
        printf("%s\n", words[w]);
    }
    
}

void add_word(char word[]) {
    
    for (int w = 0; w < nwords; w++) {
        if (strcmp(words[w], word) == 0) {
            return;
        }
    }
    
    // Grow words to be more (nwords + 1) in size
    // words = realloc( words, (nwords + 1) * sizeof(char *) );
    words = realloc( words, (nwords + 1) * sizeof(words[0]) );
    words[nwords] = strdup(word);
    ++nwords;
    
}

void find_words(char str[]) {
    
    int s = 0;
    
    while (str[s] != '\0') {
        
        char word[BUFSIZ];
        int  w = 0;
        
        while ( isalpha(str[s]) ) {
            
            word[w++] = str[s++];
            
        }
        
        word[w] = '\0';
        
        if (w > 0) {
            add_word(word);
        }
        
        // Once we found the word, skip whitespace, punctuation
        //      or numbers to get the next alphabetic character
        while (str[s] != '\0' && !isalpha(str[s])) {
            ++s;
        }
    }
    
    
    
//    char string[80] = "This is a pointer to a string\n";
//
//    char *pointer[80];
//
//    pointer[0] = strdup(string);
//
//    printf("%s\n", pointer[0]);
    
}

void read_file(FILE *fp) {
    
    // Read the file line by line
    // The memory is cheap
    char        line[BUFSIZ];
    
    while (fgets(line, sizeof line, fp) != NULL) {
        
        find_words(line);
        
    }
    
}


int main(int argc, char *argv[]) {
    
    // process command-line arguments
    if (argc > 1) {
        
        FILE *fp = fopen(argv[1], "r");
        
        if (fp == NULL) {
            printf("Error.....\n");
            exit(EXIT_FAILURE);
        }
        
        read_file(fp);
        fclose(fp);
        
    }
    else {
        
        // The text file could be redirected from a file or from pipe
        // We could pass the standard input that are made from the system
        read_file( stdin );
        
        // Since this file is opened by someone else, we don't need to close it.
        
    }
    
    print_words();
    
    return 0;
    
}
