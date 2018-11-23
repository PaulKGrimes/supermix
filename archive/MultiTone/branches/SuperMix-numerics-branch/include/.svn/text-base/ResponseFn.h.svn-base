/* ResponseFn.h
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Definition of a response function object for storing SIS IV and KK 
* curve data
*
*/

/* Changes
*    01/07/04 : PKG : Changed long doubles to doubles to fit with SuperMix storage 
*               convention used elsewhere.  Response function as not been changed 
*               to use SuperMix vectors, as .erase is not implemented for them.
*/

#ifndef ResponseFnH
#define ResponseFnH

#include <vector>
#include <string>

#include "Integrator.h"

class ResponseFn
{
private:
    /* Data members */
    std::vector<double> Vdc;  /*!< DC IV voltage data */
    std::vector<double> Idc;  /*!< DC IV current  data */
    std::vector<double> Ikk;  /*!< KK transform of DC IV curve data */
    
    double vGap;         /*!< IV curve gap voltage */
    double iGap;         /*!< IV curve critical current */
    double Rn;           /*!< IV normal state resistance */
    
    std::string idcFile;      /*!< Filename from which Vdc and Idc was read, or "Kennedy Fit <n>" */
    std::string ikkFile;      /*!< Filename from which Ikk was read, or "Calculated" */
    bool readyFlag;           /*!< Flags if vectors and data are consistent */     
    
    /* Internal variables */
    double yIntercept;   /*!< Current defect (yIntercept of linear portion of IV curve) */
    double sNormal;      /*!< Normal conductance (slope of linear portion of IV curve) */
    double deltaV;       /*!< Voltage difference between adjacent data points?? */ 
    double vMax;         /*!< Maximum normalised voltage in table */
    Integrator ITG;           /*!< For use in calculating Ikk transform */
    
    
    /* Private methods */
    void findIkk();           /*!< Calculate Ikk data from Idc data */
    void normalise();         /*!< Normalise Idc data */
    void addMorePoints();     /*!< Use extrapolated slope and intercept to add points to data vectors */
    double bubbleFind(double, std::vector<double> &, std::vector<double> &, int, int); /*!< Finds interpolated value in yVals that corresponds to x in xVals */
    bool loadData(std::string dcFile); /*!< function for loading data from file */
    bool loadData(std::string dcFile, std::string kkFile);
    

public:
    ResponseFn();             /*!< Constructor */
    
    bool ready() { return readyFlag; };    /*!< Returns true is data is consistent, otherwise false */
    void kennedy(int n, int noPoints);     /*!< Fills data vectors using Kennedy approximation to SIS curve */
    void data( std::string dcFile );           /*!< Fills Vdc, Idc and Ikk from file. Calculates KK transform if Ikk data isn't valid */
    void data(std::string dcFile, std::string kkFile);
    
    double get_Idc( double V ); /*!< Returns interpolated value of IV curve */  
    double get_Ikk( double V ); /*!< Returns interpolated value of KK transform */    
         
    double get_Vdc( int n ); /*!< Returns nth voltage in Vdc */
    double get_Idc( int n ); /*!< Returns nth current in Idc */
    double get_Ikk( int n ); /*!< Returns nth current in Ikk */
    
    /* Accessors */
    double get_vGap() { return vGap; }; /*!< Return gap voltage */
    double get_iGap() { return iGap; }; /*!< Return critical current */
    double get_Rn() { return Rn; };     /*!< Return normal resistance */
    double get_vMax() { return vMax; }; /*!< Return highest voltage in data vector */ 
    int get_no_points() { return Vdc.size(); };   /*!< Return number of points in data vectors */
    std::string get_idcFile() { return idcFile; }; /*!< Return the filename used to fill the Vdc and Idc vectors */
    std::string get_ikkFile() { return ikkFile; }; /*!< Return the filename used ti fill the Ikk vector */
    
    void set_vGap(double V) { vGap=V; iGap = vGap/Rn; }; /*!< Set the gap voltage */
    void set_iGap(double I) { iGap=I; Rn = vGap/iGap; }; /*!< Set the gap current (critical current) */
    void set_Rn(double R) { Rn = R; iGap = vGap/Rn; };   /*!< Set the normal resistance */ 
    void set_vMax(double V) { vMax=V; };                 /*!< Set the maximum voltage to extend new data to */
    
};

/*! Exception thrown when data is requested from an unready response function */
struct ResponseFn_not_ready { };
        
#endif
