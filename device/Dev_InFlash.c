/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_InFlash.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_InFlash_D
#include "..\block\app\App_Macro.h"

/*
****************************************************************************************************
** 名    称 ：Dev_InFlash_SafeTest()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_InFlash_SafeTest(void)
{
	INT8 nRet;
	UINT8 data[1024];
	UINT8 buff[1024];
	UINT8 v; 

 	SF_Init(512,896);	// 安全存储区初始化，0x10040000--0x10080000

	Trng_Init(0);
	Trng_GenRandom(&v,1);
	memset(data,v,1024);
	memset(buff,0,1024);

	Trng_GenRandom(&v,1);
	SF_WriteData(data,v,1024);	  // 向0x10040000开始的1K地址安全写入data
	SF_ReadData(buff,v,1024);	 // 安全读出0x10040000开始的1K地址数据，存入buff

	nRet = memcmp(data,buff,1024); // 将写入的数据与读出数据做比较
	if(nRet==0)
		DDEBUG("SFlash read and write OK!\n"); //写入正确
	else
		DDEBUG("SFlash read and write Error!\n");	//写入错误

}
/*
****************************************************************************************************
** 名    称 ：Dev_InFlash_CodeTest()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_InFlash_CodeTest(void)
{
    UINT8 nRet, nErr = 0;
    UINT16 i;
	UINT8 data[FLASH_PAGE_SIZE];
	UINT8 buff[FLASH_PAGE_SIZE];
       
    memset(data,0x51,FLASH_PAGE_SIZE);      
	memset(buff,0,FLASH_PAGE_SIZE);
    
    EFC_ConfigCodeSz(512);
		
	if(EfcRegs.DATA != FLASH_BASE_ADDR + 512*FLASH_PAGE_SIZE)
		DDEBUG("DATAPRO error!\n");
	
	SCM_SetCacheRegion0(0, 192*FLASH_PAGE_SIZE);

	EFC_WrProtect(AREA_PROG_WE);	//使能程序区读写
	
	for(i=192;i<194;i++)			//程序代码500页，从512页开始擦除
	{
		//EFC_Erase(i*FLASH_PAGE_SIZE, 1);
		
		//if(EFC_IsEraseOK(i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4, 0) != RT_OK) 
		//	DDEBUG("erase fail\n");
		
		EFC_WriteInfo(data, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		
		memset(buff,0,FLASH_PAGE_SIZE);
		
		EFC_ReadInfo(buff, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);

		DDEBUGHEX("data",data,FLASH_PAGE_SIZE);
		
		DDEBUGHEX("buff",buff,FLASH_PAGE_SIZE);
		
		nRet = memcmp(data,buff,FLASH_PAGE_SIZE); // 将写入的数据与读出数据做比较
		
		if(nRet!=0)
		{
			DDEBUG("EFC read and write page %d Error!\n", i);	//写入错误
			nErr = 1;
		}
	}
	
	if(nErr == 0) 
		DDEBUG("EFC read and write in Program area OK!\n");  //写入正确
	
#if 0
	nErr = 0; 
	EFC_WrProtect(AREA_DATAPRO_WE);	  //使能数据区读写
	for(i=512;i<960;i++)
	{
		EFC_Erase(i*FLASH_PAGE_SIZE, 1);
		if(EFC_IsEraseOK(i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4, 0) != RT_OK) DDEBUG("erase fail\n");
		EFC_WriteInfo(data, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		memset(buff,0,FLASH_PAGE_SIZE);
		EFC_ReadInfo(buff, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		nRet = memcmp(data,buff,FLASH_PAGE_SIZE); // 将写入的数据与读出数据做比较
		if(nRet!=0)
		{
			DDEBUG("EFC read and write page %d Error!\n", i);	//写入错误
			nErr = 1;	
		}
	}
	
	if(nErr == 0) DDEBUG("EFC read and write in data area OK!\n"); //写入正确 
	
  #endif
	
	EFC_WrProtect(0);	 
}

/****************************************End Of File************************************************
***************************************************************************************************/


