// beamsplitter.h
//======================================================================
// Paul Grimes, 3rd March, 2003
//======================================================================
// A 3 port object representing a beamsplitter, including noise due to
// reflection and transmission losses when the beamsplitter is not at
// device::T.
//
// Ports are laid out as follows:
//      Port 1, straight through input, antennuated by transmission loss
//      Port 2, reflected input, antennuated by reflection loss
//      Port 3, output
//
// Beamsplitter does not operate in 4 port mode (with port 4 output being
// port 1 reflected + port 2 transmitted) at the moment, as this requires
// careful termination of port 4, and we have no need for this object at
// the moment.
//
// Also includes a 2 port beamsplitter for use with hot/cold load
// simulations using the LO temperature as the temperature of the load,
// and for simulation of dielectric windows.
//
//=======================================================================
// Changes:
//      Made the transmission lossy.  Added a parameter loss_tangent
//      equal to tan delta
//=======================================================================

#include "nport.h"

// The 3 port beamsplitter
class beamsplitter: public nport
{
public:
    // Parameters
    // Dielectric constant of the material used
    parameter epsilon;

    // Dielectric loss tangent (tan delta)
    parameter loss_tangent;

    // Thickness of the beamsplitter
    parameter thickness;

    // Angle of incidence
    parameter angle;

    // Physical temperature
    parameter Temp;

    // Polarisation parallel (1) or perpendicular (0)
    int polarisation;


    // Methods
    // Constructor
    //   Represents a beamsplitter
    //   in perpendicular polaristion by default
    beamsplitter(int p = 0);

    // Beamsplitter is a 3 port object
    int size() { return 3; }

    // Beamsplitter is an active noise source if device::T != 0, and the beamsplitter
    // is not null (due to zero thickness, or same dielectric constant as air/free space)
    const nport::data_info & get_data_info()
    { info.active = (Temp != device::T && thickness != 0.0 && epsilon != 1.0); return info; }

private:
    // The calculation is done here.
    double reflect();
    double transmit();
    void recalc();
    void recalc_S();
};

// The reduced beamsplitter
class beamsplitter_red: public nport
{
public:
    // Parameters
    // Dielectric constant of the material used
    parameter epsilon;

    // Dielectric loss tangent (tan delta)
    parameter loss_tangent;

    // Thickness of the beamsplitter
    parameter thickness;

    // Angle of incidence
    parameter angle;

    // Physical temperature
    parameter Temp;

    // Polarisation parallel (1) or perpendicular (0)
    int polarisation;


    // Methods
    // Constructor
    //   Represents a beamsplitter
    //   in perpendicular polaristion by default
    beamsplitter_red(int p = 0);

    // Reduced beamsplitter is a 2 port object
    int size() { return 2; }

    // Beamsplitter is an active noise source in device::T != 0, and the beamsplitter
    // is not null (due to zero thickness, or same dielectric constant as air/free space)
    const nport::data_info & get_data_info()
    { info.active = (Temp != device::T && thickness != 0.0 && epsilon != 1.0); return info; }

private:
    // The calculation is done here.
    double reflect();
    double transmit();
    void recalc();
    void recalc_S();
};
