/* 
 * File:   CvEntropyServer.h
 * Author: mony
 *
 * Created on November 9, 2012, 2:35 PM
 */

#ifndef CVENTROPYSERVER_H
#define	CVENTROPYSERVER_H

extern "C"
{
        #include "miracl.h"
}

#define CvByte unsigned char*

#include "CvCommon.h"

#include <string>

using namespace std;

class CvEntropyServer
{
public:
	CvEntropyServer( const string& aUrl );
	virtual ~CvEntropyServer();
	
	enum enAlgorithm_t
	{
		enAlgorithm_Unknown = -1,
		enAlgorithm_Unsafe = 0,
		enAlgorithm_Alf = 2,
		enAlgorithm_Mt19937 = 3,
		enAlgorithm_XorShift128 = 4
	};

	enum enEncoding_t
	{
		enEncoding_Unknown = -1,
		enEncoding_Raw = 0,
		enEncoding_Base64 = 1
	};

	bool Generate( enAlgorithm_t aAlgorithm, enEncoding_t aEncoding, int aLength, OUT string& aEntropy );

	static const char* AlgorithmToString( enAlgorithm_t aAlgorithm );
	static const char* EncodingToString( enEncoding_t aEncoding );
	static enAlgorithm_t StringToAlgorithm( const string& aAlgorithm );
	static enEncoding_t StringToEncoding( const string& aEncoding );
	
private:
	CvEntropyServer(const CvEntropyServer& orig){}
	void operator=(const CvEntropyServer){}
	string	m_url;
};

class SystemCSPRNG
{
public:
    
    SystemCSPRNG();
    virtual ~SystemCSPRNG();
    
    void rndPool(OUT CvByte pMemory, size_t req_len);
    csprng& Csprng(); 
    
private:
    
    csprng m_csprng;
};

#endif	/* CVENTROPYSERVER_H */

