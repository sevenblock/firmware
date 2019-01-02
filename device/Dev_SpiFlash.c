/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_SpiFlash.c
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#define Dev_SpiFlash_D
#include "..\block\app\App_Macro.h"


/* Private define ------------------------------------------------------------*/
#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */
#define WRDIS      0x04  /* Write disable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define FREAD      0x0B  /* Fast Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */

#define SE         0x20  /* Sector Erase instruction */
#define BE         0xD8  /* Bulk Erase instruction */
#define CE         0xC7  /* Chip Erase instruction */

#define PD         0xB9  /* Power down enable */
#define RPD        0xAB  /* Release from Power down mode */

#define RDMID      0x90  /* Read Device identification */
#define RDJID      0x9F  /* Read JEDEC identification */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define DUMMY_BYTE 0xA5

/* Private macro -------------------------------------------------------------*/
#define   TIME_OUT    100000 


void SpiFlashWaitWriteEnd(void);

/*******************************************************************************
* ��������: SpiFlashWriteEnable
* ��������: �Դ��� flash дʹ�� 
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void  SpiFlashWriteEnable(void)
{
	u32 len = 1;
	u8 command = WREN;
	
	Spi_Enable(SPI_FLASH_CS);
	Spi_Comm(&command, len,NULL, 0);
	Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������: SpiFlashWriteDisable
* ��������: �Դ��� flash д��ֹ
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void  SpiFlashWriteDisable(void)
{
	u32 len = 1;
	u8 command = WRDIS;
	
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(&command,len, NULL, 0);
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������: SpiFlashReadStatus
* ��������: ��״̬�Ĵ���
* �������: ��
* �������: ��
* ���ز���: ��ǰflash�ڲ�״̬�Ĵ�����ֵ
*******************************************************************************/
u8  SpiFlashReadStatus(void)
{
	u32 len = 2;
	u8 command[2];
	u8 status[2];

	command[0] = WRDIS;
	command[1] = DUMMY_BYTE;
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len,status, len);
    Spi_Disable(SPI_FLASH_CS);
    return(status[1]);
}

/*******************************************************************************
* ��������: SpiFlashBusyWait
* ��������: �ȴ�flash�ڲ��������
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SpiFlashBusyWait(void)
{
	
	u32 len = 1;
	u8 command = RDSR;
	u8 status;

    Spi_Enable(SPI_FLASH_CS);
	Spi_Comm(&command,len, NULL, 0); 
	
	do
	{
		Spi_Comm(NULL,0,&status, len);
	}while(status & 0x01);
	
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������: SpiFlashWriteStatus
* ��������: д״̬�Ĵ���
*           ע�⣺���ܺ��������3307ԭ��ͼ��Ƶģ����ԭ��ͼwpλ��Ч�����޸ģ���
* �������: new_status����ǰflash�ڲ�״̬�Ĵ�����ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void  SpiFlashWriteStatus(u8 new_status)
{
	
	u32 len = 2;
	u8 command[2];

	command[0] = WRSR;
	command[1] = new_status;
    SpiFlashWriteEnable();
    Spi_Enable(SPI_FLASH_CS);
	Spi_Comm(command,len, NULL, 0); 
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������: SpiFlashReadByteData
* ��������: �Ӵ��� flash �̶���ַ����һ���ֽ�����
* �������: address��Ҫ�������ݵĵ�ַ
* �������: ��
* ���ز���: ����������
*******************************************************************************/
u8  SpiFlashReadByteData(u32 address)
{
	u32 len = 5;
	u8 command[5];
	u8 data[5];

	command[0] = READ;
	command[1] = (u8)(address>>16);
	command[2] = (u8)(address>>8);
	command[3] = (u8)(address);
	command[4] = DUMMY_BYTE;
	data[4] = 0;
	
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command,len, data, len); 
    Spi_Disable(SPI_FLASH_CS);
	
    return(data[4]);    
}

/*******************************************************************************
* ��������: SpiFlashReadMoreByteData
* ��������: �Ӵ��� flash ĳ��ַ��ʼ������һ������
* �������: address_from��Ҫ�������ݵ���ʼ��ַ
*           save_pointer��Ҫ�Ѷ��������ݷŵ��ڴ�ָ��
*           data_num��Ҫ�����������ֽ���
* �������: ��
* ���ز���: ��������������� FALSE ���򷵻� TRUE
*******************************************************************************/
u8 SpiFlashReadMoreByteData(u32 address_from, u8 *save_pointer, u16 data_num)
{
	
	u32 len = 4;
	u8 command[4];

    if(address_from + data_num >= 0x1f0000)
    {
        return(FALSE);
    }
	command[0] = READ;
	command[1] = (u8)(address_from>>16);
	command[2] = (u8)(address_from>>8);
	command[3] = (u8)(address_from);
	
    Spi_Enable(SPI_FLASH_CS);
   	Spi_Comm(command, len,save_pointer, data_num); 
    Spi_Disable(SPI_FLASH_CS);
	
    return(TRUE);
}

/*******************************************************************************
* ��������: SpiFlashFastReadMoreByteData
* ��������: ���ÿ��ָ��Ӵ��� flash ĳ��ַ��ʼ������һ������
* �������: address_from��Ҫ�������ݵ���ʼ��ַ
*           save_pointer��Ҫ�Ѷ��������ݷŵ��ڴ�ָ��
*           data_num��Ҫ�����������ֽ���
* �������: ��
* ���ز���: ��������������� FALSE  ���򷵻� TRUE
*******************************************************************************/
u8 SpiFlashFastReadMoreByteData(u32 address_from, u8 *save_pointer,
                                           u32 data_num)
{
	u32 len = 5;
	u8 command[5];

    if(address_from + data_num >= 0x1f0000)
    {
        return(FALSE);
    }
	command[0] = FREAD;
	command[1] = (u8)(address_from>>16);
	command[2] = (u8)(address_from>>8);
	command[3] = (u8)(address_from);
	command[4] = DUMMY_BYTE;
	
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len, save_pointer, data_num); 
    Spi_Disable(SPI_FLASH_CS);
	
    return(TRUE);
}
/*******************************************************************************
* ��������  : SpiFlashSectorErase
* ��������  : ��������flash��һ��������ע��д����������������������ɹ���
* �������  : sector_addr: ������Ҫ����������ַ�ռ��ڵ�һ����ַ
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashSectorErase(u32 sector_addr)
{
	u32 len = 4;
	u8 command[4];

	command[0] = SE;
	command[1] = (u8)(sector_addr>>16);
	command[2] = (u8)(sector_addr>>8);
	command[3] = (u8)(sector_addr);
    /* -----дʹ��-------- */
    SpiFlashWriteEnable();
    
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len,NULL, 0); 
    Spi_Disable(SPI_FLASH_CS);

    /*------ȷ��flash����û���κ�д����---*/
    SpiFlashWaitWriteEnd();    
}

/*******************************************************************************
* ��������  : SpiFlashBulkErase
* ��������  : ��������flash��һ���飬ע��д����������������������ɹ���
* �������  : bulk_addr: ������Ҫ����������ַ�ռ��ڵ�һ����ַ
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashBulkErase(u32 bulk_addr)
{
	u32 len = 4;
	u8 command[4];

	command[0] = BE;
	command[1] = (u8)(bulk_addr>>16);
	command[2] = (u8)(bulk_addr>>8);
	command[3] = (u8)(bulk_addr);
	
    /* Send write enable instruction */
     SpiFlashWriteEnable();
    
    /* Sector Erase */     
    /* Select the FLASH: Chip Select low */
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len, NULL, 0);
    Spi_Disable(SPI_FLASH_CS);

    /*------ȷ��flash����û���κ�д����---*/
    SpiFlashWaitWriteEnd();
}

/*******************************************************************************
* ��������  : SpiFlashChipErase
* ��������  : ������������flash��ע��д����������������������ɹ���
* �������  : bulk_addr: ������Ҫ����������ַ�ռ��ڵ�һ����ַ
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashChipErase(void)
{
	u32 len = 1;
	u8 command = CE;
	
    /*----��ȥ������-------*/
    //SpiFlashWriteStatus(0x00);
    
    /* ---����дʹ��------*/
    SpiFlashWriteEnable();

    Spi_Enable(SPI_FLASH_CS);
    
    /*----����ָ��--------*/
    Spi_Comm(&command, len,NULL, 0);
    
    Spi_Disable(SPI_FLASH_CS);

    /*------ȷ��flash����û���κ�д����---*/
    SpiFlashWaitWriteEnd();   
}

/*******************************************************************************
* ��������  : SpiFlashByteWrite
* ��������  : ��flash��ַ�ռ�дһ���ֽ�����
* �������  : address : �ֽ�����Ҫд���Ĵ���flash�ĵ�ַ
*             data: Ҫд���ֽ����ݡ�             
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashByteWrite(u32 address, u8 data)
{
	u32 len = 5;
	u8 command[5];

	command[0] = WRITE;
	command[1] = (u8)(address>>16);
	command[2] = (u8)(address>>8);
	command[3] = (u8)(address);
	command[4] = data;
    
    /* Send write enable instruction */
    SpiFlashWriteEnable();

    /* Chip Erase */
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len,NULL, 0);
    Spi_Disable(SPI_FLASH_CS);

    /*------��ȷ��flash����û���κ�д����---*/
    SpiFlashWaitWriteEnd();       
}

/*******************************************************************************
* ��������  : SpiFlashPageWrite
* ��������  : д256�ֽ� flash ��һҳ��
* �������  : pbuffer : Ҫд��flash���������ڻ�������ָ��
*             addr_start : Ҫд��flash�ڲ��Ŀռ����ʼ��ַ             
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashPageWrite(u8* pbuffer, u32 addr_start)
{
	u32 len = 4;
	u8 command[4];

	command[0] = WRITE;
	command[1] = (u8)(addr_start>>16);
	command[2] = (u8)(addr_start>>8);
	command[3] = 0;
    /* Enable the write access to the FLASH */
    SpiFlashWriteEnable();

    Spi_Enable(SPI_FLASH_CS);
	Spi_Comm(command, len,NULL, 0);
    /* while there is data to be written on the FLASH */
	Spi_Comm(pbuffer, 256,NULL, 0);
    Spi_Disable(SPI_FLASH_CS);
    
    /*--ȷ��flash����û���κ�д����--*/
    SpiFlashWaitWriteEnd();  
}

/*******************************************************************************
* ��������  : SpiFlashWrite
* ��������  : дһ�����ݵ� flash �Ĵ洢�ռ�
* �������  : pbuffer : Ҫд��flash���������ڻ�������ָ��
*             addr_start : Ҫд��flash�ڲ��Ŀռ����ʼ��ַ
*             data_byte_number : д��flash�ڲ��ռ����ݵ��ֽ���
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashWrite(u8* pbuffer, u32 addr_start, u16 data_byte_number)
{
	u32 len;
	u8 command[4];

    while (data_byte_number)
    {    
		command[0] = WRITE;
		command[1] = (u8)(addr_start>>16);
		command[2] = (u8)(addr_start>>8);
		command[3] = (u8)(addr_start);
        /* Enable the write access to the FLASH */
        SpiFlashWriteEnable();
        Spi_Enable(SPI_FLASH_CS);
		len = 4;
    	Spi_Comm(command,len, NULL, 0);
		len = 256 - (addr_start & 0xff);
		
		if(len < data_byte_number)
		{
			Spi_Comm(pbuffer, len,NULL, 0);
			data_byte_number -= len;
			pbuffer += len;
			addr_start += len;
		}
		else
		{
			Spi_Comm(pbuffer, data_byte_number,NULL, 0);
			data_byte_number = 0;
			addr_start += data_byte_number;
			pbuffer += data_byte_number;
		}
		
        Spi_Disable(SPI_FLASH_CS);
        
        /*--ȷ��flash����û���κ�д����--*/
        SpiFlashWaitWriteEnd();       
    }
}

/*******************************************************************************
* ��������  : SpiFlashReadMID
* ��������  : ��flash M ID
* �������  : ��
* �������  : ��
* ���ز���  : flash M ID 
*******************************************************************************/
u32 SpiFlashReadMID(void)
{
	u32 MID;
	u32 len = 4;
	u8 command[4];
	u8 data[2];

	command[0] = RDMID;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
    /* Select the FLASH: Chip Select low */
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command,len, data, 2);
    Spi_Disable(SPI_FLASH_CS);

	MID = data[0];
	MID = (MID<<8) + data[1];
         
    return MID;
}

/*******************************************************************************
* ��������  : SpiFlashReadJID
* ��������  : ��flash JEDEC ID
* �������  : ��
* �������  : ��
* ���ز���  : flash JEDEC ID 
*******************************************************************************/
u32 SpiFlashReadJID(void)
{
	u32 JID;
	u32 len = 1;
	u8 command = RDJID;
	u8 data[3];
    
    /* Select the FLASH: Chip Select low */
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(&command, len,data, 3);
    Spi_Disable(SPI_FLASH_CS);
    
    JID = data[0];
	JID = (JID<<8) + data[1];
	JID = (JID<<8) + data[2];
    
    return JID;
}

/*******************************************************************************
* ��������  : SpiFlashPowerDown
* ��������  : ����ڵ�ģʽ��ʹ����˯�ߡ��ڻ���Ӧ�õ��ʱ���Ƚ�����
* �������  : ��
* �������  : ��
* ���ز���  : �� 
*******************************************************************************/
void  SpiFlashPowerDown(void)
{
	u32 len = 1;
	u8 command = PD;
	
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(&command,len, NULL, 0);
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������  : SpiFlashPowerDownReturn
* ��������  : ʹflash�ӽڵ�ģʽ����
* �������  : ��
* �������  : ��
* ���ز���  : �� 
*******************************************************************************/
void  SpiFlashPowerDownReturn(void)
{
	u32 len = 1;
	u8 command = RPD;
	
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(&command,len, NULL, 0);
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������  : SpiFlashWaitWriteEnd
* ��������  : �ȴ��Դ��������ڲ�д���߲�����������
* �������  : ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void SpiFlashWaitWriteEnd(void)
{
    u8 flash_status = 0;
	u32 len = 1;
	u8 command = RDSR;
    
    /* Select the FLASH: Chip Select low */
    Spi_Enable(SPI_FLASH_CS);
    
    /* Send "Read Status Register" instruction */
	Spi_Comm(&command, len,NULL, 0);
    /* Loop as long as the memory is busy with a write cycle */
	
    do
    {
		/*
		if(t++ > 0x10000)	// ��ʱ
			break;
		*/
        Spi_Comm(NULL, 0,&flash_status, len);
    }
    while ((flash_status & 0x01) != 0); /* Write in progress */
    
    /* Deselect the FLASH: Chip Select high */
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* ��������  : SpiFlashOpen
* ��������  : �򿪴��ڣ�����һЩ��ʼ�����ã��Լ��ж�flash�Ƿ���������
* �������  : ��
* �������  : ��
* ���ز���  : FALSE �� TRUE
*******************************************************************************/
u8  SpiFlashOpen(void)
{
    if(SpiFlashReadJID() == 0x00ef3015)
    {
        return(TRUE);
    }
	
	//flash_debug("flash JID����\r\n");
	
    return(FALSE);
}

/*
*********************************************************************************************************
** �������� ��SF_ErasePage()
** �������� ��ҳ����  
** ��ڲ��� ��page_addr   Ҫ������ҳ��ַ
** ���ڲ��� ��0ʧ��  1�ɹ�
** ��    �� : ��
** ��    ע :
*********************************************************************************************************
*/
u8 SF_ErasePage(u32 cPage_addr)
{ 
	if(M25PE16_MAXPAGE<=cPage_addr)
	{
		return FALSE;
	}
	
	SpiFlashSectorErase(cPage_addr*M25PE16_PAGESIZE);
	
	return TRUE;
}

#if 0
/*
********************************************************************************************************
** �������� ��SF_Read()
** �������� ��page_addrҳ��ַ,in_page_byte_addrҳ�ڵ�ַ. array���ݻ�����, counter���ݳ���
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� : ��
** ��    ע :
*********************************************************************************************************
*/
u8 SF_Read(u32 cPage_addr, u32 cInPage_addr, u8 *cArray, u16 cCount)
{
	u32 mPage_addr;
	
	if(M25PE16_MAXPAGE<=cPage_addr||(cInPage_addr+cCount)>M25PE16_PAGESIZE)
	{
		return FALSE;
	}
	
	mPage_addr = (cPage_addr * M25PE16_PAGESIZE) + cInPage_addr;
	
	SpiFlashReadMoreByteData(mPage_addr, cArray, cCount);
	
	return TRUE;
}
#endif

/*
********************************************************************************************************
** �������� ��SF_Read()
** �������� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� : ��
** ��    ע :
*********************************************************************************************************
*/
u8 SF_Read(u32 cOffset_addr, u8 *cArray, u16 cCount)
{
	if(cOffset_addr >= 512*M25PE16_PAGESIZE)
	{
		return FALSE;
	}
	
	return SpiFlashReadMoreByteData(cOffset_addr, cArray, cCount);
}

/*
********************************************************************************************************
** �������� ��SF_Read()
** �������� ��page_addrҳ��ַ,in_page_byte_addrҳ�ڵ�ַ. array���ݻ�����, counter���ݳ���			  
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� : ��
** ��    ע :
*********************************************************************************************************
*/
u8 SF_Write(u32 cPage_addr, u32 cInPage_addr, u8  *cArray, u16 cCount,u8 cIsErase)
{
	u32 mPage_addr;
	u8 cFlashBuf[256];
	u16 i = 0;
	
	if(M25PE16_MAXPAGE<=cPage_addr||(cInPage_addr+cCount)>M25PE16_PAGESIZE)
	{
		return FALSE;
	}

	if(cIsErase)
	{
		SF_ErasePage(cPage_addr);
	}
	
	mPage_addr = (cPage_addr * M25PE16_PAGESIZE) + cInPage_addr;
	
	SpiFlashWrite(cArray, mPage_addr, cCount);

	while((i+256) < cCount)
	{
		if(SF_Read(mPage_addr+i,cFlashBuf,256) == FALSE)
		{
			return FALSE;
		}
		
		if(memcmp(&cArray[i],cFlashBuf,256) !=0)
		{
			return FALSE;
		}

		i+=256;
	}

	if(SF_Read(mPage_addr+i,cFlashBuf,cCount-i) == FALSE)
	{
		return FALSE;
	}
	
	if(memcmp(&cArray[i],cFlashBuf,cCount-i) !=0)
	{
		return FALSE;
	}

	return TRUE;
}

/*******************************************************************************
* ��������  : SpiFlashTest
* ��������  : �򿪴��ڣ�����һЩ��ʼ�����ã��Լ��ж�flash�Ƿ���������
* �������  : ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
#ifdef DEBUG_FLASH

void SpiFlashTest(void)
{
    u32 addr = 0x00000000;
    u16 tmp;
    u8  data, i = 1;
	u8 databuffertmp[1000];
    
    SpiFlashChipErase();
    
    //SpiFlashWriteEnable();
    
    data = SpiFlashReadStatus();
    //flash_debug("flash_s0=%x\r\n",data);
    
    //SpiFlashWriteStatus(0x00);
    
    data = SpiFlashReadStatus();
    //flash_debug("flash_s1=%x\r\n",data);
    
    SpiFlashByteWrite(0x00020000, 0x5f);
		
    data = SpiFlashReadByteData(0x00020000);
   //flash_debug("flash_s=%x\r\n",data);
	 
	SpiFlashByteWrite(0x00020000, 0x55);
    
    data = SpiFlashReadByteData(0x00020000);
   //flash_debug("flash_s=%x\r\n",data);

	SpiFlashByteWrite(0x00020000, 0xff);
    
    data = SpiFlashReadByteData(0x00020000);
  	//flash_debug("flash_s=%x\r\n",data);
				
    for(tmp = 0; tmp < 1000; tmp++)
    {
        data = SpiFlashReadByteData(addr);        
       	//flash_debug("flash_s=%x ",data);
        SpiFlashByteWrite(addr, i);        
        data = SpiFlashReadByteData(addr);        
        //flash_debug("flash_s=%x\r\n",data);
        
        addr++;
        i++;       
    }
    
    SpiFlashReadMoreByteData(0, databuffertmp, 1000);
    
    //SpiFlashPageWrite(databuffertmp, 2000);
    //SpiFlashPageWrite(databuffertmp, 2256);
    //SpiFlashPageWrite(databuffertmp, 2512);
    SpiFlashWrite(databuffertmp, 2000, 1000);
    
    addr = 2000; 
    
    for(tmp = 0; tmp < 1000; tmp++)
    {
        data = SpiFlashReadByteData(addr);        
        //flash_debug("flash_s=%x\r\n",data);   
              
        addr++;        
    }
		
    addr = SpiFlashReadJID();
    //flash_debug("flash_jid=%x\r\n",addr);
    
    addr = SpiFlashReadMID();    
	//flash_debug("flash_mid=%x\r\n",addr);
    
}
#endif

/****************************************End Of File************************************************
***************************************************************************************************/

