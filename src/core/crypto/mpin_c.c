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


/* MPIN Functions */

/* Version 2.0 - supports Time Permits */

/* Use this mirdef.h for 32-bit processor 

#define MR_LITTLE_ENDIAN
#define MIRACL 32
#define mr_utype int
#define mr_dltype long long  
#define mr_unsign64 unsigned long long
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define MR_ALWAYS_BINARY
#define MR_STATIC 8
#define MR_GENERIC_MT
#define MR_STRIPPED_DOWN
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO
#define MR_NOASM
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
*/

/* Use this mirdef.h for 64-bit processor 

#define MR_LITTLE_ENDIAN
#define MIRACL 64
#define mr_utype long long
#define mr_unsign64 unsigned long long
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define MR_ALWAYS_BINARY
#define MR_STATIC 4
#define MR_GENERIC_MT
#define MR_STRIPPED_DOWN
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8

*/

/* Link to these files 

mrcore.c
mrarth0.c
mrarth1.c
mrarth2.c
mrio1.c
mrgcd.c
mrxgcd.c
mrarth3.c
mrbits.c
mrmonty.c
mrcurve.c
mrshs256.c
mrstrong.c
mraes.c
mrzzn2.c
mrzzn4.c
mrecn2.c
mrsroot.c
mrjack.c
mrlucas.c
mrzzn2b.c

For 64-bit build using Microsoft compiler mrmuldv.w64 must be included as well
For 64-bit build using Linux and Intel chips, mrmuldv.g64 must be included as well

However note that this code will also work with a standard MIRACL header

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpin_c.h"

#define ROM_SIZE 8

/*
#if MIRACL==64

const mr_small mprom[]={
0x4080000000000001,0x0,0x0,0x8000000000000000, // x 
0x2,0x0,0x0,0x0,                               // B 
0x353F63AD74319C04,0xF68AFDBF9B933998,0x28E05B3AAF153F82,0x3C67A5CB50A75BD, // Qxa 
0x23559C8A12B5637F,0x5B5051B1119E373B,0x278F3D149BAC8FAA,0x86C6D36FDAF0244, // Qxb 
0x8AB9CC634607E059,0x51430509C32A6440,0xBA739B657113D84,0x62039BE3E8F0691,  // Qya 
0xC51DD369F21FF550,0xE12AC7E5BA650CC3,0x3861D7D21AE532BD,0xAB7E3D96F16C979, // Qyb 
0xE17DE6C06F2A6DE9,0x850974924D3F77C2,0xB6499B50A846953F,0x1B377619212E7C8C, // Fx 
0xC582193F90D5922A,0xDC178B6DB2C08850,0x3EAB22F57B96AC8,0x9EBEE691ED18375};

#endif

#if MIRACL==32

const mr_small mprom[]={
0x1,0x40800000,0x0,0x0,0x0,0x0,0x0,0x80000000,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x74319C04,0x353F63AD,0x9B933998,0xF68AFDBF,0xAF153F82,0x28E05B3A,0xB50A75BD,0x3C67A5C,
0x12B5637F,0x23559C8A,0x119E373B,0x5B5051B1,0x9BAC8FAA,0x278F3D14,0xFDAF0244,0x86C6D36,
0x4607E059,0x8AB9CC63,0xC32A6440,0x51430509,0x57113D84,0xBA739B6,0x3E8F0691,0x62039BE,
0xF21FF550,0xC51DD369,0xBA650CC3,0xE12AC7E5,0x1AE532BD,0x3861D7D2,0x6F16C979,0xAB7E3D9,
0x6F2A6DE9,0xE17DE6C0,0x4D3F77C2,0x85097492,0xA846953F,0xB6499B50,0x212E7C8C,0x1B377619,
0x90D5922A,0xC582193F,0xB2C08850,0xDC178B6D,0x57B96AC8,0x3EAB22F,0x1ED18375,0x9EBEE69};

#endif
*/

#if MIRACL==64

const mr_small mprom[]={
0x4000000003C012B1,0x0,0x0,0x8000000000000000,
0x2,0x0,0x0,0x0,
0xC0851CEEE4D2EC74,0x907C85BFA03E2726,0x358B2500F5C34BBB,0x19682D2C7053B256,
0x47A58E8B2E29CFE1,0xF81B97B0C209C30F,0xA011C937A8E99743,0x1466B9EC3E19F64A,
0xCFBFCEBCF0BE09F,0x6361B33D847EC1B3,0xD81E22157DAEE209,0xA79EDD972332B8D,
0xED904B228898EE9D,0xC2864EA569D2EDEB,0x35C6E4512D8D3461,0x6160C39ECC4C090,
0x5BD9083355C80EA3,0x68677326F173F821,0xAFE18B8AACA71898,0x1359082FA63A0164,
0x628D1BBC06534710,0xBBD863C7269546C0,0xDC53D9CDBC4E3A,0x10A6F7D0623628A9};

#endif

#if MIRACL==32

const mr_small mprom[]={
0x3C012B1,0x40000000,0x0,0x0,0x0,0x0,0x0,0x80000000,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE4D2EC74,0xC0851CEE,0xA03E2726,0x907C85BF,0xF5C34BBB,0x358B2500,0x7053B256,0x19682D2C,
0x2E29CFE1,0x47A58E8B,0xC209C30F,0xF81B97B0,0xA8E99743,0xA011C937,0x3E19F64A,0x1466B9EC,
0xCF0BE09F,0xCFBFCEB,0x847EC1B3,0x6361B33D,0x7DAEE209,0xD81E2215,0x72332B8D,0xA79EDD9,
0x8898EE9D,0xED904B22,0x69D2EDEB,0xC2864EA5,0x2D8D3461,0x35C6E451,0xECC4C090,0x6160C39,
0x55C80EA3,0x5BD90833,0xF173F821,0x68677326,0xACA71898,0xAFE18B8A,0xA63A0164,0x1359082F,
0x6534710,0x628D1BBC,0x269546C0,0xBBD863C7,0xCDBC4E3A,0xDC53D9,0x623628A9,0x10A6F7D0};

#endif


static void start_hash(HASHFUNC *sha)
{
	SHS_INIT(sha);
}

static void add_to_hash(HASHFUNC *sha,octet *x)
{
	int i;
	for (i=0;i<x->len;i++) {/*printf("%d,",(unsigned char)x->val[i]);*/ SHS_PROCESS(sha,x->val[i]);  }
}

static void finish_hash(HASHFUNC *sha,octet *w)
{
	int i,hlen=HASH_BYTES;
	char hh[HASH_BYTES];
    SHS_HASH(sha,hh);
   
    OCTET_EMPTY(w);
    OCTET_JOIN_BYTES(hh,hlen,w);
    for (i=0;i<hlen;i++) hh[i]=0;
}

/*
void ecn_print(_MIPD_ epoint *P)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	if (P->marker==MR_EPOINT_INFINITY)
	{
		printf("[Infinity]\n");
		return;
	}
	epoint_norm(_MIPP_ P);
	printf("["); 
	redc(_MIPP_ P->X,P->X);
	otstr(_MIPP_ P->X,mr_mip->IOBUFF);
	nres(_MIPP_ P->X,P->X);
	printf("%s,",mr_mip->IOBUFF);
	redc(_MIPP_ P->Y,P->Y);
	otstr(_MIPP_ P->Y,mr_mip->IOBUFF);
	nres(_MIPP_ P->Y,P->Y);
	printf("%s]\n",mr_mip->IOBUFF);
}

void zzn2_print(_MIPD_ zzn2 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); 
	redc(_MIPP_ x->a,x->a);
	otstr(_MIPP_ x->a,mr_mip->IOBUFF); 
	nres(_MIPP_ x->a,x->a);
	printf("%s,",mr_mip->IOBUFF); 
	redc(_MIPP_ x->b,x->b);
	otstr(_MIPP_ x->b,mr_mip->IOBUFF); 
	nres(_MIPP_ x->b,x->b);
	printf("%s)",mr_mip->IOBUFF);
}

void zzn4_print(_MIPD_ zzn4 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); zzn2_print(_MIPP_ &(x->a)); printf(","); zzn2_print(_MIPP_ &(x->b));  printf(")");
}

void zzn12_print(_MIPD_ zzn12 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); zzn4_print(_MIPP_ &(x->a)); printf(","); zzn4_print(_MIPP_ &(x->b)); printf(","); zzn4_print(_MIPP_ &(x->c)); printf(")"); printf("\n");
}

void ecn2_print(_MIPD_ ecn2 *P)
{
	#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
	#endif
	if (P->marker==MR_EPOINT_INFINITY)
	{
		printf("[Infinity]\n");
		return;
	}
	ecn2_norm(_MIPP_ P);
	printf("[");
	zzn2_print(_MIPP_ &(P->x));
	printf(",");
	zzn2_print(_MIPP_ &(P->y));
	printf("]\n");
}
*/


/* Initialise the MPIN_BN domain structure
 * It is assumed that the EC domain details are obtained from ROM
 */
#define MR_MPIN_BN_INIT_RESERVE 20

int MPIN_DOMAIN_INIT_NEW(mpin_domain *DOM)
{ /* get domain details from ROM     */
	int pt,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,2*PFS,16);
#else
    miracl *mr_mip=mirsys(2*PFS,16);
#endif
    big x,q,r,a,b,beta,xx,yy;
	ecn2 Q;
	zzn2 f,qx,qy;
    int words,promptr,err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_BN_INIT_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE));
#endif
	DOM->nibbles=2*PFS;
	words=MR_ROUNDUP(PFS*8,MIRACL);

	if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;

    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
		b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        xx=mirvar_mem(_MIPP_ mem, num++);
		yy=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);

/* read in from PROM and make simple integrity checks */

		promptr=0;
		init_big_from_rom(x,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);  /* Read in BN parameter from ROM   */
		sftbit(_MIPP_ x,-(PFS*8-2),xx);  /* top 2 bits encode extra info */
		pt=size(xx);
		expb2(_MIPP_ (PFS*8-2),xx);
		divide(_MIPP_ x,xx,xx);

		mr_mip->TWIST=MR_SEXTIC_D;
		if ((pt&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((pt&2)==2) negify(x,x); 

		init_big_from_rom(b,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);  /* Read in curve parameter b from ROM   */

		getprb(_MIPP_ x,q,r,beta);
		zero(a);
		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

		nres(_MIPP_ beta,beta);

		init_big_from_rom(xx,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qx);
		init_big_from_rom(xx,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qy);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{	
		init_big_from_rom(xx,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)mprom,words*ROM_SIZE,&promptr);
		zzn2_from_bigs(_MIPP_ xx,yy,&f);
	}
	if (res==0)
	{
		DOM->flags=pt;
		big_to_bytes(_MIPP_ PFS,x,DOM->X,TRUE); /* bigs here */
		big_to_bytes(_MIPP_ PFS,q,DOM->Q,TRUE);
		big_to_bytes(_MIPP_ PFS,a,DOM->A,TRUE);
		big_to_bytes(_MIPP_ PFS,b,DOM->B,TRUE);
		big_to_bytes(_MIPP_ PGS,r,DOM->R,TRUE);
 		big_to_bytes(_MIPP_ PFS,beta,DOM->Beta,TRUE); /* nresidues from here */
		big_to_bytes(_MIPP_ PFS,qx.a,DOM->Qxa,TRUE); 
		big_to_bytes(_MIPP_ PFS,qx.b,DOM->Qxb,TRUE);
		big_to_bytes(_MIPP_ PFS,qy.a,DOM->Qya,TRUE);
		big_to_bytes(_MIPP_ PFS,qy.b,DOM->Qyb,TRUE);
		big_to_bytes(_MIPP_ PFS,f.a,DOM->Fa,TRUE);
		big_to_bytes(_MIPP_ PFS,f.b,DOM->Fb,TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_BN_INIT_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* Initialise the MPIN_BN domain structure
 * It is assumed that the EC domain details are obtained from ROM
 */

#define MR_MPIN_BN_INIT_RESERVE 20

int MPIN_DOMAIN_INIT(mpin_domain *DOM,const void *rom)
{ /* get domain details from ROM     */
	int pt,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,2*PFS,16);
#else
    miracl *mr_mip=mirsys(2*PFS,16);
#endif
    big x,q,r,a,b,beta,xx,yy;
	ecn2 Q;
	zzn2 f,qx,qy;
    int words,promptr,err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_BN_INIT_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE));
#endif
	DOM->nibbles=2*PFS;
	words=MR_ROUNDUP(PFS*8,MIRACL);

	if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;

    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
		b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        xx=mirvar_mem(_MIPP_ mem, num++);
		yy=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);

/* read in from PROM and make simple integrity checks */

		promptr=0;
		init_big_from_rom(x,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in BN parameter from ROM   */
		sftbit(_MIPP_ x,-(PFS*8-2),xx);  /* top 2 bits encode extra info */
		pt=size(xx);
		expb2(_MIPP_ (PFS*8-2),xx);
		divide(_MIPP_ x,xx,xx);

		mr_mip->TWIST=MR_SEXTIC_D;
		if ((pt&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((pt&2)==2) negify(x,x);

		init_big_from_rom(b,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter b from ROM   */

		getprb(_MIPP_ x,q,r,beta);
		zero(a);
		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

		nres(_MIPP_ beta,beta);

		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qx);
		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qy);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		zzn2_from_bigs(_MIPP_ xx,yy,&f);
	}
	if (res==0)
	{
		DOM->flags=pt;
		big_to_bytes(_MIPP_ PFS,x,DOM->X,TRUE); /* bigs here */
		big_to_bytes(_MIPP_ PFS,q,DOM->Q,TRUE);
		big_to_bytes(_MIPP_ PFS,a,DOM->A,TRUE);
		big_to_bytes(_MIPP_ PFS,b,DOM->B,TRUE);
		big_to_bytes(_MIPP_ PGS,r,DOM->R,TRUE);
 		big_to_bytes(_MIPP_ PFS,beta,DOM->Beta,TRUE); /* nresidues from here */
		big_to_bytes(_MIPP_ PFS,qx.a,DOM->Qxa,TRUE);
		big_to_bytes(_MIPP_ PFS,qx.b,DOM->Qxb,TRUE);
		big_to_bytes(_MIPP_ PFS,qy.a,DOM->Qya,TRUE);
		big_to_bytes(_MIPP_ PFS,qy.b,DOM->Qyb,TRUE);
		big_to_bytes(_MIPP_ PFS,f.a,DOM->Fa,TRUE);
		big_to_bytes(_MIPP_ PFS,f.b,DOM->Fb,TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_BN_INIT_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_BN_INIT_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

void MPIN_DOMAIN_KILL(mpin_domain *DOM)
{
	int i;
	for (i=0;i<PFS;i++)
	{
		DOM->X[i]=0;
		DOM->Q[i]=0;
		DOM->A[i]=0;
		DOM->B[i]=0;
		DOM->Beta[i]=0;
		DOM->Qxa[i]=0;
		DOM->Qxb[i]=0;		
		DOM->Qya[i]=0;		
		DOM->Qyb[i]=0;	
		DOM->Fa[i]=0;
		DOM->Fb[i]=0;
	}
	for (i=0;i<PGS;i++)
		DOM->R[i]=0;
}

/* Recombine on G1 from 2 parts - R=R1+R2  */

#define MR_MPIN_RECOMBINE_G1_RESERVE 5

int MPIN_RECOMBINE_G1(mpin_domain *DOM,octet *R1,octet *R2,octet *R)
{
	int num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,rx,ry;
    epoint *P,*T;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_RECOMBINE_G1_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G1_RESERVE)];
    char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G1_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        rx=mirvar_mem(_MIPP_ mem, num++);
        ry=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
		T=epoint_init_mem(_MIPP_ mem1,1);

		bytes_to_big(_MIPP_ PFS,&(R1->val[1]),rx);
		bytes_to_big(_MIPP_ PFS,&(R1->val[PFS+1]),ry);
        if (!epoint_set(_MIPP_ rx,ry,0,P)) res=MPIN_INVALID_POINT;

		bytes_to_big(_MIPP_ PFS,&(R2->val[1]),rx);
		bytes_to_big(_MIPP_ PFS,&(R2->val[PFS+1]),ry);
        if (!epoint_set(_MIPP_ rx,ry,0,T)) res=MPIN_INVALID_POINT; 
	}
    if (res==0)
    {
		ecurve_add(_MIPP_ T,P);  /* R = R1+R2 */
		epoint_norm(_MIPP_ P);
        epoint_get(_MIPP_ P,rx,ry);

		R->len=2*PFS+1;	R->val[0]=4;
		big_to_bytes(_MIPP_ PFS,rx,&(R->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,ry,&(R->val[PFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_RECOMBINE_G1_RESERVE);
    ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G1_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* Recombine on G2 from 2 parts - W=W1+W2 */

#define MR_MPIN_RECOMBINE_G2_RESERVE 19

int MPIN_RECOMBINE_G2(mpin_domain *DOM,octet *W1,octet *W2,octet *W)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b;
	zzn2 qx,qy; 
    ecn2 Q,T;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_RECOMBINE_G2_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G2_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G2_RESERVE));
#endif
    if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		ecn2_alloc(_MIPP_ &T,mem,&num);

		flags=DOM->flags;
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

        bytes_to_big(_MIPP_ PFS,&(W1->val[0]),qx.a); 
        bytes_to_big(_MIPP_ PFS,&(W1->val[PFS]),qx.b); 
        bytes_to_big(_MIPP_ PFS,&(W1->val[2*PFS]),qy.a); 
        bytes_to_big(_MIPP_ PFS,&(W1->val[3*PFS]),qy.b); 
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&Q)) res=MPIN_INVALID_POINT;

        bytes_to_big(_MIPP_ PFS,&(W2->val[0]),qx.a);
        bytes_to_big(_MIPP_ PFS,&(W2->val[PFS]),qx.b);
        bytes_to_big(_MIPP_ PFS,&(W2->val[2*PFS]),qy.a);
        bytes_to_big(_MIPP_ PFS,&(W2->val[3*PFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);
		if (!ecn2_set(_MIPP_ &qx,&qy,&T)) res=MPIN_INVALID_POINT;
	}
    if (res==0)
    {
		ecn2_add(_MIPP_ &T,&Q);  /* R = R1+R2 */
		ecn2_norm(_MIPP_ &Q);
        ecn2_getxy(&Q,&qx,&qy);

		redc(_MIPP_ qx.a,qx.a); redc(_MIPP_ qx.b,qx.b); redc(_MIPP_ qy.a,qy.a); redc(_MIPP_ qy.b,qy.b);
		W->len=4*PFS;
		big_to_bytes(_MIPP_ PFS,qx.a,&(W->val[0]),TRUE);
		big_to_bytes(_MIPP_ PFS,qx.b,&(W->val[PFS]),TRUE);
		big_to_bytes(_MIPP_ PFS,qy.a,&(W->val[2*PFS]),TRUE);
		big_to_bytes(_MIPP_ PFS,qy.b,&(W->val[3*PFS]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_RECOMBINE_G2_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RECOMBINE_G2_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* Use RNG to generate random integer in Group */

#define MR_MPIN_RANDOM_RESERVE 2

int MPIN_RANDOM_GENERATE(mpin_domain *DOM,csprng *RNG,octet* S)
{
	int num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big r,s;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_RANDOM_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_RANDOM_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RANDOM_RESERVE));
#endif
 
    if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
        strong_bigrand(_MIPP_ RNG,r,s);
	S->len=big_to_bytes(_MIPP_ PGS,s,S->val,TRUE);
    }

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_RANDOM_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_RANDOM_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}



#define MR_MPIN_EXTRACT_PIN_RESERVE 5

/* subtract PIN component from client private key to form Token */ 

int MPIN_EXTRACT_PIN(mpin_domain *DOM,octet *CID,int pin,octet *TOKEN)
{
	int flags,num=0;

#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,px,py;
    epoint *P,*R;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_EXTRACT_PIN_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_EXTRACT_PIN_RESERVE)];
    char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_EXTRACT_PIN_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
		
		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		P=epoint_init_mem(_MIPP_ mem1,0);
		R=epoint_init_mem(_MIPP_ mem1,1);
  		
		bytes_to_big(_MIPP_ PFS,&(TOKEN->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(TOKEN->val[PFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,P)) res=MPIN_INVALID_POINT; 
	}
	if (res==0)
	{
		hash(NULL,-1,CID,NULL,TOKEN);	
		bytes_to_big(_MIPP_ TOKEN->len,TOKEN->val,px);
		divide(_MIPP_ px,q,q);

		while (!epoint_set(_MIPP_ px,px,0,R))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

		convert(_MIPP_ pin,a);
		ecurve_mult(_MIPP_ a,R,R);
		ecurve_sub(_MIPP_ R,P);   /* TOKEN = TOKEN-pin*ID */

		epoint_get(_MIPP_ P,px,py);

		TOKEN->len=2*PFS+1;	TOKEN->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(TOKEN->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(TOKEN->val[PFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_EXTRACT_PIN_RESERVE);
	ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_EXTRACT_PIN_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#define MR_MPIN_CLIENT_3_RESERVE 54

int	MPIN_CLIENT_3(mpin_domain *DOM,octet *X,octet *Y,octet *M,octet *mPAIR,octet *K)
{
	int flags,num=0;
	HASHFUNC sha;
	char ht[PFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,a,b,q,px,py,r,m,beta,y;
	zzn12 g,gp;
	zzn4 c,cp,cpm1,cpm2;
	zzn2 f,f2;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_  MR_MPIN_CLIENT_3_RESERVE);
#else
    char mem[MR_BIG_RESERVE( MR_MPIN_CLIENT_3_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_CLIENT_3_RESERVE));
#endif
 
    if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
		a=mirvar_mem(_MIPP_ mem, num++);
		b=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        y=mirvar_mem(_MIPP_ mem, num++);
		m=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);		
		beta=mirvar_mem(_MIPP_ mem, num++);  
		zzn4_alloc(_MIPP_ &c,mem,&num);
		zzn4_alloc(_MIPP_ &cp,mem,&num);
		zzn4_alloc(_MIPP_ &cpm1,mem,&num);
		zzn4_alloc(_MIPP_ &cpm2,mem,&num);
		zzn12_alloc(_MIPP_ &g,mem,&num);
		zzn12_alloc(_MIPP_ &gp,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		zzn2_alloc(_MIPP_ &f2,mem,&num);
		
		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);

		prepare_monty(_MIPP_ q);

		bytes_to_big(_MIPP_ PFS,&mPAIR->val[0],a);			nres(_MIPP_ a,g.a.a.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[PFS],a);		nres(_MIPP_ a,g.a.a.b);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[2*PFS],a);		nres(_MIPP_ a,g.a.b.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[3*PFS],a);		nres(_MIPP_ a,g.a.b.b);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[4*PFS],a);		nres(_MIPP_ a,g.b.a.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[5*PFS],a);		nres(_MIPP_ a,g.b.a.b);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[6*PFS],a);		nres(_MIPP_ a,g.b.b.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[7*PFS],a);		nres(_MIPP_ a,g.b.b.b);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[8*PFS],a);		nres(_MIPP_ a,g.c.a.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[9*PFS],a);		nres(_MIPP_ a,g.c.a.b);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[10*PFS],a);		nres(_MIPP_ a,g.c.b.a);
		bytes_to_big(_MIPP_ PFS,&mPAIR->val[11*PFS],a);		nres(_MIPP_ a,g.c.b.b);
		g.unitary=TRUE;

		if (!member(_MIPP_ &f,x,&g)) res=MPIN_WRONG_ORDER;
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ X->len,X->val,x);
		bytes_to_big(_MIPP_ Y->len,Y->val,y);
		add(_MIPP_ x,y,x);
		bytes_to_big(_MIPP_ M->len,M->val,m);
		xgcd(_MIPP_ m,r,m,m,m);  mad(_MIPP_ x,m,m,r,r,x); 

		copy(q,m);    /* divide exponent up into v mod (t-1) and v div (t-1). Note these numbers are half size - hence the speed-up */
		divide(_MIPP_ m,r,r);

		copy(x,a);
		divide(_MIPP_ a,m,b);
		trace(_MIPP_ &g,&c);

		zzn12_copy(&g,&gp);
		zzn12_powq(_MIPP_ &f,&gp);

		trace(_MIPP_ &gp,&cp);

		zzn12_conj(_MIPP_ &g,&g);
		zzn12_mul(_MIPP_ &gp,&g,&gp);
		trace(_MIPP_ &gp,&cpm1);
		zzn12_mul(_MIPP_ &gp,&g,&gp);
		trace(_MIPP_ &gp,&cpm2);

		xtr_pow2(_MIPP_ &cp,&c,&cpm1,&cpm2,a,b,&c); /* g^z - this is faster */

		HT.len=PFS;
		start_hash(&sha);
		redc(_MIPP_ c.a.a,x); big_to_bytes(_MIPP_ PFS,x,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.a.b,x); big_to_bytes(_MIPP_ PFS,x,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.b.a,x); big_to_bytes(_MIPP_ PFS,x,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.b.b,x); big_to_bytes(_MIPP_ PFS,x,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);	

		finish_hash(&sha,&HT);
		OCTET_EMPTY(K);
		OCTET_JOIN_BYTES(HT.val,PAS,K);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_CLIENT_3_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_CLIENT_3_RESERVE));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


/* MPin - second Pass - S=-(x+y)*S */

#define MR_MPIN_CLIENT_2_RESERVE 8

int MPIN_CLIENT_2(mpin_domain *DOM,octet *X,octet *Y,octet *SEC)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,px,py,r,beta;
    epoint *P;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_  MR_MPIN_CLIENT_2_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_CLIENT_2_RESERVE)];
    char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE(MR_MINI_CLIENT_2_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);		
		beta=mirvar_mem(_MIPP_ mem, num++);  
		
		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		P=epoint_init_mem(_MIPP_ mem1,0);
	
// Get Secret	
		bytes_to_big(_MIPP_ PFS,&(SEC->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(SEC->val[PFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,P)) res=MPIN_INVALID_POINT; 
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ X->len,X->val,px);
		bytes_to_big(_MIPP_ Y->len,Y->val,py);

		add(_MIPP_ px,py,px);
		divide(_MIPP_ px,r,r);
		subtract(_MIPP_ r,px,px);

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ px, P, P);
#else
		G1_mult(_MIPP_ P,px,beta,r,x,P);  
#endif

		epoint_get(_MIPP_ P,px,py);

		SEC->len=2*PFS+1;	SEC->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(SEC->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(SEC->val[PFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_CLIENT_2_RESERVE);
	ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_CLIENT_2_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


/* For MPin First Pass set M=NULL for MINI, otherwise its FULL. Set date=0 PERMIT=NULL if no permits */

/* Hashes Client ID to point on G1
   generates (or is passed) random X, and calculates X.H(ID)
   Recombines TOKEN and PIN to create SEC
   If PERMIT, calculates x(H(ID)+H_T(date|ID))
   If FULL returns M.SEC

*/


#define MR_MPIN_CLIENT_1_RESERVE 10

int MPIN_CLIENT_1(mpin_domain *DOM,int date,octet *CLIENT_ID,csprng *RNG,octet *X,int pin,octet *TOKEN,octet *SEC,octet *xID,octet *PERMIT,octet *xCID,octet *M,octet *mSEC)
{
	int flags,num=0;
        /* kmc 21/5/2014 
           added HASHCID
	*/
	char hashCID[HASH_BYTES];
	octet HASHCID={0,sizeof(hashCID),hashCID};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,px,py,r,s,beta,m;
    epoint *P,*T,*W;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_  MR_MPIN_CLIENT_1_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 3);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_CLIENT_1_RESERVE)];
    char mem1[MR_ECP_RESERVE(3)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_CLIENT_1_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(3));
#endif
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
		m=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);		
		
		beta=mirvar_mem(_MIPP_ mem, num++);  
		
		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);

		if (RNG!=NULL)
		{
			strong_bigrand(_MIPP_ RNG,r,s);
			X->len=big_to_bytes(_MIPP_ PGS,s,X->val,TRUE);
			if (M!=NULL)
			{
				strong_bigrand(_MIPP_ RNG,r,m);
				M->len=big_to_bytes(_MIPP_ PGS,m,M->val,TRUE);
			}
		}
		else
		{	
			bytes_to_big(_MIPP_ X->len,X->val,s);
			if (M!=NULL) bytes_to_big(_MIPP_ M->len,M->val,m);
		}

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		P=epoint_init_mem(_MIPP_ mem1,0);
		T=epoint_init_mem(_MIPP_ mem1,1);
		W=epoint_init_mem(_MIPP_ mem1,2);

/* Set P = IDa */ 
		hash(NULL,-1,CLIENT_ID,NULL,xID);	
		bytes_to_big(_MIPP_ xID->len,xID->val,px);
		divide(_MIPP_ px,q,q);

		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}
/* T=Token */	
		bytes_to_big(_MIPP_ PFS,&(TOKEN->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(TOKEN->val[PFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,T)) res=MPIN_INVALID_POINT; 
	}
	if (res==0)
	{
		convert(_MIPP_ pin,a);
		ecurve_mult(_MIPP_ a,P,W);
		ecurve_add(_MIPP_ W,T);   
 /* Secret T = T+pin*ID */

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ s, P, P);
#else
		G1_mult(_MIPP_ P,s,beta,r,x,P);  
#endif

		epoint_get(_MIPP_ P,px,py);

		xID->len=2*PFS+1;	xID->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(xID->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(xID->val[PFS+1]),TRUE);
/* output x.H(ID) */

	}
	if (date)
	{
                /* kmc 21/5/2014 
                   added next two lines as now the time permit is;
                     TP =  H1(DATE|hash(IDc))
                */
                hash(NULL,-1,CLIENT_ID,NULL,&HASHCID);	
		hash(NULL,date,&HASHCID,NULL,xCID);	/* H1(DATE|hash(ID)) */
		/* hash(NULL,date,CLIENT_ID,NULL,xCID);	/* H1(DATE|ID) */
		bytes_to_big(_MIPP_ xCID->len,xCID->val,px);
		divide(_MIPP_ px,q,q);

		while (!epoint_set(_MIPP_ px,px,0,W))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ s, W, W);
#else
		G1_mult(_MIPP_ W,s,beta,r,x,W);  
#endif


		ecurve_add(_MIPP_ W,P);
		epoint_get(_MIPP_ P,px,py);

/* output x.(H(ID)+H_T(date|ID)) */

		xCID->len=2*PFS+1;	xCID->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(xCID->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(xCID->val[PFS+1]),TRUE);

/* Add permit to secret */
		bytes_to_big(_MIPP_ PFS,&(PERMIT->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(PERMIT->val[PFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,W)) res=MPIN_INVALID_POINT; 
		ecurve_add(_MIPP_ W,T);

	}
	if (res==0)
	{
		epoint_get(_MIPP_ T,px,py);

		SEC->len=2*PFS+1;	SEC->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(SEC->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(SEC->val[PFS+1]),TRUE);

		if (M!=NULL)
		{
#ifdef AVOID_PATENT
                        ecurve_mult(_MIPP_ m, T, T);
#else
			G1_mult(_MIPP_ T,m,beta,r,x,T);  
#endif


			epoint_get(_MIPP_ T,px,py);

			mSEC->len=2*PFS+1;	mSEC->val[0]=4;
			big_to_bytes(_MIPP_ PFS,px,&(mSEC->val[1]),TRUE);
			big_to_bytes(_MIPP_ PFS,py,&(mSEC->val[PFS+1]),TRUE);
		}
	} 


#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_CLIENT_1_RESERVE);
	ecp_memkill(_MIPP_ mem1,3);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_CLIENT_1_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(3));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* MPin Mini Server Calculation. E^\delta=F, where \delta is error in PIN */

#define MR_MPIN_MINI_SERVER_RESERVE 39

int MPIN_MINI_SERVER(mpin_domain *DOM,int date,octet *CID,octet *Y,octet *SST,octet *xID,octet *xCID,octet *mSEC,octet *E,octet *F)
{
	int flags,num=0;
	char ht[PFS];
	octet HT={0,sizeof(ht),ht};
        /* kmc 21/5/2014 
           added HASHCID
	*/
	char hashCID[HASH_BYTES];
	octet HASHCID={0,sizeof(hashCID),hashCID};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,r,beta,px,py,y;
	zzn2 f,qx,qy; 
	zzn12 g; 
    ecn2 Q,sQ;
	epoint *P,*R;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_MINI_SERVER_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_MINI_SERVER_RESERVE)];    
	char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_MINI_SERVER_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
	y=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);	
	beta=mirvar_mem(_MIPP_ mem, num++);  

	zzn2_alloc(_MIPP_ &qx,mem,&num);
	zzn2_alloc(_MIPP_ &qy,mem,&num);
	zzn2_alloc(_MIPP_ &f,mem,&num);
	ecn2_alloc(_MIPP_ &Q,mem,&num);
	ecn2_alloc(_MIPP_ &sQ,mem,&num);
	zzn12_alloc(_MIPP_ &g,mem,&num);

	flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
	mr_mip->TWIST=MR_SEXTIC_D;
	if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
	if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
	bytes_to_big(_MIPP_ PGS,DOM->R,r);
	bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
 	P=epoint_init_mem(_MIPP_ mem1,0);
	R=epoint_init_mem(_MIPP_ mem1,1);
  
        bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
      
	if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		hash(NULL,-1,CID,NULL,&HT);	
		bytes_to_big(_MIPP_ HT.len,HT.val,px);
		divide(_MIPP_ px,q,q);
		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

		if (date)
		{
                        /* kmc 21/5/2014 
                           added next two lines as now the time permit is;
                             TP =  H1(DATE|hash(IDc))
                        */
                        hash(NULL,-1,CID,NULL,&HASHCID);	
			hash(NULL,date,&HASHCID,NULL,&HT);	/* H0(hash(identity)) plus H1(date|hash(identity)) */
			/* hash(NULL,date,CID,NULL,&HT);	/* H0(identity) plus H1(date|identity) */
			bytes_to_big(_MIPP_ HT.len,HT.val,px);
			divide(_MIPP_ px,q,q);
			while (!epoint_set(_MIPP_ px,px,0,R))
			{
				if (mr_mip->ERNUM!=0) break;
				incr(_MIPP_ px,1,px);
			}
			ecurve_add(_MIPP_ R,P);   // A+AT
			bytes_to_big(_MIPP_ PFS,&(xCID->val[1]),px);
			bytes_to_big(_MIPP_ PFS,&(xCID->val[PFS+1]),py);
		}
		else
		{
			bytes_to_big(_MIPP_ PFS,&(xID->val[1]),px);
			bytes_to_big(_MIPP_ PFS,&(xID->val[PFS+1]),py);
		}

		bytes_to_big(_MIPP_ Y->len,Y->val,y);

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ y, P, P);
#else
		G1_mult(_MIPP_ P,y,beta,r,x,P);  // y(A+AT)
#endif



		if (!epoint_set(_MIPP_ px,py,0,R)) res=MPIN_INVALID_POINT; // x(A+AT)
	}
	if (res==0)
	{
		ecurve_add(_MIPP_ R,P);  // x(A+AT)+y(A+T)
		bytes_to_big(_MIPP_ PFS,&(mSEC->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(mSEC->val[PFS+1]),py);

		if (!epoint_set(_MIPP_ px,py,0,R)) res=MPIN_INVALID_POINT; // V
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ PFS,&(SST->val[0]),qx.a);
		bytes_to_big(_MIPP_ PFS,&(SST->val[PFS]),qx.b);
		bytes_to_big(_MIPP_ PFS,&(SST->val[2*PFS]),qy.a);
		bytes_to_big(_MIPP_ PFS,&(SST->val[3*PFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&sQ))  res=MPIN_INVALID_POINT;
	}
	if (res==0)
	{
		rate_double_miller(_MIPP_ &Q,R,&sQ,P,x,&f,&g);
                rate_fexp(_MIPP_ x,&f,&g);
                if (!zzn12_isunity(_MIPP_ &g)) 
                   res=MPIN_BAD_PIN;
/* KMcC commented out 3/3/14 as is cause of timing attack success  */
/* 		if (!zzn12_isunity(_MIPP_ &g)) */
/* 		{ /\* it didn't work out - capture evidence to find error  *\/ */
/* 			E->len=12*PFS; */

/* 			big_to_bytes(_MIPP_ PFS,g.a.a.a,&(E->val[0]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.a.a.b,&(E->val[PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.a.b.a,&(E->val[2*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.a.b.b,&(E->val[3*PFS]),TRUE); */

/* 			big_to_bytes(_MIPP_ PFS,g.b.a.a,&(E->val[4*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.b.a.b,&(E->val[5*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.b.b.a,&(E->val[6*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.b.b.b,&(E->val[7*PFS]),TRUE); */

/* 			big_to_bytes(_MIPP_ PFS,g.c.a.a,&(E->val[8*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.c.a.b,&(E->val[9*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.c.b.a,&(E->val[10*PFS]),TRUE); */
/* 			big_to_bytes(_MIPP_ PFS,g.c.b.b,&(E->val[11*PFS]),TRUE); */

/* // Note error is in the PIN, not in the time permit! Hence the need to exclude Time Permit from this check */

/* 			if (date) */
/* 			{ /\* Compensate for Time Permit *\/ */
/* 				hash(NULL,-1,CID,NULL,&HT);	 */
/* 				bytes_to_big(_MIPP_ HT.len,HT.val,px); */
/* 				divide(_MIPP_ px,q,q); */
/* 				while (!epoint_set(_MIPP_ px,px,0,P)) */
/* 				{ */
/* 					if (mr_mip->ERNUM!=0) break; */
/* 					incr(_MIPP_ px,1,px); */
/* 				} */
/* #ifdef AVOID_PATENT */
/*                                 ecurve_mult(_MIPP_ y, P, P); */
/* #else */
/*  				G1_mult(_MIPP_ P,y,beta,r,x,P);  // yA  */
/* #endif */

/* 				bytes_to_big(_MIPP_ PFS,&(xID->val[1]),px); */
/* 				bytes_to_big(_MIPP_ PFS,&(xID->val[PFS+1]),py); */

/* 				if (!epoint_set(_MIPP_ px,py,0,R)) res=MPIN_INVALID_POINT; // U */

/* 				ecurve_add(_MIPP_ R,P);   // yA+xA */
/* 			} */
/* 			if (res==0) */
/* 			{ */
/* 				rate_miller(_MIPP_ &Q,P,x,&f,&g); */
/* 				rate_fexp(_MIPP_ x,&f,&g); */

/* 				F->len=12*PFS; */

/* 				big_to_bytes(_MIPP_ PFS,g.a.a.a,&(F->val[0]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.a.a.b,&(F->val[PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.a.b.a,&(F->val[2*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.a.b.b,&(F->val[3*PFS]),TRUE); */

/* 				big_to_bytes(_MIPP_ PFS,g.b.a.a,&(F->val[4*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.b.a.b,&(F->val[5*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.b.b.a,&(F->val[6*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.b.b.b,&(F->val[7*PFS]),TRUE); */

/* 				big_to_bytes(_MIPP_ PFS,g.c.a.a,&(F->val[8*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.c.a.b,&(F->val[9*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.c.b.a,&(F->val[10*PFS]),TRUE); */
/* 				big_to_bytes(_MIPP_ PFS,g.c.b.b,&(F->val[11*PFS]),TRUE); */

/* 				res=MPIN_BAD_PIN; */
/* 			} */
/* 		} */
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_MINI_SERVER_RESERVE);
	ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_MINI_SERVER_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#if PINDIGITS == 4
#define MAXPIN 10000
#define MR_TS 9
#define TRAP 150
#endif

#if PINDIGITS == 6
#define MAXPIN 1000000
#define MR_TS 12
#define TRAP 800
#endif

#define MR_MPIN_KANGAROO_RESERVE 42+12*MR_TS

int MPIN_KANGAROO(mpin_domain *DOM,octet *E,octet *F)
{
	int i,j,m,s,dn,dm,steps,flags,num=0;
	int distance[MR_TS];
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b;
	zzn2 f; 
	zzn12 ge,gf,t,table[MR_TS]; 
    int res=0;

#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_KANGAROO_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_KANGAROO_RESERVE)];    
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_KANGAROO_RESERVE));
#endif
    if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		zzn12_alloc(_MIPP_ &ge,mem,&num);
		zzn12_alloc(_MIPP_ &gf,mem,&num);
		zzn12_alloc(_MIPP_ &t,mem,&num);
		for (i=0;i<MR_TS;i++)
			zzn12_alloc(_MIPP_ &table[i],mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

		ge.unitary=TRUE; ge.miller=FALSE;
        bytes_to_big(_MIPP_ PFS,&(E->val[0]),ge.a.a.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[PFS]),ge.a.a.b);
		bytes_to_big(_MIPP_ PFS,&(E->val[2*PFS]),ge.a.b.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[3*PFS]),ge.a.b.b);
        bytes_to_big(_MIPP_ PFS,&(E->val[4*PFS]),ge.b.a.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[5*PFS]),ge.b.a.b);
		bytes_to_big(_MIPP_ PFS,&(E->val[6*PFS]),ge.b.b.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[7*PFS]),ge.b.b.b);
        bytes_to_big(_MIPP_ PFS,&(E->val[8*PFS]),ge.c.a.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[9*PFS]),ge.c.a.b);
		bytes_to_big(_MIPP_ PFS,&(E->val[10*PFS]),ge.c.b.a);
		bytes_to_big(_MIPP_ PFS,&(E->val[11*PFS]),ge.c.b.b);

		gf.unitary=TRUE; gf.miller=FALSE;
        bytes_to_big(_MIPP_ PFS,&(F->val[0]),gf.a.a.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[PFS]),gf.a.a.b);
		bytes_to_big(_MIPP_ PFS,&(F->val[2*PFS]),gf.a.b.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[3*PFS]),gf.a.b.b);
        bytes_to_big(_MIPP_ PFS,&(F->val[4*PFS]),gf.b.a.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[5*PFS]),gf.b.a.b);
		bytes_to_big(_MIPP_ PFS,&(F->val[6*PFS]),gf.b.b.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[7*PFS]),gf.b.b.b);
        bytes_to_big(_MIPP_ PFS,&(F->val[8*PFS]),gf.c.a.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[9*PFS]),gf.c.a.b);
		bytes_to_big(_MIPP_ PFS,&(F->val[10*PFS]),gf.c.b.a);
		bytes_to_big(_MIPP_ PFS,&(F->val[11*PFS]),gf.c.b.b);

/* Pollards Kangaroos */
		zzn12_copy(&gf,&t);

/* optimal table size 9 for 4 digit PIN */
/* optimal table size 12 for 6 digit PIN */
/* Note average of (1+2+4+8+16+32+64+128+256)/9 =  57 = alpha, which should be 0.51*sqrt(10000) */
/* Note average of (1+2+4+8+16+32+64+128+256+512+1024+2048)/12 =  341 = alpha, which should ideally  be 0.51*sqrt(10000) */

		for (s=1,m=0;m<MR_TS;m++)
		{ /* create table */
			distance[m]=s;
			zzn12_copy(&t,&table[m]);
			s*=2;
			zzn12_sqr(_MIPP_ &t,&t);
		}

/* Start Tame Kangaroo from middle of range */

		zzn12_from_int(_MIPP_ 1,&t);

/* Trap should ideally be set after 1.39*alpha jumps - 150 for a 4 digit PIN, 800 for a 6-digit PIN */
/* Here we set Trap twice as far out - to minimize failures, and give paths more time to converge */

		for (dn=0,j=0;j<TRAP;j++) 
		{
			i=remain(_MIPP_ t.a.a.a,MR_TS);  /* random function */
			zzn12_mul(_MIPP_ &t,&table[i],&t);
			dn+=distance[i];
		}

/* Trap set at t */

//printf("dn= %d\n",dn);
		zzn12_conj(_MIPP_ &t,&gf);
		steps=0; dm=0;
		while (dm-dn<MAXPIN)
		{ /* unlease Wild Kangaroo - boing - boing ... */
			steps++;
			i=remain(_MIPP_ ge.a.a.a,MR_TS); 
			zzn12_mul(_MIPP_ &ge,&table[i],&ge);
			dm+=distance[i];
			if (zzn12_compare(&ge,&t))
			{
				res=dm-dn;
		//		printf("PIN is out by %d\n",dm-dn);
				break;
			}
			if (zzn12_compare(&ge,&gf))
			{
				res=dn-dm;
			//	printf("PIN is out by -%d\n",dm-dn);
				break;
			}
		}
		/* printf("After %d iterations\n",steps); */
		if (dm-dn>=MAXPIN) {res=0; /*printf("Trap Failed\n");*/}    // Trap Failed  - probable invalid token
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_KANGAROO_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_KANGAROO_RESERVE));
#endif

    mirexit(_MIPPO_ );
    return res;
}

#define MR_MPIN_FULL_SERVER_1_RESERVE 34

int MPIN_FULL_SERVER_1(mpin_domain *DOM,csprng *RNG,octet *Y,octet *N,octet *mSEC,octet *E)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,r,beta,px,py,y,n;
	zzn2 f,qx,qy; 
	zzn12 g; 
    ecn2 Q;
	epoint *P;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_FULL_SERVER_1_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_1_RESERVE)];    
	char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_1_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		r=mirvar_mem(_MIPP_ mem, num++);
		y=mirvar_mem(_MIPP_ mem, num++);
		n=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);	
		beta=mirvar_mem(_MIPP_ mem, num++);  

		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn12_alloc(_MIPP_ &g,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
		bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);

		if (RNG!=NULL)
		{
			strong_bigrand(_MIPP_ RNG,r,y);
			Y->len=big_to_bytes(_MIPP_ 0,y,Y->val,FALSE);
			strong_bigrand(_MIPP_ RNG,r,n);
			N->len=big_to_bytes(_MIPP_ 0,n,N->val,FALSE);

		}
		else
		{	
			bytes_to_big(_MIPP_ Y->len,Y->val,y);
			bytes_to_big(_MIPP_ N->len,N->val,n);
		}

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
 		P=epoint_init_mem(_MIPP_ mem1,0);
  
        bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
    
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}

	if (res==0)
	{
		bytes_to_big(_MIPP_ PFS,&(mSEC->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(mSEC->val[PFS+1]),py);

		if (!epoint_set(_MIPP_ px,py,0,P)) res=MPIN_INVALID_POINT; 
	}
	if (res==0)
	{
#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ n, P, P);
#else
		G1_mult(_MIPP_ P,n,beta,r,x,P);  
#endif


		rate_miller(_MIPP_ &Q,P,x,&f,&g);
        rate_fexp(_MIPP_ x,&f,&g);

		E->len=12*PFS;
		
		redc(_MIPP_ g.a.a.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[0]),TRUE);
		redc(_MIPP_ g.a.a.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[PFS]),TRUE);
		redc(_MIPP_ g.a.b.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[2*PFS]),TRUE);
		redc(_MIPP_ g.a.b.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[3*PFS]),TRUE);

		redc(_MIPP_ g.b.a.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[4*PFS]),TRUE);
		redc(_MIPP_ g.b.a.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[5*PFS]),TRUE);
		redc(_MIPP_ g.b.b.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[6*PFS]),TRUE);
		redc(_MIPP_ g.b.b.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[7*PFS]),TRUE);

		redc(_MIPP_ g.c.a.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[8*PFS]),TRUE);
		redc(_MIPP_ g.c.a.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[9*PFS]),TRUE);
		redc(_MIPP_ g.c.b.a,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[10*PFS]),TRUE);
		redc(_MIPP_ g.c.b.b,a); big_to_bytes(_MIPP_ PFS,a,&(E->val[11*PFS]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_FULL_SERVER_1_RESERVE);
	ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_1_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


#define MR_MPIN_FULL_SERVER_2_RESERVE 50

int MPIN_FULL_SERVER_2(mpin_domain *DOM,int date,octet *CLIENT_ID,octet *Y,octet *N,octet *SEC,octet *xID,octet *xCID,octet *SST,octet *K,octet *E,octet *F)
{
	int flags,num=0;
	HASHFUNC sha;
	char ht[PFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,r,beta,px,py,y,n;
	zzn2 f,qx,qy; 
	zzn4 c;
	zzn12 g,rg; 
    ecn2 Q;
	epoint *P,*R;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_FULL_SERVER_2_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_2_RESERVE)];    
	char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_2_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		r=mirvar_mem(_MIPP_ mem, num++);
		y=mirvar_mem(_MIPP_ mem, num++);
		n=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);	
		beta=mirvar_mem(_MIPP_ mem, num++);  

		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn12_alloc(_MIPP_ &g,mem,&num);
		zzn12_alloc(_MIPP_ &rg,mem,&num);
		zzn4_alloc(_MIPP_ &c,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
		bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);

		bytes_to_big(_MIPP_ Y->len,Y->val,y);
		bytes_to_big(_MIPP_ N->len,N->val,n);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
 		P=epoint_init_mem(_MIPP_ mem1,0);
		R=epoint_init_mem(_MIPP_ mem1,1);
  
        bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
    
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ PFS,&(SEC->val[1]),px);
		bytes_to_big(_MIPP_ PFS,&(SEC->val[PFS+1]),py);

		if (!epoint_set(_MIPP_ px,py,0,P)) res=MPIN_INVALID_POINT; //Px
	}
	if (res==0)
	{
#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ n, P, P);
#else
		G1_mult(_MIPP_ P,n,beta,r,x,P);  
#endif



		rate_miller(_MIPP_ &Q,P,x,&f,&rg);
        rate_fexp(_MIPP_ x,&f,&rg);
	}
	if (res==0)
	{
		hash(NULL,-1,CLIENT_ID,NULL,&HT);	
		bytes_to_big(_MIPP_ HT.len,HT.val,px);
		divide(_MIPP_ px,q,q);
		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

		if (date)
		{
			hash(NULL,date,CLIENT_ID,NULL,&HT);	/* H0(identity) plus HT(date|identity) */
			bytes_to_big(_MIPP_ HT.len,HT.val,px);
			divide(_MIPP_ px,q,q);
			while (!epoint_set(_MIPP_ px,px,0,R))
			{
				if (mr_mip->ERNUM!=0) break;
				incr(_MIPP_ px,1,px);
			}
			ecurve_add(_MIPP_ R,P);   // A+AT
			bytes_to_big(_MIPP_ PFS,&(xCID->val[1]),px);
			bytes_to_big(_MIPP_ PFS,&(xCID->val[PFS+1]),py);
		}
		else
		{
			bytes_to_big(_MIPP_ PFS,&(xID->val[1]),px);
			bytes_to_big(_MIPP_ PFS,&(xID->val[PFS+1]),py);
		}

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ y, P, P);
#else
		G1_mult(_MIPP_ P,y,beta,r,x,P);  // P = y(A+AT)  
#endif



		if (!epoint_set(_MIPP_ px,py,0,R)) res=MPIN_INVALID_POINT; //U

	}
	if (res==0)
	{
		ecurve_add(_MIPP_ P,R);
#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ n, R, R);
#else
		G1_mult(_MIPP_ R,n,beta,r,x,R);  
#endif



		bytes_to_big(_MIPP_ PFS,&(SST->val[0]),qx.a);
		bytes_to_big(_MIPP_ PFS,&(SST->val[PFS]),qx.b);
		bytes_to_big(_MIPP_ PFS,&(SST->val[2*PFS]),qy.a);
		bytes_to_big(_MIPP_ PFS,&(SST->val[3*PFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MPIN_INVALID_POINT;
	}
	if (res==0)
	{
		rate_miller(_MIPP_ &Q,R,x,&f,&g);
        rate_fexp(_MIPP_ x,&f,&g);

		zzn12_mul(_MIPP_ &rg,&g,&rg);
		if (!zzn12_isunity(_MIPP_ &rg))
		{
			E->len=12*PFS;
		
			big_to_bytes(_MIPP_ PFS,rg.a.a.a,&(E->val[0]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.a.a.b,&(E->val[PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.a.b.a,&(E->val[2*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.a.b.b,&(E->val[3*PFS]),TRUE);

			big_to_bytes(_MIPP_ PFS,rg.b.a.a,&(E->val[4*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.b.a.b,&(E->val[5*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.b.b.a,&(E->val[6*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.b.b.b,&(E->val[7*PFS]),TRUE);

			big_to_bytes(_MIPP_ PFS,rg.c.a.a,&(E->val[8*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.c.a.b,&(E->val[9*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.c.b.a,&(E->val[10*PFS]),TRUE);
			big_to_bytes(_MIPP_ PFS,rg.c.b.b,&(E->val[11*PFS]),TRUE);

// Note error is in the PIN, not in the time permit! Hence the need to exclude Time Permit from this check

			if (date)
			{ /* Compensate for Time Permit */
				hash(NULL,-1,CLIENT_ID,NULL,&HT);	
				bytes_to_big(_MIPP_ HT.len,HT.val,px);
				divide(_MIPP_ px,q,q);
				while (!epoint_set(_MIPP_ px,px,0,P))
				{
					if (mr_mip->ERNUM!=0) break;
					incr(_MIPP_ px,1,px);
				}

#ifdef AVOID_PATENT
                                ecurve_mult(_MIPP_ y, P, P);
#else
				G1_mult(_MIPP_ P,y,beta,r,x,P);  // yA
#endif



				bytes_to_big(_MIPP_ PFS,&(xID->val[1]),px);
				bytes_to_big(_MIPP_ PFS,&(xID->val[PFS+1]),py);

				if (!epoint_set(_MIPP_ px,py,0,R)) res=MPIN_INVALID_POINT; // xA
				ecurve_add(_MIPP_ P,R);   // yA+xA
#ifdef AVOID_PATENT
                                ecurve_mult(_MIPP_ n, R, R);
#else
				G1_mult(_MIPP_ R,n,beta,r,x,R);  
#endif

			}
			if (res==0)
			{
				bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
				bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
				bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
				bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
    
				ecn2_set(_MIPP_ &qx,&qy,&Q);

				rate_miller(_MIPP_ &Q,R,x,&f,&g);
				rate_fexp(_MIPP_ x,&f,&g);

				F->len=12*PFS;

				big_to_bytes(_MIPP_ PFS,g.a.a.a,&(F->val[0]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.a.a.b,&(F->val[PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.a.b.a,&(F->val[2*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.a.b.b,&(F->val[3*PFS]),TRUE);

				big_to_bytes(_MIPP_ PFS,g.b.a.a,&(F->val[4*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.b.a.b,&(F->val[5*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.b.b.a,&(F->val[6*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.b.b.b,&(F->val[7*PFS]),TRUE);

				big_to_bytes(_MIPP_ PFS,g.c.a.a,&(F->val[8*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.c.a.b,&(F->val[9*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.c.b.a,&(F->val[10*PFS]),TRUE);
				big_to_bytes(_MIPP_ PFS,g.c.b.b,&(F->val[11*PFS]),TRUE);

				res=MPIN_BAD_PIN;
			}
		}
	}
	if (res==0)
	{
		trace(_MIPP_ &g,&c);

		HT.len=PFS;
		start_hash(&sha); 
		redc(_MIPP_ c.a.a,c.a.a); big_to_bytes(_MIPP_ PFS,c.a.a,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.a.b,c.a.b); big_to_bytes(_MIPP_ PFS,c.a.b,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.b.a,c.b.a); big_to_bytes(_MIPP_ PFS,c.b.a,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);
		redc(_MIPP_ c.b.b,c.b.b); big_to_bytes(_MIPP_ PFS,c.b.b,&(HT.val[0]),TRUE);
		add_to_hash(&sha,&HT);		
		finish_hash(&sha,&HT);
	
		OCTET_EMPTY(K);
		OCTET_JOIN_BYTES(HT.val,PAS,K);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_FULL_SERVER_2_RESERVE);
	ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_FULL_SERVER_2_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


/***PRIVATE***/


#define MR_MPIN_GET_SERVER_SECRET_RESERVE 18

int MPIN_GET_SERVER_SECRET(mpin_domain *DOM,octet *S,octet *SST)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,r,s;
	zzn2 f,qx,qy; 
    ecn2 Q;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_GET_SERVER_SECRET_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_GET_SERVER_SECRET_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_GET_SERVER_SECRET_RESERVE));
#endif
 
    if (mr_mip==NULL || mem==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
		bytes_to_big(_MIPP_ PGS,DOM->R,r);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);
		        
        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

        bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
      
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{

		bytes_to_big(_MIPP_ S->len,S->val,s);
		
		//	G2_mult(_MIPP_ &Q,s,&f,r,x,&Q); 
		ecn2_mul(_MIPP_ s,&Q);

		ecn2_norm(_MIPP_ &Q);
        ecn2_getxy(&Q,&qx,&qy);

		redc(_MIPP_ qx.a,qx.a); redc(_MIPP_ qx.b,qx.b); redc(_MIPP_ qy.a,qy.a); redc(_MIPP_ qy.b,qy.b);
		SST->len=4*PFS;
		big_to_bytes(_MIPP_ PFS,qx.a,&(SST->val[0]),TRUE);
		big_to_bytes(_MIPP_ PFS,qx.b,&(SST->val[PFS]),TRUE);
		big_to_bytes(_MIPP_ PFS,qy.a,&(SST->val[2*PFS]),TRUE);
		big_to_bytes(_MIPP_ PFS,qy.b,&(SST->val[3*PFS]),TRUE);
    }

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_GET_SERVER_SECRET_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_GET_SERVER_SECRET_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


/* Issue public time permit */

#define MR_MPIN_GET_CLIENT_PERMIT_RESERVE 9

int MPIN_GET_CLIENT_PERMIT(mpin_domain *DOM,int date,octet *S,octet *hash_CID,octet *CTT)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,px,py,r,s,beta;
    epoint *P;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_  MR_MPIN_GET_CLIENT_PERMIT_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_PERMIT_RESERVE)];
    char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_PERMIT_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);		
		beta=mirvar_mem(_MIPP_ mem, num++);  
		
		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		P=epoint_init_mem(_MIPP_ mem1,0);
  		
		hash(NULL,date,hash_CID,NULL,CTT);	/* include date in hash */
		bytes_to_big(_MIPP_ CTT->len,CTT->val,px);
		divide(_MIPP_ px,q,q);

		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

		bytes_to_big(_MIPP_ S->len,S->val,s);

#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ s, P, P);
#else
		G1_mult(_MIPP_ P,s,beta,r,x,P);  
#endif


		epoint_get(_MIPP_ P,px,py);

		CTT->len=2*PFS+1;	CTT->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(CTT->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(CTT->val[PFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_GET_CLIENT_PERMIT_RESERVE);
	ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_PERMIT_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


#define MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE 9
/* kmc 20/5/2014
   changed *CID to *hash_val
*/
int MPIN_GET_CLIENT_MULTIPLE(mpin_domain *DOM,octet *S,octet *hash_val,octet *CST)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,px,py,r,s,beta;
    epoint *P;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_  MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE)];
    char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
	x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);		
        beta=mirvar_mem(_MIPP_ mem, num++);  
		
	flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
	mr_mip->TWIST=MR_SEXTIC_D;
	if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
	if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
        bytes_to_big(_MIPP_ PGS,DOM->R,r);
	bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		P=epoint_init_mem(_MIPP_ mem1,0);
  		/* kmc 20/5/2014 
                   commented out this line
		   hash(NULL,-1,CID,NULL,CST);
                   and changed this line 
                   bytes_to_big(_MIPP_ CST->len,CST->val,px);
                   to
                   bytes_to_big(_MIPP_ hash_val->len,hash_val->val,px);
                   as the server will no longer receive the M-Pin ID only 
                   hash of the value.
                */
		bytes_to_big(_MIPP_ hash_val->len,hash_val->val,px);
		divide(_MIPP_ px,q,q);

		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

		bytes_to_big(_MIPP_ S->len,S->val,s);
#ifdef AVOID_PATENT
                ecurve_mult(_MIPP_ s, P, P);
#else
		G1_mult(_MIPP_ P,s,beta,r,x,P);  
#endif

		epoint_get(_MIPP_ P,px,py);

		CST->len=2*PFS+1;	CST->val[0]=4;
		big_to_bytes(_MIPP_ PFS,px,&(CST->val[1]),TRUE);
		big_to_bytes(_MIPP_ PFS,py,&(CST->val[PFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE);
	ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE( MR_MPIN_GET_CLIENT_MULTIPLE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;

    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}


#define MR_MPIN_TEST_PAIRING_RESERVE 36

int MPIN_TEST_PAIRING(mpin_domain *DOM,octet *CID,octet *R)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big x,q,a,b,r,s,ra,rs,beta,px,py,w;
	zzn2 f,qx,qy; 
	zzn12 g;
    ecn2 Q;
	epoint *P;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_MPIN_TEST_PAIRING_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_MPIN_TEST_PAIRING_RESERVE)];    
	char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_TEST_PAIRING_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= MPIN_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

	if (res==0)
    {
		x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		r=mirvar_mem(_MIPP_ mem, num++);
		s=mirvar_mem(_MIPP_ mem, num++);
		w=mirvar_mem(_MIPP_ mem, num++);
		ra=mirvar_mem(_MIPP_ mem, num++);
        rs=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);	
		beta=mirvar_mem(_MIPP_ mem, num++);  

		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn12_alloc(_MIPP_ &g,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ PFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ PFS,DOM->Q,q);
        bytes_to_big(_MIPP_ PFS,DOM->A,a);
        bytes_to_big(_MIPP_ PFS,DOM->B,b);
		bytes_to_big(_MIPP_ PGS,DOM->R,r);
		bytes_to_big(_MIPP_ PFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ PFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ PFS,DOM->Fb,f.b);
		        
        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
 		P=epoint_init_mem(_MIPP_ mem1,0);

		hash(NULL,-1,CID,NULL,R);	
		bytes_to_big(_MIPP_ R->len,R->val,px);
		divide(_MIPP_ px,q,q);
		while (!epoint_set(_MIPP_ px,px,0,P))
		{
			if (mr_mip->ERNUM!=0) break;
			incr(_MIPP_ px,1,px);
		}

        bytes_to_big(_MIPP_ PFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ PFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ PFS,DOM->Qyb,qy.b);
      
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		rate_miller(_MIPP_ &Q,P,x,&f,&g);
        rate_fexp(_MIPP_ x,&f,&g);
		trace(_MIPP_ &g,&(g.a));

		redc(_MIPP_ g.a.a.a,b); printf("trace pairing= "); otnum(_MIPP_ b,stdout);
		redc(_MIPP_ g.a.a.b,b); printf("trace pairing= "); otnum(_MIPP_ b,stdout);
		redc(_MIPP_ g.a.b.a,b); printf("trace pairing= "); otnum(_MIPP_ b,stdout);
		redc(_MIPP_ g.a.b.b,b); printf("trace pairing= "); otnum(_MIPP_ b,stdout);

	}
#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_MPIN_TEST_PAIRING_RESERVE);
	ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_MPIN_TEST_PAIRING_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return MPIN_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return MPIN_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

