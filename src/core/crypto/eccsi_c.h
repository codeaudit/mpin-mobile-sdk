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
 *  MIRACL ECCSI header file
 *  Author: M. Scott 2012
 */

#ifndef ECCSI_H
#define ECCSI_H

#include "certivox.h"

#define EGS 32 /* ECCSI Group Size */
#define EFS 32 /* ECCSI Field Size */

#define ECCSI_OK                     0
#define ECCSI_DOMAIN_ERROR          -31
#define ECCSI_BAD_KEY		    -32
#define ECCSI_ERROR                 -33
#define ECCSI_INVALID_POINT         -34
#define ECCSI_DOMAIN_NOT_FOUND      -35
#define ECCSI_OUT_OF_MEMORY         -36
#define ECCSI_DIV_BY_ZERO           -37
#define ECCSI_BAD_SIG		    -38

#define PERMITS_ON 1
#define PERMITS_OFF 0

extern const mr_small esrom[];

/* ECp domain parameters */

typedef struct
{
	int nibbles;
    char Q[EFS];
    char A[EFS];
    char B[EFS];
    char R[EGS];
    char Gx[EFS];
    char Gy[EFS];
} ecs_domain;

/* ECCSI support functions */

extern void ECS_DOMAIN_KILL(ecs_domain *);
extern int  ECS_DOMAIN_INIT(ecs_domain *,const void *);

/* ECCSI primitives */

extern int  ECCSI_USER_KEY_VALIDATE(ecs_domain *,int,octet *,octet *,octet *);
extern int  ECCSI_SIGN(ecs_domain *,csprng *,octet*,octet *,octet *,octet *,octet *,octet *,octet *);
extern int  ECCSI_VERIFY(ecs_domain *,int,octet *,octet *,octet *,octet *);

#ifdef BUILD_PKG

extern int  ECCSI_RECOMBINE_PUB(ecs_domain *,octet *,octet *,octet *);
extern int  ECCSI_RECOMBINE_PRV(ecs_domain *,octet *,octet *,octet *);
extern int  ECCSI_MASTER_KEY_PAIR_GENERATE(ecs_domain *,csprng *,octet *,octet *);
extern int  ECCSI_USER_KEY_GENERATE(ecs_domain *,int,csprng *,octet*,octet *,octet *,octet *,octet *);

#endif

#endif

