//
//  demo2.c
//  
//
//  Created by 何瑞达 on 14/8/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

double degrees_to_radians(double degrees)
{
    return (degrees * M_PI / 180.0);
}


double radians_to_degrees(double radians)
{
    return (radians * 180.0 / M_PI);
}


bool valid_location(double lat, double lon)
{
    return ( lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon <= 180.0 );
}


double haversine(double lat1, double lon1, double lat2, double lon2)
{
    double deltalat = (lat2 - lat1) / 2.0;
    double deltalon = (lon2 - lon1) / 2.0;
    
    // Use radians to calculate
    double sin1     = sin( degrees_to_radians(deltalat) );
    
    double cos1     = cos( degrees_to_radians(lat1) );
    double cos1     = cos( degrees_to_radians(lat2) );
    
    double sin2     = sin( degrees_to_radians(deltalon) );
    
    double x        = 
    
    return 0.0;
}


int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: lat1 lon1 lat2 lon2 %d\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else {
        double lat1     = atof(argc[1]);
        double lon1     = atof(argc[2]);
        double lat2     = atof(argc[3]);
        double lon2     = atof(argc[4]);
        
        if (valid_location(lat1, lon1) == false){
            printf("Invalid starting location.\n");
            exit(EXIT_FAILURE);
        }
        
        if (valid_location(lat2, lon2) == false){
            printf("Invalid starting location.\n");
            exit(EXIT_FAILURE);
        }
        
        printf("distance = %lfm\n", haversine(lat1, lon1, lat2, lon2) );
    }
    exit(EXIT_SUCCESS);
    return 0;
}
