/* ResponseFn.cpp
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Implementation of a response function object for storing SIS IV and KK 
* curve data
*
*/

/* Changes
*
*/

#include "ResponseFn.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <sstream>
#include <iterator>
#include "math.h"

ResponseFn* Rfn2;  /*!< Dummy ResponseFn for passing to integrator */
long double GlobalBias; /*!< Global bias used for Ikk integration */

long double I_unpump(long double V)
{
    long double result;
    result = (Rfn2->get_Idc(V+GlobalBias) - (V+GlobalBias)) / V;
    result += (Rfn2->get_Idc(-V+GlobalBias) - (-V+GlobalBias)) / (-V);
    
    return result;
};


ResponseFn::ResponseFn()
{
    Rfn2 = this;
    
    Vdc.clear();
    Idc.clear();
    Ikk.clear();
    readyFlag = false;
    
    // Fill data with nominal values
    vGap = 1.0;
    iGap = 1.0;
    Rn = 1.0;
    
    yIntercept = 0.0;
    sNormal = 1.0;
    deltaV = 1.0e-3;
    vMax = 10.0;
}


long double ResponseFn::bubbleFind(long double x, std::vector<long double > & xVals, std::vector<long double> & yVals, int a, int b)
{
    // Have we found the closest points?
    if ( abs(b-a) <= 1) // Yes
    {
        // Have we hit the point exactly?
        if (a==b) // Yes
        {
            return yVals[a];
        }
        else // No, interpolate
        {
            return yVals[a] + (yVals[b]-yVals[a])*(x-xVals[a])/(xVals[b]-xVals[a]);
        }
    }
    else // No, move furthest limit
    {
        int c = a+(b-a)/2;
        if (xVals[c] < x ) //New limit below required value
        {
            return bubbleFind(x, xVals, yVals, c, b);
        }
        else // New limit above required value
        {
            return bubbleFind(x, xVals, yVals, a, c);
        }
    }
}


void ResponseFn::normalise()
{
    int n = Vdc.size();
    
    // Find Rn
    Rn = 0.0;
    sNormal = 0.0;
    // Search backwards to find index of top vGap/5 th of curve
    int start = n-1;
    while( (Vdc[n-1] - Vdc[start]) < vGap/5.0)
        start--;
    
    std::cout << "# Vdc[n-1]: " << Vdc[n-1] << " vGap: " << vGap << std::endl;    
    std::cout << "# starting normalisation at " << Vdc[start] << std::endl;
        
    long double vMid = 0.0;
    long double iMid = 0.0;
    
    // Average slope of top vGap/5 th of curve
    for (int i=start; i<n; i++)
    {
        Rn += (Vdc[i]-Vdc[i-5])/(Idc[i]-Idc[i-5]);
        sNormal += (Idc[i]-Idc[i-5])/(Vdc[i]-Vdc[i-5]);
        vMid += Vdc[i];
        iMid += Idc[i];
    }
    // Normalise averages
    Rn /= (long double)(n-start);
    sNormal /= (long double)(n-start);
    vMid /= (long double)(n-start);
    iMid /= (long double)(n-start);
    iGap = vGap/Rn;
    
    // Normalise data
    for (int i=0; i<n; i++)
    {
        Vdc[i] /= vGap;
        Idc[i] /= iGap;
    }
    
    // Set internal data for extrapolation
    sNormal = sNormal/iGap*vGap;
    yIntercept = iMid/iGap - vMid/vGap*sNormal;
    deltaV = Vdc[n-1]/(long double)(n-1);
    
    // If necessary, add more points for Ikk calculation
    if (Vdc[n-1] < vMax)
    {
        addMorePoints();
    }
}


void ResponseFn::addMorePoints()
{
    long double V = Vdc[Vdc.size()-1] + deltaV;
    while ( V < vMax )
    {
        Vdc.push_back(V);
        Idc.push_back(yIntercept + sNormal*V);
        Ikk.push_back(Ikk[Ikk.size()-1]);
        V+=deltaV;
    }
}


void ResponseFn::findIkk()
{
    long double I_unpump(long double);    

    for (int i=0; i<Vdc.size(); i++)
    {
        GlobalBias = Vdc[i];
        Ikk.push_back( ITG.qtrap(I_unpump, 0.0, 20.0) / M_PI );
    }            
}


void ResponseFn::kennedy(int n, int noPoints)
{
    readyFlag = false;    

    yIntercept = 0.0;
    sNormal = 1.0;
    deltaV = 1.0e-6; // Need real value
    vMax = 2.0;
    long double V;
    
    // Generate Kennedy fit
    Vdc.clear();
    Idc.clear();
    
    for (int j=0; j<noPoints; j++)
    {
        V = j*vMax/(noPoints);
        Vdc.push_back( V );
        Idc.push_back( pow(V,2*n+1)/(1+pow(V,2*n)) );
    }
    
    readyFlag = true;
    
    findIkk();
}


bool ResponseFn::loadData(std::string dcFile )
{   
    std::vector<long double> tempV;
    std::vector<long double> tempI;
    std::vector<long double> tempK;
    
    bool kkComplete = true;
     
    // Read data file
    std::ifstream inFile(dcFile.data());
    if (!inFile.is_open())
    {
        std::cerr << "# MultiTone: Could not open file: " << dcFile << std::endl;
        return false;
    }
    
    while ( inFile )
    {
        std::string line;
        std::getline(inFile, line);
        if (!line.size()) break;
        if (!std::isdigit(line[0])) continue;

        std::stringstream ss(line);
        long double v, i, k;
        ss >> v >> i;
        if (ss.bad()) continue;
        ss >> k;
        if (ss.bad()) kkComplete = false;
        
        tempV.push_back(v);
        tempI.push_back(i);
        tempK.push_back(k);
    }
    
    inFile.close();
    
    if (tempV.size() < 20)
    {
        std::cerr << "# MultiTone: Not enough data in file: " << dcFile << std::endl;
        return false;
    }
    
    // insert voltages and currents into Vdc and Idc, sorting by voltage as we go
    while (tempV.size() > 0)
    {
        // Find smallest element in tempV
        std::vector<long double>::iterator minElementV = tempV.begin();
        std::vector<long double>::iterator minElementI = tempI.begin();
        std::vector<long double>::iterator minElementK = tempK.begin();
        std::vector<long double>::iterator nV = tempV.begin();
        std::vector<long double>::iterator nI = tempI.begin();
        std::vector<long double>::iterator nK = tempK.begin();
        
        while ( nV != tempV.end())
        {
            if (*nV < *minElementV)
            {
                minElementV = nV;
                minElementI = nI;
                minElementK = nK;
            }
            ++nV;
            ++nI;
            ++nK;
        }
        
        // Discard negative values
        if ((*minElementV < 0.0) || (*minElementI < 0.0)) 
        {
            tempV.erase(minElementV);
            tempI.erase(minElementI);
            tempK.erase(minElementK);
            continue;
        }
        
        Vdc.push_back(*minElementV);
        Idc.push_back(*minElementI);
        Ikk.push_back(*minElementK);
        tempV.erase(minElementV);
        tempI.erase(minElementI);
        tempK.erase(minElementK);
        
    }

    readyFlag = true;
        
    normalise();
    
    if (!kkComplete) findIkk();
    
    return true;
}


bool ResponseFn::loadData(std::string dcFile , std::string kkFile)
{   
    std::vector<long double> tempV;
    std::vector<long double> tempI;
    std::vector<long double> tempKV;
    std::vector<long double> tempK;
    std::vector<long double> temp2KV;
    std::vector<long double> temp2K;
    
    bool kkComplete = true;
     
    // Read DC data file
    std::ifstream inFile(dcFile.data());
    if (!inFile.is_open())
    {
        std::cerr << "# MultiTone: Could not open file: " << dcFile << std::endl;
        return false;
    }
    
    while ( inFile )
    {
        std::string line;
        std::getline(inFile, line);
        if (!line.size()) break;
        if (!std::isdigit(line[0])) continue;

        std::stringstream ss(line);
        double v, i;
        ss >> v >> i;
        if (ss.bad()) continue;
            
        tempV.push_back(v);
        tempI.push_back(i);
    }
    
    inFile.close();
    
    if (tempV.size() < 20)
    {
        std::cerr << "# MultiTone: Not enough data in file: " << dcFile << std::endl;
        return false;
    }
    
    // insert voltages and currents into Vdc and Idc, sorting by voltage as we go
    while (tempV.size() > 0)
    {
        // Find smallest element in tempV
        std::vector<long double>::iterator minElementV = tempV.begin();
        std::vector<long double>::iterator minElementI = tempI.begin();
        std::vector<long double>::iterator nV = tempV.begin();
        std::vector<long double>::iterator nI = tempI.begin();
        
        while ( nV != tempV.end())
        {
            if (*nV < *minElementV)
            {
                minElementV = nV;
                minElementI = nI;
            }
            ++nV;
            ++nI;
        }
        
        // Discard negative values
        if ((*minElementV < 0.0) || (*minElementI < 0.0)) 
        {
            tempV.erase(minElementV);
            tempI.erase(minElementI);
            continue;
        }
        
        Vdc.push_back(*minElementV);
        Idc.push_back(*minElementI);
        tempV.erase(minElementV);
        tempI.erase(minElementI);
    }
    
    // Read KK data file
    std::ifstream inKKFile(kkFile.data());
    if (!inKKFile.is_open())
    {
        std::cerr << "# MultiTone: Could not open file: " << kkFile << std::endl;
        kkComplete = false;
    } 
    else
    {
        while ( inKKFile )
        {
            std::string line;
            std::getline(inKKFile, line);
            if (!line.size()) break;
            if (!std::isdigit(line[0])) continue;

            std::stringstream ss(line);
            long double kv, k;
            ss >> kv >> k;
            if (ss.bad()) continue;
            
            tempKV.push_back(kv);
            tempK.push_back(k);
        }
    
        inKKFile.close();
        
        if (tempKV.size() < 20)
        {
            std::cerr << "# MultiTone: Not enough data in file: " << kkFile << std::endl;
            kkComplete = false;
        }
    }
        
    if (kkComplete)
    {
        // sort KK values by voltage
        while (tempKV.size() > 0)
        {
            // Find smallest element in tempV
            std::vector<long double>::iterator minElementKV = tempKV.begin();
            std::vector<long double>::iterator minElementK = tempK.begin();
            std::vector<long double>::iterator nKV = tempKV.begin();
            std::vector<long double>::iterator nK = tempK.begin();
        
            while ( nKV != tempKV.end())
            {
                if (*nKV < *minElementKV)
                {
                    minElementKV = nKV;
                    minElementK = nK;
                }
                ++nKV;
                ++nK;
            }
        
            // Discard negative values
            if ((*minElementKV < 0.0)) 
            {
                tempKV.erase(minElementKV);
                tempK.erase(minElementK);
                continue;
            }
            temp2KV.push_back(*minElementKV);
            temp2K.push_back(*minElementK);
            tempKV.erase(minElementKV);
            tempK.erase(minElementK);
        }

        // Insert KK values into Ikk by interpolation
        Ikk.resize(Vdc.size());
        for (int i=0; i<Vdc.size(); i++)
        {
            Ikk[i] = bubbleFind(Vdc[i], temp2KV, temp2K, 0, temp2KV.size()-1);
        }
    }
    
    normalise();
    
    if (!kkComplete) findIkk();
    
    return true;
}


void ResponseFn::data(std::string dcFile )
{
    readyFlag = false;
    
    readyFlag = loadData(dcFile);
}


void ResponseFn::data(std::string dcFile, std::string kkFile)
{
    readyFlag = false;
    
    readyFlag = loadData(dcFile, kkFile);
}
   

long double ResponseFn::get_Vdc(int n)
{
    if (!readyFlag)
        throw ResponseFn_not_ready();
        
    return Vdc.at(n);
};


long double ResponseFn::get_Idc(int n)
{
    if (!readyFlag)
        throw ResponseFn_not_ready();
        
    return Idc.at(n);
};


long double ResponseFn::get_Ikk(int n)
{
    if (!readyFlag)
        throw ResponseFn_not_ready();
        
    return Ikk.at(n);
};


long double ResponseFn::get_Idc(long double V)
{
    if (!readyFlag)
        throw ResponseFn_not_ready();
        
    long double result;
    
    if ( fabs(V) >= Vdc[Vdc.size()-1])
    {
        result = yIntercept + sNormal*fabs(V);
    }
    else
    {
        result = bubbleFind(fabs(V), Vdc, Idc, 0, Vdc.size()-1);
    }
    
    if (V < 0)
        result = -result;
        
    return result;
}


long double ResponseFn::get_Ikk(long double V)
{
    if (!readyFlag)
        throw ResponseFn_not_ready();
        
    long double result;
    
    if ( fabs(V) >= Vdc[Vdc.size()-1])
    {
        result = Ikk[Vdc.size()-1];
    }
    else
    {
        result = bubbleFind(fabs(V), Vdc, Ikk, 0, Vdc.size()-1);
    }
        
    return result;
}

