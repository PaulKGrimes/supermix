

// Error term that returns the (negative) gain of a mixer
class mixer_gain : public error_term
{
    public:
    // Constructor takes a reference to a mixer and it's relevant ports
        mixer_gain(mixer & Mix, int IF_Port, int RF_Port):
            mix(&Mix),
            IF_PORT(IF_Port),
            RF_PORT(RF_Port) { }

        // calculate the input referenced noise temperature of the mixer
        virtual double get(state_tag)
        {
            // perform the harmonic balance
            mix->balance();

            sdata s = mix->get_data(); // small signal analysis data
            
            return -abs(s.S[IF_PORT][RF_PORT]);
        };

        // This is called by error_func objects, so must exist even if it does nowt
        virtual void reset() { };

    private:
        mixer *mix;
        int IF_PORT, RF_PORT;
};


// Error term that returns a large number if the SIS mixer stability
// criterion is not satisifed
//
// The stability criterion is in general that Y[0][0]-Yload+Re(Yload) > 0
//
// For a mixer with real IF load, this reduces to Y[0][0] > 0

class mixer_stability : public error_term
{
public:
    // Constructor takes a reference to the mixer
    mixer_stability(mixer & Mix, int IF_Port, complex_parameter * ZLoad):
        mix(&Mix),
        IF_PORT(IF_Port),
        Zload(ZLoad)
        { }
        
    virtual double get(state_tag)
    {
        // perform the harmonic balance
        mix->balance();
        
        sdata s = mix->get_data(); // small signal analysis data
        
        ydata y(s);
        
        Complex Yif = y.Y[IF_PORT][IF_PORT] - ( 1.0/ *Zload);
        
        if (real(Yif+ (1.0/ *Zload)) >= 0)
        {
            //cerr << "Operating point stable\n";
            return 0.0;
        }
        else
        {
            //return 1.0;
            return norm(Yif+ (1.0/ *Zload));
        }
    };
    
    virtual void reset() { };
    
    private:
        mixer *mix;
        int IF_PORT;
        complex_parameter * Zload;
};
        
        
