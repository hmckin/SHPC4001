/*
 *  bouncing5.c
 *  Written by: Harry McKinney Student #: 25258748
 *  Feb 25, 2026
 *  Assignment 1
 *  Exercise 5
 *  Description: Investigating changes to elastic conditions.
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
    dt = 0.01;
    t = 0.0;
    
    output_file = fopen("bounce5.txt","w");
    
    for(steps=1;steps<=300;steps++)
    { 
        t += dt;
        v -= g*dt;  
        x += v*dt;   
        if(x < 0)
        {
            x = 0;
            v = -0.90*v; /*Small change here*/
        }
        
        fprintf(output_file,"%f %.7f %f \n",t,x,v); 
       
    }
     fclose(output_file);
}

