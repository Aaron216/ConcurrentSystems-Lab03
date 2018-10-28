/* File:    lab3part1.c
 * Purpose: Estimate pi using OpenMP and a monte carlo method
 * 
 * Compile: gcc ...
 *          *needs my_rand.c, my_rand.h
 *
 * Run:     ./lab3part1 <number of threads> <number of tosses>
 *
 * Input:   None
 * Output:  Estimate of pi
 *
 * Note:    The estimated value of pi depends on both the number of threads and the number of "tosses".  
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_rand.h"
#include "timer.h"

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double pi_estimate;
    int thread_count;
    long long int number_in_circle;
    long long int number_of_tosses;
    double start, finish;

    // Check correct number of arguments
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        printf("%s <# of threads> <# of tosses>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    thread_count = strtol(argv[1], NULL, 10);
    number_of_tosses = strtoll(argv[2], NULL, 10);

    number_in_circle = 0;
    GET_TIME(start)
    #pragma omp parallel num_threads(thread_count) reduction(+: number_in_circle)
    {
        int my_rank = omp_get_thread_num();
        unsigned seed = my_rank + 1;
        long long int toss;
        double x, y, distance_squared;

        #pragma omp for
        for(toss = 0; toss < number_of_tosses; toss++) {
            x = 2*my_drand(&seed) - 1;
            y = 2*my_drand(&seed) - 1;
            distance_squared = x*x + y*y;
            if (distance_squared <= 1) {
                number_in_circle++;
            }
        }
    }
    GET_TIME(finish);

    pi_estimate = 4*number_in_circle/((double) number_of_tosses);
    printf("Estimated pi: %e\n", pi_estimate);
    printf("Time: %e\n", finish-start);

    return EXIT_SUCCESS;
}
