/*
 *  CLINT RSA header file
 *  Author: M. Scott 2015
 */

#ifndef RSA_H
#define RSA_H

#include "clint.h"

#define RFS MODBYTES*FFLEN

/* RSA Auxiliary Functions */

extern void RSA_KEY_PAIR(csprng *,sign32,rsa_private_key*,rsa_public_key*);
extern int	OAEP_ENCODE(octet *,csprng *,octet *,octet *); 
extern int  OAEP_DECODE(octet *,octet *);
extern void RSA_ENCRYPT(rsa_public_key*,octet *,octet *);   
extern void RSA_DECRYPT(rsa_private_key*,octet *,octet *);  
extern void RSA_PRIVATE_KEY_KILL(rsa_private_key *);

#endif
