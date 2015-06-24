/*
 *  CLINT SOK header file
 */

#ifndef SOK_H
#define SOK_H

#include "clint.h"

/* Field size is assumed to be greater than or equal to group size */

#define PGS 32  /* SOK Group Size */
#define PFS 32  /* SOK Field Size */
#define PAS 16  /* AES Symmetric Key Size */

#define SOK_OK                     0
#define SOK_DOMAIN_ERROR          -11
#define SOK_INVALID_PUBLIC_KEY    -12
#define SOK_ERROR                 -13
#define SOK_INVALID_POINT         -14
#define SOK_DOMAIN_NOT_FOUND      -15
#define SOK_OUT_OF_MEMORY         -16
#define SOK_DIV_BY_ZERO           -17
#define SOK_WRONG_ORDER           -18
#define SOK_BAD_PIN               -19

#define PINDIGITS 4

#define TIME_SLOT_MINUTES 1440 /* Time Slot = 1 day */
#define HASH_BYTES 32

/* create random secret S */
DLL_EXPORT int SOK_RANDOM_GENERATE(csprng *RNG,octet* S);

/* G2 right hand secret RHS = s*H2(CID) where HCID is client ID hash and s is the 
   master secret */
DLL_EXPORT int SOK_GET_G2_SECRET(octet *S,octet *CID,octet *PK);

/* G2 right hand time termit RHTP=s*H2(date|H(CID)) where CID is client ID hash 
   and s is the master secret */
DLL_EXPORT int SOK_GET_G2_PERMIT(int date,octet *S,octet *HCID,octet *TP);

/* Calculate AES Key; f_key( e( (s*A+s*H(date|H(AID))) , (B+H(date|H(BID))) )) */
DLL_EXPORT int SOK_PAIR1(int date,octet *A_ECP_SECRET,octet *A_ECP_TP,octet *BID,octet *KEY);

/* Calculate AES Key; f_key( e( (A+H(date|H(AID))) , (s*B+s*H(date|H(BID))) )) */
DLL_EXPORT int SOK_PAIR2(int date,octet *B_ECP2_SECRET,octet *B_ECP2_TP,octet *AID,octet *KEY);

/* AES-GCM Encryption */
DLL_EXPORT void AES_GCM_ENCRYPT(octet *K,octet *IV,octet *H,octet *P,octet *C,octet *T);

/* AES-GCM Decryption */
DLL_EXPORT void AES_GCM_DECRYPT(octet *K,octet *IV,octet *H,octet *C,octet *P,octet *T);

DLL_EXPORT void SOK_HASH_ID(octet *,octet *);
DLL_EXPORT int SOK_RECOMBINE_G1(octet *,octet *,octet *);
DLL_EXPORT int SOK_RECOMBINE_G2(octet *,octet *,octet *);
DLL_EXPORT unsign32 today(void);
DLL_EXPORT void CREATE_CSPRNG(csprng *,octet *);
DLL_EXPORT void KILL_CSPRNG(csprng *RNG);
DLL_EXPORT int SOK_GET_G1_SECRET(octet *,octet *,octet *); 
DLL_EXPORT int SOK_GET_G1_PERMIT(int,octet *,octet *,octet *); 

#endif
