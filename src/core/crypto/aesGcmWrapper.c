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
/*! \file  aesGcmWrapper.c
    \brief Definitions for the AES GSM algorithm wrappers

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

#include "aesGcmWrapper.h"
#include <math.h>

#define AS 16
#define IVLength 12

/*! \brief Encrypt data using AES GCM
 *
 *  AES is run as a block cypher in the GCM  mode of operation. The key size is 128 bits.
 *  This function will encrypt any data length.
 *
 *  @param  key           128 bit secret key
 *  @param  IV            96 bit initialization vector
 *  @param  header        Additional authenticated data (AAD). This data is authenticated, but not encrypted.
 *  @param  plaintext     Data to be encrypted
 *  @return ciphertext    Encrypted data. It is the same length as the plaintext.
 *  @return tag           128 bit authentication tag.
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int aesGcmEncrypt(char* key, char* IV, char* header, int headerLength, 
                  char* plaintext, int plaintextLength, char* ciphertext, 
                  char* tag)
{
  gcm g;
  int keyLength=AS;
  int tagLength=AS;
  gcm_init(&g,keyLength,key,IVLength,IV);

  if(!gcm_add_header(&g,header,headerLength))
    {
      return AES_INIT_ERROR;
    }
  
  if(!gcm_add_cipher(&g,GCM_ENCRYPTING,plaintext,plaintextLength,ciphertext))
    {
      return AES_ENCRYPT_ERROR;
    }

  gcm_finish(&g,tag); 
  tagLength=16;
  return 0;
}

/*! \brief Decrypt data using AES GCM
 *
 *  AES is run as a block cypher in the GCM  mode of operation. The key size is 128 bits.
 *  This function will decrypt any data length.
 *
 *  @param  key           128 bit secret key
 *  @param  IV            96 bit initialization vector
 *  @param  header        Additional authenticated data (AAD). This data is authenticated, but not decrypted.
 *  @param  ciphertext    Encrypted data. 
 *  @return plaintext     Decrypted data. It is the same length as the ciphertext.
 *  @return tag           128 bit authentication tag.
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int aesGcmDecrypt(char* key, char* IV, char* header, int headerLength, 
                  char* ciphertext, int ciphertextLength, char* plaintext, 
                  char* tag)
{
  gcm g;
  int keyLength=AS;
  int tagLength=AS;
  gcm_init(&g,keyLength,key,IVLength,IV);

  if(!gcm_add_header(&g,header,headerLength))
    {
      return AES_INIT_ERROR;
    }
  
  if(!gcm_add_cipher(&g,GCM_DECRYPTING,plaintext,ciphertextLength,ciphertext))
    {
      return AES_DECRYPT_ERROR;
    }

  gcm_finish(&g,tag); 
  tagLength=16;
  return 0;
}

/*! \brief Generate 128-bit AES Key
 *
 *  Generates a 128-bit AES key from random source 
 * 
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return AESKEY           128-bit AES Key
 */
DLL_EXPORT int generateAESKey(char* AESKEY, char* seedValue, int seedValueLength)
{
  octet octetAESKEY={AS,AS,AESKEY};
  int rtn=0;

  /* Crypto string RNG */
  csprng RNG;
  RNG = generateRNG(seedValue, seedValueLength);

  AES_KEY(&RNG, &octetAESKEY);
  return rtn;
}

/*! \brief Generate value that is used for random seeding
 *
 *  Generate value that is used for random seeding 
 * 
 *  @param  seedValue       random seed value
 *  @param  seedValueLength length of seedValue in bytes
 */
DLL_EXPORT int generateSeedValue(char* seedValue, int seedValueLength)
{
  int rtn=0;
  int i=0;

  /* Crypto string RNG */
  csprng RNG;
  RNG = generateRNG(seedValue, seedValueLength);

  for (i=0;i<seedValueLength;i++)
    seedValue[i]=strong_rng(&RNG);

  return rtn;
}


