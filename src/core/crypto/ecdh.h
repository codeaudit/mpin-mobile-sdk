/*
 *  CLINT ECDH header file
 *  Author: M. Scott 2014
 */

#ifndef ECDH_H
#define ECDH_H

#include "clint.h"

#define EAS 16 /* Symmetric Key size - 128 bits */
#define EGS 32 /* ECCSI Group Size */
#define EFS 32 /* ECCSI Field Size */

#define ECDH_OK                     0
#define ECDH_DOMAIN_ERROR          -1
#define ECDH_INVALID_PUBLIC_KEY    -2
#define ECDH_ERROR                 -3
#define ECDH_INVALID               -4
#define ECDH_DOMAIN_NOT_FOUND      -5
#define ECDH_OUT_OF_MEMORY         -6
#define ECDH_DIV_BY_ZERO           -7
#define ECDH_BAD_ASSUMPTION        -8

/* ECDH Auxiliary Functions */

extern void HASH(octet *,octet *);
extern int HMAC(octet *,octet *,int,octet *);
extern void KDF1(octet *,int,octet *);
extern void KDF2(octet *,octet *,int,octet *);
extern void PBKDF2(octet *,octet *,int,int,octet *);
extern void AES_CBC_IV0_ENCRYPT(octet *,octet *,octet *);
extern int AES_CBC_IV0_DECRYPT(octet *,octet *,octet *);

/* ECDH primitives - support functions */

extern int  ECP_KEY_PAIR_GENERATE(csprng *,octet *,octet *);
extern int  ECP_PUBLIC_KEY_VALIDATE(int,octet *);

/* ECDH primitives */

extern int ECPSVDP_DH(octet *,octet *,octet *);
extern int ECPSVDP_DHC(octet *,octet *,int,octet *);

#if CURVETYPE!=MONTGOMERY
/* ECIES functions */
extern void ECP_ECIES_ENCRYPT(octet *,octet *,csprng *,octet *,octet *,int,octet *,octet *,octet *);
extern int ECP_ECIES_DECRYPT(octet *,octet *,octet *,octet *,octet *,octet *,octet *);

/* ECDSA functions */
extern int ECPSP_DSA(csprng *,octet *,octet *,octet *,octet *);
extern int ECPVP_DSA(octet *,octet *,octet *,octet *);
#endif

#endif

