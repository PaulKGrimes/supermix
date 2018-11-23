// junctionDesigner.cc
//
// Simulation of a trivially simple mixer, taken from the SuperMix example mixer.cc
// that is optimised for junction area and critical current, given an upper limit on
// the junction critical current and a fixed specific capacitance.
//
// Scans LO frequency, setting the inductance of the tuning inductor at each 
// frequency. At each frequency the area is adjusted to give maximum DSB gain,
// and for each area tested the LO power is also adjusted to give peak DSB gain.
//
#include "supermix.h"

// Define a mixer DSB gain error term.
// Returns the mixer DSB noise
class mixer_DSB_gain_error_term : public error_term
{
public:
    // Constructor takes a reference to a mixer and the relevant ports
    mixer_DSB_gain_error_term(mixer & Mix, int IF_Port, int LSB_Port, int USB_Port):
        mix(&Mix),
        IF_PORT(IF_Port),
        LSB_PORT(LSB_Port),
        USB_PORT(USB_Port) { }
        
    // Calculate the DSB Gain
    virtual double get(state_tag)
    {
        // Perform the harmonic balance
        mix->balance();
        
        sdata s = mix->get_data(); // Small signal analysis
        
        double result = norm(s.S[IF_PORT][LSB_PORT]) + norm(s.S[IF_PORT][USB_PORT]);
        return -result;
    }
    
    virtual ~mixer_DSB_gain_error_term() { /*cout << "mixer_DSB_gain_error_term destructor called\n"; */}
    
    virtual void reset() { /*cout << "mixer_DSB_gain_error_term.reset() called\n";*/ }
     
private:
    mixer *mix;
    int IF_PORT, LSB_PORT, USB_PORT;
};


// Define a mixer DSB gain error term that optimises the LO power
// Returns the mixer DSB noise
class mixer_opt_DSB_gain_error_term : public error_term
{
public:
    // Constructor takes a reference to the mixer, the numbers of various 
    // ports and a reference to the LO power parameter
    mixer_opt_DSB_gain_error_term(mixer & Mix, int IF_Port, int LSB_Port, int USB_Port, 
            parameter scap, parameter &OptJc, parameter & Area,
            parameter &LO_power, parameter &bias, sis_device & SIS):
        mix(&Mix),
        IF_PORT(IF_Port),
        LSB_PORT(LSB_Port),
        USB_PORT(USB_Port),
        sCap(scap),
        optJc(&OptJc),
        area(&Area),
        lo_power(&LO_power),
        bias(&bias),
        sis(&SIS) { }
        
    virtual double get(state_tag)
    {
        // Set sis Capacitance and Rn
        sis->Cap = *area * sCap;
        sis->Rn  = sis->Vn/(*optJc * *area);
        
        // Vary LO power applied to the mixer
        error_func ef;
            *lo_power = ef.vary( 10.0, 100.0, 10000.0, Nano*Watt);
            *bias = ef.vary( 0.5, 2.0, 2.7, mVolt);
            
        mixer_DSB_gain_error_term et(*mix, IF_PORT, LSB_PORT, USB_PORT);
        
        ef.add_term(1.0, et);
        
        mix->initialize_operating_state();
        mix->balance();
        
        powell opt(ef);
        opt.FTOL = 0.0001;
        opt.CLOSENESS = 1;
        opt.FOCUS = 0.01;
        
        double error_val = opt.minimize();
        
        return error_val;
    }

    virtual ~mixer_opt_DSB_gain_error_term() {/* cout << "mixer_opt_DSB_gain_error_term destructor called\n"; */}
    
    virtual void reset() {/* cout << "mixer_opt_DSB_gain_error_term.reset() called\n"; */}
    
private:
    mixer *mix;
    int IF_PORT, LSB_PORT, USB_PORT;
    real_parameter sCap, *optJc, *area;
    real_parameter *lo_power, *bias;
    sis_device *sis;
};
        

int main()
{
    // Global values
    device::T  = 2.5*Kelvin;    // Device temperature
    device::f  = 5*GHz;         // IF output frequency
    device::Z0 = 20*Ohm;        // The RF source impedance
    parameter Zif = 50*Ohm;     // The IF load impedance
    parameter kAcm = 1.0*Kilo*Amp/(Centi*Meter*Centi*Meter);
    
    // Barrier settings
    parameter maxJc = 20*kAcm;
    parameter sCap = 65*fFarad/(Micron*Micron);
    parameter area = 1.0 *Micron*Micron;
    
    // Some variables
    parameter optimumJc;
    parameter optimumArea;
    parameter lo_power;
    double gain;
    
    // Create the mixer components
    
    // IV data object
    ivcurve IV("idc.dat", "ikk.dat");
    
    // SIS junction
    sis_device sis;
        sis.set_iv(IV);
        sis.Vn    = 2.81*mVolt;   // Measured for our junction
        sis.Rn    = 20*Ohm;       // Starting value only
        sis.Cap   = 65*fFarad;    // Starting value only
        
    // Tuning inductor
    inductor tune;
        tune.parallel();
        tune.L = 1.0;             // Starting value only
        
    // IF transformer
    transformer ifTrans;
        ifTrans.Z1 = Zif;              // port 1 faces SIS
        //ifTrans.Z1 = & sis.Rn;
            
    // Bias source
    voltage_source bias;
        bias.source_f = 0.0;
        bias.source_voltage = sis.Vn/2.0;
        
    // LO source
    generator lo;
        lo.source_f = 100*GHz;    // Starting value only
        lo.source_power = &lo_power;  // ditto
        lo.Temp = 0;
        
    // Build the mixer
    mixer mix;
        mix.add_junction(sis);
        mix.set_rf(tune);
        mix.set_if(ifTrans);
        mix.set_bias(bias);
        mix.set_balance_terminator(lo, 2);
        mix.set_LO(& lo.source_f);
        mix.harmonics(1);
        
    cout << "# Mixer complete? "
         << ( ( mix.flag_mixer_incomplete() ) ? "No" : "Yes" )
         << endl;
         
    int IF_PORT  = mix.port(2,0);
    int USB_PORT = mix.port(2,1);
    int LSB_PORT = mix.port(2,-1);
    
    // Mixer is ready to go
    
    // Create the error function for area
    error_func ef;
        // This needs to also alter sis properties
        // do this by passing a reference to the sis and area to the
        // error term
        area = ef.vary(0.01, 1.0, 100.0, Micron*Micron);
        
    // Create the error terms
    mixer_opt_DSB_gain_error_term et(mix, IF_PORT, LSB_PORT, USB_PORT,
            sCap, optimumJc, area, lo_power, bias.source_voltage, sis); 
           
    ef.add_term( 1.0, et);
           
    // Create the optimisers
    powell opt(ef); // For optimising area and LO power
    //opt.very_verbose();
    opt.FTOL = 0.000001;
    opt.FOCUS = 0.5;
    opt.CLOSENESS = 1;
    
    mix.initialize_operating_state();
    mix.balance();
    
    // Start loop over frequency
    for (int f=200; f<=1200; f+=5)
    {
        // Set the frequency
        lo.source_f = double(f)*GHz;
        
        // Bias to the middle of the photon step
        bias.source_voltage = (abs(sis.Vn - (lo.source_f / (2.0*VoltToFreq))) + sis.Vn)/2.0;

        // Set tuning inductance
        tune.L = 1.0/(4*Pi*Pi* f*GHz * f*GHz * sis.Cap);
        
        // Calculate optimum area
        optimumArea = 255000./(f*(device::Z0/Ohm)*(sCap/fFarad));
        optimumJc = f*(sis.Vn/mVolt)/(2.5*device::Z0*optimumArea)*kAcm;
            
        cout << setprecision(4) << f << "\t" << optimumArea/(Micron*Micron) << "\t" << optimumJc/kAcm;            
        
        // Do we need to optimise, or can we just use these values?
        if (optimumJc >= maxJc)
        { 
            optimumJc = maxJc;
        }
        gain = -opt.minimize();
        cout << "\t" << area/(Micron*Micron);
        cout << "\t" << optimumJc/kAcm;
        cout << "\t" << gain;
        //cout << "\t" << bias.source_voltage/mVolt;
        //cout << "\t" << lo_power/(Nano*Watt); 
        cout << "\t" << sis.Rn/Ohm;
        cout << "\t" << sis.Cap/fFarad << endl;
    }
    
    return 0;
}     
