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
/*! \file  eccsiWrapper.c
    \brief Definitions for the ECCSI algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 08/11/2012
*-  Last update : 21/03/2012
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides an API for the ECCSI algorithm

*/

/* #define DEBUG  */

#include "eccsiWrapper.h"

/*! \brief Generate today's date for use in Time Permit generation
 *
 *  Generates todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 * 
 *  @return rtn   Returns todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 */
DLL_EXPORT int todayECCSI(void)
{
  int rtn=0;
  rtn = today();

#ifdef DEBUG
  printf("todayECCSI: date: %d \n", rtn);
#endif

  return rtn;
}



/*! \brief Validate the Secret Signing Key (SSK) or the Time Permit (TP) 
 *
 *  The SSK must be validated before being used as a signing key.
 *  Thus function uses the ID and the KPAK to validate a received (SSK,PVT)
 *  pair by performing these steps:
 * 
 *  <ol>
 *  <li> Validate that the PVT lies on the elliptic curve E.
 *  <li> Compute HS = hash( G || KPAK || ID || PVT ), an N-octet integer.
 *  <li> Validate that KPAK = [SSK]G - [HS]PVT.
 *  </ol>
 *
 *  @param  date      zero means validate (SSK,PVT): A date value means validate the Time Permit
 *  @param  ID        Signer's identity
 *  @param  IDLength  The length of the identity in bytes
 *  @param  KPAK      KMS Public Authentication Key
 *  @param  PVTSSK    (PVT||SSK) or the Time Permit depending on the date
 *  @return rtn       Returns 0 if (SSK,PVT) / TP is valid or else an error code  
 */
DLL_EXPORT int validateECCSIUserKey(int date, char* ID, int IDLength, char* KPAK, char* PVTSSK)
{
  octet octetID={IDLength,IDLength,ID};
  octet octetKPAK={2*EFS+1,2*EFS+1,KPAK};
  octet octetPVTSSK={2*EFS+EGS+1,2*EFS+EGS+1,PVTSSK};
  ecs_domain esdom;
  int rtn=0;

#ifdef DEBUG
  printf("validateECCSIUserKey: ID = "); OCTET_OUTPUT(&octetID);
  printf("validateECCSIUserKey: KPAK = "); OCTET_OUTPUT(&octetKPAK);
  printf("validateECCSIUserKey: PVTSSK = "); OCTET_OUTPUT(&octetPVTSSK);
#endif

  rtn = ECS_DOMAIN_INIT(&esdom,esrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = ECCSI_USER_KEY_VALIDATE(&esdom, date, &octetID,&octetKPAK,&octetPVTSSK);

#ifdef DEBUG
  printf("validateECCSIUserKey: rtn: %d \n", rtn);
#endif

  ECS_DOMAIN_KILL(&esdom);

  return rtn;
}

/*! \brief Sign a message
 *
 *  To sign a message (M), the Signer performs these steps:
 * 
 *  <ol>
 *  <li> Choose a random (ephemeral) non-zero value \f$ j \in  F_q \f$
 *  <li> Compute J = [j]G then assign to r the N-octet integer representing Jx;
 *  <li> Compute a hash value HE = hash( HS || r || M )
 *  <li> Verify that HE + r * SSK is non-zero modulo q else abort
 *  <li> Compute\f$  s' = ( (( HE + r * SSK )^{-1}) * j )\f$ modulo q; the Signer MUST 
 *       then erase the value j
 *  <li> If s' is too big to fit within an N-octet integer, then set the N-octet 
 *       integer s = q - s'; otherwise, set the N-octet integers = s'
 *  <li> Return the signature = ( r || s || PVT )
 *  </ol>
 *
 *  @param  TPE              Time Permit enable - 1: Time Permits On  0: Time Permits Off
 *  @param  M                Message to be signed
 *  @param  MLength          Message length to be signed
 *  @param  ID               Signer's identity
 *  @param  IDLength         The length of the identity in bytes
 *  @param  KPAK             KMS Public Authentication Key
 *  @param  PVTSSK           (PVT||SSK)
 *  @param  TP               Time Permit
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return SIGNATURE        The calculated signature
 *  @return rtn              Returns 0 if successful or else an error code  
 */
DLL_EXPORT int createSignature(int TPE, char* M, int MLength, char* ID,  int IDLength, char* KPAK,  
				 char* PVTSSK, char* TP, char* SIGNATURE, char* seedValue, int seedValueLength)
{
  octet octetM={MLength,MLength,M};
  octet octetID={IDLength,IDLength,ID};
  octet octetKPAK={2*EFS+1,2*EFS+1,KPAK};
  octet octetPVTSSK={2*EFS+EGS+1,2*EFS+EGS+1,PVTSSK};
  octet octetTP={2*EFS+EGS+1,2*EFS+EGS+1,TP};
  octet octetSIGNATURE={2*EGS+2*EFS+1,2*EGS+2*EFS+1,SIGNATURE};
  ecs_domain esdom;
  int rtn=0;
  int i=0;
  char j[EGS];
  csprng RNG;
  octet octetJ={sizeof(j),sizeof(j),j};
  if (TPE == 1)
    {
      octetSIGNATURE.len = 2*EGS+4*EFS+6;
      octetSIGNATURE.max = 2*EGS+4*EFS+6;
    }


  /* Only used for testing */
  for (i=0; i<EGS; i++)
    octetJ.val[i]=0x00;  

#ifdef DEBUG
  printf("createSignature: TPE = %d\n",TPE);
  printf("createSignature: octetSIGNATURE.len = %d\n",octetSIGNATURE.len);
  printf("createSignature: octetSIGNATURE.max = %d\n",octetSIGNATURE.max);
  printf("createSignature: M = "); OCTET_OUTPUT(&octetM);
  printf("createSignature: ID = "); OCTET_OUTPUT(&octetID);
  printf("createSignature: KPAK = "); OCTET_OUTPUT(&octetKPAK);
  printf("createSignature: PVTSSK = "); OCTET_OUTPUT(&octetPVTSSK);
#endif

  /* Initialise elliptic curve from ROM */
  rtn = ECS_DOMAIN_INIT(&esdom,esrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

  if (TPE == 1)
    rtn = ECCSI_SIGN(&esdom, &RNG, &octetJ, &octetM, &octetID, &octetKPAK,
                     &octetPVTSSK, &octetTP, &octetSIGNATURE);
  else
    rtn = ECCSI_SIGN(&esdom, &RNG, &octetJ, &octetM, &octetID, &octetKPAK,
                     &octetPVTSSK, NULL, &octetSIGNATURE);



#ifdef DEBUG
  printf("createSignature: J := 0x "); OCTET_OUTPUT(&octetJ);
  printf("createSignature: SIGNATURE := "); OCTET_OUTPUT(&octetSIGNATURE);
  printf("createSignature: rtn: %d \n", rtn);
#endif

  ECS_DOMAIN_KILL(&esdom);

  return rtn;
}

/*! \brief Verify the signature
 *
 * To verify a Signature ( r || s || PVT ) against a Signer's Identifier
 * (ID), a message (M), and a pre-installed root of trust (KPAK), the
 * Verifier must perform a procedure equivalent to the following:
 * 
 *  <ol>
 *  <li> Check that the PVT lies on the elliptic curve E;
 *  <li> Compute HS = hash( G || KPAK || ID || PVT );
 *  <li> Compute HE = hash( HS || r || M );
 *  <li> Y = [HS]PVT + KPAK;
 *  <li> Compute J = [s]( [HE]G + [r]Y );
 *  <li> Check that \f$ J_x \f$= r modulo p, and that \f$ J_x \f$ modulo p is non-zero,
 *       before accepting the Signature as valid.
 *  </ol>
 *
 *  @param  TPE       Time Permit enable - 1: Time Permits On  0: Time Permits Off
 *  @param  M         Message to be verified
 *  @param  MLength   Message length
 *  @param  ID        Signer's identity
 *  @param  IDLength  The length of the identity in bytes
 *  @param  KPAK      KMS Public Authentication Key
 *  @param  SIGNATURE The signature to be verified
 *  @return rtn       Returns 0 if successful or else an error code  
 */
DLL_EXPORT int verifySignature(int TPE, char* M, int MLength, char* ID, int IDLength, 
                    char* KPAK, char* SIGNATURE)
{
  octet octetM={MLength,MLength,M};
  octet octetID={IDLength,IDLength,ID};
  octet octetKPAK={2*EFS+1,2*EFS+1,KPAK};
  octet octetSIGNATURE={2*EGS+2*EFS+1,2*EGS+2*EFS+1,SIGNATURE};
  ecs_domain esdom;
  int rtn=0;
  if (TPE == 1)
    {
      octetSIGNATURE.len = 2*EGS+4*EFS+6;
      octetSIGNATURE.max = 2*EGS+4*EFS+6;
    }


#ifdef DEBUG
  printf("verifySignature: TPE: %d", TPE);
  printf("verifySignature: octetSIGNATURE.len = %d\n",octetSIGNATURE.len);
  printf("verifySignature: octetSIGNATURE.max = %d\n",octetSIGNATURE.max);
  printf("verifySignature: M = "); OCTET_OUTPUT(&octetM);
  printf("verifySignature: ID = "); OCTET_OUTPUT(&octetID);
  printf("verifySignature: KPAK = "); OCTET_OUTPUT(&octetKPAK);
  printf("verifySignature: SIGNATURE = "); OCTET_OUTPUT(&octetSIGNATURE);
#endif

  /* Initialise elliptic curve from ROM */
  rtn = ECS_DOMAIN_INIT(&esdom,esrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = ECCSI_VERIFY(&esdom, TPE, &octetM, &octetID, &octetKPAK, &octetSIGNATURE);


#ifdef DEBUG
  printf("verifySignature: rtn: %d \n", rtn);
#endif

  ECS_DOMAIN_KILL(&esdom);

  return rtn;
}

#ifdef BUILD_PKG

/*! \brief Generate master key pair
 *
 *  Generates the KMS Secret Authentication Key (KSAK), which is the root of trust for 
 *  all other key material in the scheme. The KMS Public Authentication Key (KPAK),
 *  which all devices will require in order to verify signatures, is calculated from the 
 *  KSAK. This will be the root of trust for verification. The following steps are 
 *  required to generate these values.
 * 
 *  <ol>
 *  <li> Choose a random non-zero value \f$ KSAK \in  F_q \f$
 *  <li> Compute KPAK = [KSAK]G 
 *  </ol>
 *
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @param  len              length of seedValue in bytes
 *  @return KSAK             KMS Secret Authentication Key 
 *  @return KPAK             KMS Public Authentication Key 
 *  @return rtn              Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateECCSIMasterKeyPair(char* KSAK,char* KPAK, char* seedValue, int seedValueLength)
{
  octet octetKSAK={EGS,EGS,KSAK};
  octet octetKPAK={2*EFS+1,2*EFS+1,KPAK};
  ecs_domain esdom;
  int rtn=0;
  csprng RNG;

  /* Initialise elliptic curve from ROM */
  rtn = ECS_DOMAIN_INIT(&esdom,esrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

  rtn =  ECCSI_MASTER_KEY_PAIR_GENERATE(&esdom, &RNG, &octetKSAK, &octetKPAK);

#ifdef DEBUG
  printf("generateECCSIMasterKeyPair: KSAK = "); OCTET_OUTPUT(&octetKSAK);
  printf("generateECCSIMasterKeyPair: KPAK = "); OCTET_OUTPUT(&octetKPAK);
  printf("generateECCSIMasterKeyPair: rtn: %d \n", rtn);
#endif

  ECS_DOMAIN_KILL(&esdom);

  return rtn;
}

/*! \brief Generate user's key pair or Time Permit
 *
 *  In order to sign a message a user requires a Secret Signing Key (SSK) and a 
 *  Public Validation Token (PVT). The SSK is an integer, and the PVT is an 
 *  elliptic curve point. To generate these values the following steps are followed;
 * 
 *  <ol>
 *  <li> Choose a random (ephemeral) non-zero value \f$ v \in  F_q \f$
 *  <li> Compute PVT  = [j]G
 *  <li> Compute HS = hash( G || KPAK || ID || PVT ), an N-octet integer.
 *  <li> Compute\f$  SSK = ( KSAK + HS * v )\f$ modulo q
 *  <li> Restart if SSK or HS is zero modulo q
 *  <li> Erase v
 *  <li> Return (PVT||SSK)
 *  </ol>
 *
 *  @param  date             Today's date to generate time permite or zero means generate (SSK,PVT)
 *  @param  ID               Signer's identity
 *  @param  IDLength         The length of the identity in bytes
 *  @param  KSAK             KMS Secret Authentication Key 
 *  @param  KPAK             KMS Public Authentication Key
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return PVTSSK           Returns (PVT||SSK) or the time permit depending on the date parameter
 *  @return rtn              Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateECCSIUserKey(int date, char* ID, int IDLength, char* KSAK, char* KPAK,
				      char* PVTSSK, char* seedValue, int seedValueLength)
{
  octet octetID={IDLength,IDLength,ID};
  octet octetKSAK={EGS,EGS,KSAK};
  octet octetKPAK={2*EFS+1,2*EFS+1,KPAK};
  octet octetPVTSSK={2*EFS+EGS+1,2*EFS+EGS+1,PVTSSK};
  ecs_domain esdom;
  int rtn=0;
  int i=0;
  csprng RNG;

  /* octetV is only used for testing */
  char v[EGS];
  octet octetV={sizeof(v),sizeof(v),v};
  for (i=0; i<EGS; i++)
    octetV.val[i]=0x00;  

  /* Initialise elliptic curve from ROM */
  rtn = ECS_DOMAIN_INIT(&esdom,esrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

  rtn = ECCSI_USER_KEY_GENERATE(&esdom, date, &RNG, &octetV, &octetID, &octetKSAK, 
                                &octetKPAK, &octetPVTSSK);

#ifdef DEBUG
  printf("generateECCSIUserKey: KSAK = "); OCTET_OUTPUT(&octetKSAK);
  printf("generateECCSIUserKey: KPAK = "); OCTET_OUTPUT(&octetKPAK);
  printf("generateECCSIUserKey: PVTSSK = "); OCTET_OUTPUT(&octetPVTSSK);
  printf("generateECCSIUserKey: V = "); OCTET_OUTPUT(&octetV);
  printf("generateECCSIUserKey: rtn: %d \n", rtn);
#endif

  ECS_DOMAIN_KILL(&esdom);

  return rtn;
}

#else /* BUILD_PKG */

/*! \brief Generate master key pair */
DLL_EXPORT int generateECCSIMasterKeyPair(char* KSAK,char* KPAK, char* seedValue, int seedValueLength)
{
  return 0;
}

/*! \brief Generate user's key pair or Time Permit */
DLL_EXPORT int generateECCSIUserKey(int date, char* ID, int IDLength, char* KSAK, char* KPAK,
				      char* PVTSSK, char* seedValue, int seedValueLength)
{
  return 0;
}

#endif /* BUILD_PKG */
