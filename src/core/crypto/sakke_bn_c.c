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


/* test driver and function exerciser for SAKKE Functions */

/* Use this mirdef.h for 32-bit processor 

#define MR_LITTLE_ENDIAN
#define MIRACL 32
#define mr_utype int
#define mr_dltype long long  
#define mr_unsign64 unsigned long long
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define MR_ALWAYS_BINARY
#define MR_STATIC 8
#define MR_GENERIC_MT
#define MR_STRIPPED_DOWN
#define MR_NOSUPPORT_COMPRESSION
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO
#define MR_NOASM
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
*/

/* Use this mirdef.h for 64-bit processor 

#define MR_LITTLE_ENDIAN
#define MIRACL 64
#define mr_utype long long
#define mr_unsign64 unsigned long long
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define MR_ALWAYS_BINARY
#define MR_STATIC 4
#define MR_GENERIC_MT
#define MR_STRIPPED_DOWN
#define MR_NOSUPPORT_COMPRESSION
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8

*/

/* Link to these files 

mrcore.c
mrarth0.c
mrarth1.c
mrarth2.c
mrio1.c
mrgcd.c
mrxgcd.c
mrarth3.c
mrbits.c
mrmonty.c
mrcurve.c
mrshs256.c
mrstrong.c
mraes.c
mrzzn2.c
mrzzn4.c
mrecn2.c

For 64-bit build using Microsoft compiler mrmuldv.w64 must be included as well
For 64-bit build using Linux and Intel chips, mrmuldv.g64 must be included as well

However note that this code will also work with a standard MIRACL header

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sakke_bn_c.h"

#define WINDOW_SIZE 4

#ifdef BN_CURVE

#define ROM_SIZE 26

#if MIRACL==64

const mr_small skrom[]={
0x4000000003C012B1,0x0,0x0,0x8000000000000000,
0x2,0x0,0x0,0x0,
0xBE6623EF5C1B55B2,0x243FD6EE18093EE1,0xB0BDDF647A6366D3,0x2400000008702A0D,
0x1,0x0,0x0,0x0,
0xC0851CEEE4D2EC74,0x907C85BFA03E2726,0x358B2500F5C34BBB,0x19682D2C7053B256,
0x47A58E8B2E29CFE1,0xF81B97B0C209C30F,0xA011C937A8E99743,0x1466B9EC3E19F64A,
0xCFBFCEBCF0BE09F,0x6361B33D847EC1B3,0xD81E22157DAEE209,0xA79EDD972332B8D,
0xED904B228898EE9D,0xC2864EA569D2EDEB,0x35C6E4512D8D3461,0x6160C39ECC4C090,
0x5BD9083355C80EA3,0x68677326F173F821,0xAFE18B8AACA71898,0x1359082FA63A0164,
0x628D1BBC06534710,0xBBD863C7269546C0,0xDC53D9CDBC4E3A,0x10A6F7D0623628A9,
0x4DCDFC06974F359A,0x184E8E5808FF62E2,0xD402B058DBBA39C4,0xA65E219961D2E33,
0x952438561BC09671,0xE13EE0A041522BEE,0x7D32DF60F4633301,0x1F383BD88BD2522,
0x4B2FD3965F83A2D7,0xD19E0DD3096F7E09,0xA51CCF6777C22EE3,0x1FFB147E17B5D6F5,
0xE5BCF1B0DEDB1554,0x2FB172AF32A2FE2D,0xC622972E640162C3,0xD05631F2E181FF1,
0x4DCDFC06974F359A,0x184E8E5808FF62E2,0xD402B058DBBA39C4,0xA65E219961D2E33,
0x2941EB99405ABF42,0x4300F64DD6B712F3,0x338B0003860033D1,0x220C7C427FB304EB,
0xF21CCA38C34D16B0,0x52F4FF2CB24DDAC7,0xF2B70E2F1D9AB2B3,0x1AEE7D3E208C775,
0xD084011D971D0341,0xBA1FD75ABC6104D3,0xBA222D096E0331C,0xFEF8138E57E4D10,
0xAFFCD5775B698E22,0x98D2FDB894E31B16,0x51A1221EDF9B95BE,0xFF1FFF5CCABA081,
0x856745FF4A4B7BE9,0xBBFFE34F4F3D2C84,0xE6CD058535B49AAD,0x1B94FD702273318D,
0xEB3BB3495C619FD4,0x29C31993968406B6,0xFF3658060DEA3B22,0x226C2908BCEAD19D,
0x213979C7DF245B73,0x75BF78726D42F7E7,0x40B566A78CF634F8,0xCF2FC7D27F70826,
0xFB02C0AAACA441C0,0xCB43A2F55103952D,0x27029CDEE2F4C60F,0x234ED3A2EEC52C3E,
0x4A03F2637BB7E14,0xEAE8E1A71E1F752A,0x9C4DF91E7182F83B,0x55B5E88A9CD3B58,
0x1D1467BDBBDD59A5,0xFE8748D833B333B7,0x2CA29FB7B56166F3,0x174207A8C5A39520,
0x9CAFCEE5F3C433F9,0xFCE103DC58CBC554,0x74D528DF09E49D57,0x1B8AEFF741A16B3};

#endif

#if MIRACL==32

const mr_small skrom[]={
0x3C012B1,0x40000000,0x0,0x0,0x0,0x0,0x0,0x80000000,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x5C1B55B2,0xBE6623EF,0x18093EE1,0x243FD6EE,0x7A6366D3,0xB0BDDF64,0x8702A0D,0x24000000,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE4D2EC74,0xC0851CEE,0xA03E2726,0x907C85BF,0xF5C34BBB,0x358B2500,0x7053B256,0x19682D2C,
0x2E29CFE1,0x47A58E8B,0xC209C30F,0xF81B97B0,0xA8E99743,0xA011C937,0x3E19F64A,0x1466B9EC,
0xCF0BE09F,0xCFBFCEB,0x847EC1B3,0x6361B33D,0x7DAEE209,0xD81E2215,0x72332B8D,0xA79EDD9,
0x8898EE9D,0xED904B22,0x69D2EDEB,0xC2864EA5,0x2D8D3461,0x35C6E451,0xECC4C090,0x6160C39,
0x55C80EA3,0x5BD90833,0xF173F821,0x68677326,0xACA71898,0xAFE18B8A,0xA63A0164,0x1359082F,
0x6534710,0x628D1BBC,0x269546C0,0xBBD863C7,0xCDBC4E3A,0xDC53D9,0x623628A9,0x10A6F7D0,
0x974F359A,0x4DCDFC06,0x8FF62E2,0x184E8E58,0xDBBA39C4,0xD402B058,0x961D2E33,0xA65E219,
0x1BC09671,0x95243856,0x41522BEE,0xE13EE0A0,0xF4633301,0x7D32DF60,0x88BD2522,0x1F383BD,
0x5F83A2D7,0x4B2FD396,0x96F7E09,0xD19E0DD3,0x77C22EE3,0xA51CCF67,0x17B5D6F5,0x1FFB147E,
0xDEDB1554,0xE5BCF1B0,0x32A2FE2D,0x2FB172AF,0x640162C3,0xC622972E,0x2E181FF1,0xD05631F,
0x974F359A,0x4DCDFC06,0x8FF62E2,0x184E8E58,0xDBBA39C4,0xD402B058,0x961D2E33,0xA65E219,
0x405ABF42,0x2941EB99,0xD6B712F3,0x4300F64D,0x860033D1,0x338B0003,0x7FB304EB,0x220C7C42,
0xC34D16B0,0xF21CCA38,0xB24DDAC7,0x52F4FF2C,0x1D9AB2B3,0xF2B70E2F,0xE208C775,0x1AEE7D3,
0x971D0341,0xD084011D,0xBC6104D3,0xBA1FD75A,0x96E0331C,0xBA222D0,0xE57E4D10,0xFEF8138,
0x5B698E22,0xAFFCD577,0x94E31B16,0x98D2FDB8,0xDF9B95BE,0x51A1221E,0xCCABA081,0xFF1FFF5,
0x4A4B7BE9,0x856745FF,0x4F3D2C84,0xBBFFE34F,0x35B49AAD,0xE6CD0585,0x2273318D,0x1B94FD70,
0x5C619FD4,0xEB3BB349,0x968406B6,0x29C31993,0xDEA3B22,0xFF365806,0xBCEAD19D,0x226C2908,
0xDF245B73,0x213979C7,0x6D42F7E7,0x75BF7872,0x8CF634F8,0x40B566A7,0x27F70826,0xCF2FC7D,
0xACA441C0,0xFB02C0AA,0x5103952D,0xCB43A2F5,0xE2F4C60F,0x27029CDE,0xEEC52C3E,0x234ED3A2,
0x37BB7E14,0x4A03F26,0x1E1F752A,0xEAE8E1A7,0x7182F83B,0x9C4DF91E,0xA9CD3B58,0x55B5E88,
0xBBDD59A5,0x1D1467BD,0x33B333B7,0xFE8748D8,0xB56166F3,0x2CA29FB7,0xC5A39520,0x174207A8,
0xF3C433F9,0x9CAFCEE5,0x58CBC554,0xFCE103DC,0x9E49D57,0x74D528DF,0x741A16B3,0x1B8AEFF};
#endif


/*
#if MIRACL==64

const mr_small skrom[]={
0x4080000000000001,0x0,0x0,0x8000000000000000, // x 
0x2,0x0,0x0,0x0,                               // B 
0xA700000000000012,0x6121000000000013,0xBA344D8000000008,0x2523648240000001, // Px = -1 
0x1,0x0,0x0,0x0, // Py = 1 
0x353F63AD74319C04,0xF68AFDBF9B933998,0x28E05B3AAF153F82,0x3C67A5CB50A75BD, // Qxa 
0x23559C8A12B5637F,0x5B5051B1119E373B,0x278F3D149BAC8FAA,0x86C6D36FDAF0244, // Qxb 
0x8AB9CC634607E059,0x51430509C32A6440,0xBA739B657113D84,0x62039BE3E8F0691,  // Qya 
0xC51DD369F21FF550,0xE12AC7E5BA650CC3,0x3861D7D21AE532BD,0xAB7E3D96F16C979, // Qyb 
0xE17DE6C06F2A6DE9,0x850974924D3F77C2,0xB6499B50A846953F,0x1B377619212E7C8C, // Fx 
0xC582193F90D5922A,0xDC178B6DB2C08850,0x3EAB22F57B96AC8,0x9EBEE691ED18375,   // Fy 
0xC7A13EEB84FC4D90,0xDED2FCCC99325126,0x243D96869388BA9D,0x2ECBA233662E4F4, // c=trace(e(Q,P)) 
0xDFF634F6E95886F4,0x69D8DC50976E5896,0x58024379211120E6,0x11BD28EB94B339B2,
0x6869D25EA266113C,0x8B1B60952876029F,0x54268FD85C6BE959,0xE7BC033696EF5EB,
0xA86D12D2BC5CC688,0x61F15304E5103800,0x2AB63413218C9570,0x75178FA782F8119,
0xC7A13EEB84FC4D90,0xDED2FCCC99325126,0x243D96869388BA9D,0x02ECBA233662E4F4, // cp 
0xC709CB0916A7791F,0xF74823AF6891A77C,0x62320A06DEEEDF21,0x13663B96AB4CC64F,
0xDD6DE7F1532247F0,0x135D457165B65D0E,0xC9DBF1FE775B479F,0x22E25A2C795F4A1C, 
0x45B88A433A12F59A,0x068BA05DA8D0EC44,0x43D26826BE07D231,0x12E2BE9C600CE50A,
0x29EA1C5A8551C392,0x96E2EAEEDEBDB55F,0xF8A94ACF31784AF2,0x08689FBFF1CB2F22, //cpm1 
0x6DEED78D33A299AD,0x40AF2BC10E724D78,0x03328E05CC661B3D,0x20D3BBE43E6D85CA,
0x5CB269BFEFC15E35,0xD2D5B79F12DA7765,0xAF3CC3E56D013B0A,0x12F16BB50C6B9085,
0x95AD600C57E3C831,0x4023FAD3B72C5CF2,0xDA6D0982419E4AD3,0x20187E255AAE01EC,
0x5FB9DCE110190D00,0x3688A570D65E0814,0xAAE9360A1C818076,0x1CD32D7FA625BC57, //cpm2 
0xD2A8DC297014396E,0x9822D8ADD4B1166B,0xC6624C736E914151,0x19E4E0324E86468E,
0x8002E21AEDF1CFBD,0xA9636B782F50609C,0xFAEEC9424384E160,0x0424EFE22B3BA893,
0x24E4CCC13B8A786C,0x89C2DB99C46CA582,0xFC4D2FB275AC64AC,0x2282E98FFEBDAD17};

#endif

#if MIRACL==32

const mr_small skrom[]={
0x1,0x40800000,0x0,0x0,0x0,0x0,0x0,0x80000000,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x12,0xA7000000,0x13,0x61210000,0x8,0xBA344D80,0x40000001,0x25236482,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x74319C04,0x353F63AD,0x9B933998,0xF68AFDBF,0xAF153F82,0x28E05B3A,0xB50A75BD,0x3C67A5C,
0x12B5637F,0x23559C8A,0x119E373B,0x5B5051B1,0x9BAC8FAA,0x278F3D14,0xFDAF0244,0x86C6D36,
0x4607E059,0x8AB9CC63,0xC32A6440,0x51430509,0x57113D84,0xBA739B6,0x3E8F0691,0x62039BE,
0xF21FF550,0xC51DD369,0xBA650CC3,0xE12AC7E5,0x1AE532BD,0x3861D7D2,0x6F16C979,0xAB7E3D9,
0x6F2A6DE9,0xE17DE6C0,0x4D3F77C2,0x85097492,0xA846953F,0xB6499B50,0x212E7C8C,0x1B377619,
0x90D5922A,0xC582193F,0xB2C08850,0xDC178B6D,0x57B96AC8,0x3EAB22F,0x1ED18375,0x9EBEE69,
0x84FC4D90,0xC7A13EEB,0x99325126,0xDED2FCCC,0x9388BA9D,0x243D9686,0x3662E4F4,0x2ECBA23, // c=trace(e(Q,P)) 
0xE95886F4,0xDFF634F6,0x976E5896,0x69D8DC50,0x211120E6,0x58024379,0x94B339B2,0x11BD28EB,
0xA266113C,0x6869D25E,0x2876029F,0x8B1B6095,0x5C6BE959,0x54268FD8,0x696EF5EB,0xE7BC033,
0xBC5CC688,0xA86D12D2,0xE5103800,0x61F15304,0x218C9570,0x2AB63413,0x782F8119,0x75178FA,
0x84FC4D90,0xC7A13EEB,0x99325126,0xDED2FCCC,0x9388BA9D,0x243D9686,0x3662E4F4,0x02ECBA23, // cp 
0x16A7791F,0xC709CB09,0x6891A77C,0xF74823AF,0xDEEEDF21,0x62320A06,0xAB4CC64F,0x13663B96,
0x532247F0,0xDD6DE7F1,0x65B65D0E,0x135D4571,0x775B479F,0xC9DBF1FE,0x795F4A1C,0x22E25A2C,
0x3A12F59A,0x45B88A43,0xA8D0EC44,0x068BA05D,0xBE07D231,0x43D26826,0x600CE50A,0x12E2BE9C,
0x8551C392,0x29EA1C5A,0xDEBDB55F,0x96E2EAEE,0x31784AF2,0xF8A94ACF,0xF1CB2F22,0x08689FBF, //cpm1 
0x33A299AD,0x6DEED78D,0x0E724D78,0x40AF2BC1,0xCC661B3D,0x03328E05,0x3E6D85CA,0x20D3BBE4,
0xEFC15E35,0x5CB269BF,0x12DA7765,0xD2D5B79F,0x6D013B0A,0xAF3CC3E5,0x0C6B9085,0x12F16BB5,
0x57E3C831,0x95AD600C,0xB72C5CF2,0x4023FAD3,0x419E4AD3,0xDA6D0982,0x5AAE01EC,0x20187E25,
0x10190D00,0x5FB9DCE1,0xD65E0814,0x3688A570,0x1C818076,0xAAE9360A,0xA625BC57,0x1CD32D7F, //cpm2 
0x7014396E,0xD2A8DC29,0xD4B1166B,0x9822D8AD,0x6E914151,0xC6624C73,0x4E86468E,0x19E4E032,
0xEDF1CFBD,0x8002E21A,0x2F50609C,0xA9636B78,0x4384E160,0xFAEEC942,0x2B3BA893,0x0424EFE2,
0x3B8A786C,0x24E4CCC1,0xC46CA582,0x89C2DB99,0x75AC64AC,0xFC4D2FB2,0xFEBDAD17,0x2282E98F};
#endif
*/

#endif


#ifdef RFC_CURVE

/* CESG Elliptic Curve parameters */

#define ROM_SIZE 7

#if MIRACL==32

const mr_small skrom[]={
0xFEA85FEB,0x666D807A,0xAC7ACE87,0x80C5DF10,0x89857DB0,0xFCE3E823,0x56971F1F,0x9F94D6AF,0x1C3C09AA,0xA7CF3C52,0x31852A82,0xB6AFF4A8,0x65681CE1,0x512AC5CD,0x326B4CD4,0xE26C6487,0xA666A6D0,0x356D27F4,0xF7C88A19,0xE791B39F,0x31A59CB0,0x228730D5,0xE2FC0F1B,0xF40AAB27,0xB3E01A2E,0xBE9AE358,0x9CB48261,0x416C0CE1,0xDAD0657A,0x65C61198,0x0A563FDA,0x997ABB1F,
0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       
0xBFAA17FB,0xD99B601E,0x2B1EB3A1,0x203177C4,0xE2615F6C,0xFF38FA08,0xD5A5C7C7,0xA7E535AB,0x870F026A,0xA9F3CF14,0x0C614AA0,0x6DABFD2A,0x595A0738,0x144AB173,0xCC9AD335,0x389B1921,0x2999A9B4,0x4D5B49FD,0xFDF22286,0x39E46CE7,0x4C69672C,0xC8A1CC35,0xF8BF03C6,0xBD02AAC9,0x2CF8068B,0x6FA6B8D6,0x672D2098,0x905B0338,0x36B4195E,0x99718466,0xC2958FF6,0x265EAEC7,
0xEAE63895,0x880DC8AB,0x967E0979,0x80EC46C4,0xB63F73EC,0xEE9163A5,0x80728D87,0xD5CFB4CC,0xBA66910D,0xA7C1514D,0x7A60DE74,0xA702C339,0x8B72F2E1,0x337C8654,0x5DD5BCCB,0x9760AF76,0x406CE890,0x718BD9E7,0xDB9DFA55,0x43D5F22C,0x30B09E10,0xAB10DB90,0xF6CE2308,0xB5EDB6C0,0xB6FF7CBF,0x98B2F204,0x0AEC69C6,0x2B1A2FD6,0x3ED9B52A,0x0A799005,0x332C29AD,0x53FC09EE,
0x1BEF16D7,0x75573FD7,0x6A67DCDE,0xADB9B570,0xD5BB4636,0x80BDAD5A,0xE9CB99A9,0x13515AD7,0xC5A4D5F2,0x492D979F,0x164AA989,0xAC6F1E80,0xB7652FE0,0xCAD696B5,0xAD547C6C,0x70DAE117,0xA9E032B9,0x416CFF0C,0x9A140B2E,0x6B598CCF,0xF0DE55F6,0xE7F7F5E5,0x654EC2B9,0xF5EA69F4,0x1E141178,0x3D778D82,0x02990696,0xD3E82016,0x3634A135,0xF9F1F053,0x3F6009F1,0x0A824906,
0x6EAEA0CB,0x73ED71A6,0x80727455,0xC1200615,0x2C14A3C7,0xC227422B,0xEBB34633,0xABDBAF4D,0xD94E56B8,0x44851CD9,0x509E6274,0xEF32A229,0xBFC5CDEF,0x11655C0E,0x21F86E49,0x392FC622,0x49B9B48E,0x634916F0,0x398B5F0D,0x9BD2C9F8,0x4ABDD959,0x4DF1311B,0xE1ABDD13,0xB90478E0,0x8E85B89A,0x0FB26753,0x2A456E1A,0x4D926F3F,0xCCB0F000,0xA26768B9,0x6CDDB19D,0x5D8F832E,
0xEC16615D,0xA1C0A15C,0x7442D166,0xA3BCBCB2,0x5DD14BF3,0x79833439,0x96C7BF67,0xF10B2764,0xDD88B2C2,0x41FC8323,0xADD105A4,0x322B8C0E,0x5E9B01DA,0xF08DF665,0x330CB3B,0xD8267711,0xDF918502,0x7E76045A,0xB63613DA,0x77840B30,0x479F6C31,0xB6FCDF2D,0xA719D3F9,0xFF756A74,0x0215E401A,0x6B192342,0x9BCD34E1,0xB47CB48C,0xC0FFDBCE,0x4803009D,0xFAAA631A,0x16A5DE75};

#endif

#if MIRACL==64

const mr_small skrom[]={
0x666D807AFEA85FEB,0x80C5DF10AC7ACE87,0xFCE3E82389857DB0,0x9F94D6AF56971F1F,0xA7CF3C521C3C09AA,0xB6AFF4A831852A82,0x512AC5CD65681CE1,0xE26C6487326B4CD4,0x356D27F4A666A6D0,0xE791B39FF7C88A19,0x228730D531A59CB0,0xF40AAB27E2FC0F1B,0xBE9AE358B3E01A2E,0x416C0CE19CB48261,0x65C61198DAD0657A,0x997ABB1F0A563FDA,
0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,           
0xD99B601EBFAA17FB,0x203177C42B1EB3A1,0xFF38FA08E2615F6C,0xA7E535ABD5A5C7C7,0xA9F3CF14870F026A,0x6DABFD2A0C614AA0,0x144AB173595A0738,0x389B1921CC9AD335,0x4D5B49FD2999A9B4,0x39E46CE7FDF22286,0xC8A1CC354C69672C,0xBD02AAC9F8BF03C6,0x6FA6B8D62CF8068B,0x905B0338672D2098,0x9971846636B4195E,0x265EAEC7C2958FF6,
0x880DC8ABEAE63895,0x80EC46C4967E0979,0xEE9163A5B63F73EC,0xD5CFB4CC80728D87,0xA7C1514DBA66910D,0xA702C3397A60DE74,0x337C86548B72F2E1,0x9760AF765DD5BCCB,0x718BD9E7406CE890,0x43D5F22CDB9DFA55,0xAB10DB9030B09E10,0xB5EDB6C0F6CE2308,0x98B2F204B6FF7CBF,0x2B1A2FD60AEC69C6,0x0A7990053ED9B52A,0x53FC09EE332C29AD,
0x75573FD71BEF16D7,0xADB9B5706A67DCDE,0x80BDAD5AD5BB4636,0x13515AD7E9CB99A9,0x492D979FC5A4D5F2,0xAC6F1E80164AA989,0xCAD696B5B7652FE0,0x70DAE117AD547C6C,0x416CFF0CA9E032B9,0x6B598CCF9A140B2E,0xE7F7F5E5F0DE55F6,0xF5EA69F4654EC2B9,0x3D778D821E141178,0xD3E8201602990696,0xF9F1F0533634A135,0x0A8249063F6009F1,
0x73ED71A66EAEA0CB,0xC120061580727455,0xC227422B2C14A3C7,0xABDBAF4DEBB34633,0x44851CD9D94E56B8,0xEF32A229509E6274,0x11655C0EBFC5CDEF,0x392FC62221F86E49,0x634916F049B9B48E,0x9BD2C9F8398B5F0D,0x4DF1311B4ABDD959,0xB90478E0E1ABDD13,0x0FB267538E85B89A,0x4D926F3F2A456E1A,0xA26768B9CCB0F000,0x5D8F832E6CDDB19D,
0xA1C0A15CEC16615D,0xA3BCBCB27442D166,0x798334395DD14BF3,0xF10B276496C7BF67,0x41FC8323DD88B2C2,0x322B8C0EADD105A4,0xF08DF6655E9B01DA,0xD82677110330CB3B,0x7E76045ADF918502,0x77840B30B63613DA,0xB6FCDF2D479F6C31,0xFF756A74A719D3F9,0x6B192342215E401A,0xB47CB48C9BCD34E1,0x4803009DC0FFDBCE,0x16A5DE75FAAA631A};

#endif

#endif

#ifdef RFC_CURVE_V2
/* CertiVox parameters */

#define ROM_SIZE 7

#if MIRACL==32

const mr_small skrom[]={
0xB260864B,0x5CF5E4B8,0x69A2990E,0x05F7BB89,0xA2F75597,0xD86425F0,0xAD27DF99,0x167EFB06,0x86F80E30,0xBE30F373,0x4FE17B4E,0x83D28A6C,0x4BCA5417,0xF47C6DA5,0xFC3B190D,0x61C3DAEE,0xD07DE23,0x537218E2,0x8ED441A1,0x9C353C7,0xDF15270D,0x85E91089,0x4E0FF1FD,0x2C360195,0x11196250,0x79B22870,0xC96854F1,0x8A7E0952,0x2C7621C6,0xD9B8FED6,0xAB257778,0xCFCB3C88,
0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       
0x2001,    0x0,       0x0,       0x0,       0x0,       0x0,       0x80000000,0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,      
0xAB76AFEF,0xEA5EDCCD,0x4D001FFF,0x288EE997,0x1F68D6E2,0xDCAF6230,0xAEBE57F4,0x74C74A83,0x1211A141,0x1F6C4F61,0x436CC5F5,0x2A736DEA,0xC942A578,0x3105F544,0xDF1E0799,0x30F820D9,0x19605DBF,0xB7B1C336,0xFC9FBF47,0xE7C08286,0x868E75D8,0x50D854BC,0xCA01888B,0xAC4A2018,0x5AA7157C,0x1EA49D3C,0xCDE3D5BA,0x77A37479,0x5D900666,0xF55E2F23,0x7028D51B,0x69FF3297,
0x88E35483,0xB4DF222C,0x6932F6D3,0x360BB7C3,0x1D1122BC,0x59CA2B5A,0xFDB0BF86,0xE8A2F1BD,0xA1E75458,0x2CBF00AE,0x85B0C78C,0xCCF695AA,0xF15A5DFC,0x1C0CBEB9,0xAAF00A76,0x58A257BD,0xF0359448,0x1DB9AD43,0xC1EF643F,0x65177380,0x57DBEB78,0xBF0D64D0,0x3CEEDAEC,0x55B6BCAA,0xC0C00859,0xB9AAC037,0x3CEB06EF,0x36409A61,0x2444726C,0xD9B2BD5F,0x7069A3F5,0x1AC0A937,
0x3AD9D197,0xFF28EB72,0x4E499732,0xC9490D61,0x60CA0EF4,0x76D94065,0x285DBD2B,0x875C3FB7,0x7058CB7C,0xA69EFCBC,0x02AC09ED,0x0F2E0A3D,0x8E3138DD,0x35A4B646,0x12B9E130,0xA1166D2C,0xC52FB66B,0x79C3B751,0x6CEA2E12,0x7E78893A,0x7ACA189F,0xB82E49F6,0xD9F7FBD2,0xBAC94B85,0x296605F3,0xBC4B775E,0x274C2DA1,0xC44A7E1A,0x19CC49DA,0xF3BF5077,0xF61D63FA,0x17EFFDB6,
0x663E08C9,0x86A0B50F,0x9CD81D8D,0xD46D6183,0xC717D96B,0xEBD909CF,0xBE18A4B5,0xB236DF03,0x2FFBE642,0x78204787,0x4DFF52B1,0xF6669B20,0x30717F7F,0x824DE375,0xE5DF6D74,0xCE70E6E8,0x5FA74F93,0x63B1E14D,0xF1213213,0xB536D8DD,0xA8C9FF06,0x945AC966,0x3B11C956,0x599C093E,0xA0F059FA,0xB94986E4,0x317C5903,0x41E9C409,0x7C309137,0x5C0985F6,0xE1A7DE9E,0x0A91A069};

#endif

#if MIRACL==64

const mr_small skrom[]={
0x5CF5E4B8B260864B,0x05F7BB8969A2990E,0xD86425F0A2F75597,0x167EFB06AD27DF99,0xBE30F37386F80E30,0x83D28A6C4FE17B4E,0xF47C6DA54BCA5417,0x61C3DAEEFC3B190D,0x537218E20D07DE23,0x9C353C78ED441A1,0x85E91089DF15270D,0x2C3601954E0FF1FD,0x79B2287011196250,0x8A7E0952C96854F1,0xD9B8FED62C7621C6,0xCFCB3C88AB257778,
0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,           
0x2001,            0x0,               0x0,               0x80000000,        0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,               0x0,           
0xEA5EDCCDAB76AFEF,0x288EE9974D001FFF,0xDCAF62301F68D6E2,0x74C74A83AEBE57F4,0x1F6C4F611211A141,0x2A736DEA436CC5F5,0x3105F544C942A578,0x30F820D9DF1E0799,0xB7B1C33619605DBF,0xE7C08286FC9FBF47,0x50D854BC868E75D8,0xAC4A2018CA01888B,0x1EA49D3C5AA7157C,0x77A37479CDE3D5BA,0xF55E2F235D900666,0x69FF32977028D51B,
0xB4DF222C88E35483,0x360BB7C36932F6D3,0x59CA2B5A1D1122BC,0xE8A2F1BDFDB0BF86,0x2CBF00AEA1E75458,0xCCF695AA85B0C78C,0x1C0CBEB9F15A5DFC,0x58A257BDAAF00A76,0x1DB9AD43F0359448,0x65177380C1EF643F,0xBF0D64D057DBEB78,0x55B6BCAA3CEEDAEC,0xB9AAC037C0C00859,0x36409A613CEB06EF,0xD9B2BD5F2444726C,0x1AC0A9377069A3F5,
0xFF28EB723AD9D197,0xC9490D614E499732,0x76D9406560CA0EF4,0x875C3FB7285DBD2B,0xA69EFCBC7058CB7C,0xF2E0A3D02AC09ED,0x35A4B6468E3138DD,0xA1166D2C12B9E130,0x79C3B751C52FB66B,0x7E78893A6CEA2E12,0xB82E49F67ACA189F,0xBAC94B85D9F7FBD2,0xBC4B775E296605F3,0xC44A7E1A274C2DA1,0xF3BF507719CC49DA,0x17EFFDB6F61D63FA,
0x86A0B50F663E08C9,0xD46D61839CD81D8D,0xEBD909CFC717D96B,0xB236DF03BE18A4B5,0x782047872FFBE642,0xF6669B204DFF52B1,0x824DE37530717F7F,0xCE70E6E8E5DF6D74,0x63B1E14D5FA74F93,0xB536D8DDF1213213,0x945AC966A8C9FF06,0x599C093E3B11C956,0xB94986E4A0F059FA,0x41E9C409317C5903,0x5C0985F67C309137,0xA91A069E1A7DE9E};

#endif

#endif

/* RFC Super-Singular Curve stuff */

#ifndef BN_CURVE

/*
   Add A=A+B  (or A=A+A) 
*/

void gg(_MIPD_ epoint *A,epoint *B,big a,big d,zzn2 *w)
{
	int type;
	big lam,ex1,ex2;

    copy(A->X,mr_mip->w9);
	type=ecurve_add(_MIPP_ B,A); /* slope= mr_mip->w8 */
	if (!type)
    {
        zzn2_from_int(_MIPP_ 1,w);
        return;
    }

	lam=mr_mip->w8; /* slope in w8 */
	ex1=mr_mip->w7;
	ex2=mr_mip->w6;

	if (A->marker!=MR_EPOINT_GENERAL)
        copy(mr_mip->one,mr_mip->w3);
    else copy(A->Z,mr_mip->w3);    

	if (type==MR_ADD)
    {
		nres_modadd(_MIPP_ a,B->X,mr_mip->w1);
		nres_modmult(_MIPP_ mr_mip->w1,lam,mr_mip->w1);
		nres_modmult(_MIPP_ mr_mip->w3,B->Y,mr_mip->w2);
		nres_modsub(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
		nres_modmult(_MIPP_ mr_mip->w3,d,mr_mip->w3);
		zzn2_from_zzns(mr_mip->w1,mr_mip->w3,w);
	}
	if (type==MR_DOUBLE)
    { 
		nres_modmult(_MIPP_ a,ex2,mr_mip->w1);
		nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w9,mr_mip->w1);
		nres_modmult(_MIPP_ mr_mip->w1,lam,mr_mip->w1);
		nres_modsub(_MIPP_ mr_mip->w1,ex1,mr_mip->w1);
		nres_modmult(_MIPP_ mr_mip->w3,ex2,mr_mip->w3);
		nres_modmult(_MIPP_ mr_mip->w3,d,mr_mip->w3);
		zzn2_from_zzns(mr_mip->w1,mr_mip->w3,w);
	}
	return;
}

void zzn2_pow(_MIPD_ zzn2 *x,big k,zzn2 *u)
{
	int i,j,nb,nbw,nzs,n;
	zzn2 t[8],u2;
#ifndef MR_STATIC
    char *mem=memalloc(_MIPP_ 16);
#else
    char mem[MR_BIG_RESERVE(16)];        
    memset(mem,0,MR_BIG_RESERVE(16));
#endif
	if (size(k)==0)
    {
        zzn2_from_int(_MIPP_ 1,u);
        return;
    }
    zzn2_copy(x,u);
    if (size(k)==1) return;

    for (j=i=0;i<8;i++)
    {
        t[i].a=mirvar_mem(_MIPP_ mem,j++);
        t[i].b=mirvar_mem(_MIPP_ mem,j++);
    }
    u2.a=mr_mip->w9;
    u2.b=mr_mip->w10;

/*
   Prepare table for windowing. 4-bit sliding window 
*/
	zzn2_sqr(_MIPP_ u,&u2);
	zzn2_copy(u,&t[0]);
   
    for (i=1;i<8;i++)
		zzn2_mul(_MIPP_ &t[i-1],&u2,&t[i]);
      
	nb=logb2(_MIPP_ k);

    if (nb>1) for (i=nb-2;i>=0;)
    {
        n=mr_window(_MIPP_ k,i,&nbw,&nzs,4);
        for (j=0;j<nbw;j++) zzn2_sqr(_MIPP_ u,u);
        if (n>0) zzn2_mul(_MIPP_ u,&t[n/2],u);
        i-=nbw;
        if (nzs)
        {
            for (j=0;j<nzs;j++) zzn2_sqr(_MIPP_ u,u);
            i-=nzs;
        }
    }
#ifndef MR_STATIC      
    memkill(_MIPP_ mem,16);
#else
    memset(mem,0,MR_BIG_RESERVE(16)); 
#endif
}

BOOL tate(_MIPD_ epoint *P,epoint *Q,big q,big cf,zzn2 *r)
{
	int i,nb;
	zzn2 w;
	big a,d;
	epoint A;

	epoint_norm(_MIPP_ P);
	epoint_norm(_MIPP_ Q);

	a=Q->X;
	d=Q->Y;
	
	A.X=mr_mip->w13;
	A.Y=mr_mip->w14;
	A.Z=mr_mip->w15;

	w.a=mr_mip->w11;
	w.b=mr_mip->w12;

	epoint_copy(P,&A);
	decr(_MIPP_ q,1,mr_mip->w10);
	nb=logb2(_MIPP_ mr_mip->w10);

	zzn2_from_int(_MIPP_ 1,r);

    for (i=nb-2;i>=0;i--)
    {
		zzn2_sqr(_MIPP_ r,r);
		gg(_MIPP_ &A,&A,a,d,&w);
		zzn2_mul(_MIPP_ r,&w,r);

		if (mr_testbit(_MIPP_ mr_mip->w10,i))
		{
			gg(_MIPP_ &A,P,a,d,&w);
			zzn2_mul(_MIPP_ r,&w,r);
		}
	}

	epoint_negate(_MIPP_ &A);
	if (!epoint_comp(_MIPP_ &A,P)) return FALSE;
	if (zzn2_iszero(r)) return FALSE;

/* ^(p+1)/q */
	zzn2_pow(_MIPP_ r,cf,r);

	if (zzn2_isunity(_MIPP_ r)) return FALSE;

	return TRUE;
}
#endif


/*
void ecn_print(_MIPD_ epoint *P)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	if (P->marker==MR_EPOINT_INFINITY)
	{
		printf("[Infinity]\n");
		return;
	}
	epoint_norm(_MIPP_ P);
	printf("["); 
	redc(_MIPP_ P->X,P->X);
	otstr(_MIPP_ P->X,mr_mip->IOBUFF);
	nres(_MIPP_ P->X,P->X);
	printf("%s,",mr_mip->IOBUFF);
	redc(_MIPP_ P->Y,P->Y);
	otstr(_MIPP_ P->Y,mr_mip->IOBUFF);
	nres(_MIPP_ P->Y,P->Y);
	printf("%s]\n",mr_mip->IOBUFF);
}

void zzn2_print(_MIPD_ zzn2 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); 
	redc(_MIPP_ x->a,x->a);
	otstr(_MIPP_ x->a,mr_mip->IOBUFF); 
	nres(_MIPP_ x->a,x->a);
	printf("%s,",mr_mip->IOBUFF); 
	redc(_MIPP_ x->b,x->b);
	otstr(_MIPP_ x->b,mr_mip->IOBUFF); 
	nres(_MIPP_ x->b,x->b);
	printf("%s)",mr_mip->IOBUFF);
}

void zzn4_print(_MIPD_ zzn4 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); zzn2_print(_MIPP_ &(x->a)); printf(","); zzn2_print(_MIPP_ &(x->b));  printf(")");
}

void zzn12_print(_MIPD_ zzn12 *x)
{
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
	printf("("); zzn4_print(_MIPP_ &(x->a)); printf(","); zzn4_print(_MIPP_ &(x->b)); printf(","); zzn4_print(_MIPP_ &(x->c)); printf(")"); printf("\n");
}

void ecn2_print(_MIPD_ ecn2 *P)
{
	#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
	#endif
	if (P->marker==MR_EPOINT_INFINITY)
	{
		printf("[Infinity]\n");
		return;
	}
	ecn2_norm(_MIPP_ P);
	printf("[");
	zzn2_print(_MIPP_ &(P->x));
	printf(",");
	zzn2_print(_MIPP_ &(P->y));
	printf("]\n");
}
*/


/* Initialise the SAKKE_BN domain structure
 * It is assumed that the EC domain details are obtained from ROM
 */
#ifdef BN_CURVE
#define MR_SAKKE_BN_INIT_RESERVE 38
#else
#define MR_SAKKE_BN_INIT_RESERVE 9
#endif


int SAKKE_DOMAIN_INIT(sak_domain *DOM,const void *rom)
{ /* get domain details from ROM     */
	int i,pt,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,2*SFS,16);
#else
    miracl *mr_mip=mirsys(2*SFS,16);
#endif
	epoint *P;
	big q,r,px,py,a,b;
#ifdef BN_CURVE
    big x,beta,xx,yy,g[16];
	ecn2 Q;
	zzn2 f,qx,qy;
#else
	big cf,sa,sb;
#endif
    int words,promptr,err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_INIT_RESERVE);
	char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_INIT_RESERVE)];
    char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_INIT_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
	DOM->nibbles=2*SFS;
	words=MR_ROUNDUP(SFS*8,MIRACL);

	if (mr_mip==NULL || mem==NULL) res= SAKKE_OUT_OF_MEMORY;

    mr_mip->ERCON=TRUE;

#ifdef BN_CURVE

    if (res==0)
    {
        x=mirvar_mem(_MIPP_ mem, num++);
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
		b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
		xx=mirvar_mem(_MIPP_ mem, num++);
		yy=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		for (i=0;i<16;i++) g[i]=mirvar_mem(_MIPP_ mem, num++);

/* read in from PROM and make simple integrity checks */

		promptr=0;
		init_big_from_rom(x,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in BN parameter from ROM   */
		sftbit(_MIPP_ x,-(SFS*8-2),xx);  /* top 2 bits encode extra info */
		pt=size(xx);
		expb2(_MIPP_ (SFS*8-2),xx);
		divide(_MIPP_ x,xx,xx);

		mr_mip->TWIST=MR_SEXTIC_D;
		if ((pt&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((pt&2)==2) negify(x,x); 

		init_big_from_rom(b,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter b from ROM   */

		getprb(_MIPP_ x,q,r,beta);
		zero(a);
		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

		nres(_MIPP_ beta,beta);

 		init_big_from_rom(px,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter gx from ROM */
		init_big_from_rom(py,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter gy from ROM */
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;

	}
	if (res==0)
	{
		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qx);
		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);

		zzn2_from_bigs(_MIPP_ xx,yy,&qy);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		init_big_from_rom(xx,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		init_big_from_rom(yy,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
		zzn2_from_bigs(_MIPP_ xx,yy,&f);

		for (i=0;i<16;i++)
		{
			init_big_from_rom(g[i],words,(const mr_small *)rom,words*ROM_SIZE,&promptr);
			nres(_MIPP_ g[i],g[i]);
		}
 	
	}
	if (res==0)
	{
		DOM->flags=pt;
		big_to_bytes(_MIPP_ SFS,x,DOM->X,TRUE); /* bigs here */
		big_to_bytes(_MIPP_ SFS,q,DOM->Q,TRUE);
		big_to_bytes(_MIPP_ SFS,a,DOM->A,TRUE);
		big_to_bytes(_MIPP_ SFS,b,DOM->B,TRUE);
		big_to_bytes(_MIPP_ SGS,r,DOM->R,TRUE);
		big_to_bytes(_MIPP_ SFS,px,DOM->Px,TRUE);
		big_to_bytes(_MIPP_ SFS,py,DOM->Py,TRUE);
		big_to_bytes(_MIPP_ SFS,beta,DOM->Beta,TRUE);/* nresidues from here */
		big_to_bytes(_MIPP_ SFS,qx.a,DOM->Qxa,TRUE); 
		big_to_bytes(_MIPP_ SFS,qx.b,DOM->Qxb,TRUE);
		big_to_bytes(_MIPP_ SFS,qy.a,DOM->Qya,TRUE);
		big_to_bytes(_MIPP_ SFS,qy.b,DOM->Qyb,TRUE);
		big_to_bytes(_MIPP_ SFS,f.a,DOM->Fa,TRUE);
		big_to_bytes(_MIPP_ SFS,f.b,DOM->Fb,TRUE);
		for (i=0;i<16;i++) big_to_bytes(_MIPP_ SFS,g[i],DOM->G[i],TRUE);
	}

#else
    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, 0);
        a=mirvar_mem(_MIPP_ mem, 1);
		b=mirvar_mem(_MIPP_ mem, 2);
        r=mirvar_mem(_MIPP_ mem, 3);
        px=mirvar_mem(_MIPP_ mem, 4);
        py=mirvar_mem(_MIPP_ mem, 5);
		cf=mirvar_mem(_MIPP_ mem, 6);
		sa=mirvar_mem(_MIPP_ mem, 7);
		sb=mirvar_mem(_MIPP_ mem, 8);

		promptr=0;
		init_big_from_rom(q,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in prime modulus q from ROM   */
		init_big_from_rom(b,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter b from ROM   */
 		init_big_from_rom(r,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter r from ROM */
 		init_big_from_rom(px,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter px from ROM */
		init_big_from_rom(py,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter py from ROM */
 		init_big_from_rom(sa,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter sa from ROM */
		init_big_from_rom(sb,words,(const mr_small *)rom,words*ROM_SIZE,&promptr);  /* Read in curve parameter sb from ROM */


		convert(_MIPP_ -3,a);
		add(_MIPP_ q,a,a);

		incr(_MIPP_ q,1,cf);
		divide(_MIPP_ cf,r,cf); /* cofactor (q+1)/r */

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
		nres(_MIPP_ sa,sa); /* convert to n-residue form */
		nres(_MIPP_ sb,sb);

		big_to_bytes(_MIPP_ SFS,q,DOM->Q,TRUE);
		big_to_bytes(_MIPP_ SFS,a,DOM->A,TRUE);
		big_to_bytes(_MIPP_ SFS,b,DOM->B,TRUE);
		big_to_bytes(_MIPP_ SGS,r,DOM->R,TRUE);
		big_to_bytes(_MIPP_ SFS,px,DOM->Px,TRUE);
		big_to_bytes(_MIPP_ SFS,py,DOM->Py,TRUE);
		big_to_bytes(_MIPP_ SFS,cf,DOM->CF,TRUE);
		big_to_bytes(_MIPP_ SFS,sa,DOM->Sa,TRUE);
		big_to_bytes(_MIPP_ SFS,sb,DOM->Sb,TRUE);

#ifdef SAKKE_DEBUG
		nres_moddiv(_MIPP_ sb,sa,b);
		redc(_MIPP_ b,b);
		printf("g= ");otnum(_MIPP_ b,stdout);
#endif

	}

#endif

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_INIT_RESERVE);
    ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_INIT_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

void SAKKE_DOMAIN_KILL(sak_domain *DOM)
{
	int i,j;
	for (i=0;i<SFS;i++)
	{

		DOM->Q[i]=0;
		DOM->A[i]=0;
		DOM->B[i]=0;
		DOM->Px[i]=0;
		DOM->Py[i]=0;
#ifdef BN_CURVE
		DOM->X[i]=0;
		DOM->Beta[i]=0;
		DOM->Qxa[i]=0;
		DOM->Qxb[i]=0;		
		DOM->Qya[i]=0;		
		DOM->Qyb[i]=0;	
		DOM->Fa[i]=0;
		DOM->Fb[i]=0;
		for (j=0;j<16;j++)
			DOM->G[j][i]=0;
#else
		DOM->CF[i]=0;
		DOM->Sa[i]=0;
		DOM->Sb[i]=0;
#endif
	}
	for (i=0;i<SGS;i++)
		DOM->R[i]=0;
}

/* Recombine on G1 from 3 parts - R=3R1-3R2+R3  */

#define MR_SAKKE_RECOMBINE_G1_RESERVE 5

int SAKKE_RECOMBINE_G1(sak_domain *DOM,octet *R1,octet *R2,octet *R3,octet *R)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,rx,ry;
    epoint *P,*T;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_RECOMBINE_G1_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G1_RESERVE)];
    char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G1_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        rx=mirvar_mem(_MIPP_ mem, num++);
        ry=mirvar_mem(_MIPP_ mem, num++);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
		T=epoint_init_mem(_MIPP_ mem1,1);

		bytes_to_big(_MIPP_ SFS,&(R1->val[1]),rx);
		bytes_to_big(_MIPP_ SFS,&(R1->val[SFS+1]),ry);
        if (!epoint_set(_MIPP_ rx,ry,0,P)) res=SAKKE_INVALID_POINT;

		bytes_to_big(_MIPP_ SFS,&(R2->val[1]),rx);
		bytes_to_big(_MIPP_ SFS,&(R2->val[SFS+1]),ry);
        if (!epoint_set(_MIPP_ rx,ry,0,T)) res=SAKKE_INVALID_POINT; 
	}
    if (res==0)
    {
		ecurve_sub(_MIPP_ T,P);  /* R = R1-R2 */
		epoint_norm(_MIPP_ P);
		epoint_copy(P,T);
		ecurve_double(_MIPP_ P);
		ecurve_add(_MIPP_ T,P); /* R=3(R1-R2) */
		bytes_to_big(_MIPP_ SFS,&(R3->val[1]),rx);
		bytes_to_big(_MIPP_ SFS,&(R3->val[SFS+1]),ry);
        if (!epoint_set(_MIPP_ rx,ry,0,T)) res=SAKKE_INVALID_POINT; 
	}
    if (res==0)
    {
		ecurve_add(_MIPP_ T,P);
		epoint_norm(_MIPP_ P);  /* R=3(R1-R2)+R3 */
        epoint_get(_MIPP_ P,rx,ry);

		R->len=2*SFS+1;	R->val[0]=4;
		big_to_bytes(_MIPP_ SFS,rx,&(R->val[1]),TRUE);
		big_to_bytes(_MIPP_ SFS,ry,&(R->val[SFS+1]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_RECOMBINE_G1_RESERVE);
    ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G1_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* Recombine on G2 from 3 parts - W=3W1-3W2+W3  */

#ifdef BN_CURVE

#define MR_SAKKE_RECOMBINE_G2_RESERVE 19

int SAKKE_RECOMBINE_G2(sak_domain *DOM,octet *W1,octet *W2,octet *W3,octet *W)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b;
	zzn2 qx,qy; 
    ecn2 Q,T;
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_RECOMBINE_G2_RESERVE);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G2_RESERVE)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G2_RESERVE));
#endif
    if (mr_mip==NULL || mem==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		ecn2_alloc(_MIPP_ &T,mem,&num);

		flags=DOM->flags;
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);

        bytes_to_big(_MIPP_ SFS,&(W1->val[0]),qx.a); 
        bytes_to_big(_MIPP_ SFS,&(W1->val[SFS]),qx.b); 
        bytes_to_big(_MIPP_ SFS,&(W1->val[2*SFS]),qy.a); 
        bytes_to_big(_MIPP_ SFS,&(W1->val[3*SFS]),qy.b); 
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&Q)) res=SAKKE_INVALID_POINT;

        bytes_to_big(_MIPP_ SFS,&(W2->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(W2->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(W2->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(W2->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);
		if (!ecn2_set(_MIPP_ &qx,&qy,&T)) res=SAKKE_INVALID_POINT;
	}
    if (res==0)
    {
		ecn2_sub(_MIPP_ &T,&Q);  /* R = R1-R2 */
		ecn2_norm(_MIPP_ &Q);
		ecn2_copy(&Q,&T);
		ecn2_add(_MIPP_ &Q,&Q);
		ecn2_add(_MIPP_ &T,&Q); /* R=3(R1-R2) */

        bytes_to_big(_MIPP_ SFS,&(W3->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(W3->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(W3->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(W3->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&T)) res=SAKKE_INVALID_POINT;
	}
    if (res==0)
    {
		ecn2_add(_MIPP_ &T,&Q);
		ecn2_norm(_MIPP_ &Q);  /* R=3(R1-R2)+R3 */
        ecn2_getxy(&Q,&qx,&qy);

		redc(_MIPP_ qx.a,qx.a); redc(_MIPP_ qx.b,qx.b); redc(_MIPP_ qy.a,qy.a); redc(_MIPP_ qy.b,qy.b);
		W->len=4*SFS;
		big_to_bytes(_MIPP_ SFS,qx.a,&(W->val[0]),TRUE);
		big_to_bytes(_MIPP_ SFS,qx.b,&(W->val[SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.a,&(W->val[2*SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.b,&(W->val[3*SFS]),TRUE);
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_RECOMBINE_G2_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_RECOMBINE_G2_RESERVE));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#endif


#ifdef BN_CURVE
#define MR_SAKKE_BN_ENCAPSULATE_RESERVE 27
#else
#define MR_SAKKE_BN_ENCAPSULATE_RESERVE 10
#endif

/* Parameters are Secret Key SSV, DPKG Public key Z, TP =0/1 Time permits, ID of recipient */
/* ALWAYS encapsulate using todays date */

int SAKKE_KEY_ENCAPSULATE(sak_domain *DOM,octet *SSV,octet *Z,int TP,octet *ID,octet *SC)
{
	int m,n,flags,num=0;
#ifdef BN_CURVE
	char ww[4*SFS];
	octet W={0,sizeof(ww),ww};
#endif
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};

#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,u,v,px,py;
    epoint *P,*ZS;
#ifdef BN_CURVE
	big x,t,beta;
	zzn4 g,gp,gpm1,gpm2;
#else
	big sa,sb;
	zzn2 S;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_ENCAPSULATE_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_ENCAPSULATE_RESERVE)];
    char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_ENCAPSULATE_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {

        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        u=mirvar_mem(_MIPP_ mem, num++);
        v=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
#ifdef BN_CURVE
		x=mirvar_mem(_MIPP_ mem, num++);
        t=mirvar_mem(_MIPP_ mem, num++);	
        beta=mirvar_mem(_MIPP_ mem, num++);
		zzn4_alloc(_MIPP_ &g,mem,&num);
		zzn4_alloc(_MIPP_ &gp,mem,&num);
		zzn4_alloc(_MIPP_ &gpm1,mem,&num);
		zzn4_alloc(_MIPP_ &gpm2,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

		bytes_to_big(_MIPP_ SFS,DOM->Beta,beta);
#else
		sa=mirvar_mem(_MIPP_ mem, num++);
		sb=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Sa,sa);
        bytes_to_big(_MIPP_ SFS,DOM->Sb,sb);

#endif
        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
#ifdef BN_CURVE
		hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);
#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif
		if (TP)
		{
			n=today();
			convert(_MIPP_ n,a);
			add(_MIPP_ b,a,b);
		}
		HT.len=big_to_bytes(_MIPP_ SGS,b,HT.val,FALSE);

		hash(SSV,-1,&HT,NULL,&HT);	

		bytes_to_big(_MIPP_ HT.len,HT.val,u);
		HashToIntegerRange(_MIPP_ u,r,v);

		bytes_to_big(_MIPP_ SFS,&(Z->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(Z->val[SFS+1]),py);
		ZS=epoint_init_mem(_MIPP_ mem1,1);
		if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT; 
	}

#ifdef BN_CURVE
	if (res==0)
	{
		bytes_to_big(_MIPP_ SFS,DOM->G[0],g.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[1],g.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[2],g.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[3],g.b.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[4],gp.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[5],gp.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[6],gp.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[7],gp.b.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[8],gpm1.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[9],gpm1.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[10],gpm1.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[11],gpm1.b.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[12],gpm2.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[13],gpm2.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[14],gpm2.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[15],gpm2.b.b);

		copy(q,t);    /* divide exponent up into v mod (t-1) and v div (t-1). Note these numbers are half size - hence the speed-up */
		divide(_MIPP_ t,r,r);
		copy(v,a);
		divide(_MIPP_ a,t,u);

		xtr_pow2(_MIPP_ &gp,&g,&gpm1,&gpm2,a,u,&g); /* g^v - this is faster */

/* hash g */

		redc(_MIPP_ g.a.a,g.a.a); redc(_MIPP_ g.a.b,g.a.b); redc(_MIPP_ g.b.a,g.b.a); redc(_MIPP_ g.b.b,g.b.b);

		W.len=4*SFS;
		big_to_bytes(_MIPP_ SFS,g.a.a,&(W.val[0]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.a.b,&(W.val[SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.b.a,&(W.val[2*SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.b.b,&(W.val[3*SFS]),TRUE);

		hash(&W,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,t);
		expb2(_MIPP_ SAS*8,u);
		HashToIntegerRange(_MIPP_ t,u,t);

		SC->len=big_to_bytes(_MIPP_ SAS,t,SC->val,TRUE);
		OCTET_XOR(SSV,SC);

		//bytes_to_big(_MIPP_ ID->len,ID->val,b);
		//hash(ID,-1,NULL,NULL,&HT);
		//bytes_to_big(_MIPP_ HT.len,HT.val,b);
		//HashToIntegerRange(_MIPP_ b,r,b);

		G1_mult(_MIPP_ P,b,beta,r,x,P); 
		ecurve_add(_MIPP_ ZS,P);
		epoint_norm(_MIPP_ P);

		G1_mult(_MIPP_ P,v,beta,r,x,P);
		epoint_norm(_MIPP_ P);
        epoint_get(_MIPP_ P,px,py);

		SC->len=2*SFS+SAS+1; SC->val[SAS]=4;
		big_to_bytes(_MIPP_ SFS,px,&(SC->val[SAS+1]),TRUE);
		big_to_bytes(_MIPP_ SFS,py,&(SC->val[SAS+SFS+1]),TRUE);
	}
#else
	if (res==0)
	{
		S.a=sa;
		S.b=sb;
		zzn2_pow(_MIPP_ &S,v,&S);

		nres_moddiv(_MIPP_ sb,sa,sb);
		redc(_MIPP_ sb,sb);
#ifdef SAKKE_DEBUG
		printf("g^r= ");
		otnum(_MIPP_ sb,stdout);
#endif

		SC->len=big_to_bytes(_MIPP_ SFS,sb,SC->val,TRUE);
		hash(SC,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,sb);
		expb2(_MIPP_ SAS*8,u);
		HashToIntegerRange(_MIPP_ sb,u,sb);
		SC->len=big_to_bytes(_MIPP_ SAS,sb,SC->val,TRUE);
		OCTET_XOR(SSV,SC);

		mad(_MIPP_ b,v,b,r,r,b);
		ecurve_mult2(_MIPP_ b,P,v,ZS,P);
		epoint_get(_MIPP_ P,px,py);

		SC->len=2*SFS+SAS+1;	SC->val[SAS]=4;
		big_to_bytes(_MIPP_ SFS,px,&(SC->val[SAS+1]),TRUE);
		big_to_bytes(_MIPP_ SFS,py,&(SC->val[SAS+SFS+1]),TRUE);

	}
#endif
	if (res==0 && TP)
	{ // append date of encapsulation
			m=2*SFS+SAS+1;
			SC->len=m+4;
			SC->val[m++]=(n>>24)&0xff;
			SC->val[m++]=(n>>16)&0xff;
			SC->val[m++]=(n>>8)&0xff;
			SC->val[m++]=n&0xff;
	}
#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_ENCAPSULATE_RESERVE);
    ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_ENCAPSULATE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#ifdef BN_CURVE
#define MR_SAKKE_BN_GET_VALIDATE_RESERVE 36
#else
#define MR_SAKKE_BN_GET_VALIDATE_RESERVE 9
#endif

/* Here Z is the PKG Public Key */
/* K is ID's Private Key */

int SAKKE_SECRET_KEY_VALIDATE(sak_domain *DOM,octet *ID,octet *Z,octet *K)
{
	int i,flags,num=0;
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,px,py;
	epoint *P,*ZS;
#ifdef BN_CURVE
	big x,beta;
    ecn2 Q;
	zzn2 f,qx,qy;
	zzn4 g;
	zzn12 g1;
#else
	big cf,sa,sb;
	zzn2 S;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_GET_VALIDATE_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 2);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_GET_VALIDATE_RESERVE)];
    char mem1[MR_ECP_RESERVE(2)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_VALIDATE_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(2));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {

        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);
#ifdef BN_CURVE
		x=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		zzn4_alloc(_MIPP_ &g,mem,&num);
		zzn12_alloc(_MIPP_ &g1,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

		bytes_to_big(_MIPP_ SFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ SFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ SFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qyb,qy.b);
        bytes_to_big(_MIPP_ SFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ SFS,DOM->Fb,f.b);		
#else
	    cf=mirvar_mem(_MIPP_ mem, num++);
        sa=mirvar_mem(_MIPP_ mem, num++);
        sb=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->CF,cf);
		bytes_to_big(_MIPP_ SFS,DOM->Sa,sa);
        bytes_to_big(_MIPP_ SFS,DOM->Sb,sb);
#endif

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{		
#ifdef BN_CURVE
       	hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);
#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif
		
		bytes_to_big(_MIPP_ SFS,&(Z->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(Z->val[SFS+1]),py);
        ZS=epoint_init_mem(_MIPP_ mem1,1);
        if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT;
	}
#ifdef BN_CURVE
	if (res==0)
	{
		G1_mult(_MIPP_ P,b,beta,r,x,P);
		ecurve_add(_MIPP_ ZS,P);
		epoint_norm(_MIPP_ P);

        bytes_to_big(_MIPP_ SFS,&(K->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(K->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q)) res=SAKKE_INVALID_POINT;
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ SFS,DOM->G[0],g.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[1],g.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[2],g.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[3],g.b.b);

		rate_miller(_MIPP_ &Q,P,x,&f,&g1);
		rate_fexp(_MIPP_ x,&f,&g1);

		trace(_MIPP_ &g1,&(g1.a));

		if (!zzn4_compare(&g,&(g1.a))) res=SAKKE_BAD_KEY;
	}
#else
	if (res==0)
	{
		ecurve_mult(_MIPP_ b,P,P);
		ecurve_add(_MIPP_ ZS,P);
		bytes_to_big(_MIPP_ SFS,&(K->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(K->val[SFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT; 
	}
	if (res==0)
	{
		S.a=px;
		S.b=py;
		tate(_MIPP_ P,ZS,r,cf,&S);
		nres_modmult(_MIPP_ px,sb,px);
		nres_modmult(_MIPP_ py,sa,py);
		if (mr_compare(px,py)!=0) res=SAKKE_BAD_KEY;
	}
#endif

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_GET_VALIDATE_RESERVE);
    ecp_memkill(_MIPP_ mem1,2);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_VALIDATE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(2));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#ifdef BN_CURVE
#define MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE 42
#define MR_SAKKE_BN_PERMIT_VALIDATE_POINTS 2
#else
#define MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE 9
#define MR_SAKKE_BN_PERMIT_VALIDATE_POINTS 3
#endif

int SAKKE_PERMIT_VALIDATE(sak_domain *DOM,int date,octet *ID,octet *Z,octet *K,octet *PMT)
{
	int i,flags,num=0;
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,px,py;
	epoint *P,*ZS;
#ifdef BN_CURVE
	big x,beta;
    ecn2 Q,T;
	zzn2 f,qx,qy;
	zzn4 g;
	zzn12 g1;
#else
	epoint *T;
	big cf,sa,sb;
	zzn2 S;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ MR_SAKKE_BN_PERMIT_VALIDATE_POINTS);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE)];
    char mem1[MR_ECP_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_POINTS)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_POINTS));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {

        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

#ifdef BN_CURVE
		x=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);

		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		ecn2_alloc(_MIPP_ &T,mem,&num);
		zzn4_alloc(_MIPP_ &g,mem,&num);
		zzn12_alloc(_MIPP_ &g1,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 


		bytes_to_big(_MIPP_ SFS,DOM->Beta,beta);
        bytes_to_big(_MIPP_ SFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ SFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qyb,qy.b);
        bytes_to_big(_MIPP_ SFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ SFS,DOM->Fb,f.b);		
#else
	    cf=mirvar_mem(_MIPP_ mem, num++);
        sa=mirvar_mem(_MIPP_ mem, num++);
        sb=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->CF,cf);
		bytes_to_big(_MIPP_ SFS,DOM->Sa,sa);
        bytes_to_big(_MIPP_ SFS,DOM->Sb,sb);
#endif

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{		
#ifdef BN_CURVE

       	hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);

#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif

		bytes_to_big(_MIPP_ SFS,&(Z->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(Z->val[SFS+1]),py);
        ZS=epoint_init_mem(_MIPP_ mem1,1);
        if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT;
	}
#ifdef BN_CURVE
	if (res==0)
	{
		convert(_MIPP_ date,a);
		add(_MIPP_ b,a,b);

		G1_mult(_MIPP_ P,b,beta,r,x,P);
		ecurve_add(_MIPP_ ZS,P);
		epoint_norm(_MIPP_ P);

        bytes_to_big(_MIPP_ SFS,&(K->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(K->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q)) res=SAKKE_INVALID_POINT;
	}
	if (res==0)
	{
        bytes_to_big(_MIPP_ SFS,&(PMT->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);  
		if (!ecn2_set(_MIPP_ &qx,&qy,&T))  res=SAKKE_INVALID_POINT;
	}
	if (res==0)
	{
		ecn2_add(_MIPP_ &T,&Q);
		ecn2_norm(_MIPP_ &Q);
		bytes_to_big(_MIPP_ SFS,DOM->G[0],g.a.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[1],g.a.b);
		bytes_to_big(_MIPP_ SFS,DOM->G[2],g.b.a);
		bytes_to_big(_MIPP_ SFS,DOM->G[3],g.b.b);

		rate_miller(_MIPP_ &Q,P,x,&f,&g1);
		rate_fexp(_MIPP_ x,&f,&g1);

		trace(_MIPP_ &g1,&(g1.a));

		if (!zzn4_compare(&g,&(g1.a))) res=SAKKE_BAD_KEY;
	}
#else
	if (res==0)
	{
		convert(_MIPP_ date,a);
		add(_MIPP_ b,a,b);

		ecurve_mult(_MIPP_ b,P,P);
		ecurve_add(_MIPP_ ZS,P);
		bytes_to_big(_MIPP_ SFS,&(K->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(K->val[SFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT; 
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ SFS,&(PMT->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(PMT->val[SFS+1]),py);
        T=epoint_init_mem(_MIPP_ mem1,2);
		if (!epoint_set(_MIPP_ px,py,0,T)) res=SAKKE_INVALID_POINT; 
	}
	if (res==0)
	{
		ecurve_add(_MIPP_ T,ZS);
		epoint_norm(_MIPP_ ZS);

		S.a=px;
		S.b=py;
		tate(_MIPP_ P,ZS,r,cf,&S);
		nres_modmult(_MIPP_ px,sb,px);
		nres_modmult(_MIPP_ py,sa,py);
		if (mr_compare(px,py)!=0) res=SAKKE_BAD_KEY;
	}
#endif

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE);
    ecp_memkill(_MIPP_ mem1,MR_SAKKE_BN_PERMIT_VALIDATE_POINTS);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(MR_SAKKE_BN_PERMIT_VALIDATE_POINTS));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

/* extract from SAKKE ciphertext the date it was encapsulated. This will be needed so that correct time permit can be requested */

mr_unsign32 SAKKE_GET_TIME_SLOT(octet *SC)
{
	int m=2*SFS+SAS+1;
	if (SC->len>2*SFS+SAS+1) return cv_getdate(&(SC->val[m]));
	return 0;
}

/* Decapsulate SAKKE ciphertext to get AES key */

#ifdef BN_CURVE
#define MR_SAKKE_BN_DECAPSULATE_RESERVE 40
#else
#define MR_SAKKE_BN_DECAPSULATE_RESERVE 9
#endif

int SAKKE_KEY_DECAPSULATE(sak_domain *DOM,octet *SC,octet *Z,octet *ID,octet *K,octet *PMT,octet *SSV)
{
	int n,flags,num=0;
#ifdef BN_CURVE
	char ww[4*SFS];
	octet W={0,sizeof(ww),ww};
#endif
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};

#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,px,py,t,u;
	epoint *P,*ZS,*RC;
#ifdef BN_CURVE
	big x,beta;
    ecn2 Q,Q2;
	zzn2 f,qx,qy;
	zzn12 g;
#else
	zzn2 S;
	big cf;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_DECAPSULATE_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 3);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_DECAPSULATE_RESERVE)];
    char mem1[MR_ECP_RESERVE(3)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_DECAPSULATE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(3));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
	    u=mirvar_mem(_MIPP_ mem, num++);
        t=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);

#ifdef BN_CURVE
		x=mirvar_mem(_MIPP_ mem, num++);
		beta=mirvar_mem(_MIPP_ mem,num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);
		ecn2_alloc(_MIPP_ &Q2,mem,&num);
		zzn12_alloc(_MIPP_ &g,mem,&num);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

		bytes_to_big(_MIPP_ SFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ SFS,DOM->Fb,f.b);	
		bytes_to_big(_MIPP_ SFS,DOM->Beta,beta);

#else
		cf=mirvar_mem(_MIPP_ mem, num++);
        bytes_to_big(_MIPP_ SFS,DOM->CF,cf);

#endif

        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
		ZS=epoint_init_mem(_MIPP_ mem1,1);
		RC=epoint_init_mem(_MIPP_ mem1,2);
/* extract R into RC */

//printf("SC= ");OCTET_OUTPUT(SC);

		bytes_to_big(_MIPP_ SFS,&(SC->val[SAS+1]),px);
		bytes_to_big(_MIPP_ SFS,&(SC->val[SFS+SAS+1]),py);
//printf("px= "); otnum(_MIPP_ px,stdout);
//printf("py= "); otnum(_MIPP_ py,stdout);

        if (!epoint_set(_MIPP_ px,py,0,RC)) res=SAKKE_INVALID_POINT;
	}

	if (res==0)
	{
/* extract K */
#ifdef BN_CURVE
        bytes_to_big(_MIPP_ SFS,&(K->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(K->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(K->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);

		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=SAKKE_INVALID_POINT;
#else
		bytes_to_big(_MIPP_ SFS,&(K->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(K->val[SFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,P)) res=SAKKE_INVALID_POINT; 
#endif
	}
	if (res==0 && PMT!=NULL)
	{
/* extract PMT and add it on */
#ifdef BN_CURVE	  
        bytes_to_big(_MIPP_ SFS,&(PMT->val[0]),qx.a);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[SFS]),qx.b);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[2*SFS]),qy.a);
        bytes_to_big(_MIPP_ SFS,&(PMT->val[3*SFS]),qy.b);
		nres(_MIPP_ qx.a,qx.a); nres(_MIPP_ qx.b,qx.b); nres(_MIPP_ qy.a,qy.a); nres(_MIPP_ qy.b,qy.b);  

		if (!ecn2_set(_MIPP_ &qx,&qy,&Q2))  res=SAKKE_INVALID_POINT;
		ecn2_add(_MIPP_ &Q2,&Q);
		ecn2_norm(_MIPP_ &Q);
#else
		bytes_to_big(_MIPP_ SFS,&(PMT->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(PMT->val[SFS+1]),py);
		if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT; 
		ecurve_add(_MIPP_ ZS,P);
		epoint_norm(_MIPP_ P);
#endif
	}
	if (res==0)
	{
		bytes_to_big(_MIPP_ SFS,&(Z->val[1]),px);
		bytes_to_big(_MIPP_ SFS,&(Z->val[SFS+1]),py);
        if (!epoint_set(_MIPP_ px,py,0,ZS)) res=SAKKE_INVALID_POINT;
	}

	if (res==0)
	{
#ifdef BN_CURVE
		rate_miller(_MIPP_ &Q,RC,x,&f,&g);
		rate_fexp(_MIPP_ x,&f,&g);

		trace(_MIPP_ &g,&(g.a));

		redc(_MIPP_ g.a.a.a,g.a.a.a); redc(_MIPP_ g.a.a.b,g.a.a.b); redc(_MIPP_ g.a.b.a,g.a.b.a); redc(_MIPP_ g.a.b.b,g.a.b.b);
//printf("g.a.a.a= "); otnum(_MIPP_ g.a.a.a,stdout);
		W.len=4*SFS;
		big_to_bytes(_MIPP_ SFS,g.a.a.a,&(W.val[0]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.a.a.b,&(W.val[SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.a.b.a,&(W.val[2*SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,g.a.b.b,&(W.val[3*SFS]),TRUE);
		hash(&W,-1,NULL,NULL,&HT);
#else

		S.a=u;
		S.b=t;
		tate(_MIPP_ RC,P,r,cf,&S); /* tate(R,K) */

		nres_moddiv(_MIPP_ t,u,t);
		redc(_MIPP_ t,t);
#ifdef SAKKE_DEBUG
		printf("w= ");
		otnum(_MIPP_ t,stdout);
#endif
		HT.len=big_to_bytes(_MIPP_ SFS,t,HT.val,TRUE);
		hash(&HT,-1,NULL,NULL,&HT);

#endif
		bytes_to_big(_MIPP_ HT.len,HT.val,t);
		expb2(_MIPP_ SAS*8,u);
		HashToIntegerRange(_MIPP_ t,u,t);
		SSV->len=big_to_bytes(_MIPP_ SAS,t,SSV->val,TRUE);
		OCTET_XOR(SC,SSV);

#ifdef BN_CURVE
       	hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);

#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif

		if (PMT!=NULL)
		{
			n=cv_getdate(&(SC->val[2*SFS+SAS+1]));
			convert(_MIPP_ n,a);
			add(_MIPP_ b,a,b);
		}

		HT.len=big_to_bytes(_MIPP_ SGS,b,HT.val,FALSE);
		hash(SSV,-1,&HT,NULL,&HT);

		bytes_to_big(_MIPP_ HT.len,HT.val,u);
		HashToIntegerRange(_MIPP_ u,r,u);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}

	if (res==0)
	{
#ifdef BN_CURVE
		G1_mult(_MIPP_ P,b,beta,r,x,P); 
		ecurve_add(_MIPP_ ZS,P);
		epoint_norm(_MIPP_ P);
		G1_mult(_MIPP_ P,u,beta,r,x,P);  
		epoint_norm(_MIPP_ P);
#else
		mad(_MIPP_ b,u,b,r,r,b);
		ecurve_mult2(_MIPP_ b,P,u,ZS,P);
#endif
		if (!epoint_comp(_MIPP_ P,RC)) res=SAKKE_BAD_KEY;
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_DECAPSULATE_RESERVE);
    ecp_memkill(_MIPP_ mem1,3);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_DECAPSULATE_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(3));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#ifdef BUILD_PKG


/* Calculate a public/private EC GF(p) key pair. W=S.G mod EC(p),
 * where S is the secret key and W is the public key
 * If RNG is NULL then the private key is provided externally in S
 * otherwise it is generated randomly internally */

#ifdef BN_CURVE
#define MR_SAKKE_BN_MASTER_KEY_RESERVE 9
#else
#define MR_SAKKE_BN_MASTER_KEY_RESERVE 7
#endif

int SAKKE_MASTER_KEY_PAIR_GENERATE(sak_domain *DOM,csprng *RNG,octet* S,octet *W)
{
	int flags,num=0;
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,s,px,py;
    epoint *P;
#ifdef BN_CURVE
	big x,beta;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_MASTER_KEY_RESERVE);
    char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_MASTER_KEY_RESERVE)];
    char mem1[MR_ECP_RESERVE(1)];
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_MASTER_KEY_RESERVE));
	memset(mem1,0,MR_ECP_RESERVE(1));
#endif
 
    if (mr_mip==NULL || mem==NULL || mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);
        px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
#ifdef BN_CURVE
		x=mirvar_mem(_MIPP_ mem, num++);
        beta=mirvar_mem(_MIPP_ mem, num++);

		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 
		bytes_to_big(_MIPP_ SFS,DOM->Beta,beta);
#endif
        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

        if (RNG!=NULL)
            strong_bigrand(_MIPP_ RNG,r,s);
        else
        {
            bytes_to_big(_MIPP_ S->len,S->val,s);
            divide(_MIPP_ s,r,r);
        }
        ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P)) res=MR_ERR_BAD_PARAMETERS;
	}
	if (res==0)
	{
#ifdef BN_CURVE
		G1_mult(_MIPP_ P,s,beta,r,x,P); 
#else
        ecurve_mult(_MIPP_ s,P,P);  
#endif
        epoint_get(_MIPP_ P,px,py);

		W->len=2*SFS+1;	W->val[0]=4;
		big_to_bytes(_MIPP_ SFS,px,&(W->val[1]),TRUE);
		big_to_bytes(_MIPP_ SFS,py,&(W->val[SFS+1]),TRUE);
    
        if (RNG!=NULL) S->len=big_to_bytes(_MIPP_ SGS,s,S->val,TRUE);
    }

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_MASTER_KEY_RESERVE);
    ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_MASTER_KEY_RESERVE));
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#ifdef BN_CURVE
#define MR_SAKKE_BN_GET_PERMIT_RESERVE 18
#else
#define MR_SAKKE_BN_GET_PERMIT_RESERVE 7
#endif

int SAKKE_GET_USER_TIME_PERMIT(sak_domain *DOM,int date,octet* Z,octet *ID,octet *PMT)
{
	int flags,num=0;
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,s;
#ifdef BN_CURVE
	big x;
	zzn2 f,qx,qy; 
    ecn2 Q;
#else
	big px,py;
    epoint *P;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_GET_PERMIT_RESERVE);
#ifndef BN_CURVE
    char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#endif
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_GET_PERMIT_RESERVE)];
#ifndef BN_CURVE
    char mem1[MR_ECP_RESERVE(1)];
#endif
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_PERMIT_RESERVE));
#ifndef BN_CURVE
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
#endif
 
    if (mr_mip==NULL || mem==NULL) res= SAKKE_OUT_OF_MEMORY;
#ifndef BN_CURVE
    if (mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
#endif
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);

#ifdef BN_CURVE
        x=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);

 		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ SFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ SFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qyb,qy.b);
        bytes_to_big(_MIPP_ SFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ SFS,DOM->Fb,f.b);
		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;

#else
	    px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P))  res=MR_ERR_BAD_PARAMETERS;
 #endif
	}
	if (res==0)
	{
#ifdef BN_CURVE

       	hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);
#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif
        bytes_to_big(_MIPP_ Z->len,Z->val,s);

		add(_MIPP_ b,s,s); /* s=S+A */
		convert(_MIPP_ date,a);
		add(_MIPP_ a,s,b); /* b=S+A+T */
		mad(_MIPP_ b,s,s,r,r,b);
		xgcd(_MIPP_ b,r,b,b,b); /* b=1/(S+A)(S+A+T) */
		negify(a,a);
		mad(_MIPP_ a,b,a,r,r,b);       /* b=-T/(S+A+T)(S+A) */

#ifdef BN_CURVE
		G2_mult(_MIPP_ &Q,b,&f,r,x,&Q);  
                /* ecn2_mul(_MIPP_ s, &Q); */
		ecn2_norm(_MIPP_ &Q);
        ecn2_getxy(&Q,&qx,&qy);

		redc(_MIPP_ qx.a,qx.a); redc(_MIPP_ qx.b,qx.b); redc(_MIPP_ qy.a,qy.a); redc(_MIPP_ qy.b,qy.b);
		PMT->len=4*SFS;
		big_to_bytes(_MIPP_ SFS,qx.a,&(PMT->val[0]),TRUE);
		big_to_bytes(_MIPP_ SFS,qx.b,&(PMT->val[SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.a,&(PMT->val[2*SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.b,&(PMT->val[3*SFS]),TRUE);
#else
        ecurve_mult(_MIPP_ b,P,P);        
        epoint_get(_MIPP_ P,px,py);

		PMT->len=2*SFS+1;	PMT->val[0]=4;
		big_to_bytes(_MIPP_ SFS,px,&(PMT->val[1]),TRUE);
		big_to_bytes(_MIPP_ SFS,py,&(PMT->val[SFS+1]),TRUE);
#endif
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_GET_PERMIT_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_PERMIT_RESERVE));
#endif
#ifndef BN_CURVE
#ifndef MR_STATIC
    ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
#endif
    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#ifdef BN_CURVE
#define MR_SAKKE_BN_GET_SECRET_RESERVE 18
#else
#define MR_SAKKE_BN_GET_SECRET_RESERVE 7
#endif

int SAKKE_GET_USER_SECRET_KEY(sak_domain *DOM,octet* Z,octet *ID,octet *RSK)
{
	int flags,num=0;
	char ht[SFS];
	octet HT={0,sizeof(ht),ht};
#ifdef MR_GENERIC_AND_STATIC
	miracl instance;
	miracl *mr_mip=mirsys(&instance,DOM->nibbles,16);
#else
	miracl *mr_mip=mirsys(DOM->nibbles,16);
#endif
    big q,a,b,r,s;
#ifdef BN_CURVE
	big x;
	zzn2 f,qx,qy; 
    ecn2 Q;
#else
	big px,py;
    epoint *P;
#endif
    int err,res=0;
#ifndef MR_STATIC
    char *mem=(char *)memalloc(_MIPP_ MR_SAKKE_BN_GET_SECRET_RESERVE);
#ifndef BN_CURVE
    char *mem1=(char *)ecp_memalloc(_MIPP_ 1);
#endif
#else
    char mem[MR_BIG_RESERVE(MR_SAKKE_BN_GET_SECRET_RESERVE)];
#ifndef BN_CURVE
    char mem1[MR_ECP_RESERVE(1)];
#endif
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_SECRET_RESERVE));
#ifndef BN_CURVE
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
#endif
 
    if (mr_mip==NULL || mem==NULL) res= SAKKE_OUT_OF_MEMORY;
#ifndef BN_CURVE
    if (mem1==NULL) res= SAKKE_OUT_OF_MEMORY;
#endif
    mr_mip->ERCON=TRUE;

    if (res==0)
    {
        q=mirvar_mem(_MIPP_ mem, num++);
        a=mirvar_mem(_MIPP_ mem, num++);
        b=mirvar_mem(_MIPP_ mem, num++);
        r=mirvar_mem(_MIPP_ mem, num++);
        s=mirvar_mem(_MIPP_ mem, num++);

        bytes_to_big(_MIPP_ SFS,DOM->Q,q);
        bytes_to_big(_MIPP_ SFS,DOM->A,a);
        bytes_to_big(_MIPP_ SFS,DOM->B,b);
        bytes_to_big(_MIPP_ SGS,DOM->R,r);

#ifdef BN_CURVE
        x=mirvar_mem(_MIPP_ mem, num++);
		zzn2_alloc(_MIPP_ &qx,mem,&num);
		zzn2_alloc(_MIPP_ &qy,mem,&num);
		zzn2_alloc(_MIPP_ &f,mem,&num);
		ecn2_alloc(_MIPP_ &Q,mem,&num);

 		flags=DOM->flags;

        bytes_to_big(_MIPP_ SFS,DOM->X,x);
		mr_mip->TWIST=MR_SEXTIC_D;
		if ((flags&1)==1) mr_mip->TWIST=MR_SEXTIC_M;
		if ((flags&2)==2) negify(x,x); 

        bytes_to_big(_MIPP_ SFS,DOM->Qxa,qx.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qxb,qx.b);
        bytes_to_big(_MIPP_ SFS,DOM->Qya,qy.a);
        bytes_to_big(_MIPP_ SFS,DOM->Qyb,qy.b);
        bytes_to_big(_MIPP_ SFS,DOM->Fa,f.a);
        bytes_to_big(_MIPP_ SFS,DOM->Fb,f.b);
		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
		if (!ecn2_set(_MIPP_ &qx,&qy,&Q))  res=MR_ERR_BAD_PARAMETERS;

#else
	    px=mirvar_mem(_MIPP_ mem, num++);
        py=mirvar_mem(_MIPP_ mem, num++);
        bytes_to_big(_MIPP_ SFS,DOM->Px,px);
        bytes_to_big(_MIPP_ SFS,DOM->Py,py);

		ecurve_init(_MIPP_ a,b,q,MR_PROJECTIVE);
        P=epoint_init_mem(_MIPP_ mem1,0);
        if (!epoint_set(_MIPP_ px,py,0,P))  res=MR_ERR_BAD_PARAMETERS;
 #endif

	}
	if (res==0)
	{
#ifdef BN_CURVE

       	hash(ID,-1,NULL,NULL,&HT);
		bytes_to_big(_MIPP_ HT.len,HT.val,b);
		HashToIntegerRange(_MIPP_ b,r,b);

#else
		bytes_to_big(_MIPP_ ID->len,ID->val,b);
		divide(_MIPP_ b,r,r);
#endif

        bytes_to_big(_MIPP_ Z->len,Z->val,s);
		add(_MIPP_ b,s,s);
		xgcd(_MIPP_ s,r,a,a,a);

#ifdef BN_CURVE
		G2_mult(_MIPP_ &Q,a,&f,r,x,&Q);  
                /* ecn2_mul(_MIPP_ a, &Q); */
		ecn2_norm(_MIPP_ &Q);
        ecn2_getxy(&Q,&qx,&qy);

		redc(_MIPP_ qx.a,qx.a); redc(_MIPP_ qx.b,qx.b); redc(_MIPP_ qy.a,qy.a); redc(_MIPP_ qy.b,qy.b);
		RSK->len=4*SFS;
		big_to_bytes(_MIPP_ SFS,qx.a,&(RSK->val[0]),TRUE);
		big_to_bytes(_MIPP_ SFS,qx.b,&(RSK->val[SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.a,&(RSK->val[2*SFS]),TRUE);
		big_to_bytes(_MIPP_ SFS,qy.b,&(RSK->val[3*SFS]),TRUE);
#else
        ecurve_mult(_MIPP_ a,P,P);        
        epoint_get(_MIPP_ P,px,py);

		RSK->len=2*SFS+1;	RSK->val[0]=4;
		big_to_bytes(_MIPP_ SFS,px,&(RSK->val[1]),TRUE);
		big_to_bytes(_MIPP_ SFS,py,&(RSK->val[SFS+1]),TRUE);

#endif
	}

#ifndef MR_STATIC
    memkill(_MIPP_ mem,MR_SAKKE_BN_GET_SECRET_RESERVE);
#else
    memset(mem,0,MR_BIG_RESERVE(MR_SAKKE_BN_GET_SECRET_RESERVE));
#endif
#ifndef BN_CURVE
#ifndef MR_STATIC
    ecp_memkill(_MIPP_ mem1,1);
#else
    memset(mem1,0,MR_ECP_RESERVE(1));
#endif
#endif

    err=mr_mip->ERNUM;
    mirexit(_MIPPO_ );
    if (err==MR_ERR_OUT_OF_MEMORY) return SAKKE_OUT_OF_MEMORY;
    if (err==MR_ERR_DIV_BY_ZERO) return SAKKE_DIV_BY_ZERO;
    if (err!=0) return -(1000+err);
    return res;
}

#else /* BUILD_PKG */

int SAKKE_MASTER_KEY_PAIR_GENERATE(sak_domain *DOM,csprng *RNG,octet* S,octet *W)
{
  return 0;
}


int SAKKE_GET_USER_SECRET_KEY(sak_domain *DOM,octet* Z,octet *ID,octet *RSK)
{
  return 0;
}

int SAKKE_GET_USER_TIME_PERMIT(sak_domain *DOM,int date,octet* Z,octet *ID,octet *PMT)
{
  return 0;
}

#endif   /* BUILD_PKG */
