/*
 *  bouncing3.c
 *  Written by: Harry McKinney Student #: 25258748
 *  Feb 25, 2026
 *  Assignment 1
 *  Exercise 3
 *  Description: Investigating changes to dt on output.
 *
 */

#include <stdio.h>

int main(void)
{

    int steps;
    float x,v,t,g,dt;
    FILE* output_file;
    
    x = 1.0;
    v = 0.0;
    g = 9.8;
    dt = 0.005;
    t = 0.0;
    
    output_file = fopen("bounce3_005.txt","w"); /*name txt according to dt*/
    
    for(steps=1;steps<=600;steps++) /*modify the steps to accumulate t=3 total*/
    { 
        t += dt;
        v -= g*dt;  
        x += v*dt;   
        if(x < 0)
        {
            x = 0;
            v = -v;
        }
        
        fprintf(output_file,"%f %.7f %f \n",t,x,v); 
       
    }
     fclose(output_file);
}

