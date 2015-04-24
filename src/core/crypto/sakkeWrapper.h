/* 
Copyright 2014 CertiVox UK Ltd, All Rights Reserved.

The CertiVox M-Pin Client and Server Libraries are free software: you can
redistribute it and/or modify it under the terms of the BSD 3-Clause
License - http://opensource.org/licenses/BSD-3-Clause

For full details regarding our CertiVox terms of service please refer to
the following links:

  * Our Terms and Conditions -
    http://www.certivox.com/about-certivox/terms-and-conditions/
  
  * Our Security and Privacy -
    http://www.certivox.com/about-certivox/security-privacy/

  * Our Statement of Position and Our Promise on Software Patents -
    http://www.certivox.com/about-certivox/patents/
*/
/*! \file  sakkeWrapper.h
    \brief Declarations for the SAKKE algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 08/11/2012
*-  Last update : 20/03/2013
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides an API for the SAKKE algorithm

*/

#ifndef SAKKEWRAPPER_H
#define SAKKEWRAPPER_H

#include "DLLDefines.h"
#include "sakke_bn_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Generate today's date for use in Time Permit generation */
DLL_EXPORT int todaySAKKE(void);

/*! \brief Encapsulate the Shared Secret Value (SSV) */
DLL_EXPORT int encapsulateSSV(char* SSV, char* ZS, int TPE, char* ID, int IDLength, char* HRbS);

/*! \brief Validate the Receiver Secret Key (RSK) */
DLL_EXPORT int validateRSK(char* ZS, char* ID,  int IDLength, char* KbS);

/*! \brief Validate the time permit */
DLL_EXPORT int validateSAKKETimePermit(int date, char* ID, int IDLength, char* ZS, char* KbS, char* TP);

/*! \brief Decapsulate the Shared Secret Value (SSV) */
DLL_EXPORT int decapsulateSSV(char* HRbS, char* ZS, int TPE, char* ID, int IDLength, char* KbS, char* TP, char* SSV);

#ifdef BUILD_PKG

/*! \brief Generate the master key pair */
DLL_EXPORT int generateSAKKEMasterKeyPair(char* ZS, char* Z, char* seedValue, int seedValueLength);

/*! \brief Generate Receiver Secret Key (RSK) i.e the user's private key */
DLL_EXPORT int generateRSK(char* Z, char* ID, int IDLength, char* KbS);

/*! \brief Generate Receiver's Time Permit */
DLL_EXPORT int generateSAKKETimePermit(int date, char* Z, char* ID, int IDLength, char* TP);

#endif

#ifdef __cplusplus
}
#endif

#endif

