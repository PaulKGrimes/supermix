/*******************************************************************************
** operating_point.inc - file included in simple.c, containing code to        **
**                       calculate the operating point values for the mixer   **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 22/07/03                                                          **
** ========================================================================== **
** get_Bias and get_Lo_power must be called after setting FREQUENCY           **
** get_Lo_Power implicitly assumes that Re(Zemb) == Rn.  Corrections can be   **
** made in the main code                                                      **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

parameter Frequency;

parameter get_Frequency(parameter F)
{
    parameter Freq;
    
    if (FREQUENCY_unitStr == "GHz") {Freq = F;}
    else // FREQUENCY_unitStr == "NORM"
    {Freq = F*VGAP*eCharge*GHz/(hPlanck*1.0e9);}
    
    return Freq;
}

parameter If_Freq;

parameter get_If_Freq(parameter F)
{
    parameter Freq;
    
    if (FREQUENCY_unitStr == "GHz") {Freq = F;}
    else // FREQUENCY_unitStr == "NORM"
    {Freq = F*VGAP*eCharge*GHz/(hPlanck*1.0e9);}
    
    return Freq;
}

parameter Bias_Voltage;

parameter get_Bias(parameter B)
{
    parameter Bias_V;
    
    if (BIAS_unitStr == "mV") {Bias_V = B;}
    else if (BIAS_unitStr == "Vph")
    { Bias_V = VGAP - B*FREQUENCY*hPlanck/(eCharge)*mVolt;}
    else // BIAS_unitStr == "NORM"
    { Bias_V = B*VGAP; }
    
    return Bias_V;
}

parameter Lo_Power;

parameter get_Lo_Power(parameter P)
{
    parameter Power;
    
    if (LO_POWER_unitStr == "nW") {Power = P;}
    else if (BIAS_unitStr == "Vs")
    { Power = pow(P,2)/(8*RNA/AREA)*1.0e-9*Nano*Watt; }
    else // BIAS_unitStr == "NORM" i.e. in alpha
    { Power = pow(P*2*Pi*FREQUENCY, 2)*AREA/RNA*Nano*Watt; }
    
    return Power;
}

