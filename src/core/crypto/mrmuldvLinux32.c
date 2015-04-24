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

#include "miracl.h"


mr_small muldiv(mr_small a,mr_small b,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    __asm__ __volatile__ (
    "movl %1,%%eax\n"
    "mull %2\n"
    "addl %3,%%eax\n"
    "adcl $0,%%edx\n"
    "divl %4\n"
    "movl %5,%%ebx\n"
    "movl %%edx,(%%ebx)\n"
    "movl %%eax,%0\n"
    : "=m"(q)
    : "m"(a),"m"(b),"m"(c),"m"(m),"m"(rp)
    : "eax","ebx","memory"
    );
    return q;
}

mr_small muldvm(mr_small a,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    __asm__ __volatile__ (
    "movl %1,%%edx\n"
    "movl %2,%%eax\n"
    "divl %3\n"
    "movl %4,%%ebx\n"
    "movl %%edx,(%%ebx)\n"
    "movl %%eax,%0\n"
    : "=m"(q)
    : "m"(a),"m"(c),"m"(m),"m"(rp)
    : "eax","ebx","memory"
    );        
    return q;
}

mr_small muldvd(mr_small a,mr_small b,mr_small c,mr_small *rp)
{
    mr_small q;
    __asm__ __volatile__ (
    "movl %1,%%eax\n"
    "mull %2\n"
    "addl %3,%%eax\n"
    "adcl $0,%%edx\n"
    "movl %4,%%ebx\n"
    "movl %%eax,(%%ebx)\n"
    "movl %%edx,%0\n"
    : "=m"(q)
    : "m"(a),"m"(b),"m"(c),"m"(rp)
    : "eax","ebx","memory"
    );
    return q;
}

void muldvd2(mr_small a,mr_small b,mr_small *c,mr_small *rp)
{
    __asm__ __volatile__ (
    "movl %0,%%eax\n"
    "mull %1\n"
    "movl %2,%%ebx\n"
    "addl (%%ebx),%%eax\n"
    "adcl $0,%%edx\n"
    "movl %3,%%esi\n"
    "addl (%%esi),%%eax\n"
    "adcl $0,%%edx\n"
    "movl %%eax,(%%esi)\n"
    "movl %%edx,(%%ebx)\n"
    : 
    : "m"(a),"m"(b),"m"(c),"m"(rp)
    : "eax","ebx","esi","memory"
    );
    
}

