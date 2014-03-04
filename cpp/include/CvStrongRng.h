/* 
 * File:   CvStrongRng.h
 * Author: mony
 *
 * Created on November 9, 2012, 4:44 PM
 */

#ifndef CVSTRONGRNG_H
#define	CVSTRONGRNG_H

#include "CvEntropyServer.h"

#include <string>
#include <memory>

using namespace std;

namespace CvShared
{

enum CSRNG_TYPE
{	OLD
};

enum CSRNG_MODE
{	NEWI
};

class CvStrongRng
{        
    //forbid copy and =
    CvStrongRng(const CvStrongRng& orig);
    void operator=(const CvStrongRng& orig);      
        
    csprng	m_csprng;
        
    class m_DongleSource 
        {
                public:
                    m_DongleSource(){};            
                    ~m_DongleSource(){strong_kill( &m_csprng );}

                    csprng& dongle_slurp( bool abEnableEntropy, const string& aEntropyServerUrl = "", const string& aEntropyAlgorithm = "" );

                private:
                    //forbid copy and =
                    m_DongleSource(const m_DongleSource&);
                    void operator=(const m_DongleSource&);
                    
                    csprng	m_csprng;
        };
        
public:
        
        CvStrongRng(CSRNG_TYPE);
        CvStrongRng(CSRNG_MODE = NEWI);
        
	virtual ~CvStrongRng();
        
        //old interface
	static void Init( bool abEnableEntropy, const string& aEntropyServerUrl = "", const string& aEntropyAlgorithm = "" );
        csprng& Csprng()
        { return m_csprng; 
        }
        
	static bool m_bEnableEntropy;
        static string m_aEntropyServerUrl;
        static string m_aEntropyAlgorithm;
        
        //new interface
        const unique_ptr<SystemCSPRNG>    ISystemSource;
        const unique_ptr<m_DongleSource>  IDongleSource;
        
};
}
#endif	/* CVSTRONGRNG_H */

