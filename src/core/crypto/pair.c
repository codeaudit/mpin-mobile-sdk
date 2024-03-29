/*
Copyright (c) 2012-2015, Certivox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For full details regarding our CertiVox terms of service please refer to
the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
*/

/* CLINT BN Curve pairing functions */

//#define HAS_MAIN

#include "clint.h"

/* Line function */
static void PAIR_line(FP12 *v,ECP2 *A,ECP2 *B,BIG Qx,BIG Qy)
{
	ECP2 P;
	FP2 Z3,X,Y,ZZ,T,NY;
	FP4 a,b,c;
	int D;
	ECP2_copy(&P,A);
	if (A==B)
		D=ECP2_dbl(A);  // check these return numbers...
	else 
		D=ECP2_add(A,B);
	if (D<0)
	{ /* Infinity */
		FP12_one(v);
		return;
	}

	FP2_copy(&Z3,&(A->z));
	FP4_zero(&c);
	FP2_sqr(&ZZ,&(P.z));    /* ZZ=Z^2 */
	if (D==0)
	{ /* addition */
		ECP2_get(&X,&Y,B);
		FP2_mul(&T,&(P.z),&Y);  /* T=Z*Y2 */

		FP2_mul(&ZZ,&ZZ,&T); 

		FP2_neg(&NY,&(P.y));
		FP2_add(&ZZ,&ZZ,&NY); /* ZZ=Z^3*Y2-Y (slope numerator) */
		FP2_pmul(&Z3,&Z3,Qy);    /* Z3*Qy */
		FP2_mul(&T,&T,&(P.x));   
		FP2_mul(&X,&X,&NY);  
		FP2_add(&T,&T,&X);       /* Z*Y2*X-X2*Y */
		FP4_from_FP2s(&a,&Z3,&T); /* a=[Z3*Qy,Z*Y2*X-X2*Y] */
		FP2_neg(&ZZ,&ZZ);   	
		FP2_pmul(&ZZ,&ZZ,Qx);
		FP4_from_FP2(&b,&ZZ);    /* b=-slope*Qx */
	}
	else
	{ /* doubling */
		FP2_sqr(&T,&(P.x));
		FP2_imul(&T,&T,3);   /* T=3X^2 (slope numerator) */
		FP2_sqr(&Y,&(P.y));

		FP2_add(&Y,&Y,&Y);   /* Y=2Y^2 */
		FP2_mul(&Z3,&Z3,&ZZ);   /* Z3=Z3*ZZ */
		FP2_pmul(&Z3,&Z3,Qy);   /* Z3=Z3*ZZ*Qy */

		FP2_mul(&X,&(P.x),&T);    
		FP2_sub(&X,&X,&Y);      /* X=X*slope-2Y^2 */
		FP4_from_FP2s(&a,&Z3,&X); /* a=[Z3*ZZ*Qy , X*slope-2Y^2] */
		FP2_neg(&T,&T);       
		FP2_mul(&ZZ,&ZZ,&T);    
		FP2_pmul(&ZZ,&ZZ,Qx);
		FP4_from_FP2(&b,&ZZ);    /* b=-slope*ZZ*Qx */
	}

	FP12_from_FP4s(v,&a,&b,&c);
}

/* Optimal R-ate pairing r=e(P,Q) */
void PAIR_ate(FP12 *r,ECP2 *P,ECP *Q)
{
	FP2 X;
	BIG x,n,Qx,Qy;
	int i,nb;
	ECP2 A,KA;
	FP12 lv;

	BIG_rcopy(Qx,CURVE_Fra);
	BIG_rcopy(Qy,CURVE_Frb);
	FP2_from_BIGs(&X,Qx,Qy);

	BIG_rcopy(x,CURVE_Bnx);
	BIG_pmul(n,x,6);

	BIG_dec(n,2);
	BIG_norm(n);
	
	ECP2_affine(P);
	ECP_affine(Q);

	BIG_copy(Qx,Q->x);
	BIG_copy(Qy,Q->y);

	ECP2_copy(&A,P);
	FP12_one(r);
	nb=BIG_nbits(n);

/* Main Miller Loop */
    for (i=nb-2;i>=1;i--)
    {
		PAIR_line(&lv,&A,&A,Qx,Qy);
		FP12_smul(r,&lv);
		if (BIG_bit(n,i))
		{

			PAIR_line(&lv,&A,P,Qx,Qy);
			FP12_smul(r,&lv);
		}
		FP12_sqr(r,r);
    }

	PAIR_line(&lv,&A,&A,Qx,Qy);
	FP12_smul(r,&lv);

/* R-ate fixup */

	ECP2_copy(&KA,P);
	ECP2_frob(&KA,&X);

	ECP2_neg(&A);
	FP12_conj(r,r);

	PAIR_line(&lv,&A,&KA,Qx,Qy);
	FP12_smul(r,&lv);
	ECP2_frob(&KA,&X);
	ECP2_neg(&KA);
	PAIR_line(&lv,&A,&KA,Qx,Qy);
	FP12_smul(r,&lv);

}

/* Optimal R-ate double pairing e(P,Q).e(R,S) */
void PAIR_double_ate(FP12 *r,ECP2 *P,ECP *Q,ECP2 *R,ECP *S)
{
	FP2 X;
	BIG x,n,Qx,Qy,Sx,Sy;
	int i,nb;
	ECP2 A,B,K;
	FP12 lv;

	BIG_rcopy(Qx,CURVE_Fra);
	BIG_rcopy(Qy,CURVE_Frb);
	FP2_from_BIGs(&X,Qx,Qy);

	BIG_rcopy(x,CURVE_Bnx);

	BIG_pmul(n,x,6);
	BIG_dec(n,2);
	BIG_norm(n);
	
	ECP2_affine(P);
	ECP_affine(Q);

	ECP2_affine(R);
	ECP_affine(S);

	BIG_copy(Qx,Q->x);
	BIG_copy(Qy,Q->y);

	BIG_copy(Sx,S->x);
	BIG_copy(Sy,S->y);

	ECP2_copy(&A,P);
	ECP2_copy(&B,R);
	FP12_one(r);
	nb=BIG_nbits(n);
	
/* Main Miller Loop */
    for (i=nb-2;i>=1;i--)
    {
		PAIR_line(&lv,&A,&A,Qx,Qy);
		FP12_smul(r,&lv);
		PAIR_line(&lv,&B,&B,Sx,Sy);
		FP12_smul(r,&lv);

		if (BIG_bit(n,i))
		{
			PAIR_line(&lv,&A,P,Qx,Qy);
			FP12_smul(r,&lv);

			PAIR_line(&lv,&B,R,Sx,Sy);
			FP12_smul(r,&lv);
		}	
		FP12_sqr(r,r);
    }

	PAIR_line(&lv,&A,&A,Qx,Qy);
	FP12_smul(r,&lv);

	PAIR_line(&lv,&B,&B,Sx,Sy);
	FP12_smul(r,&lv);

/* R-ate fixup */

	FP12_conj(r,r);

	ECP2_copy(&K,P);
	ECP2_frob(&K,&X);
	ECP2_neg(&A);
	PAIR_line(&lv,&A,&K,Qx,Qy);
	FP12_smul(r,&lv);
	ECP2_frob(&K,&X);
	ECP2_neg(&K);
	PAIR_line(&lv,&A,&K,Qx,Qy);
	FP12_smul(r,&lv);

	ECP2_copy(&K,R);
	ECP2_frob(&K,&X);
	ECP2_neg(&B);
	PAIR_line(&lv,&B,&K,Sx,Sy);
	FP12_smul(r,&lv);
	ECP2_frob(&K,&X);
	ECP2_neg(&K);
	PAIR_line(&lv,&B,&K,Sx,Sy);
	FP12_smul(r,&lv);
}

/* final exponentiation - keep separate for multi-pairings and to avoid thrashing stack */
void PAIR_fexp(FP12 *r)
{
	FP2 X;
	BIG x,a,b;
	FP12 t0,y0,y1,y2,y3;

	BIG_rcopy(x,CURVE_Bnx);
	BIG_rcopy(a,CURVE_Fra);
	BIG_rcopy(b,CURVE_Frb);
	FP2_from_BIGs(&X,a,b);

/* Easy part of final exp */

	FP12_inv(&t0,r);
	FP12_conj(r,r);

	FP12_mul(r,&t0);
	FP12_copy(&t0,r);

	FP12_frob(r,&X);
	FP12_frob(r,&X);
	FP12_mul(r,&t0);

/* Hard part of final exp - see Duquesne & Ghamman eprint 2015/192.pdf */

	FP12_pow(&t0,r,x); // t0=f^-u
	FP12_usqr(&y3,&t0); // y3=t0^2
	FP12_copy(&y0,&t0); FP12_mul(&y0,&y3); // y0=t0*y3
	FP12_copy(&y2,&y3); FP12_frob(&y2,&X); // y2=y3^p
	FP12_mul(&y2,&y3); //y2=y2*y3
	FP12_usqr(&y2,&y2); //y2=y2^2
	FP12_mul(&y2,&y3); // y2=y2*y3

	FP12_pow(&t0,&y0,x);  //t0=y0^-u
	FP12_conj(&y0,r);     //y0=~r
	FP12_copy(&y1,&t0); FP12_frob(&y1,&X); FP12_frob(&y1,&X); //y1=t0^p^2
	FP12_mul(&y1,&y0); // y1=y0*y1
	FP12_conj(&t0,&t0); // t0=~t0
	FP12_copy(&y3,&t0); FP12_frob(&y3,&X); //y3=t0^p
	FP12_mul(&y3,&t0); // y3=t0*y3
	FP12_usqr(&t0,&t0); // t0=t0^2
	FP12_mul(&y1,&t0); // y1=t0*y1

	FP12_pow(&t0,&y3,x); // t0=y3^-u
	FP12_usqr(&t0,&t0); //t0=t0^2
	FP12_conj(&t0,&t0); //t0=~t0
	FP12_mul(&y3,&t0); // y3=t0*y3

	FP12_frob(r,&X); FP12_copy(&y0,r);
	FP12_frob(r,&X); FP12_mul(&y0,r);
	FP12_frob(r,&X); FP12_mul(&y0,r); 

	FP12_usqr(r,&y3);  //r=y3^2
	FP12_mul(r,&y2);   //r=y2*r
	FP12_copy(&y3,r); FP12_mul(&y3,&y0); // y3=r*y0
	FP12_mul(r,&y1); // r=r*y1
	FP12_usqr(r,r); // r=r^2
	FP12_mul(r,&y3); // r=r*y3
	FP12_reduce(r);


/* our way */
/*
//	FP12 lv,x0,x1,x2,x3,x4,x5;

	FP12_copy(&lv,r);
	FP12_frob(&lv,&X);
	FP12_copy(&x0,&lv);
	FP12_frob(&x0,&X);
	FP12_mul(&lv,r);
	FP12_mul(&x0,&lv);
	FP12_frob(&x0,&X);

	FP12_conj(&x1,r);
	FP12_pow(&x4,r,x);
	FP12_copy(&x3,&x4);
	FP12_frob(&x3,&X);

	FP12_pow(&x2,&x4,x);
	FP12_conj(&x5,&x2);
	FP12_pow(&lv,&x2,x);
	FP12_frob(&x2,&X);
	FP12_conj(r,&x2);

	FP12_mul(&x4,r);
	FP12_frob(&x2,&X);

	FP12_copy(r,&lv);
	FP12_frob(r,&X);
	FP12_mul(&lv,r);

	FP12_usqr(&lv,&lv);
	FP12_mul(&lv,&x4);
	FP12_mul(&lv,&x5);
	FP12_copy(r,&x3);
	FP12_mul(r,&x5);
	FP12_mul(r,&lv);
	FP12_mul(&lv,&x2);
	FP12_usqr(r,r);
	FP12_mul(r,&lv);
	FP12_usqr(r,r);
	FP12_copy(&lv,r);
	FP12_mul(&lv,&x1);
	FP12_mul(r,&x0);
	FP12_usqr(&lv,&lv);
	FP12_mul(r,&lv);
	FP12_reduce(r); */
}

/* GLV method */
static void glv(BIG u[2],BIG e)
{
	int i,j;
	BIG v[2],t,q;
	DBIG d;
	BIG_rcopy(q,CURVE_Order);
	for (i=0;i<2;i++)
	{
		BIG_rcopy(t,CURVE_W[i]);
		BIG_mul(d,t,e);
		BIG_ddiv(v[i],d,q);
		BIG_zero(u[i]);
	}
	BIG_copy(u[0],e);
	for (i=0;i<2;i++)
		for (j=0;j<2;j++)
		{
			BIG_rcopy(t,CURVE_SB[j][i]);
			BIG_modmul(t,v[j],t,q);
			BIG_add(u[i],u[i],q);
			BIG_sub(u[i],u[i],t);
			BIG_mod(u[i],q);
		}
	return;
}

/* Galbraith & Scott Method */
static void gs(BIG u[4],BIG e)
{
	int i,j;
	BIG v[4],t,q;
	DBIG d;
	BIG_rcopy(q,CURVE_Order);	
	for (i=0;i<4;i++)
	{
		BIG_rcopy(t,CURVE_WB[i]);
		BIG_mul(d,t,e);
		BIG_ddiv(v[i],d,q);
		BIG_zero(u[i]);
	}

	BIG_copy(u[0],e);
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
		{
			BIG_rcopy(t,CURVE_BB[j][i]);
			BIG_modmul(t,v[j],t,q);
			BIG_add(u[i],u[i],q);
			BIG_sub(u[i],u[i],t);
			BIG_mod(u[i],q);
		}
	return;
}

/* Multiply P by e in group G1 */
void PAIR_G1mul(ECP *P,BIG e)
{
#ifdef USE_GLV   /* Note this method is patented */
	int i,np,nn;
	ECP Q;
	BIG cru,t,q;
	BIG u[2];

	BIG_rcopy(q,CURVE_Order);	
	glv(u,e);

	ECP_affine(P);
	ECP_copy(&Q,P);
	BIG_rcopy(cru,CURVE_Cru);
	FP_nres(cru);
	FP_mul(Q.x,Q.x,cru);

/* note that -a.B = a.(-B). Use a or -a depending on which is smaller */

	np=BIG_nbits(u[0]);
	BIG_modneg(t,u[0],q);
	nn=BIG_nbits(t);
	if (nn<np)
	{
		BIG_copy(u[0],t);
		ECP_neg(P);
	}

	np=BIG_nbits(u[1]);
	BIG_modneg(t,u[1],q);
	nn=BIG_nbits(t);
	if (nn<np)
	{
		BIG_copy(u[1],t);
		ECP_neg(&Q);
	}


	ECP_mul2(P,&Q,u[0],u[1]);

#else
	ECP_mul(P,e);
#endif
}

/* Multiply P by e in group G2 */
void PAIR_G2mul(ECP2 *P,BIG e)
{
#ifdef USE_GS_G2   /* Well I didn't patent it :) */
	int i,np,nn;
	ECP2 Q[4];
	FP2 X;
	BIG x,y;
	BIG u[4];

	BIG_rcopy(x,CURVE_Fra);
	BIG_rcopy(y,CURVE_Frb);
	FP2_from_BIGs(&X,x,y);

	BIG_rcopy(y,CURVE_Order);	
	gs(u,e);


	ECP2_affine(P);

	ECP2_copy(&Q[0],P);
	for (i=1;i<4;i++)
	{
		ECP2_copy(&Q[i],&Q[i-1]);
		ECP2_frob(&Q[i],&X);
	}

	for (i=0;i<4;i++)
	{
		np=BIG_nbits(u[i]);
		BIG_modneg(x,u[i],y);
		nn=BIG_nbits(x);
		if (nn<np)
		{
			BIG_copy(u[i],x);
			ECP2_neg(&Q[i]);
		}
	}

	ECP2_mul4(P,Q,u);

#else
	ECP2_mul(P,e);
#endif
}

/* f=f^e */
void PAIR_GTpow(FP12 *f,BIG e)
{
#ifdef USE_GS_GT   /* Note that this option requires a lot of RAM! Maybe better to use compressed XTR method, see clint_fp4.c */
	int i,np,nn;
	FP12 g[4];
	FP2 X;
	BIG t,q,x,y;
	BIG u[4];

	BIG_rcopy(x,CURVE_Fra);
	BIG_rcopy(y,CURVE_Frb);
	FP2_from_BIGs(&X,x,y);

	BIG_rcopy(q,CURVE_Order);	
	gs(u,e);

	FP12_copy(&g[0],f);
	for (i=1;i<4;i++)
	{
		FP12_copy(&g[i],&g[i-1]);
		FP12_frob(&g[i],&X);
	}

	for (i=0;i<4;i++)
	{
		np=BIG_nbits(u[i]);
		BIG_modneg(t,u[i],q);
		nn=BIG_nbits(t);
		if (nn<np)
		{
			BIG_copy(u[i],t);
			FP12_conj(&g[i],&g[i]);
		}
	}
	FP12_pow4(f,g,u);

#else
	FP12_pow(f,f,e);
#endif
}

/* test group membership */
/* with GT-Strong curve, now only check that m!=1, conj(m)*m==1, and m.m^{p^4}=m^{p^2} */
int PAIR_GTmember(FP12 *m)
{
	BIG a,b;
	FP2 X;
	FP12 r,w;
	if (FP12_isunity(m)) return 0;
	FP12_conj(&r,m);
	FP12_mul(&r,m);
	if (!FP12_isunity(&r)) return 0;

	BIG_rcopy(a,CURVE_Fra);
	BIG_rcopy(b,CURVE_Frb);
	FP2_from_BIGs(&X,a,b);


	FP12_copy(&r,m); FP12_frob(&r,&X); FP12_frob(&r,&X);
	FP12_copy(&w,&r); FP12_frob(&w,&X); FP12_frob(&w,&X);
	FP12_mul(&w,m);


#ifndef GT_STRONG
	if (!FP12_equals(&w,&r)) return 0;

	BIG_rcopy(a,CURVE_Bnx);

	FP12_copy(&r,m); FP12_pow(&w,&r,a); FP12_pow(&w,&w,a); 
	FP12_sqr(&r,&w); FP12_mul(&r,&w); FP12_sqr(&r,&r);

	FP12_copy(&w,m); FP12_frob(&w,&X);
 #endif

	return FP12_equals(&w,&r);
}

#ifdef HAS_MAIN

int main()
{
	int i;
	char byt[32];
	csprng rng;
	BIG xa,xb,ya,yb,w,a,b,t1,q,u[2],v[4],m,r;
	ECP2 P,G;
	ECP Q,R;
	FP12 g,gp;
	FP4 t,c,cp,cpm1,cpm2;
	FP2 x,y,X;


	BIG_rcopy(a,CURVE_Fra);
	BIG_rcopy(b,CURVE_Frb);
	FP2_from_BIGs(&X,a,b);

	BIG_rcopy(xa,CURVE_Gx);
	BIG_rcopy(ya,CURVE_Gy);

	ECP_set(&Q,xa,ya);
	if (Q.inf) printf("Failed to set - point not on curve\n");
	else printf("G1 set success\n");

	printf("Q= "); ECP_output(&Q); printf("\n");

	BIG_rcopy(r,CURVE_Order); BIG_dec(r,7); BIG_norm(r);
	BIG_rcopy(xa,CURVE_Pxa);
	BIG_rcopy(xb,CURVE_Pxb);
	BIG_rcopy(ya,CURVE_Pya);
	BIG_rcopy(yb,CURVE_Pyb);

	FP2_from_BIGs(&x,xa,xb);
	FP2_from_BIGs(&y,ya,yb);

	ECP2_set(&P,&x,&y);
	if (P.inf) printf("Failed to set - point not on curve\n");
	else printf("G2 set success\n");

	printf("P= "); ECP2_output(&P); printf("\n");

//for (i=0;i<1000;i++ )
//{

	PAIR_ate(&g,&P,&Q);
	PAIR_fexp(&g);

	PAIR_GTpow(&g,xa);

//}
	printf("g3= ");FP12_output(&g); printf("\n");

}

#endif
