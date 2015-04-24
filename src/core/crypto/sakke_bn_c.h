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
 *  MIRACL SAKKE header file
 *  Author: M. Scott 2012
 */

#ifndef SAKKE_H
#define SAKKE_H

#include "certivox.h"

// Define one of these..

#define BN_CURVE
//#define RFC_CURVE
//#define RFC_CURVE_V2

#define SAS 16  /* Symmetric Key size - 128 bits */

/* Field size is assumed to be greater than or equal to group size. MIRACL is initialised for big numbers of size SFS */

#ifdef BN_CURVE
#define SGS 32  /* SAKKE Group Size */
#define SFS 32  /* SAKKE Field Size */
#define G1S 2*SFS+1 /* Group 1 Size */
#define G2S 4*SFS   /* Group 2 Size */
#endif

#ifdef RFC_CURVE
#define SGS 128
#define SFS 128
#define G1S 2*SFS+1
#define G2S G1S
#endif

#ifdef RFC_CURVE_V2
#define SGS 32
#define SFS 128
#define G1S 2*SFS+1
#define G2S G1S
#endif

#define SAKKE_OK                     0
#define SAKKE_DOMAIN_ERROR          -21
#define SAKKE_INVALID_PUBLIC_KEY    -22
#define SAKKE_ERROR                 -23
#define SAKKE_INVALID_POINT         -24
#define SAKKE_DOMAIN_NOT_FOUND      -25
#define SAKKE_OUT_OF_MEMORY         -26
#define SAKKE_DIV_BY_ZERO           -27
#define SAKKE_BAD_KEY               -28

extern const mr_small skrom[];

/* SAKKE domain parameters */

typedef struct
{
	int nibbles;
	int flags;
    char Q[SFS];
    char A[SFS];
    char B[SFS];
    char R[SGS];
    char Px[SFS];
    char Py[SFS];
#ifdef BN_CURVE
	char X[SFS];
	char Beta[SFS];
	char Qxa[SFS];
	char Qxb[SFS];
	char Qya[SFS];
	char Qyb[SFS];
	char Fa[SFS];
	char Fb[SFS];
	char G[16][SFS];
#else
	char CF[SFS];
	char Sa[SFS];
	char Sb[SFS];
#endif
} sak_domain;

/* SAKKE support functions */

extern void SAKKE_DOMAIN_KILL(sak_domain *);
extern int  SAKKE_DOMAIN_INIT(sak_domain *,const void *);

/* SAKKE primitives */

extern int  SAKKE_RECOMBINE_G1(sak_domain *,octet *,octet *,octet *,octet *);
extern int  SAKKE_RECOMBINE_G2(sak_domain *,octet *,octet *,octet *,octet *);
extern int  SAKKE_KEY_ENCAPSULATE(sak_domain *,octet *,octet *,int,octet *,octet *);
extern int  SAKKE_KEY_DECAPSULATE(sak_domain *,octet *,octet *,octet *,octet *,octet *,octet *);
extern int  SAKKE_SECRET_KEY_VALIDATE(sak_domain *,octet *,octet *,octet *);
extern int  SAKKE_PERMIT_VALIDATE(sak_domain *,int,octet *,octet *,octet *,octet *);
extern mr_unsign32 SAKKE_GET_TIME_SLOT(octet *);

#ifdef BUILD_PKG

extern int  SAKKE_MASTER_KEY_PAIR_GENERATE(sak_domain *,csprng *,octet *,octet *);
extern int  SAKKE_GET_USER_SECRET_KEY(sak_domain *,octet* ,octet *,octet *);
extern int  SAKKE_GET_USER_TIME_PERMIT(sak_domain *,int,octet*,octet *,octet *);

#endif

#endif

