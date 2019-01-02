/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_InFlash.c
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#define Dev_InFlash_D
#include "..\block\app\App_Macro.h"

/*
****************************************************************************************************
** ��    �� ��Dev_InFlash_SafeTest()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
****************************************************************************************************
*/
void Dev_InFlash_SafeTest(void)
{
	INT8 nRet;
	UINT8 data[1024];
	UINT8 buff[1024];
	UINT8 v; 

 	SF_Init(512,896);	// ��ȫ�洢����ʼ����0x10040000--0x10080000

	Trng_Init(0);
	Trng_GenRandom(&v,1);
	memset(data,v,1024);
	memset(buff,0,1024);

	Trng_GenRandom(&v,1);
	SF_WriteData(data,v,1024);	  // ��0x10040000��ʼ��1K��ַ��ȫд��data
	SF_ReadData(buff,v,1024);	 // ��ȫ����0x10040000��ʼ��1K��ַ���ݣ�����buff

	nRet = memcmp(data,buff,1024); // ��д�������������������Ƚ�
	if(nRet==0)
		DDEBUG("SFlash read and write OK!\n"); //д����ȷ
	else
		DDEBUG("SFlash read and write Error!\n");	//д�����

}
/*
****************************************************************************************************
** ��    �� ��Dev_InFlash_CodeTest()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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

	EFC_WrProtect(AREA_PROG_WE);	//ʹ�ܳ�������д
	
	for(i=192;i<194;i++)			//�������500ҳ����512ҳ��ʼ����
	{
		//EFC_Erase(i*FLASH_PAGE_SIZE, 1);
		
		//if(EFC_IsEraseOK(i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4, 0) != RT_OK) 
		//	DDEBUG("erase fail\n");
		
		EFC_WriteInfo(data, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		
		memset(buff,0,FLASH_PAGE_SIZE);
		
		EFC_ReadInfo(buff, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);

		DDEBUGHEX("data",data,FLASH_PAGE_SIZE);
		
		DDEBUGHEX("buff",buff,FLASH_PAGE_SIZE);
		
		nRet = memcmp(data,buff,FLASH_PAGE_SIZE); // ��д�������������������Ƚ�
		
		if(nRet!=0)
		{
			DDEBUG("EFC read and write page %d Error!\n", i);	//д�����
			nErr = 1;
		}
	}
	
	if(nErr == 0) 
		DDEBUG("EFC read and write in Program area OK!\n");  //д����ȷ
	
#if 0
	nErr = 0; 
	EFC_WrProtect(AREA_DATAPRO_WE);	  //ʹ����������д
	for(i=512;i<960;i++)
	{
		EFC_Erase(i*FLASH_PAGE_SIZE, 1);
		if(EFC_IsEraseOK(i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4, 0) != RT_OK) DDEBUG("erase fail\n");
		EFC_WriteInfo(data, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		memset(buff,0,FLASH_PAGE_SIZE);
		EFC_ReadInfo(buff, i*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
		nRet = memcmp(data,buff,FLASH_PAGE_SIZE); // ��д�������������������Ƚ�
		if(nRet!=0)
		{
			DDEBUG("EFC read and write page %d Error!\n", i);	//д�����
			nErr = 1;	
		}
	}
	
	if(nErr == 0) DDEBUG("EFC read and write in data area OK!\n"); //д����ȷ 
	
  #endif
	
	EFC_WrProtect(0);	 
}

/****************************************End Of File************************************************
***************************************************************************************************/


