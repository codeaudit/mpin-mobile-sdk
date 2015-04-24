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

#define ASM _asm

int muldiv(a,b,c,m,rp)
int a,b,c,m,*rp;
{
        ASM mov   eax,DWORD PTR a      
        ASM mul   DWORD PTR b          
        ASM add   eax,DWORD PTR c      
        ASM adc   edx,0h                 
        ASM div   DWORD PTR m          
        ASM mov   ebx,DWORD PTR rp     
        ASM mov   [ebx],edx              
}

int muldvm(a,c,m,rp)
int a,c,m,*rp;
{
        ASM mov   edx,DWORD PTR a      
        ASM mov   eax,DWORD PTR c      
        ASM div   DWORD PTR m          
        ASM mov   ebx,DWORD PTR rp     
        ASM mov   [ebx],edx              
}

int muldvd(a,b,c,rp)
int a,b,c,*rp;
{
        ASM mov   eax,DWORD PTR a      
        ASM mul   DWORD PTR b          
        ASM add   eax,DWORD PTR c      
        ASM adc   edx,0h                 
        ASM mov   ebx,DWORD PTR rp     
        ASM mov   [ebx],eax              
        ASM mov   eax,edx
}

void muldvd2(a,b,c,rp)
int a,b,*c,*rp;
{
        ASM mov   eax,DWORD PTR a      
        ASM mul   DWORD PTR b          
        ASM mov   ebx,DWORD PTR c
        ASM add   eax,[ebx]
        ASM adc   edx,0h
        ASM mov   esi,DWORD PTR rp
        ASM add   eax,[esi]
        ASM adc   edx,0h
        ASM mov   [esi],eax              
        ASM mov   [ebx],edx
}

