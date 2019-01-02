/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_SpiFlash.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
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
* 函数名称: SpiFlashWriteEnable
* 功能描述: 对串口 flash 写使能 
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
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
* 函数名称: SpiFlashWriteDisable
* 功能描述: 对串口 flash 写禁止
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
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
* 函数名称: SpiFlashReadStatus
* 功能描述: 读状态寄存器
* 输入参数: 无
* 输出参数: 无
* 返回参数: 当前flash内部状态寄存器的值
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
* 函数名称: SpiFlashBusyWait
* 功能描述: 等待flash内部操作完成
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
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
* 函数名称: SpiFlashWriteStatus
* 功能描述: 写状态寄存器
*           注意：本很函数是针对3307原理图设计的，如果原理图wp位有效，需修改！！
* 输入参数: new_status：当前flash内部状态寄存器的值
* 输出参数: 无
* 返回参数: 无
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
* 函数名称: SpiFlashReadByteData
* 功能描述: 从串口 flash 固定地址读出一个字节数据
* 输入参数: address：要读出数据的地址
* 输出参数: 无
* 返回参数: 读出的数据
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
* 函数名称: SpiFlashReadMoreByteData
* 功能描述: 从串口 flash 某地址起始处读出一组数据
* 输入参数: address_from：要读出数据的起始地址
*           save_pointer：要把读出的数据放到内存指针
*           data_num：要读出的数据字节数
* 输出参数: 无
* 返回参数: 如果参数不合理返回 FALSE 否则返回 TRUE
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
* 函数名称: SpiFlashFastReadMoreByteData
* 功能描述: 利用快读指令从串口 flash 某地址起始处读出一组数据
* 输入参数: address_from：要读出数据的起始地址
*           save_pointer：要把读出的数据放到内存指针
*           data_num：要读出的数据字节数
* 输出参数: 无
* 返回参数: 如果参数不合理返回 FALSE  否则返回 TRUE
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
* 函数名称  : SpiFlashSectorErase
* 功能描述  : 擦除串口flash的一个扇区，注意写保护。如果保护将擦除不成功。
* 输入参数  : sector_addr: 包含在要擦除扇区地址空间内的一个地址
* 输出参数  : 无
* 返回参数  : 无
*******************************************************************************/
void SpiFlashSectorErase(u32 sector_addr)
{
	u32 len = 4;
	u8 command[4];

	command[0] = SE;
	command[1] = (u8)(sector_addr>>16);
	command[2] = (u8)(sector_addr>>8);
	command[3] = (u8)(sector_addr);
    /* -----写使能-------- */
    SpiFlashWriteEnable();
    
    Spi_Enable(SPI_FLASH_CS);
    Spi_Comm(command, len,NULL, 0); 
    Spi_Disable(SPI_FLASH_CS);

    /*------确保flash里面没有任何写操作---*/
    SpiFlashWaitWriteEnd();    
}

/*******************************************************************************
* 函数名称  : SpiFlashBulkErase
* 功能描述  : 擦除串口flash的一个块，注意写保护。如果保护将擦除不成功。
* 输入参数  : bulk_addr: 包含在要擦除扇区地址空间内的一个地址
* 输出参数  : 无
* 返回参数  : 无
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

    /*------确保flash里面没有任何写操作---*/
    SpiFlashWaitWriteEnd();
}

/*******************************************************************************
* 函数名称  : SpiFlashChipErase
* 功能描述  : 擦除整个串口flash。注意写保护。如果保护将擦除不成功。
* 输入参数  : bulk_addr: 包含在要擦除扇区地址空间内的一个地址
* 输出参数  : 无
* 返回参数  : 无
*******************************************************************************/
void SpiFlashChipErase(void)
{
	u32 len = 1;
	u8 command = CE;
	
    /*----先去除保护-------*/
    //SpiFlashWriteStatus(0x00);
    
    /* ---发送写使能------*/
    SpiFlashWriteEnable();

    Spi_Enable(SPI_FLASH_CS);
    
    /*----发送指令--------*/
    Spi_Comm(&command, len,NULL, 0);
    
    Spi_Disable(SPI_FLASH_CS);

    /*------确保flash里面没有任何写操作---*/
    SpiFlashWaitWriteEnd();   
}

/*******************************************************************************
* 函数名称  : SpiFlashByteWrite
* 功能描述  : 向flash地址空间写一个字节数据
* 输入参数  : address : 字节数据要写到的串口flash的地址
*             data: 要写的字节数据。             
* 输出参数  : 无
* 返回参数  : 无
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

    /*------先确保flash里面没有任何写操作---*/
    SpiFlashWaitWriteEnd();       
}

/*******************************************************************************
* 函数名称  : SpiFlashPageWrite
* 功能描述  : 写256字节 flash 的一页内
* 输入参数  : pbuffer : 要写入flash的数据所在缓冲区的指针
*             addr_start : 要写入flash内部的空间的起始地址             
* 输出参数  : 无
* 返回参数  : 无
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
    
    /*--确保flash里面没有任何写操作--*/
    SpiFlashWaitWriteEnd();  
}

/*******************************************************************************
* 函数名称  : SpiFlashWrite
* 功能描述  : 写一组数据到 flash 的存储空间
* 输入参数  : pbuffer : 要写入flash的数据所在缓冲区的指针
*             addr_start : 要写入flash内部的空间的起始地址
*             data_byte_number : 写入flash内部空间数据的字节数
* 输出参数  : 无
* 返回参数  : 无
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
        
        /*--确保flash里面没有任何写操作--*/
        SpiFlashWaitWriteEnd();       
    }
}

/*******************************************************************************
* 函数名称  : SpiFlashReadMID
* 功能描述  : 读flash M ID
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : flash M ID 
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
* 函数名称  : SpiFlashReadJID
* 功能描述  : 读flash JEDEC ID
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : flash JEDEC ID 
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
* 函数名称  : SpiFlashPowerDown
* 功能描述  : 进入节电模式，使闪存睡眠。在机器应用电池时，比较有用
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : 无 
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
* 函数名称  : SpiFlashPowerDownReturn
* 功能描述  : 使flash从节电模式返回
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : 无 
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
* 函数名称  : SpiFlashWaitWriteEnd
* 功能描述  : 等待对串口闪存内部写或者擦除操作结束
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : 无
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
		if(t++ > 0x10000)	// 超时
			break;
		*/
        Spi_Comm(NULL, 0,&flash_status, len);
    }
    while ((flash_status & 0x01) != 0); /* Write in progress */
    
    /* Deselect the FLASH: Chip Select high */
    Spi_Disable(SPI_FLASH_CS);
}

/*******************************************************************************
* 函数名称  : SpiFlashOpen
* 功能描述  : 打开串口，包括一些初始化设置，以及判断flash是否正常工作
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : FALSE 和 TRUE
*******************************************************************************/
u8  SpiFlashOpen(void)
{
    if(SpiFlashReadJID() == 0x00ef3015)
    {
        return(TRUE);
    }
	
	//flash_debug("flash JID错误\r\n");
	
    return(FALSE);
}

/*
*********************************************************************************************************
** 函数名称 ：SF_ErasePage()
** 函数功能 ：页擦除  
** 入口参数 ：page_addr   要擦除的页地址
** 出口参数 ：0失败  1成功
** 返    回 : 无
** 备    注 :
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
** 函数名称 ：SF_Read()
** 函数功能 ：page_addr页地址,in_page_byte_addr页内地址. array数据缓冲区, counter数据长度
** 入口参数 ：无
** 出口参数 ：无
** 返    回 : 无
** 备    注 :
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
** 函数名称 ：SF_Read()
** 函数功能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 : 无
** 备    注 :
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
** 函数名称 ：SF_Read()
** 函数功能 ：page_addr页地址,in_page_byte_addr页内地址. array数据缓冲区, counter数据长度			  
** 入口参数 ：无
** 出口参数 ：无
** 返    回 : 无
** 备    注 :
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
* 函数名称  : SpiFlashTest
* 功能描述  : 打开串口，包括一些初始化设置，以及判断flash是否正常工作
* 输入参数  : 无
* 输出参数  : 无
* 返回参数  : 无
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

