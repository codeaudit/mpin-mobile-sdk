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
 *   MIRACL F_p^2 support functions 
 *   mrzzn2b.c
 */

#include <stdlib.h> 
#include "miracl.h"

BOOL zzn2_qr(_MIPD_ zzn2 *u)
{
    int j;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif

    if (mr_mip->ERNUM) return FALSE;
    if (zzn2_iszero(u)) return TRUE;
    if (size(u->b)==0) return TRUE;

    if (mr_mip->qnr==-1 && size(u->a)==0) return TRUE;
    

    MR_IN(203)  

    nres_modmult(_MIPP_ u->b,u->b,mr_mip->w1);
    if (mr_mip->qnr==-2) nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w1);
    nres_modmult(_MIPP_ u->a,u->a,mr_mip->w2);
    nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
    redc(_MIPP_ mr_mip->w1,mr_mip->w1); 
    j=jack(_MIPP_ mr_mip->w1,mr_mip->modulus);

    MR_OUT
    if (j==1) return TRUE; 
    return FALSE; 
}

BOOL zzn2_sqrt(_MIPD_ zzn2 *u,zzn2 *w)
{ /* sqrt(a+ib) = sqrt(a+sqrt(a*a-n*b*b)/2)+ib/(2*sqrt(a+sqrt(a*a-n*b*b)/2))
     where i*i=n */
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return FALSE;

    zzn2_copy(u,w);
    if (zzn2_iszero(w)) return TRUE;

    MR_IN(204)  

    if (size(w->b)==0)
    {
        if (!nres_sqroot(_MIPP_ w->a,mr_mip->w15))
        {
            nres_negate(_MIPP_ w->a,w->b);
            zero(w->a);
            if (mr_mip->qnr==-2) nres_div2(_MIPP_ w->b,w->b); 
            nres_sqroot(_MIPP_ w->b,w->b);    
        }
        else
            copy(mr_mip->w15,w->a);

        MR_OUT
        return TRUE;
    }

    if (mr_mip->qnr==-1 && size(w->a)==0)
    {
        nres_div2(_MIPP_ w->b,w->b);
        if (nres_sqroot(_MIPP_ w->b,mr_mip->w15))
        {
            copy(mr_mip->w15,w->b);
            copy(w->b,w->a);
        }
        else
        {
            nres_negate(_MIPP_ w->b,w->b);
            nres_sqroot(_MIPP_ w->b,w->b);
            nres_negate(_MIPP_ w->b,w->a);
        }

        MR_OUT
        return TRUE;
    }

    nres_modmult(_MIPP_ w->b,w->b,mr_mip->w7);
    if (mr_mip->qnr==-2) nres_modadd(_MIPP_ mr_mip->w7,mr_mip->w7,mr_mip->w7);
    nres_modmult(_MIPP_ w->a,w->a,mr_mip->w1);
    nres_modadd(_MIPP_ mr_mip->w7,mr_mip->w1,mr_mip->w7);

    if (!nres_sqroot(_MIPP_ mr_mip->w7,mr_mip->w7)) /* s=w7 */
    {
        zzn2_zero(w);
        MR_OUT
        return FALSE;
    }

    nres_modadd(_MIPP_ w->a,mr_mip->w7,mr_mip->w15);
    nres_div2(_MIPP_ mr_mip->w15,mr_mip->w15);

    if (!nres_sqroot(_MIPP_ mr_mip->w15,mr_mip->w15))
    {

        nres_modsub(_MIPP_ w->a,mr_mip->w7,mr_mip->w15);
        nres_div2(_MIPP_ mr_mip->w15,mr_mip->w15);
        if (!nres_sqroot(_MIPP_ mr_mip->w15,mr_mip->w15))
        {
            zzn2_zero(w);
            MR_OUT
            return FALSE;
        }
    }

    copy(mr_mip->w15,w->a);
    nres_modadd(_MIPP_ mr_mip->w15,mr_mip->w15,mr_mip->w15);
    nres_moddiv(_MIPP_ w->b,mr_mip->w15,w->b);

    MR_OUT
    return TRUE;
}

/* y=1/x, z=1/w 

BOOL zzn2_double_inverse(_MIPD_ zzn2 *x,zzn2 *y,zzn2 *w,zzn2 *z)
{
    zzn2 t1,t2;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    MR_IN(214)

    t1.a=mr_mip->w8;
    t1.b=mr_mip->w9;  
    t2.a=mr_mip->w10;
    t2.b=mr_mip->w11;

    zzn2_mul(_MIPP_ x,w,&t1);
    if (zzn2_iszero(_MIPP_ &t1))
    {
        mr_berror(_MIPP_ MR_ERR_DIV_BY_ZERO);
        MR_OUT
        return FALSE;
    }
    zzn2_inv(_MIPP_ &t1);
    
    zzn2_mul(_MIPP_ &w,&t1,&t2);
    zzn2_mul(_MIPP_ &x,&t1,&z);
    zzn2_copy(&t2,&y);

    MR_OUT
    return TRUE;

}
*/

