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
/*! \file  aesGcmWrapper.h
    \brief Declarations for the AES GSM algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 04/12/2012
*-  Last update : 
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides a wrapper around the Miracl AES GCM functions to aid the development
    of language extensions and a simple API in C.

*/

#ifndef AESGCMWRAPPER_H
#define AESGCMWRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miracl.h"
#include "certivox.h"
#include "DLLDefines.h"

#define AES_INIT_ERROR    -41
#define AES_ENCRYPT_ERROR -42
#define AES_DECRYPT_ERROR -43
#define AES_TAG_MISMATCH -44

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Encrypt data using AES GSM */
DLL_EXPORT int aesGcmEncrypt(char* key, char* IV, char* header, int headerLength, 
                  char* plaintext, int plaintextLength, char* ciphertext, 
                  char* tag);

/*! \brief Decrypt data using AES GSM */
DLL_EXPORT int aesGcmDecrypt(char* key, char* IV, char* header, int headerLength, 
                  char* ciphertext, int ciphertextLength, char* plaintext, 
                  char* tag);

/*! \brief Generate 128-bit AES Key */
DLL_EXPORT int generateAESKey(char* AESKEY, char* seedValue, int seedValueLength);

/*! \brief Generate 100 Byte Value that is used for random seeding */
DLL_EXPORT int generateSeedValue(char* seedValue, int len);

#ifdef __cplusplus
}
#endif

#endif
