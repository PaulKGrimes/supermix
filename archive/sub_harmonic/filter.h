// filter.h
//=======================================================================
// Paul Grimes, 14th Feb, 2006
//=======================================================================
// Idealised filter objects with square passbands.  Defined by band edge 
// frequencies. 2 port objects.
//
// Filter transmission is lossless within the band, and filters are
// either perfectly reflecting or perfectly absorbing out of band.
//=======================================================================
// Changes:
//=======================================================================

#include "nport.h"

// The reflective filter
class filter_refl: public nport
{
public:
    // Parameters
    // Lower edge of bandpass
    parameter freq_low;

    // Upper edge of bandpass
    parameter freq_hi;

    // Physical temperature
    parameter Temp;

    // Methods
    // Constructor
    filter_refl();

    // Filter is a 2 port object
    int size() { return 2; }

    // Filter is perfectly reflective and therefore a passive noise source
    const nport::data_info & get_data_info()
    { info.active = false; return info; }

private:
    // The calculation is done here.
    void recalc();
    void recalc_S();
};

// The absorbing filter
class filter_abs: public nport
{
public:
    // Parameters
    // Lower edge of bandpass
    parameter freq_low;

    // Upper edge of bandpass
    parameter freq_hi;

    // Physical temperature
    parameter Temp;

    // Methods
    // Constructor
    filter_abs();

    // Filter is a 2 port object
    int size() { return 2; }

    // Filter is perfectly absorbing and therefore an active noise source if
	// Temp not equal to ambient
    const nport::data_info & get_data_info()
    { info.active = (Temp != device::T); return info; }

private:
    // The calculation is done here.
    void recalc();
    void recalc_S();
};
