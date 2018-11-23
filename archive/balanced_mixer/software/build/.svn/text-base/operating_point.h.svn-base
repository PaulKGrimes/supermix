/*******************************************************************************
** operating_point.h -   file included in balanced.c, containing code to      **
**                       calculate the operating point values for the mixer   **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04 - created from SuperMix-realistic version                **
** ========================================================================== **
** get_Bias and get_Lo_power must be called after setting FREQUENCY           **
** get_Lo_Power implicitly assumes that Re(Zemb) == Rn.  Corrections can be   **
** made in the main code                                                      **
** ========================================================================== **
** Changes:                                                                   **
**      21/10/04 - PKG - Changed Bias_Voltage declaration to Bias_Voltage_1   **
**                       and Bias_Voltage_2, and altered functions so that    **
**                       they now do nothing, to avoid the need for unit      **
**                       strings.                                             **
*******************************************************************************/

parameter Frequency;

parameter get_Frequency(parameter F)
{
    parameter Freq;
    
    Freq = F;
    
    return Freq;
}

parameter Bias_Voltage_1;
parameter Bias_Voltage_2;

parameter get_Bias(parameter B)
{
    parameter Bias_V;
    
    Bias_V = B;
    
    return Bias_V;
}

parameter Lo_Power;

parameter get_Lo_Power(parameter P)
{
    parameter Power;
    
    Power = P;
    
    return Power;
}

