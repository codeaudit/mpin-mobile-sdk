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
/*! \file  sakkeWrapper.c
    \brief Definitions for the SAKKE algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 08/11/2012
*-  Last update : 20/03/2013
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Provides an API for the SAKKE algorithm

*/

/* #define DEBUG */
/* #define TEST  */

#include "sakkeWrapper.h"

/*! \brief Generate today's date for use in Time Permit generation
 *
 *  Generates todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 * 
 *  @return rtn   Returns todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 */
DLL_EXPORT int todaySAKKE(void)
{
  int rtn=0;
  rtn = today();

#ifdef DEBUG
  printf("todaySAKKE: date: %d \n", rtn);
#endif

  return rtn;
}


/*! \brief Encapsulate the Shared Secret Value (SSV)
 *
 *  The \f$ R_{(b,S)} \f$ and H values are calculated using these steps;
 *  <ol>
 *  <li> Compute r = HashToIntegerRange( SSV || b, q, Hash );
 *  <li> Compute \f$ R_{(b,S)} = [r]([b]P + Z_S) \f$ in \f$ E(F_p) \f$;
 *  <li> Compute the Hint, H;
 *  <ul>
 *      <li> Compute \f$ g^r \f$. 
 *      <li> Compute H := SSV XOR HashToIntegerRange( \f$ g^r \f$, \f$ 2^n \f$, Hash );
 *  </ul>
 *  <li> Return the Encapsulated Data (  H, \f$ R_{(b,S)} \f$ )
 *  </ol>
 *
 *  @param  SSV       Shared Secret Value    
 *  @param  ZS        Public key of \f$ KMS_S \f$ equates to \f$ Z_S \f$ in the preceding text
 *  @param  TPE       Time permit enabled 1: Use Time Permits 0: Do not use Time Permits
 *  @param  ID        The identity of the receiving party equates to b in the preceding text
 *  @param  IDLength  The length of the identity in bytes
 *  @return HRbS      Equates to (  H, \f$ R_{(b,S)} \f$ )||Date
 *  @return           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int encapsulateSSV(char* SSV, char* ZS, int TPE, char* ID, int IDLength, char* HRbS)
{
  octet octetSSV={SAS,SAS,SSV};
  octet octetZS={G1S,G1S,ZS};
  octet octetID={IDLength,IDLength,ID};
  octet octetHRbS={G1S+SAS,G1S+SAS,HRbS};
  sak_domain skdom;
  int rtn=0;
  if (TPE == 1)
    {
      octetHRbS.len = G1S+SAS+4;
      octetHRbS.max = G1S+SAS+4;
    }

#ifdef DEBUG
  printf("encapsulateSSV: TPE: %d \n", TPE);
  printf("encapsulateSSV: octetHRbS.len: %d \n", octetHRbS.len);
  printf("encapsulateSSV: octetHRbS.max: %d \n", octetHRbS.max);
  printf("encapsulateSSV: SSV := 0x"); OCTET_OUTPUT(&octetSSV);
  printf("encapsulateSSV: ZS  := 0x"); OCTET_OUTPUT(&octetZS);
  printf("encapsulateSSV: ID  := 0x"); OCTET_OUTPUT(&octetID);
#endif

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = SAKKE_KEY_ENCAPSULATE(&skdom,&octetSSV,&octetZS,TPE, &octetID,&octetHRbS);

#ifdef DEBUG
  printf("encapsulateSSV: HRbS := 0x"); OCTET_OUTPUT(&octetHRbS);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}


/*! \brief Validate the Receiver Secret Key (RSK)
 *
 *  Upon receipt of keying material the user must verify its Receiver Secret Key (RSK).
 * 
 *  <ol>
 *  <li> Compute \f$ < [b]P + Z_S, K_{(b,S)} >\f$ If this is equal to g then output 0 else output an error code.
 *  </ol>
 *
 *  @param ZS        Public key of \f$ KMS_S \f$ equates to \f$ Z_S \f$ in the preceding text
 *  @param ID        The identity of the receiving party equates to b in the preceding text
 *  @param IDLength  The length of the identity in bytes
 *  @param KbS       The Receiver Secret Key (RSK) equates to \f$ K_{(b,S)} \f$ in the preceding text
 *  @return          Returns 0 if key is valid else an error code  
 */
DLL_EXPORT int validateRSK(char* ZS, char* ID,  int IDLength, char* KbS)
{
  octet octetZS={G1S,G1S,ZS};
  octet octetID={IDLength,IDLength,ID};
  octet octetKbS={G2S,G2S,KbS};
  sak_domain skdom;
  int rtn=0;

#ifdef DEBUG
  printf("validateRSK: ZS  := 0x"); OCTET_OUTPUT(&octetZS);
  printf("validateRSK: ID  := 0x"); OCTET_OUTPUT(&octetID);
  printf("validateRSK: KbS := 0x"); OCTET_OUTPUT(&octetKbS);
#endif

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = SAKKE_SECRET_KEY_VALIDATE(&skdom,&octetID,&octetZS,&octetKbS);

#ifdef DEBUG
  printf("validateRSK: rtn : %d\n", rtn);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}



/*! \brief Validate the time permit
 *
 *  Validate the time permit by checking it is a valid point on the curve.
 *
 *  @param date      Today's date
 *  @param ID        The identity of the receiving party 
 *  @param IDLength  The length of the identity in bytes
 *  @param ZS        Public key of \f$ KMS_S \f$
 *  @param KbS       The Receiver Secret Key (RSK) 
 *  @param TP        The time permit
 *  @return          Returns 0 if time permit is valid else an error code  
 */
DLL_EXPORT int validateSAKKETimePermit(int date, char* ID, int IDLength, char* ZS, char* KbS, char* TP)
{
  octet octetID={IDLength,IDLength,ID};
  octet octetZS={G1S,G1S,ZS};
  octet octetKbS={G2S,G2S,KbS};
  octet octetTP={G2S,G2S,TP};
  sak_domain skdom;
  int rtn=0;

#ifdef DEBUG
  printf("validateSAKKETimePermit: ID  := 0x"); OCTET_OUTPUT(&octetID);
  printf("validateSAKKETimePermit: ZS  := 0x"); OCTET_OUTPUT(&octetZS);
  printf("validateSAKKETimePermit: KbS := 0x"); OCTET_OUTPUT(&octetKbS);
  printf("validateSAKKETimePermit: TP  := 0x"); OCTET_OUTPUT(&octetTP);
#endif

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = SAKKE_PERMIT_VALIDATE(&skdom, date, &octetID, &octetZS, &octetKbS, &octetTP);

  printf("validateSAKKETimePermit: rtn : %d\n", rtn);

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}


/*! \brief Decapsulate the Shared Secret Value (SSV)
 *
 *  Device B receives Encapsulated Data from device A. In order to
 *  process this, it requires its RSK, \f$ K_{(b,S)} \f$, which will have been
 *  provisioned in advance by \f$ KMS_S \f$. The function carries out the 
 *  following steps to derive and verify the SSV:
 * 
 *  <ol>
 *  <li> Compute \f$ w := < R_{(b,S)}, K_{(b,S)} > \f$ . Note that by bilinearity, \f$ w = g^r\f$;
 *  <li> Compute \f$ SSV = H XOR HashToIntegerRange( w, 2^n, Hash )\f$;
 *  <li> Compute \f$ r = HashToIntegerRange( SSV || b, q, Hash )\f$;
 *  <li> Compute \f$ TEST = [r]([b]P + Z_S)\f$ in \f$E(F_p)\f$. If TEST does not equal \f$R_{(b,S)}\f$, then B MUST NOT use the SSV to derive key material;
 *  <li> Output SSV for use to derive key material for the application to be keyed.
 *  </ol>
 *
 *  @param HRbS      Equates to (  H, \f$ R_{(b,S)} \f$ )||Date
 *  @param ZS        Public key of \f$ KMS_S \f$ equates to \f$ Z_S \f$ in the preceding text
 *  @param TPE       Time permit enabled 1: Use Time Permits 0: Do not use Time Permits
 *  @param ID        The identity of the receiving party equates to b in the preceding text
 *  @param IDLength  The length of the identity in bytes
 *  @param KbS       The Receiver Secret Key (RSK) equates to \f$ K_{(b,S)} \f$ in the preceding text
 *  @param TP        The time permit
 *  @return SSV      Shared Secret Value    
 *  @return          Returns 0 if successful or else an error code  
 */
DLL_EXPORT int decapsulateSSV(char* HRbS, char* ZS, int TPE, char* ID, int IDLength, char* KbS, char* TP, char* SSV)
{
  octet octetZS={G1S,G1S,ZS};
  octet octetID={IDLength,IDLength,ID};
  octet octetKbS={G2S,G2S,KbS};
  octet octetTP={G2S,G2S,TP};
  octet octetSSV={SAS,SAS,SSV};
  octet octetHRbS={G1S+SAS,G1S+SAS,HRbS};
  sak_domain skdom;
  int rtn=0;
  if (TPE == 1)
    {
      octetHRbS.len = G1S+SAS+4;
      octetHRbS.max = G1S+SAS+4;
    }


#ifdef DEBUG
  printf("decapsulateSSV: TPE: %d \n", TPE);
  printf("decapsulateSSV: octetHRbS.len: %d \n", octetHRbS.len);
  printf("decapsulateSSV: octetHRbS.max: %d \n", octetHRbS.max);
  printf("decapsulateSSV: ZS  := 0x"); OCTET_OUTPUT(&octetZS);
  printf("decapsulateSSV: ID  := 0x"); OCTET_OUTPUT(&octetID);
  printf("decapsulateSSV: HRbS := 0x"); OCTET_OUTPUT(&octetHRbS);
  printf("decapsulateSSV: KbS := 0x"); OCTET_OUTPUT(&octetKbS);
  printf("decapsulateSSV: SSV (IN):= 0x"); OCTET_OUTPUT(&octetSSV);
#endif

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  if (TPE == 1)
    {
      rtn = SAKKE_KEY_DECAPSULATE(&skdom,&octetHRbS, &octetZS, &octetID, &octetKbS, &octetTP, &octetSSV);
#ifdef DEBUG
  printf("decapsulateSSV: TP := 0x"); OCTET_OUTPUT(&octetTP);
#endif
    }
  else
    rtn = SAKKE_KEY_DECAPSULATE(&skdom,&octetHRbS, &octetZS, &octetID, &octetKbS, NULL, &octetSSV);

#ifdef DEBUG
  printf("decapsulateSSV: SSV (OUT):= 0x"); OCTET_OUTPUT(&octetSSV);
  printf("decapsulateSSV: rtn : %d \n", rtn);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}

#ifdef BUILD_PKG

/*! \brief Generate the master key pair
 *
 *  \f$ KMS_S \f$ chooses its KMS Master Secret, \f$ z_S \f$. It must randomly select a
 *  value in the range 2 to q-1, and assigns this value to \f$ z_S \f$. It must
 *  derive its KMS Public Key, \f$ Z_S \f$, by performing the calculation;
 * 
 *  <ol>
 *  <li> \f$ Z_S =[z_S]P \f$
 *  </ol>
 *
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return ZS               Public key of \f$ KMS_S \f$ equates to \f$ Z_S \f$ in the preceding text
 *  @return Z                Master secret of \f$ KMS_S \f$ equates to \f$ z_S \f$ in the preceding text
 *  @return                  Returns 0 if keys are valid or else an error code  
 */
DLL_EXPORT int generateSAKKEMasterKeyPair(char* ZS, char* Z, char* seedValue, int seedValueLength)
{
  octet octetZ={SGS,SGS,Z};
  octet octetZS={G1S,G1S,ZS};
  sak_domain skdom;
  int rtn=0;
  csprng RNG;

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

  /* Generate a random value for the master key */
  rtn = SAKKE_MASTER_KEY_PAIR_GENERATE(&skdom, &RNG, &octetZ, &octetZS);

#ifdef DEBUG
  printf("generateSAKKEMasterKeyPair: Z  : 0x"); OCTET_OUTPUT(&octetZ);
  printf("generateSAKKEMasterKeyPair: ZS  : 0x"); OCTET_OUTPUT(&octetZS);
  printf("generateSAKKEMasterKeyPair: rtn : %d \n", rtn);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}

/*! \brief Generate Receiver Secret Key (RSK) i.e the user's private key
 *
 *  The KMS derives each RSK from an Identifier and its KMS Master
 *  Secret. For Identifier 'b', the RSK \f$ K_{(b,S)} \f$ provided by \f$ KMS_S \f$ is calculated by
 * 
 *  <ol>
 *  <li> Compute \f$ K_{(b,S)} = [(b + z_S)^{-1}]P \f$ where b is an integer, 
         P a point on the curve, and \f$ z_S \f$ the master secret.
 *  </ol>
 *
 *  @param  Z         Master secret of \f$ KMS_S \f$ equates to \f$ z_S \f$ in the preceding text
 *  @param  ID        The identity of the receiving party equates to b in the preceding text
 *  @param  IDLength  The length of the identity in bytes
 *  @return KbS       The Receiver Secret Key (RSK) equates to \f$ K_{(b,S)} \f$ in the preceding text
 *  @return           Returns 0 if key is valid or else an error code  
 */
DLL_EXPORT int generateRSK(char* Z, char* ID, int IDLength, char* KbS)
{
  octet octetZ={SGS,SGS,Z};
  octet octetID={IDLength,IDLength,ID};
  octet octetKbS={G2S,G2S,KbS};
  sak_domain skdom;
  int rtn=0;

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = SAKKE_GET_USER_SECRET_KEY(&skdom,&octetZ,&octetID,&octetKbS);

#ifdef DEBUG
  printf("generateRSK: Z  := 0x"); OCTET_OUTPUT(&octetZ);
  printf("generateRSK: ID  := 0x"); OCTET_OUTPUT(&octetID);
  printf("generateRSK: KbS := 0x"); OCTET_OUTPUT(&octetKbS);
  printf("generateRSK: rtn : %d\n", rtn);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}

/*! \brief Generate Receiver's Time Permit
 *
 *  A time permit for a receiver is generated that allows a user to decapsulate the secret 
 *  key.
 * 
 *  <ol>
 *  <li> Compute \f$ TP = \frac{-t}{(Z+ID+t)(Z+ID)}.P \f$ where t is an integer, 
         representation of a data and P is a curve point.
 *  </ol>
 *
 *  @param  date      Today's date
 *  @param  Z         Master secret of \f$ KMS_S \f$
 *  @param  ID        The identity of the receiving party
 *  @param  IDLength  The length of the identity in bytes
 *  @return TP        The Receiver's Time Permit
 *  @return           Returns 0 if time permit is valid or else an error code  
 */
DLL_EXPORT int generateSAKKETimePermit(int date, char* Z, char* ID, int IDLength, char* TP)
{
  octet octetZ={SGS,SGS,Z};
  octet octetID={IDLength,IDLength,ID};
  octet octetTP={G2S,G2S,TP};
  sak_domain skdom;
  int rtn=0;

  rtn = SAKKE_DOMAIN_INIT(&skdom,skrom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    } 

  rtn = SAKKE_GET_USER_TIME_PERMIT(&skdom,date,&octetZ,&octetID,&octetTP);

#ifdef DEBUG
  printf("generateSAKKETimePermit: Z  := 0x"); OCTET_OUTPUT(&octetZ);
  printf("generateSAKKETimePermit: ID := 0x"); OCTET_OUTPUT(&octetID);
  printf("generateSAKKETimePermit: TP := 0x"); OCTET_OUTPUT(&octetTP);
  printf("generateSAKKETimePermit: rtn : %d\n", rtn);
#endif

  SAKKE_DOMAIN_KILL(&skdom);

  return rtn;
}

#else /* BUILD_PKG */

DLL_EXPORT int generateSAKKEMasterKeyPair(char* ZS, char* Z, char* seedValue, int seedValueLength)
{
  return 0;
}
DLL_EXPORT int generateRSK(char* Z, char* ID, int IDLength, char* KbS)
{
  return 0;
}
DLL_EXPORT int generateSAKKETimePermit(int date, char* Z, char* ID, int IDLength, char* TP)
{
  return 0;
}

#endif  /* BUILD_PKG */
