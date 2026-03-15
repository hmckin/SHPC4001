/*
 *  bouncing4.c
 *  Written by: Harry McKinney Student #: 25258748
 *  Feb 25, 2026
 *  Assignment 1
 *  Exercise 4
 *  Description: Investigating changes to initial position and velocity.
 *
 */

#include <stdio.h>

int main(void)
{

    int steps;
    float x,v,t,g,dt;
    FILE* output_file;
    
    x = 1.0; /* Varied values here*/
    v = 5.0; /*Varied values here*/
    g = 9.8;
    dt = 0.01;
    t = 0.0;
    
    output_file = fopen("bounce4_vpos5.txt","w"); /*name txt according to variation*/
    
    for(steps=1;steps<=300;steps++)
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

