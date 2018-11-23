/* Integrator.h
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Definition of integrator class, capable of using qtrap integration from
* Numerical Recipes in C
*
*/

/* Changes
*    01/07/04 : PKG : Changed long doubles to doubles to fit with SuperMix storage 
*               convention used elsewhere.
*/

#ifndef IntegratorH
#define IntegratorH

class Integrator
{
public:
    Integrator();
    
    double qtrap(double (*func)(double), double a, double b);
    double midinf(double (*func)(double), double a, double b, int n);
    double midpnt(double (*func)(double), double a, double b, int n);
    
    double eps;
    int jMax;
    int jMin;
};

#endif

