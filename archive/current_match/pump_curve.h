/*******************************************************************************
** pump_curve.h - Contains definition of pump_curve class for matching LO     **
**            power level of simulated curve to measured curve                **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 08/08/03                                                          **
** ========================================================================== **
** pump_curve is a subclass of iv_match that knows its own LO power.          **
** The get function sets the LO power, calculates the error value and then    **
** resets the LO power to its initial value                                   **
**                                                                            **
** To use this with the optimiser, the error_func must be told to vary        **
** parameter pump_curve.matched_lo_power                                      **
** ========================================================================== **
** Changes:                                                                   **
**   11/08/03 - Changed order of constructor parameters and included *Lo_f as **
**       a pointer to the parameter shadowed by the LO's frequency attribute. **
**       Added accessor functions and required attributes.                    **
**   12/08/03 - Copied get_a() and get_b() methods from iv_match, added       **
**      assignment to mix and V pointers in constructor.  This class is now   **
**      almost a complete rewrite of the iv_match class, which we should      **
**      complete at a later date, for neatness's sake.                        **
** ========================================================================== **
** Bugs:                                                                      **
*******************************************************************************/


class pump_curve : public iv_match
{
public:
    // Constructor
    pump_curve(mixer &m,                     // The mixer to simulate.
        const abstract_real_parameter &bias, // Parameter controlling bias voltage
        real_parameter *Lo_p,                // LO Power parameter shadowed by the LO source
        real_parameter *Lo_f,                // LO Frequency parameter shadowed by the LO source
        std::string file,                    // Name of file holding measured pumped IV.
        real_parameter Freq,                 // LO Frequency corresponding to measured data
        double V_units=mVolt,                // Units of file's bias voltage data
        double I_units=Micro*Amp             // Units of file's current data
        ) :
            iv_match(m, bias, file.c_str(), V_units, I_units)
            {
                mix = &m;
                V = &bias;
                Lo_Power = Lo_p;
                Lo_Freq = Lo_f;
                filename = file;
                frequency = Freq;
                vUnits = V_units;
                iUnits = I_units;
            }
    // Copy constructor
    pump_curve(const pump_curve& rhs);
    
    // Overloaded assignment operator
    pump_curve& operator=(const pump_curve& rhs);

    parameter matched_lo_power; // The current best fit LO power
    parameter frequency;        // The value of the LO frequency corresponding to the measured data
    double vUnits;              // The units of the bias voltage in the file
    double iUnits;              // The units of the DC current in the file
    std::string filename;       // The name of the measured data file
    real_parameter * Lo_Power;  // pointer to the parameter shadowed by the LO generator's power attribute
    real_parameter * Lo_Freq;   // pointer to the parameter shadowed by the LO generator's freq attribute
    // Override iv_match private attributes with public versions
    mixer * mix;                // The mixer object.  Public ptr will override private ptr?
    const abstract_real_parameter * V; // The bias voltage parameter
    parameter V_off;            // Offset to apply to the voltage
    parameter I_off;            // Offset to apply to the current
    Vector currents;            // The vector of currents
    real_interp measured;       // The measured data interpolator

    // Function for returning the error value
    double get();
    double get(state_tag);
    double get_a() { return measured(V_off + *V) - I_off; }
    double get_b();

    // Accessor functions
    void setMixer(mixer &m) { mix = &m;};
    void setBias(const abstract_real_parameter &bias) {V = &bias;}
    void setVunits(double V_units=mVolt);
    void setIunits(double I_units=Micro*Amp);
    void setFilename(std::string file);
};

// Copy constructor
pump_curve::pump_curve(const pump_curve& rhs)
            : iv_match::iv_match(*rhs.mix, *rhs.V, rhs.filename.c_str(), rhs.vUnits, rhs.iUnits),
              matched_lo_power(rhs.matched_lo_power), frequency(rhs.frequency),
              vUnits(rhs.vUnits), iUnits(rhs.iUnits), filename(rhs.filename),
              Lo_Power(rhs.Lo_Power), Lo_Freq(rhs.Lo_Freq),
              mix(rhs.mix), V(rhs.V),
              V_off(rhs.V_off), I_off(rhs.I_off),
              currents(rhs.currents), measured(rhs.measured)
            {
                measured.file(filename.c_str(), vUnits, iUnits);
            }


// Overloaded assignment operator
pump_curve& pump_curve::operator=(const pump_curve& rhs)
{
    mix = rhs.mix;
    V = rhs.V;
    filename = rhs.filename;
    vUnits = rhs.vUnits;
    iUnits = rhs.iUnits;
    Lo_Power = rhs.Lo_Power;
    Lo_Freq = rhs.Lo_Freq;
    frequency = rhs.frequency;
    matched_lo_power = rhs.matched_lo_power;
    measured = rhs.measured;
    V_off = rhs.V_off;
    I_off = rhs.I_off;
    currents = rhs.currents;

    measured.file(filename.c_str(), vUnits, iUnits);

    return *this;
}


void pump_curve::setVunits(double V_units)
{
    vUnits = V_units;
    // Rebuild the real_interp object .measured with a new x unit
    measured.file(filename.c_str(), vUnits, iUnits);
}

void pump_curve::setIunits(double I_units)
{
    iUnits = I_units;
    // Rebuild the real_interp object .measured with a new y unit
    measured.file(filename.c_str(), vUnits, iUnits);
}

void pump_curve::setFilename(std::string file)
{
    filename = file;
    // Rebuild the real_interp object .measured with the new dataset
    measured.file(filename.c_str(), vUnits, iUnits);
}

double pump_curve::get()
{
    // set the Power and Frequency of the LO to the local value
    real_parameter temp_power = *Lo_Power;
    real_parameter temp_freq  = *Lo_Freq;
    *Lo_Power = matched_lo_power;
    *Lo_Freq  = frequency;

    // Use the iv_match get_a/b functions
    double i1 = get_a();
    double i2 = get_b();

    // Reset the mixer's LO power and frequency back to its original value
    *Lo_Power = temp_power;
    *Lo_Freq  = temp_freq;

    return (i1-i2)*(i1-i2);
}

double pump_curve::get(state_tag)
{
    return get();
}

double pump_curve::get_b()
{
    mix->balance();

    // Sum all junction DC currents into the variable c
    currents = mix->I_junc(0);  // vector of DC bias currents
    int min = currents.minindex();
    int max = currents.maxindex();
    double c = 0.0;
    for(int i=min; i<=max; i++) c += currents[i].real;

    return c;
}
