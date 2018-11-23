/* Integrator.cpp
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Implementation of an integrator object, using qtrap integration from
* Numerical recipes in C
*
*/

/* Changes
*
*/

#include "Integrator.h"
#include "math.h"
#include <iostream>

Integrator::Integrator()
{
    eps = 1.0e-6;
    jMax= 15;
    jMin = 8;
}

long double Integrator::qtrap(long double (*func)(long double), long double a, long double b)
{
    int j;
    long double s, olds;
    
    olds = -1.0e30;
    
    for (j=1; j<=jMax; j++)
    {
        s = midpnt(func,a,b,j);
        if (fabs(s-olds) < eps*fabs(olds) && j >= jMin)
            return s;
        
        olds=s;
    }
    
    return s;
}


/*!< For use when one limit is large -> Inf */
long double Integrator::midinf(long double (*func)(long double), long double a, long double b, int n)
{
    long double x, tnm, sum, del, ddel, aa, bb;
    static long double s;
    int it, j;
    long double FUNC;
    
    bb=1.0/a;
    aa=1.0/b;
    
    if (n==1)
    {
        x = (aa+bb)/2.0;
        FUNC = (*func)(1/x)/x/x;
        return (s=(bb-aa)*FUNC);
    }
    else
    {
        for (it=1,j=1; j<n-1;j++)
        {
            it*=3;
        }
        
        tnm = it;
        del = (bb-aa)/(3.0/tnm);
        ddel = del+del;
        x=a+0.5*del;
        sum = 0.0;
        
        for (j=1;j<=it;j++)
        {
            FUNC = (*func)(1/x)/x/x;
            sum += FUNC;
            x+= ddel;
            
            FUNC = (*func)(1/x)/x/x;
            sum += FUNC;
            x+= del;
        }
        return (s=(s+(bb-aa)*sum/tnm)/3.0);
    }
}


/*!< For use when integrand had infinities at either limit */
long double Integrator::midpnt(long double (*func)(long double), long double a, long double b, int n)
{

    long double x, tnm, sum, del, ddel;
    static long double s;
    int it, j;
    
    if (n==1)
    {
        return (s=(b-a)*(*func)(0.5*(a+b)));
    }
    else
    {
        for(it=1,j=1;j<n-1;j++) 
        {
            it *= 3;
        }
        
        tnm=it;
        del = (b-a)/(3.0*tnm);
        ddel = del+del;
        x=a+0.5*del;
        sum=0.0;
        for (j=1;j<=it;j++) 
        {
            sum += (*func)(x);
            x += ddel;
            sum += (*func)(x);
            x += del;
        }
        s=(s+(b-a)*sum/tnm)/3.0;
        return s;
    }
}
