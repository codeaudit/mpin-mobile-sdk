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

/*
 *  CLINT MPIN header file
 *  Author: M. Scott 2014
 */

#ifndef MPIN_H
#define MPIN_H

#include "clint.h"

/* Field size is assumed to be greater than or equal to group size */

#define PGS 32  /* MPIN Group Size */
#define PFS 32  /* MPIN Field Size */
#define PAS 16  /* MPIN Symmetric Key Size */

#define MPIN_OK                     0
#define MPIN_DOMAIN_ERROR          -11
#define MPIN_INVALID_PUBLIC_KEY    -12
#define MPIN_ERROR                 -13
#define MPIN_INVALID_POINT         -14
#define MPIN_DOMAIN_NOT_FOUND      -15
#define MPIN_OUT_OF_MEMORY         -16
#define MPIN_DIV_BY_ZERO           -17
#define MPIN_WRONG_ORDER           -18
#define MPIN_BAD_PIN               -19


/* Configure your PIN here */

#define MAXPIN 10000
#define PBLEN 14   /* max length of PIN in bits */

#define TIME_SLOT_MINUTES 1440 /* Time Slot = 1 day */
#define HASH_BYTES 32

/* MPIN support functions */

/* MPIN primitives */

DLL_EXPORT void MPIN_HASH_ID(octet *,octet *);
DLL_EXPORT int MPIN_EXTRACT_PIN(octet *,int,octet *); 
DLL_EXPORT int MPIN_CLIENT_1(int,octet *,csprng *,octet *,int,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_RANDOM_GENERATE(csprng *,octet *);
DLL_EXPORT int MPIN_CLIENT_2(octet *,octet *,octet *);
DLL_EXPORT void	MPIN_SERVER_1(int,octet *,octet *,octet *);
DLL_EXPORT int MPIN_SERVER_2(int,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_SERVER(int,int,octet *,octet *,octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_RECOMBINE_G1(octet *,octet *,octet *);
DLL_EXPORT int MPIN_RECOMBINE_G2(octet *,octet *,octet *);
DLL_EXPORT int MPIN_KANGAROO(octet *,octet *);

DLL_EXPORT int MPIN_ENCODING(csprng *,octet *);
DLL_EXPORT int MPIN_DECODING(octet *);

DLL_EXPORT unsign32 today(void);
DLL_EXPORT void CREATE_CSPRNG(csprng *,octet *);
DLL_EXPORT void KILL_CSPRNG(csprng *);

DLL_EXPORT int MPIN_GET_G1_MULTIPLE(csprng *,int,octet *,octet *,octet *);
DLL_EXPORT int MPIN_GET_CLIENT_SECRET(octet *,octet *,octet *); 
DLL_EXPORT int MPIN_GET_CLIENT_PERMIT(int,octet *,octet *,octet *); 
DLL_EXPORT int MPIN_GET_SERVER_SECRET(octet *,octet *); 
DLL_EXPORT int MPIN_TEST_PAIRING(octet *,octet *);

/* For M-Pin Full */

DLL_EXPORT int MPIN_PRECOMPUTE(octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_SERVER_KEY(octet *,octet *,octet *,octet *,octet *,octet *);
DLL_EXPORT int MPIN_CLIENT_KEY(octet *,octet *,int ,octet *,octet *,octet *,octet *);

#endif

