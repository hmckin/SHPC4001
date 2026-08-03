#include <stdio.h>
#include <math.h> // For sqrt(), pow(), etc.
#include <stdio.h>
#include "common.h"
#include <omp.h> 

/*
* Loop Parallelism version:
* Uses updated serial version with various pragmas to parallelize for loops
* in acceleration, velocity and position.
* */


/** Calculate the acceleration for each particle
 * @param opt A pointer to the global options structure.
 * @param parts A pointer to an array of particle structures (will be modified).
 */
void accel_update(struct Options *opt, struct Particle *parts) {
    double rad, rad2, invrad, accel, maxaccel;
    double minrad = DBL_MAX;
    double massave, newstep;
    double delta[3];
    int i, j, k;
    struct timeval time1;

    // Get start time
    time1 = init_time();

    // Initialize particle accelerations to zero
    // #pragma omp parallel for: This parallelizes the loop.
    // Each iteration of the loop (for a different 'i') will be executed by a different thread.
    // private(k): The loop variable 'k' is declared private to each thread because its value is specific
    // to each iteration and threads should not share or interfere with each other's 'k'.
    #pragma omp parallel for private(k)
    for (i = 0; i < opt->nparts; i++) {
        for (k = 0; k < 3; k++) {
            parts[i].accel[k] = 0.0;
        }
    }

    // Calculate gravitational and collision forces
    // #pragma omp parallel for: parallelizes the outer for loop.
    // private(j, k, delta, rad, rad2, invrad): These variables are declared private to each thread.
    // They are used within each iteration of the outer i loop (and inner 'j', 'k' loops) and shouldn't
    // be shared among threads to avoid race conditions.
    // reduction(min:minrad): 'minrad' tracks the minimum distance found across all particle pairs.
    // This is a reduction operation since each thread will find its local minimum and then these
    // local minimums need to be combined into a single global minimum.
    #pragma omp parallel for private(j, k, delta, rad, rad2, invrad) reduction(min:minrad)
    for (i = 0; i < opt->nparts; i++) {
        for (j = 0; j < opt->nparts; j++) {
            if (i != j) {
                // Get the position difference
                for (k = 0; k < 3; k++) {
                    delta[k] = parts[j].position[k] - parts[i].position[k];
                }
                period_wrap_delta(opt->period, delta);

                // Calculate squared distance and radius
                rad2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
                rad = sqrt(rad2);

                // Update minrad for smoothing
                if (rad < minrad) minrad = rad;

                // Gravitational force with smoothing (attractive)
                invrad = 1.0 / (rad2 + 0.05 * parts[i].radius * parts[i].radius);
                for (k = 0; k < 3; k++) {
                    parts[i].accel[k] += invrad * delta[k] / rad * parts[j].mass * opt->grav_unit;
                }

                // Collisional force (uses opposite delta direction of accel, original serial file had different
                // defintion for delta in the collision loop)                
                if (rad < parts[i].radius) {
                    invrad = 1.0 / (parts[i].radius * parts[i].radius);
                    for (k = 0; k < 3; k++) {
                        parts[i].accel[k] += invrad * (-delta[k]) / rad * parts[j].mass * opt->grav_unit * opt->collision_unit;
                    }
                }
            }
        }
    }

    // Adaptive time step calculation based on closest particle pair
    // This section stays serial, it depends on the global 'minrad' the parallel region ^^ 
    if (opt->itimestepcrit == TimeStepCrit_Adaptive) {
        // get average mass between particle pairs with min distance
        // massave = 0.5*(parts(iminrad)%mass + parts(jminrad)%mass)
        newstep = opt->time_step_fac * sqrt(2.0 * minrad * minrad * minrad / (opt->grav_unit * opt->vlunittolunit * opt->massave));
        if (newstep < opt->tunit) {
            opt->time_step = newstep;
        } else {
            opt->time_step = opt->tunit;
        }
    }

    printf("Finished calculating acceleration\n");
    get_elapsed_time(time1);
}


/**
 * Updates the velocities of all particles based on their accelerations.
 * @param opt A pointer to the global options structure.
 * @param parts A pointer to an array of particle structures (will be modified).
 */
void velocity_update(const struct Options *opt, struct Particle *parts) {
    struct timeval time1 = init_time();
    double dkin_min = HUGE_VAL;
    double dkin_max = 0.0;
    double dkin_ave = 0.0;

    // #pragma omp parallel for: Parallelizes the loop.
    // reduction(min:dkin_min), reduction(max:dkin_max), reduction(+:dkin_ave) are reduction clauses.
    // Each thread will compute its local min, max, and sum for kinetic energy changes.
    // OpenMP combines these into the global dkin_min, dkin_max and 'dkin_ave' vars.
    #pragma omp parallel for reduction(min:dkin_min) reduction(max:dkin_max) reduction(+:dkin_ave)
    for (int i = 0; i < opt->nparts; i++)
    {
        // Update velocities by determining velocity change
        double delta[3];
        for (int k = 0; k < 3; k++) delta[k] = parts[i].accel[k] * opt->time_step;

        // Update the particle's velocity
        // and get the fractional change in kinetic energy
        double dkin_num = 0, vel_sq = 0;
        for (int k = 0; k < 3; k++)
        {
            dkin_num += delta[k] * delta[k];
            vel_sq += parts[i].velocity[k] * parts[i].velocity[k];
            parts[i].velocity[k] += delta[k];
        }
        double current_dkin = (vel_sq > 1e-12) ? dkin_num / vel_sq : 0.0;

        dkin_min = (current_dkin < dkin_min) ? current_dkin : dkin_min;
        dkin_max = (current_dkin > dkin_max) ? current_dkin : dkin_max;
        dkin_ave += current_dkin;
    }
    dkin_ave /= opt->nparts;

    printf("Finished calculating velocity\n");
    printf("Particles specific kinetic energy changed by [min, ave, max]: %e %e %e\n",
           dkin_min, dkin_ave, dkin_max);
    get_elapsed_time(time1);
}

/**
 * Updates the positions of all particles based on their velocities.
 * @param opt A pointer to the global options structure.
 * @param parts A pointer to an array of particle structures (will be modified).
 */
void position_update(const struct Options *opt, struct Particle *parts) {
    double *rads = malloc(opt->nparts * sizeof(double));
    struct timeval time1 = init_time();
    double rad_average = 0.0;
    double rmin = HUGE_VAL;
    double rmax = 0.0;

    // #pragma omp parallel for: Parallelizes the loop updating positions.
    // reduction(+:rad_average): 'rad_average' is a sum across all particles, so needs
    // a sum reduction.
    #pragma omp parallel for reduction(+:rad_average)
    for (int i = 0; i < opt->nparts; i++)
    {
        // Calculate delta position
        double delta[3];
        for (int k = 0; k < 3; k++) {
            delta[k] = parts[i].velocity[k] * opt->time_step * opt->vlunittolunit;
            parts[i].position[k] += delta[k];
        }
        period_wrap(opt->period, parts[i].position);
        // get fractional change in position normalized by approximate average density
        double norm_delta_squared =
            (delta[0]*delta[0]) +
            (delta[1]*delta[1]) +
            (delta[2]*delta[2]);
        rads[i] = sqrt(norm_delta_squared) / (opt->initial_size / pow((double)opt->nparts, 1.0 / 3.0));
        rad_average += rads[i];
    }
    rad_average /= opt->nparts;

    // Find min and max of rads
    // #pragma omp parallel for: This loop finds the minimum and maximum values in the 'rads' array.
    // reduction(min:rmin), reduction(max:rmax): These are reduction clauses to correctly find
    // the global minimum and maximum 'rad' values across all threads.
    #pragma omp parallel for reduction(min:rmin) reduction(max:rmax)
    for (int i = 0; i < opt->nparts; i++) {
        rmin = (rads[i] < rmin) ? rads[i] : rmin;
        rmax = (rads[i] > rmax) ? rads[i] : rmax;
    }

    printf("Finished calculating position\n");
    printf("Particles moved by [min, ave, max]: %e %e %e\n",
           rmin, rad_average, rmax);
    get_elapsed_time(time1);
    free(rads);
}

/**
 * @brief Writes output data for the current simulation step to a file.
 * @param opt Pointer to the Options structure containing configuration.
 * @param parts Pointer to the array of particle data.
 * @param step The current simulation step number.
 */
void nbody_output(const struct Options *opt, const struct Particle *parts, int step) {
    FILE *fp;
    if (step == 0) {
            fp = fopen(opt->outfile, "w");
    } else {
            fp = fopen(opt->outfile, "a");
    }
    if (!fp) {
            printf("Error opening output file %s\n", opt->outfile);
            return;
    }
    for (int i = 0; i < opt->nparts; i++) {
        fprintf(fp,
            "%d %f %lld %f %f %f %f %f %f %f %f %e %e %e %lld\n",
            step, opt->time,
            parts[i].ID,
            parts[i].mass, parts[i].radius,
            parts[i].position[0], parts[i].position[1], parts[i].position[2],
            parts[i].velocity[0], parts[i].velocity[1], parts[i].velocity[2],
            parts[i].accel[0], parts[i].accel[1], parts[i].accel[2],
            parts[i].PID
        );
    }
    fclose(fp);
}


/**
 * Main function: The entry point of the NBody simulation program.
 */
int main(int argc, char *argv[]) {

    // Create an instance of Options
    struct Options opt;

    // Call the function to get input (similar to Fortran's getinput)
    getinput(argc, argv, &opt);
    // generate initial conditions for the particles
    struct Particle *parts = generate_IC(&opt);

    // Variables for timing
    struct timeval time1, time2;

    printf("Running simulation for %d steps...\n", opt.nsteps);
    time1 = init_time();

    // Loop over each step in the simulation
    for (int step = 0; step < opt.nsteps; step++)
    {
        time2 = init_time();
        visualise(&opt, parts, step);
        nbody_output(&opt, parts, step);
        accel_update(&opt, parts);
        velocity_update(&opt, parts);
        position_update(&opt, parts);
        get_elapsed_time(time2);
        opt.time += opt.time_step;
        printf("Finished step %d moving %e in time to %e\n", step, opt.time_step, opt.time);
    }
    get_elapsed_time(time1);

    free(parts);

    return 0;
}
