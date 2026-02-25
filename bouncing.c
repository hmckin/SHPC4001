/*
 *  bouncing.c
 *  
 *  A c program calculating the trajectory of a bouncing ball.
 *  Uses the Euler methof to compute the trajectory of a bouncing 
 *  ball assuming perfect reflection at the surface x = 0
 *  USE SI units (meters and seconds)
 *  Created by Filip Welna on Wed Aug 14 2002.
 *  Copyright (c) 2002 UWA physics All rights reserved.
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
    
    output_file = fopen("bounce.dat","w");
    
    for(steps=1;steps<=300;steps++)
    { 
        t += dt;     /*shorthand equivalent to t = t+dt */ 
        x += v*dt;
        v -= g*dt;
        if(x < 0)
        {
            x = -x;
            v = -v;
        }
        
        fprintf(output_file,"%f %.7f %f \n",t,x,v); 
       
    }
     fclose(output_file);
}

