
/* test driver and function exerciser for Brian's Credit Card idea */

/* gcc -O3 brian.c mpin_c.c clint.a -o brian.exe */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpin.h"

int main()
{
	int i,pin,pin1,pin2,pin3,pin4,err;
	int ccn1,ccn2,ccn3,ccn4,cvv;
	int date=0;

	unsigned long ran;
	char x[PGS],s[PGS],y[PGS],client_id1[100],client_id2[100],client_id3[100],client_id4[100],client_id5[100],raw[100],sst[4*PFS],token1[2*PFS+1],token2[2*PFS+1],token3[2*PFS+1],token4[2*PFS+1],token5[2*PFS+1],sec[2*PFS+1],xid[2*PFS+1],e[12*PFS],f[12*PFS];
	octet S={0,sizeof(s),s};
	octet X={0,sizeof(x),x};
	octet Y={0,sizeof(y),y};
	octet RAW={0,sizeof(raw),raw};
	octet CLIENT_ID1={0,sizeof(client_id1),client_id1};
	octet CLIENT_ID2={0,sizeof(client_id2),client_id2};
	octet CLIENT_ID3={0,sizeof(client_id3),client_id3};
	octet CLIENT_ID4={0,sizeof(client_id4),client_id4};
	octet CLIENT_ID5={0,sizeof(client_id5),client_id5};
	octet SST={0,sizeof(sst),sst};
	octet TOKEN1={0,sizeof(token1),token1};
	octet TOKEN2={0,sizeof(token2),token2};
	octet TOKEN3={0,sizeof(token3),token3};
	octet TOKEN4={0,sizeof(token4),token4};
	octet TOKEN5={0,sizeof(token5),token5};
	octet SEC={0,sizeof(sec),sec};
	octet xID={0,sizeof(xid),xid};
	octet E={0,sizeof(e),e};
	octet F={0,sizeof(f),f};
	
    csprng RNG;                /* Crypto Strong RNG */
                               /* fake random seed source */
	time((time_t *)&ran);

    RAW.len=100;
    RAW.val[0]=ran;
    RAW.val[1]=ran>>8;
    RAW.val[2]=ran>>16;
    RAW.val[3]=ran>>24;
    for (i=4;i<100;i++) RAW.val[i]=i+1;    
	
	CREATE_CSPRNG(&RNG,&RAW);   /* initialise strong RNG */

/* Trusted Authority set-up */

	MPIN_RANDOM_GENERATE(&RNG,&S);

/* Server Secret issued by TA */
	MPIN_GET_SERVER_SECRET(&S,&SST);
//	printf("Server Secret= "); OCT_output(&SST);


/* Client issued secrets by TA */
	OCT_jstring(&CLIENT_ID1,"fred_1");
	MPIN_GET_CLIENT_MULTIPLE(&S,&CLIENT_ID1,&TOKEN1);

	OCT_jstring(&CLIENT_ID2,"fred_2");
	MPIN_GET_CLIENT_MULTIPLE(&S,&CLIENT_ID2,&TOKEN2);

	OCT_jstring(&CLIENT_ID3,"fred_3");
	MPIN_GET_CLIENT_MULTIPLE(&S,&CLIENT_ID3,&TOKEN3);

	OCT_jstring(&CLIENT_ID4,"fred_4");
	MPIN_GET_CLIENT_MULTIPLE(&S,&CLIENT_ID4,&TOKEN4);

	OCT_jstring(&CLIENT_ID5,"fred_5");
	MPIN_GET_CLIENT_MULTIPLE(&S,&CLIENT_ID5,&TOKEN5);

// TA extracts PINs from secrets 

	printf("Present Credit card to TA\n");
	printf("4318 0002 2876 3692 (243)\n\n");

	pin1=4318;
	pin2=0002;
	pin3=2876;
	pin4=3692;
	cvv = 243;

	printf("TA extracts Credit Card Number= %04d\n",pin1);
	MPIN_EXTRACT_PIN(&CLIENT_ID1,pin1,&TOKEN1);
	printf("Client Secret= "); OCT_output(&TOKEN1);

	printf("TA extracts Credit Card Number= %04d\n",pin2);
	MPIN_EXTRACT_PIN(&CLIENT_ID2,pin2,&TOKEN2);
	printf("Client Secret= "); OCT_output(&TOKEN2);

	printf("TA extracts Credit Card Number= %04d\n",pin3);
	MPIN_EXTRACT_PIN(&CLIENT_ID3,pin3,&TOKEN3);
	printf("Client Secret= "); OCT_output(&TOKEN3);

	printf("TA extracts Credit Card Number= %04d\n",pin4);
	MPIN_EXTRACT_PIN(&CLIENT_ID4,pin4,&TOKEN4);
	printf("Client Secret= "); OCT_output(&TOKEN4);

	printf("TA extracts CVV= %03d\n",cvv);
	MPIN_EXTRACT_PIN(&CLIENT_ID5,cvv,&TOKEN5);
	printf("Client Secret= "); OCT_output(&TOKEN5);

// MPin Protocol

// Client First pass: Inputs CLIENT_ID, optional RNG, pin, TOKEN and PERMIT. Output x.H(CLIENT_ID) and re-combined secret SEC
// If PERMITS are is use, then date!=0 and PERMIT is added to secret and xCID = x.(H(CLIENT_ID)+H_T(date|CLIENT_ID))
// Random value x is supplied externally if RNG=NULL, otherwise generated and passed out by RNG

	printf("\nUsing M-Pin protocol to communicate Credit Card Number\n");

	do
	{
		pin=0; // do not use PIN
		if (MPIN_CLIENT_1(date,&CLIENT_ID1,&RNG,&X,pin,&TOKEN1,&SEC,&xID,NULL,NULL,NULL,NULL)!=0)
		{
			printf("Error from Client side - First Pass\n");
			return 0;
		}

// Server generates Random number Y and sends it to Client

		MPIN_RANDOM_GENERATE(&RNG,&Y);

//  Client Second Pass: Inputs Client secret SEC, x and y. Outputs -(x+y)*SEC

		if (MPIN_CLIENT_2(&X,&Y,&SEC)!=0)
		{
			printf("Error from Client side - Second Pass\n");
			return 0;
		}	
// Server Second pass. Inputs client id, random Y, -(x+y)*SEC, xID and xCID and Server secret SST. E and F help kangaroos to find error.

		MPIN_SERVER_1(date,&CLIENT_ID1,&Y,&SST,&xID,NULL,&SEC,&E,&F,NULL,NULL);
		ccn1=MPIN_KANGAROO(&E,&F);
		if (ccn1==0) printf("Failed - trying again\n");
	} while (ccn1==0);

	printf("Credit Card Number is %04d\n",-ccn1);


	do
	{
		pin=0; // do not use PIN
		if (MPIN_CLIENT_1(date,&CLIENT_ID2,&RNG,&X,pin,&TOKEN2,&SEC,&xID,NULL,NULL,NULL,NULL)!=0)
		{
			printf("Error from Client side - First Pass\n");
			return 0;
		}

// Server generates Random number Y and sends it to Client

		MPIN_RANDOM_GENERATE(&RNG,&Y);

//  Client Second Pass: Inputs Client secret SEC, x and y. Outputs -(x+y)*SEC

		if (MPIN_CLIENT_2(&X,&Y,&SEC)!=0)
		{
			printf("Error from Client side - Second Pass\n");
			return 0;
		}	
// Server Second pass. Inputs client id, random Y, -(x+y)*SEC, xID and xCID and Server secret SST. E and F help kangaroos to find error.

		MPIN_SERVER_1(date,&CLIENT_ID2,&Y,&SST,&xID,NULL,&SEC,&E,&F,NULL,NULL);
		ccn2=MPIN_KANGAROO(&E,&F);
		if (ccn2==0) printf("Failed - trying again\n");
	} while (ccn2==0);

	printf("Credit Card Number is %04d %04d\n",-ccn1,-ccn2);

	do
	{
		pin=0; // do not use PIN
		if (MPIN_CLIENT_1(date,&CLIENT_ID3,&RNG,&X,pin,&TOKEN3,&SEC,&xID,NULL,NULL,NULL,NULL)!=0)
		{
			printf("Error from Client side - First Pass\n");
			return 0;
		}

// Server generates Random number Y and sends it to Client

		MPIN_RANDOM_GENERATE(&RNG,&Y);

//  Client Second Pass: Inputs Client secret SEC, x and y. Outputs -(x+y)*SEC

		if (MPIN_CLIENT_2(&X,&Y,&SEC)!=0)
		{
			printf("Error from Client side - Second Pass\n");
			return 0;
		}	
// Server Second pass. Inputs client id, random Y, -(x+y)*SEC, xID and xCID and Server secret SST. E and F help kangaroos to find error.

		MPIN_SERVER_1(date,&CLIENT_ID3,&Y,&SST,&xID,NULL,&SEC,&E,&F,NULL,NULL);
		ccn3=MPIN_KANGAROO(&E,&F);
		if (ccn3==0) printf("Failed - trying again\n");
	} while (ccn3==0);

	printf("Credit Card Number is %04d %04d %04d\n",-ccn1,-ccn2,-ccn3);


	do
	{
		pin=0; // do not use PIN
		if (MPIN_CLIENT_1(date,&CLIENT_ID4,&RNG,&X,pin,&TOKEN4,&SEC,&xID,NULL,NULL,NULL,NULL)!=0)
		{
			printf("Error from Client side - First Pass\n");
			return 0;
		}

// Server generates Random number Y and sends it to Client

		MPIN_RANDOM_GENERATE(&RNG,&Y);

//  Client Second Pass: Inputs Client secret SEC, x and y. Outputs -(x+y)*SEC

		if (MPIN_CLIENT_2(&X,&Y,&SEC)!=0)
		{
			printf("Error from Client side - Second Pass\n");
			return 0;
		}	
// Server Second pass. Inputs client id, random Y, -(x+y)*SEC, xID and xCID and Server secret SST. E and F help kangaroos to find error.

		MPIN_SERVER_1(date,&CLIENT_ID4,&Y,&SST,&xID,NULL,&SEC,&E,&F,NULL,NULL);
		ccn4=MPIN_KANGAROO(&E,&F);
		if (ccn4==0) printf("Failed - trying again\n");
	} while (ccn4==0);

	printf("Credit Card Number is %04d %04d %04d %04d\n",-ccn1,-ccn2,-ccn3,-ccn4);


	do
	{
		pin=0; // do not use PIN
		if (MPIN_CLIENT_1(date,&CLIENT_ID5,&RNG,&X,pin,&TOKEN5,&SEC,&xID,NULL,NULL,NULL,NULL)!=0)
		{
			printf("Error from Client side - First Pass\n");
			return 0;
		}

// Server generates Random number Y and sends it to Client

		MPIN_RANDOM_GENERATE(&RNG,&Y);

//  Client Second Pass: Inputs Client secret SEC, x and y. Outputs -(x+y)*SEC

		if (MPIN_CLIENT_2(&X,&Y,&SEC)!=0)
		{
			printf("Error from Client side - Second Pass\n");
			return 0;
		}	
// Server Second pass. Inputs client id, random Y, -(x+y)*SEC, xID and xCID and Server secret SST. E and F help kangaroos to find error.

		MPIN_SERVER_1(date,&CLIENT_ID5,&Y,&SST,&xID,NULL,&SEC,&E,&F,NULL,NULL);
		cvv=MPIN_KANGAROO(&E,&F);
		if (cvv==0) printf("Failed - trying again\n");
	} while (cvv==0);

	printf("Credit Card Number is %04d %04d %04d %04d (%03d)\n",-ccn1,-ccn2,-ccn3,-ccn4,-cvv);

}
