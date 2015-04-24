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
/*! \file  mpinWrapper.h
    \brief Declarations for the MPIN algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 21/11/2012
*-  Last update : 
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides an API for the MPIN algorithm

*/

#ifndef MPINWRAPPER_H
#define MPINWRAPPER_H

#include <math.h>
#include "mpin_c.h"
#include "DLLDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Generate today's date for use in Time Permit generation */
DLL_EXPORT int todayMPin(void);

/*! \brief Generates a random 256 bit integer */
DLL_EXPORT int generateMPINRandom(char* S, char* seedValue, int seedValueLength);

/*! \brief Generate the token */
DLL_EXPORT int generateToken(char* IDc, int IDcLength, int PIN, char* token);

/*! \brief Generate the parameters for the client first pass to send to the server  */
DLL_EXPORT int clientFirstPass(int TPE, int date, char* IDc, int IDcLength, char* X, int PIN,
                                char* TOKEN, char* clientSecret, char* U, char* TP, 
				char* UT, char* M,char* maskedSecret, char* seedValue, int seedValueLength );

/*! \brief Generate the parameter for the client second pass to send to server */
DLL_EXPORT int clientSecondPass(char* X, char* Y, char* clientSecret);  

/*! \brief  Authenticate the client */
DLL_EXPORT int serverAuthentication(int TPE, int date, char* IDc, int IDcLength, char* Y,
                                     char* serverSecret, char* U, char* UT, char* V, 
                                     char* E, char* F);

/*! \brief Determine whether a client has a token and if so what is the error in the PIN */
DLL_EXPORT int tokenValidation(char* E, char* F);

/*! \brief Calculate a sha256 hash of the input value */
DLL_EXPORT int mpinHash(char* input, int inputLength, char* output, int outputLength);

/*! \brief Combine the client secret shares */
DLL_EXPORT int combineClientSecretShares(char* CS1, char* CS2, char* clientSecret);

/*! \brief Combine the server secret shares */
DLL_EXPORT int combineServerSecretShares(char* SS1, char* SS2, char* serverSecret);

/*! \brief Generate the server secret */
DLL_EXPORT int generateServerSecret(char* S, char* serverSecret);

/*! \brief Generate the client secret */
DLL_EXPORT int generateClientSecret(char* S, char* hashIDc, int hashIDcLength, char* clientSecret);

/*! \brief Generate the client's Time Permit */
DLL_EXPORT int generateMPINTimePermit(int date, char* S, char* hashIDc, int hashIDcLength, char* TP);

/*! \brief Generate a random value */
DLL_EXPORT int getRandom(char* seedValue, int seedValueLength, char* randomValue, int randomValueLength);

/*! \brief Initialize a random number generator */
DLL_EXPORT int initializeRNG(char* seedValue, int seedValueLength, int* ira, int* rndptr, int* borrow, int* pool_ptr, char* pool);

#ifdef __cplusplus
}
#endif

#endif

