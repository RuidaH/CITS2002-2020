#include <stdlib.h>     // declares printf()
#include <stdio.h>      // declares exit() and EXIT_SUCCESS

/*
    August 2020       
Su Mo Tu We Th Fr Sa  
                   1  
 2  3  4  5  6  7  8  
 9 10 11 12 13 14 15  
16 17 18 19 20 21 22  
23 24 25 26 27 28 29  
30 31 
*/

#define N_WEEKS             6
#define N_DAYS_IN_WEEK      7

#define DEF_MONTH           2
#define DEF_YEAR            2020

int first_day_of_month(int month, int year){
    
    return 6;                   // for 1st Aug 2020  
}


void mycal(int month, int year)
{
    int days_in_month;    

    switch(month) {
        case 1:
                days_in_month = 31;    
                printf("    January %i\n", year);  
                break;
        case 2:
                days_in_month = 28;                                             
                printf("    February %i\n", year);                               
                break;
        case 8:                                                                 
                days_in_month = 31;                                             
                printf("    August %i\n", year);                             
                break;   
    }

    printf("Su Mo Tu We Th Fr Sa\n");
    
    int cell = 1;           
    int day1 = first_day_of_month(month, year);

    for (int w = 0; w < N_WEEKS; ++w) {
        for (int c = 0; c < N_DAYS_IN_WEEK; ++c) {
            int diff = cell - day1;

            if (diff < 1 || diff > days_in_month){
                printf("   ");
            }
            else {
                printf("%2i ", diff);
            }
            ++cell;
        }
        printf("\n");   
    }
}

int main(int argcount, char * argvalues[])
{
    int month  = DEF_MONTH;
    int year   = DEF_YEAR;

    if (argcount == 2){
        year = atoi(argvalues[1]);          // ./mycal year
    }
    else if (argcount == 3) {
        month = atoi(argvalues[1]); 
        year  = atoi(argvalues[2]);         // ./mycal month year 
    }
    
    mycal(month, year);
    exit(EXIT_SUCCESS);
}
