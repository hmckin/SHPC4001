/*
 *  bouncing2.c
 *  Written by: Harry McKinney Student #: 25258748
 *  Feb 25, 2026
 *  Assignment 1
 *  Exercise 1
 *  Description: Fixing the algorithmic error by updating velocity first, then position.
 *  As well as swapping negative position in if to 0.
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
    
    output_file = fopen("bounce2.txt","w");
    
    for(steps=1;steps<=300;steps++)
    { 
        t += dt;
        v -= g*dt;  /*swap 32 and 33*/
        x += v*dt;   
        if(x < 0)
        {
            x = 0; /*change x=-x to x=0*/
            v = -v;
        }
        
        fprintf(output_file,"%f %.7f %f \n",t,x,v); 
       
    }
     fclose(output_file);
}

