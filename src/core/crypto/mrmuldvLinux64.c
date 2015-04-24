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
    "movq %1,%%rax\n"
    "mulq %2\n"
    "addq %3,%%rax\n"
    "adcq $0,%%rdx\n"
    "divq %4\n"
    "movq %5,%%rbx\n"
    "movq %%rdx,(%%rbx)\n"
    "movq %%rax,%0\n"
    : "=m"(q)
    : "m"(a),"m"(b),"m"(c),"m"(m),"m"(rp)
    : "rax","rbx","memory"
    );
    return q;
}

mr_small muldvm(mr_small a,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    __asm__ __volatile__ (
    "movq %1,%%rdx\n"
    "movq %2,%%rax\n"
    "divq %3\n"
    "movq %4,%%rbx\n"
    "movq %%rdx,(%%rbx)\n"
    "movq %%rax,%0\n"
    : "=m"(q)
    : "m"(a),"m"(c),"m"(m),"m"(rp)
    : "rax","rbx","memory"
    );        
    return q;
}

mr_small muldvd(mr_small a,mr_small b,mr_small c,mr_small *rp)
{
    mr_small q;
    __asm__ __volatile__ (
    "movq %1,%%rax\n"
    "mulq %2\n"
    "addq %3,%%rax\n"
    "adcq $0,%%rdx\n"
    "movq %4,%%rbx\n"
    "movq %%rax,(%%rbx)\n"
    "movq %%rdx,%0\n"
    : "=m"(q)
    : "m"(a),"m"(b),"m"(c),"m"(rp)
    : "rax","rbx","memory"
    );
    return q;
}

void muldvd2(mr_small a,mr_small b,mr_small *c,mr_small *rp)
{
    __asm__ __volatile__ (
    "movq %0,%%rax\n"
    "mulq %1\n"
    "movq %2,%%rbx\n"
    "addq (%%rbx),%%rax\n"
    "adcq $0,%%rdx\n"
    "movq %3,%%rsi\n"
    "addq (%%rsi),%%rax\n"
    "adcq $0,%%rdx\n"
    "movq %%rax,(%%rsi)\n"
    "movq %%rdx,(%%rbx)\n"
    : 
    : "m"(a),"m"(b),"m"(c),"m"(rp)
    : "rax","rbx","rsi","memory"
    );
    
}

