// cpw_new.c
// 
// Code to calculate coplanar waveguides (for eventual use as a supermix object)
// using the calculations outlined in Ghione and Naldi, IEEE-MTT 35 vol. 3, pp 
// 260-267, 1987, and citations.
// 
// Based on S-C conformal transformation of various CPW geometries, and using
// GSL complete elliptic integral of first kind code
//
// Paul Grimes, 2009
//
// This code uses the GNU Scientific Library, and as such is released subject to
// the terms of the GNU Public Licence v3
//================================================================
//
// Test code: Frequency in GHz, width, gap and substrate height in micron,
//            wiring layer thickness in nanometres


#include <iostream>

// Include the general GSL Code
#include <gsl/gsl_math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_const_mksa.h>

// Include the GSL Elliptic Integral code
#include <gsl/gsl_sf_ellint.h>

int main(int argc, char **argv)
{
    // Set up the basic input and output variables
    double freq, width, gap, wire_thick, sub_thick, er;
    double Zchar, Eeff;

    if (argc != 7) 
    {
        std::cout << "# Give me a frequency, width, gap, wiring layer thickness, substrate thickness and dielectric constant"
                  << std::endl;
        return 1;

    } else {
        freq = atof(argv[1])*1.0e9;
        width = atof(argv[2])*1.0e-6;
        gap = atof(argv[3])*1.0e-6;
        wire_thick = atof(argv[4])*1.0e-9;
        sub_thick = atof(argv[5])*1.0e-6;
        er = atof(argv[6]);
    }

    // Correct width and gap for metallization thickness
    double delta = ( 1.24*wire_thick/M_PI )*( 1 + log(4*M_PI*width/wire_thick) );
    double w = width + delta;
    double g = gap - delta;

    // Calculate k and kprime
    double k = w / (w+2*g);
    double kprime = sqrt(1-gsl_pow_2(k));


    std::cout << "##      width = " << width << std::endl
             << "##        gap = " << gap << std::endl
             << "## wire_thick = " << wire_thick << std::endl
             << "##  sub_thick = " << sub_thick << std::endl
             << "##         er = " << er << std::endl 
             << "##      delta = " << delta << std::endl
             << "##          w = " << w << std::endl
             << "##          g = " << g << std::endl
             << "##          k = " << k << std::endl;

    // Calculate the elliptic integrals
    double K = gsl_sf_ellint_Kcomp(k, GSL_PREC_DOUBLE);
    double Kprime = gsl_sf_ellint_Kcomp(kprime, GSL_PREC_DOUBLE);

    // Calculate Eeff
    Eeff = (er + 1)/2.0;

    // Correct Eeff for finite substrate thickness (1st approximation)
    // (After Kittara 2002, Method I.2 and Devis 1973)
    double devis = tanh(1.785*log(sub_thick/g) + 1.75) + 
                    (k*g/sub_thick)*( 0.04 - 0.7*k + 0.01*(1-0.1*er)*(0.25+k) );
    Eeff = Eeff*devis;

    // Correct Eeff for surface wave emission
    // (After Kittara 2002, Method I.3 and Hashain et al 1986)
    double p = log(w/sub_thick);
    double v = 0.43 - 0.86*p + 0.54*gsl_pow_2(p);
    double u = 0.54 - 0.64*p + 0.015*gsl_pow_2(p);
    double a = exp(u*log(w/g)) + v;
    double freqTE = GSL_CONST_MKSA_SPEED_OF_LIGHT / (4*sub_thick*sqrt(er+1));

    double hashain = ( sqrt(er) - sqrt(Eeff) )/( 1+a*pow((freq/freqTE),-1.8) );

    Eeff = gsl_pow_2(sqrt(Eeff)+hashain);

    // Calculate Zchar
    Zchar = (30*M_PI*Kprime) / (sqrt(Eeff) * K);
    
    
    // Calculate conduction loss according to Wheeler's method
    // (After Kittara 2002 and Gupta 1996)
    
    // Calculate the surface impedance from the (complex) conductivity
    double Cond_real = 1.6e7;
    double MU_0 = GSL_CONST_MKSA_VACUUM_PERMEABILITY;
    gsl_complex j = gsl_complex_rect(0.0, 1.0);
    gsl_complex cond = gsl_complex_rect(Cond_real, 0.0);
    gsl_complex Zsurf;
    
    Zsurf = gsl_complex_sqrt(gsl_complex_div(gsl_complex_mul_real(j,2*M_PI*freq*MU_0),cond));
    Zsurf = gsl_complex_mul(Zsurf, gsl_complex_coth(gsl_complex_mul_real(gsl_complex_sqrt(gsl_complex_mul(gsl_complex_mul_real(j, 2*M_PI*freq*MU_0), cond)), wire_thick)));
    
    double P;
    if (k <= sqrt(2.0)) {
    	P = k / ((1-kprime)*pow(kprime, 3.0/2.0));
	} else {
		P = 1.0/((1-k)*sqrt(k)) * gsl_pow_2(Kprime/K);
	}
	
    double Pprime = gsl_pow_2(K/Kprime)*P;
    double wheeler_num = 1.0 + (1.25/M_PI)*(wire_thick/width + log(4*M_PI*width/wire_thick));
    double wheeler_don = 2 + (width/gap) - (1.25*wire_thick/(M_PI*gap))*(1+log(4*M_PI*gap/wire_thick));
    gsl_complex wheeler_fac = gsl_complex_mul_real(Zsurf, 4.88e-4*Eeff*Zchar*Pprime/(M_PI*gap)*(1+width/gap));
    gsl_complex alpha_c = gsl_complex_mul_real(wheeler_fac, wheeler_num/gsl_pow_2(wheeler_don));


    std::cout << "# Coplanar waveguide calculation" << std::endl
              << "# Schwartz-Christoffel method with thick metal, finite substrate " << std::endl 
              << "# and TE_0 surface waves" << std::endl
              << "# Eeff  = " << Eeff << std::endl
              << "# Zchar = " << Zchar << std::endl
              << "# Zsurf = " << GSL_REAL(Zsurf) << " + " << GSL_IMAG(Zsurf) << "i" << std::endl
              << "# Alpha = " << 10*log(GSL_REAL(alpha_c))/1.0e3 << " + " << 10*log(GSL_IMAG(alpha_c))/1.0e3 << "i" << std::endl;

    return 0;
}



