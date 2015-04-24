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

/* certivox.c - basic BN curve stuff, EC curve support, and AES-GCM */
/* M. Scott November 2012 */

#include "certivox.h"

DLL_EXPORT mr_unsign32 today(void)
{ // return time in slots since epoch
	mr_unsign32 ti=(mr_unsign32)time(NULL);
	return (long)(ti/(60*TIME_SLOT_MINUTES));
}

DLL_EXPORT mr_unsign32 cv_getdate(char *d)
{
	return ((unsigned char)d[0]<<24)|((unsigned char)d[1]<<16)|((unsigned char)d[2]<<8)|((unsigned char)d[3]);
}

DLL_EXPORT void thedate(mr_unsign32 slot,char *date)
{
	struct tm *t;
	time_t tt;
	slot*=(60*TIME_SLOT_MINUTES);
	tt=(time_t)slot;
	t=gmtime(&tt);
	if (TIME_SLOT_MINUTES>=1440) strftime(date,20,"%d-%m-%Y",t);
	else strftime(date,20,"%d-%m-%Y %H:%M",t);
}

/* todays date. Format is 24 bits: year (12 bits) | day_of_month (5 bits) | month (4 bits) | weekday (3 bits) */
/*mr_unsign32 today(void)
{
    time_t now = time(0);
    struct tm date;
	int d;

    date = *localtime(&now);
	d=(((((((1900+date.tm_year)<<5)+date.tm_mday)<<4)+date.tm_mon)<<3)+date.tm_wday);

	return d;
}
// Get date from 4 bytes 

mr_unsign32 cv_getdate(char *d)
{
	return ((unsigned char)d[0]<<24)|((unsigned char)d[1]<<16)|((unsigned char)d[2]<<8)|((unsigned char)d[3]);
}

// Extract date as day/month/year 

void thedate(mr_unsign32 date,int *d,int *m,int *y)
{
	*d=(date>>7)&0x1f;
	*m=1+(date>>3)&0xf;
	*y=(date>>12);
}
*/
/* a couple of base64 utilities. Convert 32-bit integer to/from 6 char base64. Leading 0's included. */

DLL_EXPORT void int_to_base64(int x, char *w)
{ /* w must be of length at least 7 */
	int i,c;
	for (i=5;i>=0;i--)
	{
		c=x&0x3F;
		x>>=6;
		if (c==62) {w[i]='+'; continue; }
		if (c==63) {w[i]='/'; continue; }
		if (c<26) {w[i]='A'+c; continue; }
		if (c<52) {w[i]='a'+c-26; continue; }
		w[i]='0'+c-52;
	}
	w[6]=0; /* null terminate it */

}

/* might end in == */

DLL_EXPORT int base64_to_int(char *w)
{
	int i,x,c,d,len,pads;
	x=0; pads=0;
	for (i=0;i<strlen(w);i++) 
	{
		if (w[i]=='=') pads++;
	}
	len=strlen(w)-pads;

	for (i=0;i<len;i++)
	{
		c=w[i];
		if (w[i]=='+') d=62;
		if (w[i]=='/') d=63;
		if (w[i]>='A' && w[i]<='Z') d=w[i]-'A';
		if (w[i]>='a' && w[i]<='z') d=26+w[i]-'a';
		if (w[i]>='0' && w[i]<='9') d=52+w[i]-'0';
		if (i<len-1)
		{
			x<<=6;
			x+=d;
		}
	}
	if (pads==2) {x<<=2; x+=(d>>4);}
	if (pads==1) {x<<=4; x+=(d>>2);}
	if (pads==0) {x<<=6; x+=d;}
	return x;
}

DLL_EXPORT void hash(octet *p,int n,octet *x,octet *y,octet *w)
{
    int i,hlen,c[4];
    HASHFUNC sha;
    char hh[HASH_BYTES];

    hlen=HASH_BYTES;

    SHS_INIT(&sha);
    if (p!=NULL)
        for (i=0;i<p->len;i++) SHS_PROCESS(&sha,p->val[i]);
	if (n>0)
    {
        c[0]=(n>>24)&0xff;
        c[1]=(n>>16)&0xff;
        c[2]=(n>>8)&0xff;
        c[3]=(n)&0xff;
		for (i=0;i<4;i++) SHS_PROCESS(&sha,c[i]);
    }
    if (x!=NULL)
        for (i=0;i<x->len;i++) SHS_PROCESS(&sha,x->val[i]);    
    if (y!=NULL)
        for (i=0;i<y->len;i++) SHS_PROCESS(&sha,y->val[i]);    
	
       
    SHS_HASH(&sha,hh);
   
    OCTET_EMPTY(w);
    OCTET_JOIN_BYTES(hh,hlen,w);
    for (i=0;i<hlen;i++) hh[i]=0;
}


DLL_EXPORT void HashToIntegerRange(_MIPD_ big n,big q,big v)
{
	int i,k,hlen,len;
	char h[HASH_BYTES],t[HASH_BYTES],a[HASH_BYTES];
	octet H={0,sizeof(h),h};
	octet T={0,sizeof(t),t};
	octet A={0,sizeof(a),a};

	hlen=8*HASH_BYTES;

	expb2(_MIPP_ hlen,mr_mip->w1);

	A.len=big_to_bytes(_MIPP_ HASH_BYTES,n,A.val,TRUE);
	k=logb2(_MIPP_ q);
	len=k/hlen; if (k%hlen!=0) len++;
	zero(v);

	H.len=HASH_BYTES;
	for (i=0;i<HASH_BYTES;i++) H.val[i]=0;

	for (i=0;i<len;i++)
	{
		hash(&H,-1,NULL,NULL,&H);
		hash(&H,-1,&A,NULL,&T);
		bytes_to_big(_MIPP_ T.len,T.val,mr_mip->w2);
		multiply(_MIPP_ v,mr_mip->w1,v);
		add(_MIPP_ v,mr_mip->w2,v);
	}
	divide(_MIPP_ v,q,q);
}

DLL_EXPORT void zzn2_alloc(_MIPD_ zzn2 *x,char *mem,int *i)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	x->a=mirvar_mem(_MIPP_ mem, (*i)++);
	x->b=mirvar_mem(_MIPP_ mem, (*i)++);

}

DLL_EXPORT void zzn4_alloc(_MIPD_ zzn4 *x,char *mem,int *i)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	zzn2_alloc(_MIPP_ &(x->a),mem,i);
	zzn2_alloc(_MIPP_ &(x->b),mem,i);
	x->unitary=FALSE;
}

DLL_EXPORT void zzn12_alloc(_MIPD_ zzn12 *x,char *mem,int *i)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	zzn4_alloc(_MIPP_ &(x->a),mem,i);
	zzn4_alloc(_MIPP_ &(x->b),mem,i);
	zzn4_alloc(_MIPP_ &(x->c),mem,i);
	x->unitary=FALSE;
	x->miller=FALSE;
}

DLL_EXPORT void ecn2_alloc(_MIPD_ ecn2 *P,char *mem,int *i)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	zzn2_alloc(_MIPP_ &(P->x),mem,i);
	zzn2_alloc(_MIPP_ &(P->y),mem,i);
	zzn2_alloc(_MIPP_ &(P->z),mem,i);
	P->marker=MR_EPOINT_INFINITY;
}
/*
void init_zzn12_from_rom(zzn12 *g,const mr_small *rom,int promptr)
{
	int words=MR_ROUNDUP(FS*8,MIRACL);
	int ptr=12*words*promptr;
	init_big_from_rom((*g).a.a.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).a.a.b,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).a.b.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).a.b.b,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).b.a.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).b.a.b,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).b.b.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).b.b.b,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).c.a.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).c.a.b,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).c.b.a,words,rom,words*12*256,&ptr);
	init_big_from_rom((*g).c.b.b,words,rom,words*12*256,&ptr);
}

*/

/* Frobenius x^p. Assumes p=1 mod 6 */

#define MR_ZZN12_POWQ_RESERVE 4

DLL_EXPORT void zzn12_powq(_MIPD_ zzn2 *w,zzn12 *x)
{
	zzn2 ww,www;
	int num=0;
	BOOL ku,km;

#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_POWQ_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_POWQ_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_POWQ_RESERVE));
#endif
	zzn2_alloc(_MIPP_ &ww,mem,&num);
	zzn2_alloc(_MIPP_ &www,mem,&num);

	zzn2_sqr(_MIPP_ w,&ww);
	zzn2_mul(_MIPP_ w,&ww,&www);

    ku=x->unitary;
	km=x->miller;

	zzn4_powq(_MIPP_ &www,&(x->a));
	zzn4_powq(_MIPP_ &www,&(x->b));
	zzn4_powq(_MIPP_ &www,&(x->c));

	zzn4_smul(_MIPP_ &(x->b),w,&(x->b));
	zzn4_smul(_MIPP_ &(x->c),&ww,&(x->c));

	x->unitary=ku;
	x->miller=km;
#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_POWQ_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_POWQ_RESERVE));
#endif
}

DLL_EXPORT BOOL zzn12_iszero(zzn12 *x)
{
    if (zzn4_iszero(&(x->a)) && zzn4_iszero(&(x->b)) && zzn4_iszero(&(x->c))) return TRUE;
    return FALSE;
}

DLL_EXPORT BOOL zzn12_isunity(_MIPD_ zzn12 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (zzn4_isunity(_MIPP_ &(x->a)) && zzn4_iszero(&(x->b)) && zzn4_iszero(&(x->c))) return TRUE;
    return FALSE;
}


DLL_EXPORT void zzn12_copy(zzn12 *x,zzn12 *w)
{
    if (x==w) return;
    zzn4_copy(&(x->a),&(w->a));
    zzn4_copy(&(x->b),&(w->b));
	zzn4_copy(&(x->c),&(w->c));
	w->unitary=x->unitary;
	w->miller=x->miller;
}

DLL_EXPORT void zzn12_from_int(_MIPD_ int i,zzn12 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	zzn4_from_int(_MIPP_ i,&(x->a));
	zzn4_zero(&(x->b));
	zzn4_zero(&(x->c));
	x->unitary=FALSE;
	x->miller=FALSE;
	if (i==1) x->unitary=TRUE;
}

DLL_EXPORT void zzn12_from_zzn4s(zzn4 *x,zzn4 *y,zzn4 *z,zzn12 *w)
{
    zzn4_copy(x,&(w->a));
    zzn4_copy(y,&(w->b));
    zzn4_copy(z,&(w->c));
	w->unitary=FALSE;
	w->miller=FALSE;
}

DLL_EXPORT void zzn12_conj(_MIPD_ zzn12 *x,zzn12 *w)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	zzn12_copy(x,w);
	zzn4_conj(_MIPP_ &(w->a),&(w->a));
	zzn4_conj(_MIPP_ &(w->b),&(w->b));
	zzn4_conj(_MIPP_ &(w->c),&(w->c));
   
    zzn4_negate(_MIPP_ &(w->b),&(w->b));
}

DLL_EXPORT BOOL zzn12_compare(zzn12 *x,zzn12 *y)
{
    if (zzn4_compare(&(x->a),&(y->a)) && zzn4_compare(&(x->b),&(y->b)) && zzn4_compare(&(x->c),&(y->c))) return TRUE;
    return FALSE;
}

#define MR_ZZN12_SQR_RESERVE 16

DLL_EXPORT void zzn12_sqr(_MIPD_ zzn12 *x,zzn12 *w)
{
	int num=0;
	zzn4 A,B,C,D;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_SQR_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_SQR_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_SQR_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &A,mem,&num);
	zzn4_alloc(_MIPP_ &B,mem,&num);
	zzn4_alloc(_MIPP_ &C,mem,&num);
	zzn4_alloc(_MIPP_ &D,mem,&num);

    zzn12_copy(x,w);
	if (x->unitary)
	{ /* Granger & Scott PKC 2010 - only 3 squarings! */

/*		A=a; a*=a; D=a; a+=a; a+=D; A.conj(); A+=A; a-=A; */
		zzn4_copy(&(w->a),&A);
		zzn4_sqr(_MIPP_ &(w->a),&(w->a));
		zzn4_copy(&(w->a),&D);
		zzn4_add(_MIPP_ &(w->a),&(w->a),&(w->a));
		zzn4_add(_MIPP_ &(w->a),&D,&(w->a));
		zzn4_conj(_MIPP_ &A,&A);
		zzn4_add(_MIPP_ &A,&A,&A);
		zzn4_sub(_MIPP_ &(w->a),&A,&(w->a));

/* 		B=c; B*=B; B=tx(B); D=B; B+=B; B+=D; */
		zzn4_copy(&(w->c),&B);
		zzn4_sqr(_MIPP_ &B,&B);
		zzn4_tx(_MIPP_ &B);
		zzn4_copy(&B,&D);
		zzn4_add(_MIPP_ &B,&B,&B);
		zzn4_add(_MIPP_ &B,&D,&B);

/* 		C=b; C*=C;          D=C; C+=C; C+=D; */
		zzn4_copy(&(w->b),&C);
		zzn4_sqr(_MIPP_ &C,&C);
		zzn4_copy(&C,&D);
		zzn4_add(_MIPP_ &C,&C,&C);
		zzn4_add(_MIPP_ &C,&D,&C);

/* 		b.conj(); b+=b; c.conj(); c+=c; c=-c;
		b+=B; c+=C; */
		zzn4_conj(_MIPP_ &(w->b),&(w->b));
		zzn4_add(_MIPP_ &(w->b),&(w->b),&(w->b));
		zzn4_conj(_MIPP_ &(w->c),&(w->c));
		zzn4_add(_MIPP_ &(w->c),&(w->c),&(w->c));
		zzn4_negate(_MIPP_ &(w->c),&(w->c));
		zzn4_add(_MIPP_ &(w->b),&B,&(w->b));
		zzn4_add(_MIPP_ &(w->c),&C,&(w->c));

	}
	else
	{
	if (!x->miller)
	{
/*			A=a; A*=A;	B=b*c; B+=B; */
		zzn4_copy(&(w->a),&A);
		zzn4_sqr(_MIPP_ &A,&A);
		zzn4_mul(_MIPP_ &(w->b),&(w->c),&B);
		zzn4_add(_MIPP_ &B,&B,&B);	

/*			C=c; C*=C;	D=a*b; D+=D; c+=(a+b); c*=c; */
		zzn4_copy(&(w->c),&C);
		zzn4_sqr(_MIPP_ &C,&C);
		zzn4_mul(_MIPP_ &(w->a),&(w->b),&D);
		zzn4_add(_MIPP_ &D,&D,&D);
		zzn4_add(_MIPP_ &(w->c),&(w->a),&(w->c));
		zzn4_add(_MIPP_ &(w->c),&(w->b),&(w->c));
		zzn4_sqr(_MIPP_ &(w->c),&(w->c));

/*			a=A+tx(B);	b=D+tx(C);	c-=(A+B+C+D); */
		zzn4_sub(_MIPP_ &(w->c),&A,&(w->c));
		zzn4_sub(_MIPP_ &(w->c),&B,&(w->c));
		zzn4_sub(_MIPP_ &(w->c),&C,&(w->c));
		zzn4_sub(_MIPP_ &(w->c),&D,&(w->c));
		zzn4_tx(_MIPP_ &B); zzn4_tx(_MIPP_ &C);
		zzn4_add(_MIPP_ &A,&B,&(w->a));
		zzn4_add(_MIPP_ &D,&C,&(w->b));

	}
	else
	{
/* 			A=a; A*=A;  C=c; C*=b; C+=C; D=c; D*=D; c+=a;  */ 
		zzn4_copy(&(w->a),&A);
		zzn4_sqr(_MIPP_ &A,&A);
		zzn4_copy(&(w->c),&C);
		zzn4_mul(_MIPP_ &C,&(w->b),&C);
		zzn4_add(_MIPP_ &C,&C,&C);
		zzn4_copy(&(w->c),&D);
		zzn4_sqr(_MIPP_ &D,&D);
		zzn4_add(_MIPP_ &(w->c),&(w->a), &(w->c));

/* 			B=b; B+=c; B*=B; c-=b; c*=c; */
		zzn4_copy(&(w->b),&B);
		zzn4_add(_MIPP_ &B,&(w->c),&B);
		zzn4_sqr(_MIPP_ &B,&B);
		zzn4_sub(_MIPP_ &(w->c),&(w->b),&(w->c));
		zzn4_sqr(_MIPP_ &(w->c),&(w->c));

/* 			C+=C; A+=A; D+=D;  */
		zzn4_add(_MIPP_ &C,&C,&C);
		zzn4_add(_MIPP_ &A,&A,&A);
		zzn4_add(_MIPP_ &D,&D,&D);

/* 			a=A+tx(C); b=B-c-C+tx(D); c+=B-A-D;   */
		zzn4_sub(_MIPP_ &B,&(w->c),&(w->b));
		zzn4_sub(_MIPP_ &(w->b),&C,&(w->b));
		zzn4_tx(_MIPP_ &C);
		zzn4_add(_MIPP_ &A,&C,&(w->a));
		zzn4_sub(_MIPP_ &(w->c),&D,&(w->c));
		zzn4_tx(_MIPP_ &D);
		zzn4_add(_MIPP_ &(w->b),&D,&(w->b));
		zzn4_sub(_MIPP_ &(w->c),&A,&(w->c));
		zzn4_add(_MIPP_ &(w->c),&B,&(w->c));
	}
	}
#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_SQR_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_SQR_RESERVE));
#endif
}


#define MR_ZZN12_MUL_RESERVE 24

DLL_EXPORT void zzn12_mul(_MIPD_ zzn12 *x,zzn12 *y,zzn12 *w)
{
	int num=0;
    zzn4 Z0,Z1,Z2,Z3,T0,T1;
	BOOL zero_c,zero_b;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_MUL_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_MUL_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MUL_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &Z0,mem,&num);
	zzn4_alloc(_MIPP_ &Z1,mem,&num);
	zzn4_alloc(_MIPP_ &Z2,mem,&num);
	zzn4_alloc(_MIPP_ &Z3,mem,&num);
	zzn4_alloc(_MIPP_ &T0,mem,&num);
	zzn4_alloc(_MIPP_ &T1,mem,&num);

	zero_c=zzn4_iszero(&(y->c));
	zero_b=zzn4_iszero(&(y->b));

/*     Z0=a*x.a; if (!zero_b) Z2=b*x.b;  */

	zzn4_mul(_MIPP_ &(x->a),&(y->a),&Z0);
	if (!zero_b) zzn4_mul(_MIPP_ &(x->b),&(y->b),&Z2);

/*     T0=a+b; T1=x.a+x.b; Z1=T0*T1; Z1-=Z0; if (!zero_b) Z1-=Z2; T0=b+c; T1=x.b+x.c; Z3=T0*T1; if (!zero_b) Z3-=Z2; */
	zzn4_add(_MIPP_ &(x->a),&(x->b),&T0);
	zzn4_add(_MIPP_ &(y->a),&(y->b),&T1);
	zzn4_mul(_MIPP_ &T0,&T1,&Z1);
	zzn4_sub(_MIPP_ &Z1,&Z0,&Z1);
	if (!zero_b) zzn4_sub(_MIPP_ &Z1,&Z2,&Z1);
	zzn4_add(_MIPP_ &(x->b),&(x->c),&T0);
	zzn4_add(_MIPP_ &(y->b),&(y->c),&T1);
	zzn4_mul(_MIPP_ &T0,&T1,&Z3);
	if (!zero_b) zzn4_sub(_MIPP_ &Z3,&Z2,&Z3);

/*     T0=a+c; T1=x.a+x.c; T0*=T1; if (!zero_b) Z2+=T0; else Z2=T0; Z2-=Z0; */
	zzn4_add(_MIPP_ &(x->a),&(x->c),&T0);
	zzn4_add(_MIPP_ &(y->a),&(y->c),&T1);
	zzn4_mul(_MIPP_ &T0,&T1,&T0);
	if (!zero_b) zzn4_add(_MIPP_ &Z2,&T0,&Z2);
	else         zzn4_copy(&T0,&Z2);
	zzn4_sub(_MIPP_ &Z2,&Z0,&Z2);

/* 	b=Z1; if (!zero_c) 	{ Z4=c*x.c; Z2-=Z4; Z3-=Z4; b+=tx(Z4);} a=Z0+tx(Z3); c=Z2; */
    zzn4_copy(&Z1,&(w->b));
	if (!zero_c)
	{
		zzn4_mul(_MIPP_ &(x->c),&(y->c),&T0);
		zzn4_sub(_MIPP_ &Z2,&T0,&Z2);
		zzn4_sub(_MIPP_ &Z3,&T0,&Z3);
		zzn4_tx(_MIPP_ &T0);
		zzn4_add(_MIPP_ &(w->b),&T0,&(w->b));
	}
	zzn4_tx(_MIPP_ &Z3);
	zzn4_add(_MIPP_ &Z0,&Z3,&(w->a));
	zzn4_copy(&Z2,&(w->c));

	if (x->unitary && y->unitary) w->unitary=TRUE;
	else w->unitary=FALSE;

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_MUL_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MUL_RESERVE));
#endif
}

#define MR_ZZN12_INV_RESERVE 16

DLL_EXPORT void zzn12_inv(_MIPD_ zzn12 *w)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	int num=0;
	zzn4 f0,f1,f2,f3;
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_INV_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_INV_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_INV_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &f0,mem,&num);
	zzn4_alloc(_MIPP_ &f1,mem,&num);
	zzn4_alloc(_MIPP_ &f2,mem,&num);
	zzn4_alloc(_MIPP_ &f3,mem,&num);

	if (w->unitary)
	{
		zzn12_conj(_MIPP_ w,w);
	}
	else
	{
		zzn4_sqr(_MIPP_ &(w->a),&f0);
		zzn4_mul(_MIPP_ &(w->b),&(w->c),&f1);
		zzn4_tx(_MIPP_ &f1);
		zzn4_sub(_MIPP_ &f0,&f1,&f0);

		zzn4_sqr(_MIPP_ &(w->c),&f1);
		zzn4_tx(_MIPP_ &f1);
		zzn4_mul(_MIPP_ &(w->a),&(w->b),&f2);
		zzn4_sub(_MIPP_ &f1,&f2,&f1);

		zzn4_sqr(_MIPP_ &(w->b),&f2);
		zzn4_mul(_MIPP_ &(w->a),&(w->c),&f3);
		zzn4_sub(_MIPP_ &f2,&f3,&f2);

		zzn4_mul(_MIPP_ &(w->b),&f2,&f3);
		zzn4_tx(_MIPP_ &f3);
		zzn4_mul(_MIPP_ &(w->a),&f0,&(w->a));
		zzn4_add(_MIPP_ &f3,&(w->a),&f3);
		zzn4_mul(_MIPP_ &(w->c),&f1,&(w->c));
		zzn4_tx(_MIPP_ &(w->c));
		zzn4_add(_MIPP_ &f3,&(w->c),&f3);

		zzn4_inv(_MIPP_ &f3);

		zzn4_mul(_MIPP_ &f0,&f3,&(w->a));
		zzn4_mul(_MIPP_ &f1,&f3,&(w->b));
		zzn4_mul(_MIPP_ &f2,&f3,&(w->c));

	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_INV_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_INV_RESERVE));
#endif
}

/* very simple powering - k is always sparse, but could be negative - w=x^k
   NOTE x and w MUST be distinct */

DLL_EXPORT void zzn12_pow(_MIPD_ zzn12 *x,big k,zzn12 *w)
{
	int i,nb,num=0;
	BOOL invert_it=FALSE;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	big e=mr_mip->w15;

	if (size(k)<0)
	{
		invert_it=TRUE;
		negify(k,e);
	}
	else copy(k,e);

	if (size(e)==0)
		zzn12_from_int(_MIPP_ 1, w);
	else
	{
		nb=logb2(_MIPP_ e);

		zzn12_copy(x,w);
		if (nb>1) for (i=nb-2;i>=0;i--)
		{
			zzn12_sqr(_MIPP_ w,w);
			if (mr_testbit(_MIPP_ e,i)) zzn12_mul(_MIPP_ w,x,w);
		}
		if (invert_it) zzn12_inv(_MIPP_ w);
	}
}

DLL_EXPORT void trace(_MIPD_ zzn12 *x,zzn4 *r)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif

	zzn4_imul(_MIPP_ &(x->a),3,r);
}

#define MR_XTR_A_RESERVE 8

DLL_EXPORT void xtr_A(_MIPD_ zzn4 *w,zzn4 *x,zzn4 *y,zzn4 *z,zzn4 *r)
{
	int num=0;
	zzn4 t1,t2;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_  MR_XTR_A_RESERVE);
#else
    char mem[MR_BIG_RESERVE( MR_XTR_A_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_A_RESERVE));
#endif

	zzn4_alloc(_MIPP_ &t1,mem,&num);
	zzn4_alloc(_MIPP_ &t2,mem,&num);
    zzn4_copy(x,r);

	zzn4_sub(_MIPP_ w,y,&t1);
	zzn4_smul(_MIPP_ &t1,&(r->a),&t1);

	zzn4_add(_MIPP_ w,y,&t2);

	zzn4_smul(_MIPP_ &t2,&(r->b),&t2);

	zzn4_tx(_MIPP_ &t2);

	zzn4_add(_MIPP_ &t1,&t2,r);
	zzn4_add(_MIPP_ r,z,r);

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_XTR_A_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_XTR_A_RESERVE));
#endif
}

#define MR_XTR_D_RESERVE 4

DLL_EXPORT void xtr_D(_MIPD_ zzn4 *x,zzn4 *r)
{
	int num=0;
	zzn4 w;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_  MR_XTR_D_RESERVE);
#else
    char mem[MR_BIG_RESERVE( MR_XTR_D_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_D_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &w,mem,&num);
    zzn4_copy(x,r);
	zzn4_conj(_MIPP_ r,&w);
	zzn4_add(_MIPP_ &w,&w,&w);
	zzn4_sqr(_MIPP_ r,r);
	zzn4_sub(_MIPP_ r,&w,r);

#ifndef MR_STATIC
    memkill(_MIPP_ mem,  MR_XTR_D_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_D_RESERVE));
#endif
}

#define MR_XTR_POW_RESERVE 17

DLL_EXPORT void xtr_pow(_MIPD_ zzn4 *x,big n,zzn4 *r)
{
	int i,par,nb,num=0;
	big v;
	zzn4 t,a,b,c;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_  MR_XTR_POW_RESERVE);
#else
    char mem[MR_BIG_RESERVE( MR_XTR_POW_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_POW_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &t,mem,&num);
	zzn4_alloc(_MIPP_ &a,mem,&num);
	zzn4_alloc(_MIPP_ &b,mem,&num);
	zzn4_alloc(_MIPP_ &c,mem,&num);
	v=mirvar_mem(_MIPP_  mem, num++);
    
	zzn4_from_int(_MIPP_ 3,&a);
	zzn4_copy(x,&b);
	xtr_D(_MIPP_ x,&c);

	par=subdiv(_MIPP_ n,2,v);
	if (par==0)	decr(_MIPP_ v,1,v);

	nb=logb2(_MIPP_ v);

    for (i=nb-1;i>=0;i--)
    {
		if (!mr_testbit(_MIPP_ v,i))
		{
			zzn4_copy(&b,&t);
			zzn4_conj(_MIPP_ x,x);
			zzn4_conj(_MIPP_ &c,&c);
			xtr_A(_MIPP_ &a,&b,x,&c,&b);
			zzn4_conj(_MIPP_ x,x);
			xtr_D(_MIPP_ &t,&c);
			xtr_D(_MIPP_ &a,&a);
		}
		else
		{
			zzn4_conj(_MIPP_ &a,&t);
			xtr_D(_MIPP_ &b,&a);
			xtr_A(_MIPP_ &c,&b,x,&t,&b);
			xtr_D(_MIPP_ &c,&c);
		}
	}

	if (par==0) zzn4_copy(&c,r);
	else zzn4_copy(&b,r);

#ifndef MR_STATIC
    memkill(_MIPP_ mem,  MR_XTR_POW_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_POW_RESERVE));
#endif
}

/* Stam's thesis */

#define MR_XTR_POW2_RESERVE 23

DLL_EXPORT void xtr_pow2(_MIPD_ zzn4 *ck,zzn4 *cl,zzn4 *ckml,zzn4 *ckm2l,big a,big b,zzn4 *r)
{
	int i,f2,num=0;
	big d,e,w;
	zzn4 t,cu,cv,cumv,cum2v;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_  MR_XTR_POW2_RESERVE);
#else
    char mem[MR_BIG_RESERVE( MR_XTR_POW2_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_POW2_RESERVE));
#endif

	d=mirvar_mem(_MIPP_ mem,num++);
	e=mirvar_mem(_MIPP_ mem,num++);
	w=mirvar_mem(_MIPP_ mem,num++);
	zzn4_alloc(_MIPP_ &t,mem,&num);
	zzn4_alloc(_MIPP_ &cu,mem,&num);
	zzn4_alloc(_MIPP_ &cv,mem,&num);
	zzn4_alloc(_MIPP_ &cumv,mem,&num);
	zzn4_alloc(_MIPP_ &cum2v,mem,&num);

	copy(a,e);
	copy(b,d);
	zzn4_copy(ck,&cu);
	zzn4_copy(cl,&cv);
	zzn4_copy(ckml,&cumv);
	zzn4_copy(ckm2l,&cum2v);

	f2=0;

	while (remain(_MIPP_ d,2)==0 && remain(_MIPP_ e,2)==0)
	{
		subdiv(_MIPP_ d,2,d);
		subdiv(_MIPP_ e,2,e);
		f2++;
	}

	while (mr_compare(e,d)!=0)
	{
		if (mr_compare(d,e)>0)
		{
			premult(_MIPP_ e,4,w);
			if (mr_compare(d,w)<=0)
			{
				copy(d,w);
				copy(e,d);
				subtract(_MIPP_ w,e,e);
				xtr_A(_MIPP_ &cu,&cv,&cumv,&cum2v,&t);
				zzn4_conj(_MIPP_ &cumv,&cum2v);
				zzn4_copy(&cv,&cumv);
				zzn4_copy(&cu,&cv);
				zzn4_copy(&t,&cu);
			}
			else if (remain(_MIPP_ d,2)==0)
			{
				subdiv(_MIPP_ d,2,d);
				zzn4_conj(_MIPP_ &cum2v,r);
				xtr_A(_MIPP_ &cu,&cumv,&cv,r,&t);
				xtr_D(_MIPP_ &cumv,&cum2v);
				zzn4_copy(&t,&cumv);
				xtr_D(_MIPP_ &cu,&cu);
			}
			else if (remain(_MIPP_ e,2)==1)
			{
				subtract(_MIPP_ d,e,d);
				subdiv(_MIPP_ d,2,d);
				xtr_A(_MIPP_ &cu,&cv,&cumv,&cum2v,&t);
				xtr_D(_MIPP_ &cu,&cu);
				xtr_D(_MIPP_ &cv,&cum2v);
				zzn4_conj(_MIPP_ &cum2v,&cum2v);
				zzn4_copy(&t,&cv);
			}
			else
			{
				copy(d,w);
				subdiv(_MIPP_ e,2,d);
				copy(w,e);
				xtr_D(_MIPP_ &cumv,&t);
				zzn4_conj(_MIPP_ &cum2v,&cumv);
				zzn4_conj(_MIPP_ &t,&cum2v);
				xtr_D(_MIPP_ &cv,&t);
				zzn4_copy(&cu,&cv);
				zzn4_copy(&t,&cu);
			}
		}
		if (mr_compare(d,e)<0)
		{
			premult(_MIPP_ d,4,w);
			if (mr_compare(e,w)<=0)
			{
				subtract(_MIPP_ e,d,e);
				xtr_A(_MIPP_ &cu,&cv,&cumv,&cum2v,&t);
				zzn4_copy(&cumv,&cum2v);
				zzn4_copy(&cu,&cumv);
				zzn4_copy(&t,&cu);

			}
			else if (remain(_MIPP_ e,2)==0)
			{
				copy(d,w);
				subdiv(_MIPP_ e,2,d);
				copy(w,e);
				xtr_D(_MIPP_ &cumv,&t);
				zzn4_conj(_MIPP_ &cum2v,&cumv);
				zzn4_conj(_MIPP_ &t,&cum2v);
				xtr_D(_MIPP_ &cv,&t);
				zzn4_copy(&cu,&cv);
				zzn4_copy(&t,&cu);

			}
			else if (remain(_MIPP_ d,2)==1)
			{
				copy(e,w);
				copy(d,e);
				subtract(_MIPP_ w,d,w);
				subdiv(_MIPP_ w,2,d);
				xtr_A(_MIPP_ &cu,&cv,&cumv,&cum2v,&t);
				zzn4_conj(_MIPP_ &cumv,&cumv);
				xtr_D(_MIPP_ &cu,&cum2v);
				zzn4_conj(_MIPP_ &cum2v,&cum2v);
				xtr_D(_MIPP_ &cv,&cu);
				zzn4_copy(&t,&cv);
			}
			else
			{
				subdiv(_MIPP_ d,2,d);
				zzn4_conj(_MIPP_ &cum2v,r);
				xtr_A(_MIPP_ &cu,&cumv,&cv,r,&t);
				xtr_D(_MIPP_ &cumv,&cum2v);
				zzn4_copy(&t,&cumv);
				xtr_D(_MIPP_ &cu,&cu);
			}
		}
	}

	xtr_A(_MIPP_ &cu,&cv,&cumv,&cum2v,r);
	for (i=0;i<f2;i++) xtr_D(_MIPP_ r,r);

	xtr_pow(_MIPP_ r,d,r);  /* could be taken out of here to ease pressure on stack */

#ifndef MR_STATIC
    memkill(_MIPP_ mem,  MR_XTR_POW2_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE( MR_XTR_POW2_RESERVE));
#endif
}

/* apply endomorphism (x,y) = (Beta*x,y) where Beta is cube root of unity */

DLL_EXPORT void endomorph(_MIPD_ big beta,epoint *P)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	nres_modmult(_MIPP_ P->X,beta,P->X);
}

/* Fast multiplication of A by p (for Trace-Zero group members only) */

#define MR_ZZN12_QPF_RESERVE 4

DLL_EXPORT void q_power_frobenius(_MIPD_ zzn2 *f,ecn2 *A)
{
	int num=0;
	zzn2 w,r;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_QPF_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_QPF_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_QPF_RESERVE));
#endif

	if (A->marker!=MR_EPOINT_INFINITY)
	{
		zzn2_alloc(_MIPP_ &w,mem,&num);
		zzn2_alloc(_MIPP_ &r,mem,&num);

		zzn2_copy(f,&r);
		if (mr_mip->TWIST==MR_SEXTIC_M) zzn2_inv(_MIPP_  &r);  /* could be precalculated */

		zzn2_sqr(_MIPP_ &r,&w);
		zzn2_conj(_MIPP_ &(A->x),&(A->x));
		zzn2_conj(_MIPP_ &(A->y),&(A->y));
		zzn2_conj(_MIPP_ &(A->z),&(A->z));

		zzn2_mul(_MIPP_ &(A->x),&w,&(A->x));
		zzn2_mul(_MIPP_ &(A->y),&w,&(A->y));
		zzn2_mul(_MIPP_ &(A->y),&r,&(A->y));
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_QPF_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_QPF_RESERVE));
#endif
}


#define MR_ZZN12_LINE_RESERVE 20

DLL_EXPORT void line(_MIPD_ ecn2 *A,ecn2 *C,ecn2 *B,zzn2 *lam,zzn2 *extra,BOOL Doubling,big Qx,big Qy,zzn12 *w)
{
	int num=0;
	zzn4 nn,dd,cc;
	zzn2 t1,t2,t3,gz;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_LINE_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_LINE_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_LINE_RESERVE));
#endif
	zzn4_alloc(_MIPP_ &nn,mem,&num);
	zzn4_alloc(_MIPP_ &dd,mem,&num);
	zzn4_alloc(_MIPP_ &cc,mem,&num);
	zzn2_alloc(_MIPP_ &t1,mem,&num);
	zzn2_alloc(_MIPP_ &t2,mem,&num);
	zzn2_alloc(_MIPP_ &t3,mem,&num);
	zzn2_alloc(_MIPP_ &gz,mem,&num);

	if (Doubling)
	{
		ecn2_getz(_MIPP_ A,&t1);
		zzn2_sqr(_MIPP_ &t1,&t1);
		ecn2_getz(_MIPP_ C,&gz);
		if (mr_mip->TWIST==MR_SEXTIC_M)
		{
			zzn2_mul(_MIPP_ &gz,&t1,&t2);
			zzn2_from_zzn(Qy,&t3);
			zzn2_txx(_MIPP_ &t3);
			zzn2_mul(_MIPP_ &t3,&t2,&t3);
			zzn2_mul(_MIPP_ lam,&(A->x),&t2);
			zzn2_sub(_MIPP_ &t2,extra,&t2);
			zzn4_from_zzn2s(&t3,&t2,&nn);
			zzn2_mul(_MIPP_ &t1,lam,&t2);
			zzn2_smul(_MIPP_ &t2,Qx,&t2);
			zzn2_negate(_MIPP_ &t2,&t2);
			zzn4_from_zzn2h(&t2,&cc);
			zzn4_zero(&dd);
		}
		if (mr_mip->TWIST==MR_SEXTIC_D)
		{
			zzn2_mul(_MIPP_ &gz,&t1,&t3);
			zzn2_smul(_MIPP_ &t3,Qy,&t3);
			zzn2_mul(_MIPP_ lam,&(A->x),&t2);
			zzn2_sub(_MIPP_ &t2,extra,&t2);
			zzn4_from_zzn2s(&t3,&t2,&nn);
			zzn4_zero(&cc);
			zzn2_mul(_MIPP_ &t1,lam,&t2);
			zzn2_smul(_MIPP_ &t2,Qx,&t2);
			zzn2_negate(_MIPP_ &t2,&t2);
			zzn4_from_zzn2(&t2,&dd);
		}
	}
	else
	{
		ecn2_getz(_MIPP_ C,&gz);
		if (mr_mip->TWIST==MR_SEXTIC_M)
		{
			zzn2_from_zzn(Qy,&t3);
			zzn2_txx(_MIPP_ &t3);
			zzn2_mul(_MIPP_ &t3,&gz,&t3);
			zzn2_mul(_MIPP_ &gz,&(B->y),&t1);
			zzn2_mul(_MIPP_ lam,&(B->x),&t2);
			zzn2_sub(_MIPP_ &t2,&t1,&t2);
			zzn4_from_zzn2s(&t3,&t2,&nn);
			zzn2_smul(_MIPP_ lam,Qx,&t2);
			zzn2_negate(_MIPP_ &t2,&t2);
			zzn4_from_zzn2h(&t2,&cc);
			zzn4_zero(&dd);
		}
		if (mr_mip->TWIST==MR_SEXTIC_D)
		{
			zzn2_smul(_MIPP_ &gz,Qy,&t3);
			zzn2_mul(_MIPP_ &gz,&(B->y),&t1);
			zzn2_mul(_MIPP_ lam,&(B->x),&t2);
			zzn2_sub(_MIPP_ &t2,&t1,&t2);
			zzn4_from_zzn2s(&t3,&t2,&nn);
			zzn2_smul(_MIPP_ lam,Qx,&t2);
			zzn2_negate(_MIPP_ &t2,&t2);
			zzn4_from_zzn2(&t2,&dd);
			zzn4_zero(&cc);
		}
	}
	zzn12_from_zzn4s(&nn,&dd,&cc,w);

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_LINE_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_LINE_RESERVE));
#endif
}

#define MR_ZZN12_G_RESERVE 10

DLL_EXPORT void g(_MIPD_ ecn2* A,ecn2 *B,big Qx,big Qy,zzn12 *w)
{
	int num=0;
	zzn2 lam,extra;
	ecn2 P;
	BOOL Doubling;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_G_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_G_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G_RESERVE));
#endif
	zzn2_alloc(_MIPP_ &lam,mem,&num);
	zzn2_alloc(_MIPP_ &extra,mem,&num);
	ecn2_alloc(_MIPP_ &P,mem,&num);

	ecn2_copy(A,&P);
	Doubling=ecn2_add2(_MIPP_ B,A,&lam,&extra);

	if (ecn2_iszero(A))
	{
		zzn12_from_int(_MIPP_ 1,w);
	}
	else
	{
		line(_MIPP_ &P,A,B,&lam,&extra,Doubling,Qx,Qy,w);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_G_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G_RESERVE));
#endif
}

/* R-ate Double Pairing e(P,Q).e(R,S) */

#define MR_ZZN12_DOUBLE_MILLER_RESERVE 43

DLL_EXPORT BOOL rate_double_miller(_MIPD_ ecn2 *P,epoint *Q,ecn2 *R,epoint *S,big x,zzn2 *f,zzn12 *res)
{
	int i,nb,num=0;
	BOOL first=TRUE;
	ecn2 A,K,B;
	zzn2 AX,AY,BX,BY;
	big n,qx,qy,sx,sy;
	zzn12 t0;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_DOUBLE_MILLER_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_DOUBLE_MILLER_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_DOUBLE_MILLER_RESERVE));
#endif

	n=mirvar_mem(_MIPP_  mem, num++);
	qx=mirvar_mem(_MIPP_  mem, num++);
	qy=mirvar_mem(_MIPP_  mem, num++);
	sx=mirvar_mem(_MIPP_  mem, num++);
	sy=mirvar_mem(_MIPP_  mem, num++);
	zzn2_alloc(_MIPP_ &AX,mem,&num);
	zzn2_alloc(_MIPP_ &AY,mem,&num);
	zzn2_alloc(_MIPP_ &BX,mem,&num);
	zzn2_alloc(_MIPP_ &BY,mem,&num);

	ecn2_alloc(_MIPP_ &A,mem,&num);
	ecn2_alloc(_MIPP_ &K,mem,&num);
	ecn2_alloc(_MIPP_ &B,mem,&num);

	zzn12_alloc(_MIPP_ &t0,mem,&num);

	premult(_MIPP_ x,6,n);
	incr(_MIPP_ n,2,n);
	if (size(x)<0) negify(n,n);

	ecn2_copy(P,&A);
	ecn2_copy(R,&B);
	nb=logb2(_MIPP_ n);
	zzn12_from_int(_MIPP_ 1,res);
	res->miller=TRUE;
	epoint_norm(_MIPP_ Q);
	epoint_norm(_MIPP_ S);
	copy(Q->X,qx);
	copy(Q->Y,qy);
	copy(S->X,sx);
	copy(S->Y,sy);

	for (i=nb-2;i>=0;i--)
	{
		zzn12_sqr(_MIPP_ res,res);
		g(_MIPP_ &A,&A,qx,qy,&t0);
		zzn12_mul(_MIPP_ res,&t0,res);
		g(_MIPP_ &B,&B,sx,sy,&t0);
		zzn12_mul(_MIPP_ res,&t0,res);

		if (mr_testbit(_MIPP_ n,i))
		{
			g(_MIPP_ &A,P,qx,qy,&t0);
			zzn12_mul(_MIPP_ res,&t0,res);
			g(_MIPP_ &B,R,sx,sy,&t0);
			zzn12_mul(_MIPP_ res,&t0,res);
		}
	}

	if (size(x)<0) zzn12_conj(_MIPP_ res,res);

	ecn2_copy(P,&K);
	q_power_frobenius(_MIPP_ f,&K);
	if (size(x)<0) ecn2_negate(_MIPP_ &A,&A); 
	
	g(_MIPP_ &A,&K,qx,qy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);
	
	q_power_frobenius(_MIPP_ f,&K);
	ecn2_negate(_MIPP_ &K,&K);
	g(_MIPP_ &A,&K,qx,qy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);

	ecn2_copy(R,&K);
	q_power_frobenius(_MIPP_ f,&K);
	if (size(x)<0) ecn2_negate(_MIPP_ &B,&B); 

	g(_MIPP_ &B,&K,sx,sy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);
	
	q_power_frobenius(_MIPP_ f,&K);
	ecn2_negate(_MIPP_ &K,&K);
	g(_MIPP_ &B,&K,sx,sy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);

	zzn12_copy(res,&t0);
	zzn12_conj(_MIPP_ res,res);
	zzn12_inv(_MIPP_ &t0);
	
	zzn12_mul(_MIPP_ res,&t0,res);
	res->miller=FALSE;

	zzn12_copy(res,&t0);
	zzn12_powq(_MIPP_ f,res);
	zzn12_powq(_MIPP_ f,res);
	zzn12_mul(_MIPP_ res,&t0,res);

	if (zzn12_iszero(res)) return FALSE;

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_DOUBLE_MILLER_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_DOUBLE_MILLER_RESERVE));
#endif
	return TRUE;
}

/* R-ate Pairing G2 x G1 -> GT */
#define MR_ZZN12_MILLER_RESERVE 31

DLL_EXPORT BOOL rate_miller(_MIPD_ ecn2 *P,epoint *Q,big x,zzn2 *f,zzn12 *res)
{
	int i,nb,num=0;
	BOOL first=TRUE;
	ecn2 A,KA;
	zzn2 AX,AY;
	big n,qx,qy;
	zzn12 t0;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_MILLER_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_MILLER_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MILLER_RESERVE));
#endif

	n=mirvar_mem(_MIPP_  mem, num++);
	qx=mirvar_mem(_MIPP_  mem, num++);
	qy=mirvar_mem(_MIPP_  mem, num++);
	zzn2_alloc(_MIPP_ &AX,mem,&num);
	zzn2_alloc(_MIPP_ &AY,mem,&num);

	ecn2_alloc(_MIPP_ &A,mem,&num);
	ecn2_alloc(_MIPP_ &KA,mem,&num);

	zzn12_alloc(_MIPP_ &t0,mem,&num);

	premult(_MIPP_ x,6,n);
	incr(_MIPP_ n,2,n);
	if (size(x)<0) negify(n,n);

	ecn2_copy(P,&A);
	nb=logb2(_MIPP_ n);
	zzn12_from_int(_MIPP_ 1,res);
	res->miller=TRUE;
	epoint_norm(_MIPP_ Q);
	copy(Q->X,qx);
	copy(Q->Y,qy);

	for (i=nb-2;i>=0;i--)
	{
		zzn12_sqr(_MIPP_ res,res);
		g(_MIPP_ &A,&A,qx,qy,&t0);
		zzn12_mul(_MIPP_ res,&t0,res);

		if (mr_testbit(_MIPP_ n,i))
		{
			g(_MIPP_ &A,P,qx,qy,&t0);
			zzn12_mul(_MIPP_ res,&t0,res);
		}
	}

	ecn2_copy(P,&KA);
	q_power_frobenius(_MIPP_ f,&KA);
	if (size(x)<0)
	{
		ecn2_negate(_MIPP_ &A,&A); 
		zzn12_conj(_MIPP_ res,res);
	}
	g(_MIPP_ &A,&KA,qx,qy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);
	
	q_power_frobenius(_MIPP_ f,&KA);
	ecn2_negate(_MIPP_ &KA,&KA);
	g(_MIPP_ &A,&KA,qx,qy,&t0);
	zzn12_mul(_MIPP_ res,&t0,res);

	zzn12_copy(res,&t0);
	zzn12_conj(_MIPP_ res,res);
	zzn12_inv(_MIPP_ &t0);
	
	zzn12_mul(_MIPP_ res,&t0,res);
	res->miller=FALSE;

	zzn12_copy(res,&t0);
	zzn12_powq(_MIPP_ f,res);
	zzn12_powq(_MIPP_ f,res);
	zzn12_mul(_MIPP_ res,&t0,res);

//redc(_MIPP_ res->a.a.a,t); printf("unique pairing= "); otnum(_MIPP_ t,stdout);

	if (zzn12_iszero(res)) return FALSE;

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_MILLER_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MILLER_RESERVE));
#endif
	return TRUE;
}

#define MR_ZZN12_FEXP_RESERVE 72

DLL_EXPORT void rate_fexp(_MIPD_ big x,zzn2 *f,zzn12 *res)
{
	int num=0;
	zzn12 x0,x1,x2,x3,x4,x5;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_FEXP_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_FEXP_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_FEXP_RESERVE));
#endif
	zzn12_alloc(_MIPP_ &x0,mem,&num);
	zzn12_alloc(_MIPP_ &x1,mem,&num);
	zzn12_alloc(_MIPP_ &x2,mem,&num);
	zzn12_alloc(_MIPP_ &x3,mem,&num);
	zzn12_alloc(_MIPP_ &x4,mem,&num);
	zzn12_alloc(_MIPP_ &x5,mem,&num);

	res->unitary=TRUE;
	negify(x,x);

	zzn12_copy(res,&x5);
	zzn12_powq(_MIPP_ f,&x5);
	zzn12_copy(&x5,&x0);
	zzn12_powq(_MIPP_ f,&x0);

	zzn12_mul(_MIPP_ &x5,res,&x5);
	zzn12_mul(_MIPP_ &x0,&x5,&x0);
	zzn12_powq(_MIPP_ f,&x0);

	zzn12_conj(_MIPP_ res,&x1);

	zzn12_pow(_MIPP_ res,x,&x4); /* pow */
	zzn12_pow(_MIPP_ &x4,x,&x2); /* pow */

	zzn12_copy(&x4,&x3);
	zzn12_powq(_MIPP_ f,&x3);

	zzn12_copy(&x2,res);

	zzn12_pow(_MIPP_ res,x,&x5); /* pow */
	zzn12_copy(&x5,res);
	zzn12_powq(_MIPP_ f,res);
	zzn12_mul(_MIPP_ &x5,res,res);

	zzn12_conj(_MIPP_ &x2,&x5);

	zzn12_powq(_MIPP_ f,&x2);
	zzn12_conj(_MIPP_ &x2,&x2);
	zzn12_mul(_MIPP_ &x4,&x2,&x4);
	zzn12_conj(_MIPP_ &x2,&x2);

	zzn12_powq(_MIPP_ f,&x2);

	negify(x,x);

	zzn12_sqr(_MIPP_ res,res);
	zzn12_mul(_MIPP_ res,&x4,&x4);
	zzn12_mul(_MIPP_ &x4,&x5,&x4);
	zzn12_mul(_MIPP_ &x3,&x5,res);
	zzn12_mul(_MIPP_ res,&x4,res);
	zzn12_mul(_MIPP_ &x4,&x2,&x4);
	zzn12_sqr(_MIPP_ res,res);
	zzn12_mul(_MIPP_ res,&x4,res);
	zzn12_sqr(_MIPP_ res,res);
	zzn12_mul(_MIPP_ res,&x1,&x4);
	zzn12_mul(_MIPP_ res,&x0,res);
	zzn12_sqr(_MIPP_ &x4,&x4);
	zzn12_mul(_MIPP_ &x4,res,&x4);
	zzn12_copy(&x4,res);

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_FEXP_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_FEXP_RESERVE));
#endif
	return;
}

#define MR_ZZN12_COF_RESERVE 12
/* Faster Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez */
DLL_EXPORT void cofactor(_MIPD_ zzn2 *f,big x,ecn2 *S)
{
	int num=0;
	ecn2 T,K;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_COF_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_COF_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_COF_RESERVE));
#endif
	ecn2_alloc(_MIPP_ &T,mem,&num);
	ecn2_alloc(_MIPP_ &K,mem,&num);

	ecn2_copy(S,&T);
	ecn2_mul(_MIPP_ x,&T);
	ecn2_norm(_MIPP_ &T);
	ecn2_copy(&T,&K);
	ecn2_add(_MIPP_ &K,&K);
	ecn2_add(_MIPP_ &T,&K);
	ecn2_norm(_MIPP_ &K);

	q_power_frobenius(_MIPP_ f,&K);
	q_power_frobenius(_MIPP_ f,S);
	q_power_frobenius(_MIPP_ f,S);
	q_power_frobenius(_MIPP_ f,S);

	ecn2_add(_MIPP_ &T,S);
	ecn2_add(_MIPP_ &K,S);

	q_power_frobenius(_MIPP_ f,&T);
	q_power_frobenius(_MIPP_ f,&T);

	ecn2_add(_MIPP_ &T,S);
	ecn2_norm(_MIPP_ S);

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_COF_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_COF_RESERVE));
#endif
}

#define MR_ZZN12_MEM_RESERVE 25
/* test if a ZZn12 element is of order q
   test r^q = r^p+1-t =1, so test r^p=r^(t-1) */
DLL_EXPORT BOOL member(_MIPD_ zzn2 *f,big x,zzn12 *m)
{
	BOOL res;
	int num=0;
	zzn12 w,r;
	big six;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_MEM_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_MEM_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MEM_RESERVE));
#endif
	zzn12_alloc(_MIPP_ &w,mem,&num);
	zzn12_alloc(_MIPP_ &r,mem,&num);
	six=mirvar_mem(_MIPP_  mem, num++);

	convert(_MIPP_ 6,six);

	zzn12_conj(_MIPP_ m,&r);
	zzn12_mul(_MIPP_ &r,m,&r);
	if (zzn12_isunity(_MIPP_ &r))
	{
		zzn12_copy(m,&r);
		zzn12_pow(_MIPP_ &r,x,&w); zzn12_pow(_MIPP_ &w,x,&r); zzn12_pow(_MIPP_ &r,six,&w); zzn12_copy(&w,&r);
		zzn12_copy(m,&w);
		zzn12_powq(_MIPP_ f,&w);
		res=zzn12_compare(&w,&r);
	}
	else res=FALSE;

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_MEM_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_MEM_RESERVE));
#endif
	return res; 
}

DLL_EXPORT void glv(_MIPD_ big e,big r,big W[2],big B[2][2],big u[2])
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	int i,j;
	big v[2];
	v[0]=mr_mip->w3;
	v[1]=mr_mip->w4;
	for (i=0;i<2;i++)
	{
		mad(_MIPP_ W[i],e,e,r,v[i],v[i]);
		zero(u[i]);
	}
	copy(e,u[0]);
	for (i=0;i<2;i++)
		for (j=0;j<2;j++)
		{
			multiply(_MIPP_ v[j],B[j][i],mr_mip->w5);
			subtract(_MIPP_ u[i],mr_mip->w5,u[i]);
		}
}

DLL_EXPORT void galscott(_MIPD_ big e,big r,big W[4],big B[4][4],big u[4])
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	int i,j;
	big v[4];
	v[0]=mr_mip->w3;
	v[1]=mr_mip->w4;
	v[2]=mr_mip->w5;
	v[3]=mr_mip->w6;
	for (i=0;i<4;i++)
	{
		mad(_MIPP_ W[i],e,e,r,v[i],v[i]);
		zero(u[i]);
	}

	copy(e,u[0]);
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
		{
			multiply(_MIPP_ v[j],B[j][i],mr_mip->w7);
			subtract(_MIPP_ u[i],mr_mip->w7,u[i]);
		}
}

/* generates p, r and beta very quickly from x for BN curves */

DLL_EXPORT void getprb(_MIPD_ big x,big p,big r,big beta)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	/* p=36*pow(x,4)+36*pow(x,3)+24*x*x+6*x+1; */

	multiply(_MIPP_ x,x,mr_mip->w1); /* x^2 */
	multiply(_MIPP_ x,mr_mip->w1,mr_mip->w2); /* x^3 */
	multiply(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w3); /* x^4 */
	
	add(_MIPP_ mr_mip->w2,mr_mip->w3,p);
	premult(_MIPP_ p,36,p);
	copy(p,r);  /* p = r = 36x^4+36x^3 */
	
	add(_MIPP_ mr_mip->w1,mr_mip->w2,beta);
	premult(_MIPP_ beta,18,beta); /* beta=18x^3+18x^2 */

	premult(_MIPP_ mr_mip->w1,6,mr_mip->w1); /* w1=6x^2 */

	premult(_MIPP_ mr_mip->w1,3,mr_mip->w4);
	add(_MIPP_ r,mr_mip->w4,r);    /* r= 36x^4+36x^3 +18x^3 */

	premult(_MIPP_ mr_mip->w1,4,mr_mip->w4);
	add(_MIPP_ p,mr_mip->w4,p); /* p= 36x^4+36x^3+24x^3 */

	premult(_MIPP_ x,3,mr_mip->w1); /* w1=3x */

	premult(_MIPP_ mr_mip->w1,2,mr_mip->w4); /* w4=6x */

	add(_MIPP_ p,mr_mip->w4,p);
	add(_MIPP_ r,mr_mip->w4,r);
	incr(_MIPP_ p,1,p);
	incr(_MIPP_ r,1,r);

	premult(_MIPP_ mr_mip->w1,3,mr_mip->w4);
	add(_MIPP_ beta,mr_mip->w4,beta);
	incr(_MIPP_ beta,2,beta);
	subtract(_MIPP_ p,beta,beta);

}

/* Calculate matrix for GLV method */
DLL_EXPORT void matrix2(_MIPD_ big x,big W[2],big B[2][2])
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
/* 6x^2+4x+1 */
	premult(_MIPP_ x,3,W[0]);
	incr(_MIPP_ W[0],2,W[0]);
	premult(_MIPP_ W[0],2,W[0]); multiply(_MIPP_ W[0],x,W[0]);
	incr(_MIPP_ W[0],1,W[0]);
/* -(2x+1) */
	premult(_MIPP_ x,2,W[1]);
	incr(_MIPP_ W[1],1,W[1]);
	negify(W[1],W[1]);
/* 6x^2+2x */
	premult(_MIPP_ x,3,B[0][0]);
	incr(_MIPP_ B[0][0],1,B[0][0]);
	premult(_MIPP_ B[0][0],2,B[0][0]); multiply(_MIPP_ B[0][0],x,B[0][0]);

	copy(W[1],B[0][1]);
	copy(W[1],B[1][0]);
	negify(W[0],B[1][1]);
}

/* calculate matrix for Galbraith-Scott method */
DLL_EXPORT void matrix4(_MIPD_ big x,big W[4],big B[4][4])
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
/* 2x^2+3x+1 */
	premult(_MIPP_ x,2,W[0]);
	incr(_MIPP_ W[0],3,W[0]);
	multiply(_MIPP_ W[0],x,W[0]);
	incr(_MIPP_ W[0],1,W[0]);
/* 12x^3+8x^2+x */
	premult(_MIPP_ x,3,W[1]);
	incr(_MIPP_ W[1],2,W[1]);
	premult(_MIPP_ W[1],4,W[1]); multiply(_MIPP_ W[1],x,W[1]);
	incr(_MIPP_ W[1],1,W[1]);
	multiply(_MIPP_ W[1],x,W[1]);
/* 6x^3+4x^2+x */
	premult(_MIPP_ x,3,W[2]);
	incr(_MIPP_ W[2],2,W[2]);
	premult(_MIPP_ W[2],2,W[2]); multiply(_MIPP_ W[2],x,W[2]);
	incr(_MIPP_ W[2],1,W[2]);
	multiply(_MIPP_ W[2],x,W[2]);
/* -2*x*x-x */
	premult(_MIPP_ x,2,W[3]);
	incr(_MIPP_ W[3],1,W[3]);
	multiply(_MIPP_ W[3],x,W[3]);
	negify(W[3],W[3]);

	incr(_MIPP_ x,1,B[0][0]);
	copy(x,B[0][1]);
	copy(x,B[0][2]);
	premult(_MIPP_ x,-2,B[0][3]);

	premult(_MIPP_ x,2,B[1][0]);
	incr(_MIPP_ B[1][0],1,B[1][0]);
	negify(x,B[1][1]);
	negify(B[0][0],B[1][2]);
	negify(x,B[1][3]);
	premult(_MIPP_ x,2,B[2][0]);
	incr(_MIPP_ B[2][0],1,B[2][1]);
	copy(B[2][1],B[2][2]);
	copy(B[2][1],B[2][3]);
	decr(_MIPP_ x,1,B[3][0]);
	premult(_MIPP_ B[2][1],2,B[3][1]);
	decr(_MIPP_ B[2][2],2,B[3][2]);
	negify(B[3][2],B[3][2]);
	copy(B[3][0],B[3][3]);
}

/* Use GLV endomorphism idea for multiplication in G1 - Q=e*P */

#define MR_ZZN12_G1M_RESERVE 8

DLL_EXPORT void G1_mult(_MIPD_ epoint *P,big e,big beta,big r,big x,epoint *Q)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	int i,j,num=0;
	big u[2];
	big W[2],B[2][2];
	epoint *PP;
#ifndef MR_STATIC
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_G1M_RESERVE);
#else
	char mem1[MR_ECP_RESERVE(1)];   
    char mem[MR_BIG_RESERVE(MR_ZZN12_G1M_RESERVE)];
    memset(mem1,0,MR_ECP_RESERVE(1)); 
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G1M_RESERVE));
#endif
	PP=epoint_init_mem(_MIPP_ mem1,0); 
	
	for (i=0;i<2;i++)
	{
		u[i]=mirvar_mem(_MIPP_  mem, num++);
		W[i]=mirvar_mem(_MIPP_ mem,num++);
		for (j=0;j<2;j++)
			B[i][j]=mirvar_mem(_MIPP_ mem,num++);
	}

	matrix2(_MIPP_ x,W,B);

	glv(_MIPP_ e,r,W,B,u);
	epoint_copy(P,Q); epoint_copy(P,PP);
	endomorph(_MIPP_ beta,Q);
	ecurve_mult2(_MIPP_ u[0],PP,u[1],Q,Q);

#ifndef MR_STATIC
	ecp_memkill(_MIPP_ mem1,1);
    memkill(_MIPP_ mem, MR_ZZN12_G1M_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G1M_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
}

/*.. for multiplication in G2 */

#define MR_ZZN12_G2M_RESERVE 48

DLL_EXPORT void G2_mult(_MIPD_ ecn2 *P,big e,zzn2 *f,big r,big x,ecn2 *Q)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	int i,j,num=0;
	big u[4];
	big W[4],B[4][4];
	ecn2 PP[4];
#ifndef MR_STATIC
    char *mem = (char *)memalloc(_MIPP_ MR_ZZN12_G2M_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_ZZN12_G2M_RESERVE)];
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G2M_RESERVE));
#endif
	for (i=0;i<4;i++)
	{
		u[i]=mirvar_mem(_MIPP_  mem, num++);
		W[i]=mirvar_mem(_MIPP_ mem,num++);
		for (j=0;j<4;j++)
			B[i][j]=mirvar_mem(_MIPP_ mem,num++);
	}

	ecn2_alloc(_MIPP_ &PP[0],mem,&num);
	ecn2_alloc(_MIPP_ &PP[1],mem,&num);
	ecn2_alloc(_MIPP_ &PP[2],mem,&num);
	ecn2_alloc(_MIPP_ &PP[3],mem,&num);

	matrix4(_MIPP_ x,W,B);
	galscott(_MIPP_ e,r,W,B,u);

	ecn2_copy(P,&PP[0]);
	for (i=1;i<4;i++)
	{
		ecn2_copy(&PP[i-1],&PP[i]); 
		q_power_frobenius(_MIPP_ f,&PP[i]);
	}

/* deal with -ve multipliers */
	for (i=0;i<4;i++)
	{
		if (size(u[i])<0)
			{negify(u[i],u[i]);  ecn2_negate(_MIPP_ &PP[i],&PP[i]);}
	}
	ecn2_mult4(_MIPP_ u,PP,Q);

#ifndef MR_STATIC
    memkill(_MIPP_ mem, MR_ZZN12_G2M_RESERVE);
#else
    memset(mem, 0, MR_BIG_RESERVE(MR_ZZN12_G2M_RESERVE));
#endif
}

/* Initialise a Cryptographically Strong Random Number Generator from 
   an octet of raw random data */


DLL_EXPORT void CREATE_CSPRNG(csprng *RNG,octet *RAW)
{
    strong_init(RNG,RAW->len,RAW->val,0L);
}

DLL_EXPORT void KILL_CSPRNG(csprng *RNG)
{
    strong_kill(RNG);
}

/* Get 128-bit AES key from random source */

DLL_EXPORT void AES_KEY(csprng *RNG,octet *K)
{
	int i;
	K->len=16;
	for (i=0;i<16;i++)
		K->val[i]=strong_rng(RNG);
}

/* AES-GCM Encryption and Decryption of octets, K is key, H is header, P is plaintext, C is ciphertext, T is authentication tag */

DLL_EXPORT void AES_GCM_ENCRYPT(octet *K,octet *IV,octet *H,octet *P,octet *C,octet *T)
{
	gcm g;
	gcm_init(&g,K->len,K->val,IV->len,IV->val);
	if (H!=NULL) gcm_add_header(&g,H->val,H->len);
	gcm_add_cipher(&g,GCM_ENCRYPTING,P->val,P->len,C->val);
	C->len=P->len;
	gcm_finish(&g,T->val); 
	T->len=16;
}

DLL_EXPORT void AES_GCM_DECRYPT(octet *K,octet *IV,octet *H,octet *C,octet *P,octet *T)
{
	gcm g;
	gcm_init(&g,K->len,K->val,IV->len,IV->val);
	if (H!=NULL) gcm_add_header(&g,H->val,H->len);
	gcm_add_cipher(&g,GCM_DECRYPTING,P->val,C->len,C->val);
	P->len=C->len;
	gcm_finish(&g,T->val); 
	T->len=16;
}

/* Crypto Strong RNG */
DLL_EXPORT csprng generateRNG(char* seedValue, int seedValueLength)
 {
   csprng RNG;                  
   octet octetRAW={seedValueLength, seedValueLength, seedValue};
   CREATE_CSPRNG(&RNG,&octetRAW); 
   return RNG;
 }

DLL_EXPORT void hex2bin( char* dest, char *text )
{
  unsigned int ch ;
  for( ; sscanf( (const char*)text, "%2x", &ch ) == 1 ; text += 2 )
  *dest++ = (unsigned char) ch ;
}

/*! \brief Print version number and information about the build
 *
 *  Print version number and information about the build
 * 
 */
DLL_EXPORT void version(char* info)
{
  sprintf(info,"M-Pin Version: %d.%d Build: %s", MPin_Crypto_VERSION_MAJOR, MPin_Crypto_VERSION_MINOR, BUILD_VERSION);
}

/*! \brief Generate a random six digit one time password
 *
 *  Generates a random six digit one time password
 * 
 *  @param  RNG             random number generator
 *  @return OTP             One Time Password
 */
DLL_EXPORT int generateOTP(csprng* RNG)
{
  int OTP=0;

  int i = 0;
  int val = 0;
  char byte[6] = {0};

  /* Generate random 6 digit random value */
  for (i=0;i<6;i++)
    {
       byte[i]=strong_rng(RNG);
       val = byte[i];
       OTP = ((abs(val) % 10) * pow(10.0,i)) + OTP ;
    }

  return OTP;
}

/*! \brief Generate a random number
 *
 *  Generate a random number
 * 
 *  @param  RNG             random number generator
 *  @return randomValue     random number
 */
DLL_EXPORT void generateRandom(csprng *RNG,octet *randomValue)
{
	int i;
	for (i=0;i<randomValue->len;i++)
          randomValue->val[i]=strong_rng(RNG);
}


