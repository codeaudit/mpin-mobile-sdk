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
/*! \file  mpinWrapper.c
    \brief Definitions for the MPIN algorithm wrappers

*-  Project     : MPin
*-  Authors     : Kealan McCusker
*-  Company     : Certivox
*-  Created     : 21/11/2012
*-  Last update : 26/07/2013
*-  Platform    : Linux (3.5)
*-  Dependency  : Miracl

    Definitions for the MPIN algorithm wrappers

*/

/* #define DEBUG */
/* #define TEST */

#include "mpinWrapper.h"


/*! \brief Generate today's date for use in Time Permit generation
 *
 *  Generates todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 * 
 *  @return rtn   Returns todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) 
 */
DLL_EXPORT int todayMPin(void)
{
  int rtn=0;
  rtn = today();

#ifdef DEBUG
  printf("todayMPin: date: %d \n", rtn);
#endif

  return rtn;
}


/*! \brief Generates a random 256 bit integer
 *
 *  Generates a random 256 bit integer
 * 
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return S                Random 256 bit integer
 *  @return rtn              Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateMPINRandom(char* S, char* seedValue, int seedValueLength)
{
  octet octetS={PGS,PGS,S};
  mpin_domain mpdom;
  int rtn=0;
  csprng RNG;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

  rtn = MPIN_RANDOM_GENERATE(&mpdom,&RNG,&octetS); 

#ifdef DEBUG
  printf("generateMPINMasterSecret: S := 0x"); OCTET_OUTPUT(&octetS);
  printf("generateMPINMasterSecret: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}

/*! \brief Generate the token
 *
 *  The client's token is calculated; token = clientSecret - ( PIN * \f$ H(ID_c) \f$) 
 *  
 *  @param  IDc           The client ID 
 *  @param  IDcLength     The length of the client ID in bytes
 *  @param  PIN           The PIN number
 *  @return token         The token. The client's secret is passed in on this parameter. 
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateToken(char* IDc, int IDcLength, int PIN, char* token)
{
  octet octetIDc={IDcLength,IDcLength,IDc};
  octet octetToken={2*PFS+1,2*PFS+1,token};
  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

#ifdef DEBUG
  printf("generateToken In: IDc := 0x"); OCTET_OUTPUT(&octetIDc);
  printf("generateToken In: IDcLength := %d\n", IDcLength);
  printf("generateToken In: Token := 0x"); OCTET_OUTPUT(&octetToken);
  printf("generateToken In: PIN := %d\n", PIN);
  printf("generateToken In: rtn: %d \n", rtn);
#endif

  rtn = MPIN_EXTRACT_PIN(&mpdom, &octetIDc, PIN, &octetToken);

#ifdef DEBUG
  printf("generateToken Out: IDc := 0x"); OCTET_OUTPUT(&octetIDc);
  printf("generateToken Out: IDcLength := %d\n", IDcLength);
  printf("generateToken Out: Token := 0x"); OCTET_OUTPUT(&octetToken);
  printf("generateToken Out: PIN := %d\n", PIN);
  printf("generateToken Out: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Generate the parameters for the client first pass to send to the server 
 *
 *  Calculates x, U and UT. U and UT are sent to the server.
 *  
 *  @param  TPE              Time permit enabled 1: Use Time Permits 0: Do not use Time Permits
 *  @param  date             Today's date
 *  @param  IDc              The client ID 
 *  @param  IDcLength        The length of the client ID in bytes
 *  @param  PIN              The PIN number
 *  @param  TOKEN            The token. 
 *  @param  clientSecret     The client secret
 *  @param  TP               The Time Permit
 *  @param  X                Returned: Random number in range 1 to q
 *  @param  U                Returned: \f$ x.H(IDc)\f$
 *  @param  UT               Returned: \f$ x.(H(IDc)+H_T(date|IDc)) \f$
 *  @param  M                Returned: Not Used
 *  @param  maskedSecret     Returned: Not Used
 *  @param  seedValue        random seed value
 *  @param  seedValueLength  length of seedValue in bytes
 *  @return rtn              Returns 0 if successful or else an error code  
 */
DLL_EXPORT int clientFirstPass(int TPE, int date, char* IDc, int IDcLength, char* X, int PIN,
                                char* TOKEN, char* clientSecret, char* U, char* TP, 
				char* UT, char* M,char* maskedSecret, char* seedValue, int seedValueLength)
{
  octet octetIDc={IDcLength,IDcLength,IDc};
  octet octetX={PGS,PGS,X};
  octet octetTOKEN={2*PFS+1,2*PFS+1,TOKEN};
  octet octetClientSecret={2*PFS+1,2*PFS+1,clientSecret};
  octet octetU={2*PFS+1,2*PFS+1,U};
  octet octetTP={2*PFS+1,2*PFS+1,TP};
  octet octetUT={2*PFS+1,2*PFS+1,UT};
  octet octetM={PGS,PGS,M};
  octet octetMaskedSecret={2*PFS+1,2*PFS+1,maskedSecret};

  mpin_domain mpdom;
  int rtn=0;
  csprng RNG;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  /* Crypto string RNG */
  RNG = generateRNG(seedValue, seedValueLength);

#ifdef DEBUG
  printf("clientFirstPass In: TPE: %d\n", TPE);
  printf("clientFirstPass In: date   := %d\n", date);
  printf("clientFirstPass In: IDc   := 0x"); OCTET_OUTPUT(&octetIDc);
  printf("clientFirstPass In: X     := 0x"); OCTET_OUTPUT(&octetX);
  printf("clientFirstPass In: PIN   := %d\n", PIN);
  printf("clientFirstPass In: TOKEN := 0x"); OCTET_OUTPUT(&octetTOKEN);
  printf("clientFirstPass In: clientSecret := 0x"); OCTET_OUTPUT(&octetClientSecret);
  printf("clientFirstPass In: U  := 0x"); OCTET_OUTPUT(&octetU);
  printf("clientFirstPass In: TP := 0x"); OCTET_OUTPUT(&octetTP);
  printf("clientFirstPass In: UT  := 0x"); OCTET_OUTPUT(&octetUT);
  printf("clientFirstPass In: rtn: %d \n", rtn);
#endif

#ifdef TEST
  /* Do not use random values */
  rtn = MPIN_CLIENT_1(&mpdom,date,&octetIDc,NULL,&octetX,PIN,&octetTOKEN,
                      &octetClientSecret,&octetU,&octetTP,&octetUT,
                      NULL,NULL);
#else
  if (TPE==1)
    rtn = MPIN_CLIENT_1(&mpdom,date,&octetIDc,&RNG,&octetX,PIN,&octetTOKEN,
                        &octetClientSecret,&octetU,&octetTP,&octetUT,
                        NULL,NULL);
  /* For MPIN Full use this code
     rtn = MPIN_CLIENT_1(&mpdom,date,&octetIDc,&RNG,&octetX,PIN,
                         &octetTOKEN,&octetClientSecret,&octetU,&octetTP,
                         &octetUT,&octetM,&octetMaskedSecret); */
  else
    rtn = MPIN_CLIENT_1(&mpdom,0,&octetIDc,&RNG,&octetX,PIN,&octetTOKEN,
                        &octetClientSecret,&octetU,&octetTP,&octetUT,
                        NULL,NULL);
#endif

#ifdef DEBUG
  printf("clientFirstPass Out: TPE: %d\n", TPE);
  printf("clientFirstPass Out: date   := %d\n", date);
  printf("clientFirstPass Out: IDc   := 0x"); OCTET_OUTPUT(&octetIDc);
  printf("clientFirstPass Out: X     := 0x"); OCTET_OUTPUT(&octetX);
  printf("clientFirstPass Out: PIN   := %d\n", PIN);
  printf("clientFirstPass Out: TOKEN := 0x"); OCTET_OUTPUT(&octetTOKEN);
  printf("clientFirstPass Out: clientSecret := 0x"); OCTET_OUTPUT(&octetClientSecret);
  printf("clientFirstPass Out: U  := 0x"); OCTET_OUTPUT(&octetU);
  printf("clientFirstPass Out: TP := 0x"); OCTET_OUTPUT(&octetTP);
  printf("clientFirstPass Out: UT  := 0x"); OCTET_OUTPUT(&octetUT);
  printf("clientFirstPass Out: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}

/*! \brief Calculate the parameter for the client second pass to send to server
 *
 *  The client calculates the value V which is sent to the server to authenticate the 
 *  client. The value V is 
 *  <ol>
 *  <li> \f$ -(x+y)((s-PIN)H(ID_c) + PIN.H(ID_c)) \f$
 *  </ol>
 *  
 *  @param  X             Random number in range 1 to q
 *  @param  Y             Random number in range 1 to q
 *  @param  clientSecret  The client secret / V is also returned using this parameter
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int clientSecondPass(char* X, char* Y, char* clientSecret)
{
  octet octetX={PGS,PGS,X};
  octet octetY={PGS,PGS,Y};
  octet octetClientSecret={2*PFS+1,2*PFS+1,clientSecret};

  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_CLIENT_2(&mpdom,&octetX,&octetY,&octetClientSecret);
  
#ifdef DEBUG
  printf("clientSecondPass: X     := 0x"); OCTET_OUTPUT(&octetX);
  printf("clientSecondPass: Y := 0x"); OCTET_OUTPUT(&octetY);
  printf("clientSecondPass: clientSecret := 0x"); OCTET_OUTPUT(&octetClientSecret);
  printf("clientSecondPass: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Authenticate the client
 *
 *  The server authenticates the client
 *
 *  @param  TPE            Time permit enabled 1: Use Time Permits 0: Do not use Time Permits
 *  @param  date           Today's date
 *  @param  IDc            The client ID 
 *  @param  IDcLength      The length of the client ID in bytes
 *  @param  Y              Random number in range 1 to q
 *  @param  serverSecret   The server secret. 
 *  @param  U              \f$ x.H(IDc)\f$
 *  @param  UT             \f$ x.(H(IDc)+H_T(date|IDc)) \f$
 *  @param  V              Value calculated by the client
 *  @param  E              Returned: Value used to find PIN error
 *  @param  F              Returned: Value used to find PIN error
 *  @return rtn            Returns 0 if successful or else an error code  
 */
DLL_EXPORT int serverAuthentication(int TPE, int date, char* IDc, int IDcLength, char* Y,
                                     char* serverSecret, char* U, char* UT, char* V, 
                                     char* E, char* F)
{
  octet octetIDc={IDcLength,IDcLength,IDc};
  octet octetY={PGS,PGS,Y};
  octet octetServerSecret={4*PFS,4*PFS,serverSecret};
  octet octetU={2*PFS+1,2*PFS+1,U};
  octet octetUT={2*PFS+1,2*PFS+1,UT};
  octet octetV={2*PFS+1,2*PFS+1,V};
  octet octetE={12*PFS,12*PFS,E};
  octet octetF={12*PFS,12*PFS,F};

  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  if (TPE==1)
    rtn = MPIN_MINI_SERVER(&mpdom, date, &octetIDc, &octetY, &octetServerSecret,
                           &octetU,&octetUT,&octetV,
                           &octetE,&octetF);
  else
    rtn = MPIN_MINI_SERVER(&mpdom, 0, &octetIDc, &octetY, &octetServerSecret,
                           &octetU,&octetUT,&octetV,
                           &octetE,&octetF);

#ifdef DEBUG
  printf("serverAuthentication: TPE: %d\n", TPE);
  printf("serverAuthentication: date   := %d\n", date);
  printf("serverAuthentication: IDc   := 0x"); OCTET_OUTPUT(&octetIDc);
  printf("serverAuthentication: Y     := 0x"); OCTET_OUTPUT(&octetY);
  printf("serverAuthentication: ServerSecret := 0x"); OCTET_OUTPUT(&octetServerSecret);
  printf("serverAuthentication: U  := 0x"); OCTET_OUTPUT(&octetU);
  printf("serverAuthentication: UT  := 0x"); OCTET_OUTPUT(&octetUT);
  printf("serverAuthentication: E   := 0x"); OCTET_OUTPUT(&octetE);
  printf("serverAuthentication: F   := 0x"); OCTET_OUTPUT(&octetF);
  printf("serverAuthentication: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Determine whether a client has a token and if so what is the error in the PIN
 *
 *  This method returns 0 if a client does not have a valid token or else it will return
 *  the error in the entered PIN
 *
 *  @param  E              Value used to find PIN error
 *  @param  F              Value used to find PIN error
 *  @return rtn            Returns 0 if client does not have token or else the error in the PIN 
 */
DLL_EXPORT int tokenValidation(char* E, char* F)
{
  octet octetE={12*PFS,12*PFS,E};
  octet octetF={12*PFS,12*PFS,F};

  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_KANGAROO(&mpdom,&octetE,&octetF);

#ifdef DEBUG
  printf("tokenValidation: E   := 0x"); OCTET_OUTPUT(&octetE);
  printf("tokenValidation: F   := 0x"); OCTET_OUTPUT(&octetF);
  printf("tokenValidation: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}

/***PRIVATE***/

/*! \brief Combine the client secret shares
 *
 *  The client secret is calculated; clientSecret =\f$ CS_1 + CS_2 \f$ where 
 *  \f$ CS_1 and CS_2\f$ are client secret shares
 *
 *  @param  CS1           A share of the client secret / Time Permit
 *  @param  CS2           A share of the client secret / Time Permit
 *  @return clientSecret  The client secret 
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int combineClientSecretShares(char* CS1, char* CS2, char* clientSecret)
{
  octet octetCS1={2*PFS+1,2*PFS+1,CS1};
  octet octetCS2={2*PFS+1,2*PFS+1,CS2};
  octet octetClientSecret={2*PFS+1,2*PFS+1,clientSecret};
  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_RECOMBINE_G1(&mpdom, &octetCS1, &octetCS2, &octetClientSecret);

#ifdef DEBUG
  printf("generateClientSecret: CS1 := 0x"); OCTET_OUTPUT(&CS1);
  printf("generateClientSecret: CS2 := 0x"); OCTET_OUTPUT(&CS2);
  printf("generateClientSecret: ClientSecret := 0x"); OCTET_OUTPUT(&octetClientSecret);
  printf("generateClientSecret: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Combine the server secret shares
 *
 *  The server secret is calculated; serverSecret =\f$ SS_1 + SS_2 \f$ where
 *  \f$ SS_1 and SS_2\f$ are server secret shares
 *
 *  @param  SS1           A share of the server secret
 *  @param  SS2           A share of the server secret
 *  @return serverSecret  The server secret
 *  @return rtn           Returns 0 if successful or else an error code
 */
DLL_EXPORT int combineServerSecretShares(char* SS1, char* SS2, char* serverSecret)
{
  octet octetSS1={4*PFS,4*PFS,SS1};
  octet octetSS2={4*PFS,4*PFS,SS2};
  octet octetServerSecret={4*PFS,4*PFS,serverSecret};
  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn;
    }

  rtn = MPIN_RECOMBINE_G2(&mpdom, &octetSS1, &octetSS2, &octetServerSecret);

#ifdef DEBUG
  printf("generateServerSecret: SS1 := 0x"); OCTET_OUTPUT(&SS1);
  printf("generateServerSecret: SS2 := 0x"); OCTET_OUTPUT(&SS2);
  printf("generateServerSecret: ServerSecret := 0x"); OCTET_OUTPUT(&octetServerSecret);
  printf("generateServerSecret: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Generate the server secret
 *
 *  The server secret is calculated; serverSecret = \f$ s.Q \f$ where 
 *  \f$ Q \f$ is a member of \f$ G_2 \f$
 *  
 *  @param  S             The master secret
 *  @return serverSecret  The server secret 
 *  @return rtn           Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateServerSecret(char* S, char* serverSecret)
{
  octet octetS={PGS,PGS,S};
  octet octetServerSecret={4*PFS,4*PFS,serverSecret};
  mpin_domain mpdom;
  int rtn=0;

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_GET_SERVER_SECRET(&mpdom,&octetS,&octetServerSecret);

#ifdef DEBUG
  printf("generateServerSecret: S := 0x"); OCTET_OUTPUT(&octetS);
  printf("generateServerSecret: ServerSecret := 0x"); OCTET_OUTPUT(&octetServerSecret);
  printf("generateServerSecret: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Generate the client secret
 *
 *  The client secret is calculated; clientSecret =\f$ SH_1(ID_c) \f$ where 
 *  \f$ H_1 \f$ hashs to a member of \f$ G_1 \f$ and \f$ ID_c \f$ is the client identity.
 *  
 *  @param  S               The master secret
 *  @param  hashIDc         The hash of the client ID 
 *  @param  hashIDcLength   The length of the client ID hash in bytes
 *  @return clientSecret    The client secret 
 *  @return rtn             Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateClientSecret(char* S, char* hashIDc, int hashIDcLength, char* clientSecret)
{
  octet octetS={PGS,PGS,S};
  octet octetHashIDc={hashIDcLength, hashIDcLength, hashIDc};
  octet octetClientSecret={2*PFS+1,2*PFS+1,clientSecret};
  mpin_domain mpdom;
  int rtn=0;

  if(hashIDcLength != 32)
    {
      return MPIN_HASH_ERROR;
    }

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_GET_CLIENT_MULTIPLE(&mpdom,&octetS,&octetHashIDc,&octetClientSecret);

#ifdef DEBUG
  printf("generateClientSecret: S := 0x"); OCTET_OUTPUT(&octetS);
  printf("generateClientSecret: octetHashIDc := 0x"); OCTET_OUTPUT(&octetHashIDc);
  printf("generateClientSecret: ClientSecret := 0x"); OCTET_OUTPUT(&octetClientSecret);
  printf("generateClientSecret: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}


/*! \brief Generate the client's Time Permit
 *
 *  The client secret is calculated; TP =\f$ SH_{11}(DATE||ID_c) \f$ where 
 *  \f$ H_{11} \f$ hashs to a member of \f$ G_1 \f$, \f$ ID_c \f$ is the client identity
 *  and DATE is the today's date..
 *  
 *  @param  date           Today's date
 *  @param  S              The master secret
 *  @param  hashIDc        The hash of the client ID  
 *  @param  hashIDcLength  The length of the client ID hash in bytes
 *  @return TP             The client's Time Permit
 *  @return rtn            Returns 0 if successful or else an error code  
 */
DLL_EXPORT int generateMPINTimePermit(int date, char* S, char* hashIDc, int hashIDcLength, char* TP)
{
  octet octetS={PGS,PGS,S};
  octet octetHashIDc={hashIDcLength, hashIDcLength, hashIDc};
  octet octetTP={2*PFS+1,2*PFS+1,TP};
  mpin_domain mpdom;
  int rtn=0;

  if(hashIDcLength != 32)
    {
      return MPIN_HASH_ERROR;
    }

  /* Initialise elliptic curve from ROM */
  rtn = MPIN_DOMAIN_INIT(&mpdom,mprom);
  if (rtn != 0)
    {
      printf("Failed to initialize\n");
      return rtn; 
    }

  rtn = MPIN_GET_CLIENT_PERMIT(&mpdom,date,&octetS,&octetHashIDc,&octetTP);

#ifdef DEBUG
  printf("generateMPINTimePermit: S := 0x"); OCTET_OUTPUT(&octetS);
  printf("generateMPINTimePermit: octetHashIDc := 0x"); OCTET_OUTPUT(&octetHashIDc);
  printf("generateMPINTimePermit: TP := 0x"); OCTET_OUTPUT(&octetTP);
  printf("generateMPINTimePermit: rtn: %d \n", rtn);
#endif

  MPIN_DOMAIN_KILL(&mpdom);

  return rtn;
}

/*! \brief Calculate a sha256 hash of the input value
 *
 *  Calculate a sha256 hash of the input value. This is the first step in generating
 *  a client secret
 * 
 *  @param  input          Value to calculate a hash value 
 *  @param  inputLength    Input length in bytes
 *  @param  output         The calculated hash value
 *  @param  outputLength   Output length in bytes
 */
DLL_EXPORT int mpinHash(char* input, int inputLength, char* output, int outputLength)
{
  octet octetInput={inputLength, inputLength, input};
  octet octetOutput={outputLength, outputLength, output};

  if(outputLength != 32)
    {
      return MPIN_HASH_ERROR;
    }

  /* Calculate hash value */
  hash(NULL, -1, &octetInput, NULL, &octetOutput);
  return 0;
}



/*! \brief Generates a random value 
 *
 *  Generates a random value 
 * 
 *  @param  seedValue         seed value
 *  @param  seedValueLength   length of seed value in bytes
 *  @param  randomValue       random value
 *  @param  randomValueLength length of random value in bytes
 */
DLL_EXPORT int getRandom(char* seedValue, int seedValueLength, char* randomValue, int randomValueLength)
{
  int rtn=0;
  int i=0;

  /* Crypto string RNG */
  csprng RNG;
  RNG = generateRNG(seedValue, seedValueLength);

  for (i=0;i<randomValueLength;i++)
    randomValue[i]=strong_rng(&RNG);

  return rtn;
}

/*! \brief Initialize a random number generator
 *
 *  Initialize a random number generator
 * 
 *  @param  seedValue         seed value
 *  @param  seedValueLength   length of seed value in bytes
 *  @param  ira               random number array
 *  @param  rndptr            array and pointer
 *  @param  borrow            
 *  @param  pool_ptr          Pointer to entropy pool
 *  @param  pool              Entropy pool
 */
DLL_EXPORT int initializeRNG(char* seedValue, int seedValueLength, int* ira, int* rndptr, int* borrow, int* pool_ptr, char* pool)
{
  int rtn=0;
  int i=0;

  /* Crypto string RNG */
  csprng RNG;
  RNG = generateRNG(seedValue, seedValueLength);

  for (i=0;i<NK;i++) ira[i] = RNG.ira[i];
  *rndptr = RNG.rndptr;
  *borrow =  RNG.borrow;
  *pool_ptr = RNG.pool_ptr;
  for (i=0;i<MR_HASH_BYTES;i++) pool[i] = RNG.pool[i];

  return rtn;
}

