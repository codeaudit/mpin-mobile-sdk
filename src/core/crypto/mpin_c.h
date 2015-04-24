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

/*
 *  MIRACL MPIN header file
 *  Author: M. Scott 2012
 */

#ifndef MPIN_H
#define MPIN_H

#include "certivox.h"
#include "DLLDefines.h"

/* Field size is assumed to be greater than or equal to group size. MIRACL is initialised for big numbers of size PFS */

#define AVOID_PATENT

#define PGS 32  /* MPIN Group Size */
#define PFS 32  /* MPIN Field Size */
#define PAS 16  /* MPIN Symmetric Key Size */

#define MPIN_OK                     0
#define MPIN_DOMAIN_ERROR          -11
#define MPIN_INVALID_PUBLIC_KEY    -12
#define MPIN_ERROR                 -13
#define MPIN_INVALID_POINT         -14
#define MPIN_DOMAIN_NOT_FOUND      -15
#define MPIN_OUT_OF_MEMORY         -16
#define MPIN_DIV_BY_ZERO           -17
#define MPIN_WRONG_ORDER           -18
#define MPIN_BAD_PIN               -19
#define MPIN_HASH_ERROR            -111

#define PINDIGITS 4

extern const mr_small mprom[];

/* M-Pin domain parameters */

typedef struct
{
	int nibbles;
	int flags;
	char X[PFS];
    char Q[PFS];
    char A[PFS];
    char B[PFS];
    char R[PGS];
	char Beta[PFS];
	char Qxa[PFS];
	char Qxb[PFS];
	char Qya[PFS];
	char Qyb[PFS];
	char Fa[PFS];
	char Fb[PFS];
} mpin_domain;

/* MPIN support functions */

DLL_EXPORT void MPIN_DOMAIN_KILL(mpin_domain *);
DLL_EXPORT int  MPIN_DOMAIN_INIT(mpin_domain *,const void *);
DLL_EXPORT int  MPIN_DOMAIN_INIT_NEW(mpin_domain *);

/* MPIN primitives */

DLL_EXPORT int MPIN_RANDOM_GENERATE(mpin_domain *, csprng *,octet *);
DLL_EXPORT int MPIN_EXTRACT_PIN(mpin_domain *,octet *,int,octet *); 
DLL_EXPORT int MPIN_CLIENT_1(mpin_domain *,int,octet *,csprng *,octet *,int,octet *,octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_CLIENT_2(mpin_domain *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_CLIENT_3(mpin_domain *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_MINI_SERVER(mpin_domain *,int,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_FULL_SERVER_1(mpin_domain *,csprng *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_FULL_SERVER_2(mpin_domain *,int,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_RECOMBINE_G1(mpin_domain *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_RECOMBINE_G2(mpin_domain *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_KANGAROO(mpin_domain *,octet *,octet *);

/***PRIVATE***/

DLL_EXPORT int MPIN_GET_CLIENT_MULTIPLE(mpin_domain *,octet *,octet *,octet *); 
DLL_EXPORT int MPIN_GET_CLIENT_PERMIT(mpin_domain *,int,octet *,octet *,octet *); 
DLL_EXPORT int MPIN_GET_SERVER_SECRET(mpin_domain *,octet *,octet *); 
DLL_EXPORT int MPIN_TEST_PAIRING(mpin_domain *,octet *,octet *);



#endif

