#include "support.h"

#define	MAX_WORDLEN		8

char    word[MAX_WORDLEN+1];

char *random_word(void)
{
    int len = 1 + rand() % (MAX_WORDLEN-1);

    for(int w=0 ; w<len ; ++w) {
        word[w] = 'a' + rand() % 26;
    }
    word[len] = '\0';
    return word;
}

long microseconds(void)
{
    struct timeval  t;

    gettimeofday( &t, NULL );
    return (t.tv_sec * 1000000) + t.tv_usec;
}

