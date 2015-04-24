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


#ifndef OCTET_H
#define OCTET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* portable representation of a big positive number */

typedef struct
{
    int len;
    int max;
    char *val;
} octet;

/* Octet string handlers */

extern void OCTET_OUTPUT(octet *);
extern void OCTET_OUTPUT_STRING(octet *);
extern void OCTET_CLEAR(octet *);
extern int  OCTET_COMPARE(octet *,octet *);
extern void OCTET_JOIN_STRING(char *,octet *);
extern void OCTET_JOIN_BYTES(char *,int,octet *);
extern void OCTET_JOIN_BYTE(int,int,octet *);
extern void OCTET_JOIN_OCTET(octet *,octet *);
extern void OCTET_XOR(octet *,octet *);
extern void OCTET_EMPTY(octet *);
extern void OCTET_PAD(int,octet *);
extern void OCTET_TO_BASE64(octet *,char *);
extern void OCTET_FROM_BASE64(char *,octet *);
extern void OCTET_COPY(octet *,octet *);
extern void OCTET_XOR_BYTE(int,octet *);
extern void OCTET_CHOP(octet *,int,octet *);
extern void OCTET_JOIN_LONG(long,int,octet *);

#endif
