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
/*! \file  eccsiWrapper.h
    \brief Declarations for the ECCSI algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 08/11/2012
*-  Last update : 21/03/2012
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides an API for the ECCSI algorithm

*/

#ifndef ECCSIWRAPPER_H
#define ECCSIWRAPPER_H

#include "eccsi_c.h"
#include "DLLDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Generate today's date for use in Time Permit generation */
DLL_EXPORT int todayECCSI(void);

/*! \brief Validate the Secret Signing Key (SSK) or the Time Permit (TP) */
DLL_EXPORT int validateECCSIUserKey(int date, char* ID, int IDLength, char* KPAK, char* PVTSSK);

/*! \brief Sign a message */
DLL_EXPORT int createSignature(int TPE, char* M, int MLength, char* ID,  int IDLength, char* KPAK,  
				 char* PVTSSK, char* TP, char* SIGNATURE, char* seedValue, int seedValueLength);

/*! \brief Verify the signature */
DLL_EXPORT int verifySignature(int TPE, char* M, int MLength, char* ID, int IDLength, char* KPAK, 
                                 char* SIGNATURE);

#ifdef BUILD_PKG

/*! \brief Generate master key pair */
DLL_EXPORT int generateECCSIMasterKeyPair(char* KSAK,char* KPAK, char* seedValue, int seedValueLength);

/*! \brief Generate user's key pair or Time Permit */
DLL_EXPORT int generateECCSIUserKey(int date, char* ID, int IDLength, char* KSAK, char* KPAK,
				      char* PVTSSK, char* seedValue, int seedValueLength);

#endif

#ifdef __cplusplus
}
#endif

#endif

