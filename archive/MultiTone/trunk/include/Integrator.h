/* Integrator.h
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Definition of integrator class, capable of using qtrap integration from
* Numerical Recipes in C
*
*/

/* Changes
*
*/

#ifndef IntegratorH
#define IntegratorH

class Integrator
{
public:
    Integrator();
    
    long double qtrap(long double (*func)(long double), long double a, long double b);
    long double midinf(long double (*func)(long double), long double a, long double b, int n);
    long double midpnt(long double (*func)(long double), long double a, long double b, int n);
    
    long double eps;
    int jMax;
    int jMin;
};

#endif

