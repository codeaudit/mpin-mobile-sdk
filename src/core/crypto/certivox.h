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
 *  Common header file
 *  Author: M. Scott 2012
 */

#ifndef CERTIVOX_H
#define CERTIVOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "miracl.h"
#include "octet.h"
#include "MPinCryptoConfig.h"
#include "DLLDefines.h"

/* Note that all of this code standardises on 112/128-bit symmetric keys, and 256-bit Hash Functions */

#define TIME_SLOT_MINUTES 1440 /* Time Slot = 1 day */
//#define TIME_SLOT_MINUTES 1  /* Time Slot = 1 minute */

#define HASH_BYTES 32

#if HASH_BYTES==20
	#define HASHFUNC sha
	#define SHS_INIT shs_init
	#define SHS_PROCESS shs_process
	#define SHS_HASH shs_hash
	#define HASH_BLOCK 64
#endif

#if HASH_BYTES==32
	#define HASHFUNC sha256
	#define SHS_INIT shs256_init
	#define SHS_PROCESS shs256_process
	#define SHS_HASH shs256_hash
	#define HASH_BLOCK 64
#endif

#if HASH_BYTES==48
	#define HASHFUNC sha384
	#define SHS_INIT shs384_init
	#define SHS_PROCESS shs384_process
	#define SHS_HASH shs384_hash
	#define HASH_BLOCK 128
#endif

#if HASH_BYTES==64
	#define HASHFUNC sha512
	#define SHS_INIT shs512_init
	#define SHS_PROCESS shs512_process
	#define SHS_HASH shs512_hash
	#define HASH_BLOCK 128
#endif

/* Random Number Functions */

DLL_EXPORT void CREATE_CSPRNG(csprng *,octet *);
DLL_EXPORT void KILL_CSPRNG(csprng *);

typedef struct
{
    zzn4 a;
    zzn4 b;
	zzn4 c;
    BOOL unitary;
	BOOL miller;
} zzn12;

DLL_EXPORT mr_unsign32 today(void);
DLL_EXPORT mr_unsign32 cv_getdate(char *);
DLL_EXPORT void thedate(mr_unsign32 ,char *);
DLL_EXPORT void int_to_base64(int , char *);
DLL_EXPORT int base64_to_int(char *);
DLL_EXPORT void hash(octet *,int ,octet *,octet *,octet *);
DLL_EXPORT void HashToIntegerRange(_MIPT_ big ,big ,big );

DLL_EXPORT void ecn2_alloc(_MIPD_ ecn2 *,char *,int *);

DLL_EXPORT void zzn2_alloc(_MIPT_ zzn2 *,char *,int *);
DLL_EXPORT void zzn4_alloc(_MIPT_ zzn4 *,char *,int *);
DLL_EXPORT void zzn12_alloc(_MIPT_ zzn12 *,char *,int *);
DLL_EXPORT void zzn12_powq(_MIPT_ zzn2 *,zzn12 *);
DLL_EXPORT BOOL zzn12_iszero(zzn12 *);
DLL_EXPORT BOOL zzn12_isunity(_MIPT_ zzn12 *);
DLL_EXPORT void zzn12_copy(zzn12 *,zzn12 *);
DLL_EXPORT void zzn12_from_int(_MIPT_ int ,zzn12 *);
DLL_EXPORT void zzn12_from_zzn4s(zzn4 *,zzn4 *,zzn4 *,zzn12 *);
DLL_EXPORT void zzn12_conj(_MIPT_ zzn12 *,zzn12 *);
DLL_EXPORT BOOL zzn12_compare(zzn12 *,zzn12 *);
DLL_EXPORT void zzn12_sqr(_MIPT_ zzn12 *,zzn12 *);
DLL_EXPORT void zzn12_mul(_MIPT_ zzn12 *,zzn12 *,zzn12 *);
DLL_EXPORT void zzn12_inv(_MIPT_ zzn12 *);
DLL_EXPORT void zzn12_pow(_MIPT_ zzn12 *,big ,zzn12 *);
DLL_EXPORT void trace(_MIPD_ zzn12 *,zzn4 *);
DLL_EXPORT void xtr_A(_MIPT_ zzn4 *,zzn4 *,zzn4 *,zzn4 *,zzn4 *);
DLL_EXPORT void xtr_D(_MIPT_ zzn4 *,zzn4 *);
DLL_EXPORT void xtr_pow(_MIPT_ zzn4 *,big ,zzn4 *);
DLL_EXPORT void xtr_pow2(_MIPT_ zzn4 *,zzn4 *,zzn4 *,zzn4 *,big ,big ,zzn4 *);

DLL_EXPORT void endomorph(_MIPT_ big ,epoint *);
DLL_EXPORT void q_power_frobenius(_MIPT_ zzn2 *,ecn2 *);
DLL_EXPORT void line(_MIPT_ ecn2 *,ecn2 *,ecn2 *,zzn2 *,zzn2 *,BOOL ,big,big,zzn12 *);
DLL_EXPORT void g(_MIPT_ ecn2* ,ecn2 *,big ,big ,zzn12 *);
DLL_EXPORT BOOL rate_double_miller(_MIPT_ ecn2 *,epoint *,ecn2 *,epoint *,big ,zzn2 *,zzn12 *);
DLL_EXPORT BOOL rate_miller(_MIPT_ ecn2 *,epoint *,big ,zzn2 *,zzn12 *);
DLL_EXPORT void rate_fexp(_MIPT_ big ,zzn2 *,zzn12 *);
DLL_EXPORT void cofactor(_MIPT_ zzn2 *,big ,ecn2 *);
DLL_EXPORT BOOL member(_MIPT_ zzn2 *,big ,zzn12 *);
DLL_EXPORT void glv(_MIPT_ big ,big ,big W[2],big B[2][2],big u[2]);
DLL_EXPORT void galscott(_MIPT_ big ,big ,big W[4],big B[4][4],big u[4]);
DLL_EXPORT void getprb(_MIPT_ big ,big ,big ,big);
DLL_EXPORT void matrix2(_MIPT_ big ,big W[2],big B[2][2]);
DLL_EXPORT void matrix4(_MIPT_ big ,big W[4],big B[4][4]);
DLL_EXPORT void G1_mult(_MIPT_ epoint *,big ,big ,big ,big ,epoint *);
DLL_EXPORT void G2_mult(_MIPT_ ecn2 *,big ,zzn2 *,big ,big ,ecn2 *);

DLL_EXPORT void AES_KEY(csprng *,octet *);
DLL_EXPORT void AES_GCM_ENCRYPT(octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT void AES_GCM_DECRYPT(octet *,octet *,octet *,octet *,octet *,octet *);

DLL_EXPORT void hex2bin( char* dest, char *text );
DLL_EXPORT csprng generateRNG(char*, int);
/*! \brief Print version number and information about the build */
DLL_EXPORT void version(char*);
DLL_EXPORT int generateOTP(csprng*);
/*! \brief Generate a random number */
DLL_EXPORT void generateRandom(csprng*, octet*);

#endif
