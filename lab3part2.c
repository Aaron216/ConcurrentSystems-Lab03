/* File:    lab3part2.c
 * Purpose: Compute a definite integral using the trapezoidal rule and OpenMP.
 *
 * Input:   a, b, n
 * Output:  Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.
 *
 * Compile: gcc ...
 * Usage:   ./lab3part2 <number of threads>
 *
 * Note:
 *      1.  f(x) is hardwired
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
 #include <omp.h>
#endif
#include "timer.h"

double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
    double  global_result = 0.0;    /* Store result in global_result */
    double  a, b;                   /* Left and right endpoints      */
    int     n;                      /* Total number of trapezoids    */
    int     thread_count;
    double  start, finish;

    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        printf("%s <number of threads>\n", argv[0]);
        return(EXIT_FAILURE);
    }

    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    GET_TIME(start);
    global_result = Trap(a, b, n, thread_count);
    GET_TIME(finish);

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
    printf("Time: %e\n", finish-start);

    return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input arg:   x
 * Return val:  f(x)
 */
double f(double x) {
    double return_val;

    return_val = x*x*x*x;
    return return_val;
}  /* f */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to estimate definite integral
 * Input args:
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Return val:
 *    approx:  estimate of integral from a to b of f(x)
 */
double Trap(double a, double b, int n, int thread_count) {
    double  h, approx;
    int  i;

    h = (b-a)/n; 
    approx = (f(a) + f(b))/2.0; 
    #pragma omp parallel for num_threads(thread_count) reduction(+: approx)
    for (i = 1; i <= n-1; i++) {
        approx += f(a + i*h);
    }
    approx = h*approx; 

    return approx;
}  /* Trap */
