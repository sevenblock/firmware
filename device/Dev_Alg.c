/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_Alg.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_Alg_D
#include "..\block\app\App_Macro.h"

/*
****************************************************************************************************
** 名    称 ：Dev_InFlash_KeyTest()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void TestTrueRand(void)
{
	UINT8 pBuf[32];
	UINT8 i;
	
	for(i=0;i<10;i++)
	{
		Trng_GenRandom(pBuf,32); 
	}

	print("trng ok\n");	 	
}

//----test DES CBC mode ----
void TestDesCBC(void)
{
	UINT8 in[32]={0x80};
	
	UINT8 out[32]={0};
	UINT8 result[32]={0};
	UINT8 iv[8]={0},pIV[8];
 
	UINT8 OKRes[32]={0x95,0xF8,0xA5,0xE5,0xDD,0x31,0xD9,0x00,0x80,
                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x95,0xF8,
                     0xA5,0xE5,0xDD,0x31,0xD9,0x00,0x80,0x00,0x00,
                     0x00,0x00,0x00,0x00,0x00};
	
	UINT8 pwd[8]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
	
	iv[0]=0;
	
	
	DES_Init(pwd,8);
	
	memcpy(pIV,iv,8);
	if(DES_Run(DES_ENCRYPT,DES_CBC,in,out,32,pIV)!=RT_OK)
	{
        print("DES CBC encry fail!\n");
		return;
	}
	if(memcmp(out,OKRes,32)!=0)
	{
		print("Des CBC encry fail!\n");
		return;
	}
	memcpy(pIV,iv,8);
	if(DES_Run(DES_DECRYPT,DES_CBC,out,result,32,pIV)!=RT_OK)
	{
        print("DES CBC decry fail!\n");
		return;
	}
	if(memcmp(result,in,32)!=0)
	{
		print("Des CBC decry fail!\n");
		return;
	}
	print("DES CBC OK!\n");
}
//----test DES ECB mode ----  	
void TestDesECB(void)
{  
	UINT8 in[32]={0x40};
	
	UINT8 out[32]={0};
	UINT8 result[32]={0};
 
 
	UINT8 OKRes[32]={0xDD,0x7F,0x12,0x1C,0xA5,0x01,0x56,0x19,0x8C,0xA6,
                    0x4D,0xE9,0xC1,0xB1,0x23,0xA7,0x8C,0xA6,0x4D,0xE9,
                    0xC1,0xB1,0x23,0xA7,0x8C,0xA6,0x4D,0xE9,0xC1,0xB1,
                    0x23,0xA7};
	
	UINT8 pwd[8]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
 
 

	DES_Init(pwd,8);
	  
	if(DES_Run(DES_ENCRYPT,DES_ECB,in,out,32,NULL)!=RT_OK)// dd7f121ca5015619
	{
        print("DES ECB encry fail!\n");
		return;
	}
 
	if(memcmp(out,OKRes,32)!=0)
	{
		print("Des ECB encry fail!\n");
		return;
	}
	if(DES_Run(DES_DECRYPT,DES_ECB,out,result,32,NULL)!=RT_OK)
	{
        print("DES ECB decry fail!\n");
		return;
	}
	if(memcmp(result,in,32)!=0)
	{
		print("Des ECB decry fail!\n");
		return;
	}
	print("DES ECB OK!\n");
    
}

//----test 3DES CBC mode ----	
void Test3DesCBC(void)
{
	
	UINT8 in[32]={0x6e,0x49,0x38,0x17,0xe4,0xef,0x64,0x77,
		0x59,0x12,0xbc,0x6f,0xaf,0x95,0xe4,0x26,0x42,0xf7,0xaf,0x2f,0x27,0x85,0x87,0x5d};
	
    UINT8 OKRes[32]={0x27,0x2B,0xE1,0x6E,0x23,0x1F,0x59,0x97,0x67,0x5D,0x41,
                    0xA0,0x21,0xC5,0xFA,0x71,0xCF,0xF3,0xC6,0x81,0xC3,0xFE,
                    0x75,0x3C,0xA7,0x14,0x9F,0x03,0x56,0x49,0xA6,0x25};
	
	UINT8 out[32]={0};
	UINT8 result[32]={0};
	UINT8 iv[8]={0xfb,0x99,0x7d,0x45,0x83,0x37,0x43,0x55},pIV[8];
	
 
	UINT8 pwd[25]={0xb0,0x6d,0xfe,0x62,0x9e,0x9e,0xb0,0xb9,0xb0,0x6d,0xfe,0x62,0x9e,0x9e,0xb0,0xb9,0xb0,0x6d,0xfe,0x62,0x9e,0x9e,0xb0,0xb9};

	DES_Init(pwd,24);
	
	memcpy(pIV,iv,8);
	if(DES_Run(DES_ENCRYPT,DES3_CBC,in,out,32,pIV)!=RT_OK)
	{
        print("3DES CBC encry fail!\n");
		return;
	}
		if(memcmp(out,OKRes,32)!=0)
	{
		print("3DES CBC  encry fail!\n");
		return;
	}
	
	memcpy(pIV,iv,8);
	if(DES_Run(DES_DECRYPT,DES3_CBC,out,result,32,pIV)!=RT_OK)
	{
         print("3DES CBC  decry fail!\n");
		return;
	}
	
	if(memcmp(result,in,32)!=0)
	{
		print("3DES CBC  decry fail!\n");
		return;
	}
	print("3DES CBC OK!\n");
}
//----test 3DES ECB mode ----	
void Test3DesECB(void)
{
	UINT8 in[32]={0x9a,0x60,0xc2,0x2d,0x25,0xc6,0xb8,0x11,0x6e,0x4f,0x2f,0xb4,0xa9,0x6f,0x9c,0x4a};
 
	UINT8 OKRes[32]={0xCA,0xD7,0xAB,0xE0,0x10,0x44,0xCC,0x54,0x22,0x05,
                     0x43,0x6A,0xF9,0x75,0xE5,0x70,0x60,0xC0,0xA7,0xF0,
                     0x55,0xE3,0x06,0x93,0x60,0xC0,0xA7,0xF0,0x55,0xE3,0x06,0x93};
	
	UINT8 out[32]={0};
	UINT8 result[32]={0};
 
 
	UINT8 pwd[25]={0x15,0x3e,0xf2,0xea,0xbf,0x45,0x1a,0x08,0x15,0x3e,0xf2,0xea,0xbf,0x45,0x1a,0x08,0x15,0x3e,0xf2,0xea,0xbf,0x45,0x1a,0x08};
	
	DES_Init(pwd,24);
	
	
	if(DES_Run(DES_ENCRYPT,DES3_ECB,in,out,32,NULL)!=RT_OK)
	{
        print("3DES ECB encry fail!\n");
		return ;
	}
	if(memcmp(out,OKRes,32)!=0)
	{
		print("3Des ECB encry fail!\n");
		return;
	}
	
	if(DES_Run(DES_DECRYPT,DES3_ECB,out,result,32,NULL)!=RT_OK)
	{
         print("3DES ECB decry fail!\n");
		return ;
	}
	
    if(memcmp(result,in,32)!=0)
	{
		print("3Des ECB decry fail!\n");
		return ;
	}
	print("3DES ECB OK!\n");
}

void Test_DesECB(void)
{
#define ALG_TEST_TIMES 		1
#define ALG_TEST_DATALEN 	512
#define ALG_TEST_FLOWENCDEC_TMS 	(1*1024*1024/ALG_TEST_DATALEN)  //10M	  
	
	UINT8 pwd[8]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
 	UINT32 j,k;
	UINT8 nRet;
	UINT32 nAddr;
	UINT8* pnAddr;
	    
/***********************************************************************************/
			Trng_GenRandom(pwd,8);
			DES_Init(pwd,8);
			nAddr = 0x20008000;
			pnAddr = (UINT8 *)0x20008000;
			Trng_GenRandom(pnAddr,ALG_TEST_DATALEN);
			print("Now = ");
			for(j=0;j<ALG_TEST_TIMES;j++)
			{
				print("%d\t",j);
				k = ALG_TEST_FLOWENCDEC_TMS; nRet = RT_OK;
				while( k-- && nRet == RT_OK)
				{
					  nRet = _Des_Run(DES_ENCRYPT,nAddr,ALG_TEST_DATALEN);
				}
				if(nRet!=RT_OK)
				{
					print("DES ECB encry fail!___ j = %d \n",j);	
				}
			}
			print("\n\r________________________\n\r");
		    
			print("Now = ");
			for(j=0;j<ALG_TEST_TIMES;j++)
			{
				print("%d\t",j);
				k = ALG_TEST_FLOWENCDEC_TMS; nRet = RT_OK;
				while( k-- && nRet == RT_OK)
				{
					nRet = _Des_Run(DES_DECRYPT,nAddr,ALG_TEST_DATALEN);
				}
				if(nRet!=RT_OK)
				{
					print("DES ECB decry fail!___ j = %d \n",j);
				}
			}
			print("\n\r________________________\n\r");
		/***********************************************************************************/    
}

void Test_3DesECB(void)
{
#define ALG_TEST_TIMES 		1
#define ALG_TEST_DATALEN 	512
#define ALG_TEST_FLOWENCDEC_TMS 	(1*1024*1024/ALG_TEST_DATALEN)  //10M	  
	
	UINT8 pwd[24]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
 	UINT32 j,k;
	UINT8 nRet;
	UINT32 nAddr;
	UINT8* pnAddr;
	    
/***********************************************************************************/
			Trng_GenRandom(pwd,24);
			DES_Init(pwd,24);
			nAddr = 0x20008000;
			pnAddr = (UINT8 *)0x20008000;
			Trng_GenRandom(pnAddr,ALG_TEST_DATALEN);
			print("Now = ");
			for(j=0;j<ALG_TEST_TIMES;j++)
			{
				print("%d\t",j);
				k = ALG_TEST_FLOWENCDEC_TMS; nRet = RT_OK;
				while( k-- && nRet == RT_OK)
				{
					  nRet = _Des_Run(DES_ENCRYPT,nAddr,ALG_TEST_DATALEN);
				}
				if(nRet!=RT_OK)
				{
					print("3DES ECB encry fail!___ j = %d \n",j);	
				}
			}
			print("\n\r________________________\n\r");
		    
			print("Now = ");
			for(j=0;j<ALG_TEST_TIMES;j++)
			{
				print("%d\t",j);
				k = ALG_TEST_FLOWENCDEC_TMS; nRet = RT_OK;
				while( k-- && nRet == RT_OK)
				{
					nRet = _Des_Run(DES_DECRYPT,nAddr,ALG_TEST_DATALEN);
				}
				if(nRet!=RT_OK)
				{
					print("3DES ECB decry fail!___ j = %d \n",j);
				}
			}
			print("\n\r________________________\n\r");
		/***********************************************************************************/    
}


UINT32 TestRSA1024(void)
{ 	
	UINT8 pOutData[128];	
	UINT8 pInData[128];		
	UINT32 nOutDataLen,nInDataLen; 	
	UINT32 DecLen=0; 	 
	R_RSA_PROTO_KEY ProtoKey;   
    R_RSA_PRIVATE_KEY PrivateKey; 
	R_RSA_PUBLIC_KEY  PublicKey;
	UINT32 nFlag=0;		
	UINT32 i;

	Rsa_Struct RsaKey;
 
	PrivateKey.modulus = RsaKey.n;
	PrivateKey.publicExponent = RsaKey.e;
	PrivateKey.exponent = RsaKey.d;
	PrivateKey.prime[0] = RsaKey.p;
	PrivateKey.prime[1] = RsaKey.q;
	PrivateKey.primeExponent[0] = RsaKey.dp; 
	PrivateKey.primeExponent[1] = RsaKey.dq;
	PrivateKey.coefficient = RsaKey.qinv; 	
	PrivateKey.dmodulus = RsaKey.dn; 

	PublicKey.modulus = RsaKey.n;
	PublicKey.exponent = RsaKey.e;
	PublicKey.bits=1024;
	
	ProtoKey.bits = 1024;
	ProtoKey.PublicExponent = 65537; 
 	
	for(i=0;i<128;i++)		
		pInData[i]=i+1;		 
	 
    print("Test RSA 1024\n");

	nFlag= RSA_GenKeyPair(ProtoKey,&PrivateKey);          
  
	if(nFlag!=RT_OK)//failed
	{
	     print("Gen RSA key Fail\n");
		 return nFlag;
	} 
	
    print("Gen RSA key OK\n");
	nInDataLen=128;      
     	
    nFlag=RSA_PubKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }

    print("RSA PubOpt OK\n");     

    nFlag=RSA_PriKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
    
    for(i=0;i<128;i++)	
    {    	  	
		if(pInData[i]!=i+1)
		{
			print("Data decry error\n");
			break;
		}		
    }
	 
    if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
    
	//RSA Private Key Encry;
	nFlag=RSA_PriKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n"); 
 
	//RSA Public Key Decry;
	nFlag=RSA_PubKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }

     for(i=0;i<128;i++)	
    {    	  	
		if(pInData[i]!=i+1)
		{
			print("Data decry error\n");
			break;
		}		
    }

    print("RSA PriOpt OK\n");   

	nInDataLen=32;     
	//RSA Public Key Encry;
	
    nFlag=RSA_PublicEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) return nFlag; //failed
   
	//RSA Private Key Decry;	 
    nFlag=RSA_PrivateDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
	 
    if(nFlag!=0) return nFlag;   
    
	//RSA Private Key Encry;
	nFlag=RSA_PrivateEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) return nFlag;
 
	//RSA Public Key Decry;
	nFlag=RSA_PublicDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) return nFlag;
	 
     return 0;
}

//----test RSA 2048 bit mode ---- 
		
UINT32 TestRSA2048(void)
{  
    R_RSA_PROTO_KEY ProtoKey;
    R_RSA_PRIVATE_KEY PrivateKey;
	R_RSA_PUBLIC_KEY  PublicKey;
	UINT8 pOutData[256];	
	UINT8 pInData[256];	
	UINT32 nOutDataLen,nInDataLen; 		
	UINT32 DecLen=0; 	
	UINT32 nFlag=0;		
	UINT32 i;    

	Rsa_Struct RsaKey;

	memset(&RsaKey,0x00,sizeof(RsaKey));
		
	PrivateKey.modulus = RsaKey.n;
	PrivateKey.publicExponent = RsaKey.e;
	PrivateKey.exponent = RsaKey.d;
	PrivateKey.prime[0] = RsaKey.p;
	PrivateKey.prime[1] = RsaKey.q;
	PrivateKey.primeExponent[0] = RsaKey.dp; 
	PrivateKey.primeExponent[1] = RsaKey.dq;
	PrivateKey.coefficient = RsaKey.qinv; 	
	PrivateKey.dmodulus = RsaKey.dn; 

	ProtoKey.bits = 2048;	
	ProtoKey.PublicExponent = 65537; 
	
	PublicKey.modulus = RsaKey.n;
	PublicKey.exponent = RsaKey.e;
	PublicKey.bits=2048;	 
	
	for(i=0;i<256;i++)		
		pInData[i]=i; 

	print("Test RSA 2048\n");

	 nFlag= RSA_GenKeyPair(ProtoKey,&PrivateKey);

	 DDEBUGHEX("RsaKey.dn",RsaKey.dn,sizeof(RsaKey.dn));
	 
    if(nFlag!=RT_OK)//failed
	{
	   print("Gen RSA key Fail\n");
		return nFlag;
	} 
	
    print("Gen RSA key OK\n"); 	

	nInDataLen=256;      
     	
    nFlag=RSA_PubKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,(R_RSA_PUBLIC_KEY*)(void*)&PrivateKey); 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }
   print("RSA PubOpt OK\n");      
   
	//RSA Private Key Decry; 
    nFlag=RSA_PriKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
	 
    if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
     
     for(i=0;i<256;i++)	
    {    	  	
		if(pInData[i]!=i)
		{
		 	print("Data decry error\n");
			return RT_FAIL;
		}		
    }
	//RSA Private Key Encry;
	nFlag=RSA_PriKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PubOpt OK\n");
   
	//RSA Public Key Decry;
	nFlag=RSA_PubKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,(R_RSA_PUBLIC_KEY*)(void*)&PrivateKey); 	 

	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
   
	 for(i=0;i<256;i++)	
    {    	  	
		if(pInData[i]!=i)
		{
			print("Data decry error\n");
			return RT_FAIL;
		}		
    }

    nInDataLen=32;     
	//RSA Public Key Encry;
	
    nFlag=RSA_PublicEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) return nFlag; //failed

	DDEBUGHEX("RSA_PrivateDecrypt pOutData = ",pOutData,nOutDataLen);
	 
	//RSA Private Key Decry;	 
    nFlag=RSA_PrivateDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  

	DDEBUGHEX("RSA_PrivateDecrypt pInData = ",pInData,DecLen);

	DDEBUGHEX("PrivateKey0 = ",PrivateKey.prime[0],128);
	DDEBUGHEX("PrivateKey1 = ",PrivateKey.prime[1],128);
	 
    if(nFlag!=0) return nFlag;   
    
	//RSA Private Key Encry;
	nFlag=RSA_PrivateEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) return nFlag;
 
	//RSA Public Key Decry;
	nFlag=RSA_PublicDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) return nFlag;
	print("\n"); 
    return 0;
}

#if 0
//----test MD5 Hash Alg ----
void TestMD5(void)
{
	UINT8 pBuf[256],Result[16];
	UINT16 i;
	MD5_CONTEXT ctx;
   	UINT8 OK[32]={0x27,0x5c,0xc8,0x0a,0x75,0xc5,0x8f,0x23,
				  0xfe,0x58,0x18,0xd,0x79,0x4a,0x9e,0x88};

	for( i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	MD5_Init(&ctx); 
	MD5_Update(&ctx,pBuf,256);
	MD5_Final(&ctx,Result);
	if(memcmp(Result,OK,16)!=0)
	{
		print("MD5 fail!\n");
		return;
	}
	print("MD5 ok!\n");
    
}
#endif

//----test SHA1 Hash Alg ----
void TestSHA1(void)
{
	UINT8 pBuf[256],Result[20];
	UINT16 i;
	SHA1_CONTEXT ctx;
	UINT8 OK[32]={0x9f,0x47,0x3c,0x38,0x4c,0x2d,0x72,0x95,0xae,0x20,
				  0x40,0xd4,0x7d,0x19,0x36,0x25,0xa6,0x77,0xdb,0x14};
	
	for( i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	SHA1_Init(&ctx); 
	SHA1_Update(&ctx,pBuf,256);
	SHA1_Final(&ctx,Result);
	if(memcmp(Result,OK,20)!=0)
	{
		print("SHA1 fail!\n");
		return;
	}
	print("SHA1 ok!\n");
    
} 

//----test SHA224 Hash Alg ----
void TestSHA224(void)
{
	UINT8 pBuf[256],Result[28];
	UINT16 i;
	SHA224_CONTEXT ctx;
	UINT8 OK[32]={0xae,0xed,0x10,0xcc,0x6d,0xaa,0xbc,0xab,
				  0xf8,0x8d,0x3f,0x55,0xf0,0x4e,0x26,0x22,
				  0x1e,0x3d,0xbe,0xbe,0x0c,0x41,0xce,0x0c,
				  0x39,0xb9,0x73,0x8a};
	
	for(i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	SHA224_Init(&ctx); 
	SHA224_Update(&ctx,pBuf,256);
	SHA224_Final(&ctx,Result);
	if(memcmp(Result,OK,28)!=0)
	{
		print("SHA224 fail!\n");
		return;
	}
	print("SHA224 ok!\n");
    
}

//----test SHA256 Hash Alg ----
void TestSHA256(void)
{
	UINT8 pBuf[256],Result[32];
	UINT16 i;
	SHA256_CONTEXT ctx;
	UINT8 OK[32]={0x9b,0xc0,0x38,0xd0,0xa0,0xfb,0x39,0x1f,
				  0x3b,0x33,0x61,0x8d,0xcf,0x8,0xb6,0x55,
				  0x35,0x60,0xef,0xa,0xe0,0xf7,0xad,0x55,
				  0x78,0x71,0x59,0x8f,0x27,0xb7,0x19,0x4b};
	
	for(i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	SHA256_Init(&ctx); 
	SHA256_Update(&ctx,pBuf,256);
	SHA256_Final(&ctx,Result);
	if(memcmp(Result,OK,32)!=0)
	{
		print("SHA256 fail!\n");
		return;
	}
	print("SHA256 ok!\n");
    
}
//----test SHA384 Hash Alg ----
void TestSHA384(void)
{
	UINT8 pBuf[256],Result[64];
	UINT16 i;
	SHA4_CONTEXT ctx;
	UINT8 OK[64]={0x37,0xf8,0x3b,0x4d,0xfe,0xfa,0xaa,0x4,
				  0x7b,0x3e,0x2b,0xb9,0x32,0x19,0x91,0x9e,
				  0xd9,0xc7,0x86,0x81,0xbd,0x38,0xce,0x79,
				  0x9,0x9d,0x80,0xb3,0xdf,0xfc,0x43,0x41,
				  0xe2,0x34,0x5b,0x47,0x50,0x8d,0xbf,0x8e,
				  0x4e,0xf1,0x5d,0xa9,0xc2,0x63,0x76,0x41,
				  0x35,0x60,0xef,0xa,0xe0,0xf7,0xad,0x55,
				  0x78,0x71,0x59,0x8f,0x27,0xb7,0x19,0x4b};
	
	for(i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	SHA4_Init(&ctx,1); 
	SHA4_Update(&ctx,pBuf,256);
	SHA4_Final(&ctx,Result);
	if(memcmp(Result,OK,64)!=0)
	{
		print("SHA384 fail!\n");
		return;
	}
	print("SHA384 ok!\n");
    
}

//----test SHA512 Hash Alg ----
void TestSHA512(void)
{
	UINT8 pBuf[256],Result[64];
	UINT16 i;
	SHA4_CONTEXT ctx;
	UINT8 OK[64]={0x11,0xc8,0xeb,0x1c,0x1b,0x9b,0xf8,0x82,
				  0xae,0x34,0xf2,0x28,0xcc,0x76,0xb0,0x47,
				  0xb7,0xc2,0xd,0x27,0x6d,0xff,0x8f,0x51,
				  0xbc,0x82,0x52,0xc2,0xf0,0x23,0xf9,0x79,
				  0xf7,0xca,0x47,0x16,0x10,0x5c,0x28,0xf3,
				  0x54,0x8d,0x8e,0x4c,0x89,0xc2,0xca,0x97,
				  0xcf,0x48,0x5e,0x77,0x2b,0xbc,0xd9,0x7a,
				  0xa3,0x7b,0xb5,0x4c,0x42,0xec,0xda,0x22};
	
	for(i=0;i<256;i++)	
	  pBuf[i]=i+1;
	
	SHA4_Init(&ctx,0); 
	SHA4_Update(&ctx,pBuf,256);
	SHA4_Final(&ctx,Result);
	if(memcmp(Result,OK,64)!=0)
	{
		print("SHA512 fail!\n");
		return;
	}
	print("SHA512 ok!\n");
	print("\n");
	   
}


/*
****************************************************************************************************
** 名    称 ：Dev_InFlash_KeyTest()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void TestAlg(void)
{
    //---获取算法库版本---
	INT8 pVer[32] = {0};  

    //--测试真随机数---- 
	TrngVersion(pVer);
	print("TrngVersion %s\n",pVer);
	TestTrueRand();
	 
	//--测试DES----
	DesVersion(pVer);
	print("DesVersion %s\n",pVer); 	
	TestDesECB();       
	Test3DesECB();

    //--测试RSA---- 
	RsaVersion(pVer);
	print("RsaVersion %s\n",pVer); 
	TestRSA2048();
	
	//---测试HASH---
    TestSHA256();
	
	//--测试结束---- 
}


/****************************************End Of File************************************************
***************************************************************************************************/

