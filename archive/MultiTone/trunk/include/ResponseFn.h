/* ResponseFn.h
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Definition of a response function object for storing SIS IV and KK 
* curve data
*
*/

/* Changes
*
*/

#ifndef ResponseFnH
#define ResponseFnH

#include <vector>
#include <complex>
#include <string>

#include "Integrator.h"

#ifndef complex
#define complex complex<long double>
#endif

class ResponseFn
{
private:
    /* Data members */
    std::vector<long double> Vdc;  /*!< DC IV voltage data */
    std::vector<long double> Idc;  /*!< DC IV current  data */
    std::vector<long double> Ikk;  /*!< KK transform of DC IV curve data */
    
    long double vGap;         /*!< IV curve gap voltage */
    long double iGap;         /*!< IV curve critical current */
    long double Rn;           /*!< IV normal state resistance */
    
    std::string idcFile;      /*!< Filename from which Vdc and Idc was read, or "Kennedy Fit <n>" */
    std::string ikkFile;      /*!< Filename from which Ikk was read, or "Calculated" */
    bool readyFlag;           /*!< Flags if vectors and data are consistent */     
    
    /* Internal variables */
    long double yIntercept;   /*!< Current defect (yIntercept of linear portion of IV curve) */
    long double sNormal;      /*!< Normal conductance (slope of linear portion of IV curve) */
    long double deltaV;       /*!< Voltage difference between adjacent data points?? */ 
    long double vMax;         /*!< Maximum normalised voltage in table */
    Integrator ITG;           /*!< For use in calculating Ikk transform */
    
    
    /* Private methods */
    void findIkk();           /*!< Calculate Ikk data from Idc data */
    void normalise();         /*!< Normalise Idc data */
    void addMorePoints();     /*!< Use extrapolated slope and intercept to add points to data vectors */
    long double bubbleFind(long double, std::vector<long double> &, std::vector<long double> &, int, int); /*!< Finds interpolated value in yVals that corresponds to x in xVals */
    bool loadData(std::string dcFile); /*!< function for loading data from file */
    bool loadData(std::string dcFile, std::string kkFile);
    

public:
    ResponseFn();             /*!< Constructor */
    
    bool ready() { return readyFlag; };    /*!< Returns true is data is consistent, otherwise false */
    void kennedy(int n, int noPoints);     /*!< Fills data vectors using Kennedy approximation to SIS curve */
    void data( std::string dcFile );           /*!< Fills Vdc, Idc and Ikk from file. Calculates KK transform if Ikk data isn't valid */
    void data(std::string dcFile, std::string kkFile);
    
    long double get_Idc( long double V ); /*!< Returns interpolated value of IV curve */  
    long double get_Ikk( long double V ); /*!< Returns interpolated value of KK transform */    
    
    long double get_Idc( double V ) { return get_Idc((long double) V); };
    long double get_Ikk( double V ) { return get_Ikk((long double) V); };
     
    long double get_Vdc( int n ); /*!< Returns nth voltage in Vdc */
    long double get_Idc( int n ); /*!< Returns nth current in Idc */
    long double get_Ikk( int n ); /*!< Returns nth current in Ikk */
    
    /* Accessors */
    long double get_vGap() { return vGap; }; /*!< Return gap voltage */
    long double get_iGap() { return iGap; }; /*!< Return critical current */
    long double get_Rn() { return Rn; };     /*!< Return normal resistance */
    long double get_vMax() { return vMax; }; /*!< Return highest voltage in data vector */ 
    int get_no_points() { return Vdc.size(); };   /*!< Return number of points in data vectors */
    std::string get_idcFile() { return idcFile; }; /*!< Return the filename used to fill the Vdc and Idc vectors */
    std::string get_ikkFile() { return ikkFile; }; /*!< Return the filename used ti fill the Ikk vector */
    
    void set_vGap(long double V) { vGap=V; iGap = vGap/Rn; }; /*!< Set the gap voltage */
    void set_iGap(long double I) { iGap=I; Rn = vGap/iGap; }; /*!< Set the gap current (critical current) */
    void set_Rn(long double R) { Rn = R; iGap = vGap/Rn; };   /*!< Set the normal resistance */ 
    void set_vMax(long double V) { vMax=V; };                 /*!< Set the maximum voltage to extend new data to */
    
};

/*! Exception thrown when data is requested from an unready response function */
struct ResponseFn_not_ready { };
        
#endif
