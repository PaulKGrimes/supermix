/*******************************************************************************
** tapered_mstrip.h - SuperMix class defining a tapered microstrip section    **
**                    Device is modelled as a series of short microstrip      **
**                    sections.  Currently uses a linear taper in width       **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 25/10/04                                                          **
** ========================================================================== **
** Class is similar to the radial_stub class, and operates in a similar way   **
** ========================================================================== **
** Changes:                                                                   **
** ========================================================================== **
** Bugs:                                                                      **
*******************************************************************************/

#ifndef TAPERED_MSTRIP_H
#define TAPERED_MSTRIP_H 1

#include "supermix.h"
#include "circuit.h"
#include "trlines.h"

class tapered_mstrip: public data_ptr_nport
{
public:
    // Port 1 width
    parameter width1;
    
    // Port 2 width
    parameter width2;
    
    // Total length
    parameter length;
    
    // Substrate thickness
    parameter sub_thick;

    // Default constructor
    tapered_mstrip();

    // Copy constructor
    tapered_mstrip(const tapered_mstrip &tm);

    // Assignment operator
    tapered_mstrip & operator=(const tapered_mstrip &r);

    // Functions to set layer pointers
    tapered_mstrip & substrate(dielectric & d);   /* substrate dielectric */
    tapered_mstrip & superstrate(dielectric & d); /* superstrate dielectric */
    tapered_mstrip & top_strip(surfimp & s);      /* top strip surface impedance */
    tapered_mstrip & ground_plane(surfimp & s);   /* ground plane surface impedance */
    
    // tapered_mstrip is a two port device
    int size() {return 2;};
    
    // Virtual destructor required
    virtual ~tapered_mstrip() { }
    
private:
    // This is the actual circuit
    circuit taper;
    
    // No. of sections used.  Needs to be 1 or more less than number below
    int nsections;

    // Define the microstrip sections
    microstrip sec[64];

    // Pointers to layers
    dielectric *sub;
    dielectric *super;
    surfimp *top;
    surfimp *ground;

    // Functions for doing the calculations
    void recalc(bool noise);
    void recalc() { recalc(true); }
    void recalc_S() { recalc(false); }
};

#endif /* TAPERED_MSTRIP_H */
    
    

