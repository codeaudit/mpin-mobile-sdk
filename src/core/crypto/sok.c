/* SOK Functions */

/* Version 3.0 - supports Time Permits */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sok.h"


/* general purpose hashing functions */
static void start_hash(hash *sha)
{
  HASH_init(sha);
}

static void add_to_hash(hash *sha,octet *x)
{
  int i;
  for (i=0;i<x->len;i++) 
  {
    /*printf("%d,",(unsigned char)x->val[i]);*/ 
    HASH_process(sha,x->val[i]);  
  }
}

static void finish_hash(hash *sha,octet *w)
{
  int i;
  char hh[32];
  HASH_hash(sha,hh);
   
  OCT_empty(w);
  OCT_jbytes(w,hh,32);
  for (i=0;i<32;i++) hh[i]=0;
}

/* map octet string to point on curve */
static void mapit(octet *h,ECP *P)
{
  BIG q,px;
  BIG_fromBytes(px,h->val);
  BIG_rcopy(q,Modulus);
  BIG_mod(px,q);

  while (!ECP_setx(P,px,0))
    BIG_inc(px,1);
}

/* needed for SOK */
static void mapit2(octet *h,ECP2 *Q) 
{
  BIG q,one,Fx,Fy,x,hv;
  FP2 X;
  ECP2 T,K;
  BIG_fromBytes(hv,h->val);
  BIG_rcopy(q,Modulus);
  BIG_one(one);
  BIG_mod(hv,q);

  for (;;)
  {
    FP2_from_BIGs(&X,one,hv);
    if (ECP2_setx(Q,&X)) break;
    BIG_inc(hv,1); 
  }

  /* Fast Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez */
  BIG_rcopy(Fx,CURVE_Fra);
  BIG_rcopy(Fy,CURVE_Frb);
  FP2_from_BIGs(&X,Fx,Fy);
  BIG_rcopy(x,CURVE_Bnx);

  ECP2_copy(&T,Q);
  ECP2_mul(&T,x);
  ECP2_neg(&T);  /* our x is negative */
  ECP2_copy(&K,&T);
  ECP2_dbl(&K);
  ECP2_add(&K,&T);
  ECP2_affine(&K);

  ECP2_frob(&K,&X);
  ECP2_frob(Q,&X); ECP2_frob(Q,&X); ECP2_frob(Q,&X); 
  ECP2_add(Q,&T);
  ECP2_add(Q,&K);
  ECP2_frob(&T,&X); ECP2_frob(&T,&X);
  ECP2_add(Q,&T);
  ECP2_affine(Q);
}

/* Hash number (optional) and octet to octet */
static void hashit(int n,octet *x,octet *h)
{
  int i,c[4];
  hash sha;
  char hh[HASH_BYTES];
  BIG px;

  HASH_init(&sha);
  if (n>0)
  {
    c[0]=(n>>24)&0xff;
    c[1]=(n>>16)&0xff;
    c[2]=(n>>8)&0xff;
    c[3]=(n)&0xff;
    for (i=0;i<4;i++) HASH_process(&sha,c[i]);
  }
  for (i=0;i<x->len;i++) HASH_process(&sha,x->val[i]);    
  HASH_hash(&sha,hh);
  OCT_empty(h);
  OCT_jbytes(h,hh,HASH_BYTES);
  for (i=0;i<32;i++) hh[i]=0;
}

/* G2 right hand secret RHS = s*H2(CID) where HCID is client ID hash and s is the 
   master secret */
int SOK_GET_G2_SECRET(octet *S,octet *HCID,octet *PK)
{
  BIG s;
  ECP2 P;

  mapit2(HCID,&P);
  BIG_fromBytes(s,S->val);
  PAIR_G2mul(&P,s);

  ECP2_toOctet(PK,&P);
  return 0;
}

/* G2 right hand time termit RHTP=s*H2(date|H(CID)) where CID is client ID hash 
   and s is the master secret */
int SOK_GET_G2_PERMIT(int date,octet *S,octet *HCID,octet *TP)
{
  BIG s;
  ECP2 P;
  char h[HASH_BYTES];
  octet H={0,sizeof(h),h};

  hashit(date,HCID,&H);

  mapit2(&H,&P);
  BIG_fromBytes(s,S->val);
  PAIR_G2mul(&P,s);

  ECP2_toOctet(TP,&P);
  return 0;
}

/* Calculate AES Key; f_key( e( (s*A+s*H(date|H(AID))) , (B+H(date|H(BID))) )) */
int SOK_PAIR1(int date,octet *A_ECP_SECRET,octet *A_ECP_TP,octet *BID, octet *KEY)
{
  int res;
  ECP sA,TP;
  ECP2 B,dateB;
  char h1[HASH_BYTES],h2[HASH_BYTES];
  octet H1={0,sizeof(h1),h1};
  octet H2={0,sizeof(h2),h2};

  // Pairing outputs
  FP12 g;
  char pair[12*PFS];
  octet PAIR={0,sizeof(pair),pair};

  // Key generation
  FP4 c;
  BIG w;
  char ht[HASH_BYTES];
  octet HT={0,sizeof(ht),ht};
  hash sha;

  res = 0;
  hashit(0,BID,&H1);
  mapit2(&H1,&B);

  if (!ECP_fromOctet(&sA,A_ECP_SECRET))
    res=SOK_INVALID_POINT;

  // Use time permits
  if (date)
    {
      if (!ECP_fromOctet(&TP,A_ECP_TP))
        res=SOK_INVALID_POINT;
    
      // H(date|H(BID))
      hashit(date,&H1,&H2);
      mapit2(&H2,&dateB);
    
      // sA = sA + TP
      ECP_add(&sA, &TP);
      // B = B + H(date|H(BID))
      ECP2_add(&B, &dateB);
    }

  PAIR_ate(&g,&B,&sA);
  PAIR_fexp(&g);
  // printf("SOK_PAIR1 e(sA,B) = ");FP12_output(&g); printf("\n");

  // Generate AES Key
  FP12_trace(&c,&g);
  HT.len=HASH_BYTES;
  start_hash(&sha); 
  BIG_copy(w,c.a.a); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.a.b); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.b.a); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.b.b); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);		
  finish_hash(&sha,&HT);
  OCT_empty(KEY);
  OCT_jbytes(KEY,HT.val,PAS);

  return res;
}

/* Calculate AES Key; f_key ( e( (A+H(date|H(AID))) , (s*B+s*H(date|H(BID))) )) */
int SOK_PAIR2(int date,octet *B_ECP2_SECRET,octet *B_ECP2_TP,octet *AID, octet *KEY)
{
  int res;
  ECP2 sB,TP;
  ECP A,dateA;
  char h1[HASH_BYTES],h2[HASH_BYTES];
  octet H1={0,sizeof(h1),h1};
  octet H2={0,sizeof(h2),h2};

  // Pairing outputs
  FP12 g;
  char pair[12*PFS];
  octet PAIR={0,sizeof(pair),pair};

  // Key generation
  FP4 c;
  BIG w;
  char ht[HASH_BYTES];
  octet HT={0,sizeof(ht),ht};
  hash sha;

  res = 0;
  hashit(0,AID,&H1);
  mapit(&H1,&A);

  if (!ECP2_fromOctet(&sB,B_ECP2_SECRET))
    res=SOK_INVALID_POINT;

  // Use time permits
  if (date)
    {
      if (!ECP2_fromOctet(&TP,B_ECP2_TP))
        res=SOK_INVALID_POINT;

      // H(date|H(AID))
      hashit(date,&H1,&H2);
      mapit(&H2,&dateA);
    
      // sB = sB + TP
      ECP2_add(&sB, &TP);
      // A = A + H(date|H(AID))
      ECP_add(&A, &dateA);
    }

  PAIR_ate(&g,&sB,&A);
  PAIR_fexp(&g);
  // printf("SOK_PAIR1 e(sA,B) = ");FP12_output(&g); printf("\n");

  // Generate AES Key
  FP12_trace(&c,&g);
  HT.len=HASH_BYTES;
  start_hash(&sha); 
  BIG_copy(w,c.a.a); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.a.b); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.b.a); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);
  BIG_copy(w,c.b.b); FP_redc(w); BIG_toBytes(&(HT.val[0]),w);
  add_to_hash(&sha,&HT);		
  finish_hash(&sha,&HT);
  OCT_empty(KEY);
  OCT_jbytes(KEY,HT.val,PAS);

  return res;

}

/* AES-GCM Encryption of octets, K is key, H is header, 
   P is plaintext, C is ciphertext, T is authentication tag */
void AES_GCM_ENCRYPT(octet *K,octet *IV,octet *H,octet *P,octet *C,octet *T)
{
  gcm g;
  GCM_init(&g,K->val,IV->len,IV->val);
  GCM_add_header(&g,H->val,H->len);
  GCM_add_plain(&g,C->val,P->val,P->len);
  C->len=P->len;
  GCM_finish(&g,T->val); 
  T->len=16;
}

/* AES-GCM Decryption of octets, K is key, H is header, 
   P is plaintext, C is ciphertext, T is authentication tag */
void AES_GCM_DECRYPT(octet *K,octet *IV,octet *H,octet *C,octet *P,octet *T)
{
  gcm g;
  GCM_init(&g,K->val,IV->len,IV->val);
  GCM_add_header(&g,H->val,H->len);
  GCM_add_cipher(&g,P->val,C->val,C->len);
  P->len=C->len;
  GCM_finish(&g,T->val); 
  T->len=16;
}

/* return time in slots since epoch */
unsign32 today(void)
{ 
  unsign32 ti=(unsign32)time(NULL);
  return (long)(ti/(60*TIME_SLOT_MINUTES));
}

void SOK_HASH_ID(octet *ID,octet *HID)
{
  hashit(0,ID,HID);
}

/* create random secret S */
int SOK_RANDOM_GENERATE(csprng *RNG,octet* S)
{
  BIG r,s;
  BIG_rcopy(r,CURVE_Order);
  BIG_randomnum(s,r,RNG);
  BIG_toBytes(S->val,s);
  S->len=32;
  return 0;
}

/* G1 left hand secret LHS=s*H1(CID) where HCID is client ID hash and s is 
   the master secret */
int SOK_GET_G1_SECRET(octet *S,octet *HCID,octet *CST)
{
  BIG s;
  ECP P;
  char h[HASH_BYTES];
  octet H={0,sizeof(h),h};

  mapit(HCID,&P);

  BIG_fromBytes(s,S->val);
  PAIR_G1mul(&P,s);

  ECP_toOctet(CST,&P);
  return 0;
}

/* G1 left hand time permit LHTP=s*H1(date|H(CID)) where HCID is client ID hash 
   and s is the master secret */
int SOK_GET_G1_PERMIT(int date,octet *S,octet *HCID,octet *CTT)
{
  BIG s;
  ECP P;
  char h[HASH_BYTES];
  octet H={0,sizeof(h),h};

  hashit(date,HCID,&H);

  mapit(&H,&P);
  BIG_fromBytes(s,S->val);
  PAIR_G1mul(&P,s);

  ECP_toOctet(CTT,&P);
  return 0;
}

/* R=R1+R2 in group G1 */
int SOK_RECOMBINE_G1(octet *R1,octet *R2,octet *R)
{
  ECP P,T;
  int res=0;
  if (!ECP_fromOctet(&P,R1)) res=SOK_INVALID_POINT;
  if (!ECP_fromOctet(&T,R2)) res=SOK_INVALID_POINT;
  if (res==0)
  {
    ECP_add(&P,&T);
    ECP_toOctet(R,&P);
  }
  return res;
}

/* W=W1+W2 in group G2 */
int SOK_RECOMBINE_G2(octet *W1,octet *W2,octet *W)
{
  ECP2 Q,T;
  int res=0;
  if (!ECP2_fromOctet(&Q,W1)) res=SOK_INVALID_POINT;
  if (!ECP2_fromOctet(&T,W2)) res=SOK_INVALID_POINT;
  if (res==0)
  {
    ECP2_add(&Q,&T);
    ECP2_toOctet(W,&Q);
  }
  return res;
}
