/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_Lcd.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_Lcd_D
#include "..\block\app\App_Macro.h"

#define __PAGE_ADDR				0xB0                     //b0~b3 Pagenumber
#define __COL_ADDR_HI			0x10                     //b0~b3 line addr high for bit
#define __COL_ADDR_LO			0x00                     //b0~b3 line addr low for bit
#define __DISPLAY_OFF			0xAE                     //close LCD display
#define __DISPLAY_ON				0xAF                     //Open LCD display
#define __DISPLAY_RESET		0xE2                     //LCD soft reset
#define __NEGATIVE_LEVEL		0x2F                     //VB VF VR all open
#define __CONT_CTRL				0x81                     //set the electronic volume, first send 0x81, the second send one command(0x00-0x3f)

#define __BIAS								0xa2
#define __BIAS_CONTROL			0xa8                     //control current
#define __RATIO							0x3f                     //contrast

#define Ra_Rb               				0x25                  	/* internal resistor ratio */
#define vopcode             				0x1D                  	/*  EV  electronic volume */

#define LCD_A0							8
#define LCD_BLight						0
#define SPI_MOSI						9
#define SPI_SCK							7
#define SPI_CS								11
#define SPI_nRST						12

#define LCD_A0_L()   					GPIO_BitClr(GPIOA, LCD_A0)
#define LCD_A0_H()   					GPIO_BitSet(GPIOA, LCD_A0)
#define LCD_BLight_L()   			GPIO_BitClr(GPIOA, LCD_BLight)
#define LCD_BLight_H()   			GPIO_BitSet(GPIOA, LCD_BLight)
#define Spi_Open()						GPIO_BitClr(GPIOA, SPI_CS)
#define Spi_Close()						GPIO_BitSet(GPIOA, SPI_CS)
#define LCD_nRST_L()   				GPIO_BitClr(GPIOA, SPI_nRST)
#define LCD_nRST_H()   			GPIO_BitSet(GPIOA, SPI_nRST)

#define LCD_MOSI_L()   				GPIO_BitClr(GPIOA, SPI_MOSI)
#define LCD_MOSI_H()   			GPIO_BitSet(GPIOA, SPI_MOSI)
#define LCD_SCK_L()   				GPIO_BitClr(GPIOA, SPI_SCK)
#define LCD_SCK_H()   				GPIO_BitSet(GPIOA, SPI_SCK)

#define LCD_DelayUs					_delay_us
#define LCD_DelayMs					_delay_ms

const u8 c_LcdLine[] = {
	LCD_FONT_ASCII_5X8_LINE,
	LCD_FONT_ASCII_6X12_LINE,
	LCD_FONT_ASCII_8X16_LINE,
	LCD_FONT_ASCII_12X24_LINE,
	LCD_FONT_CHINESE_12X12_LINE,
	LCD_FONT_CHINESE_16X16_LINE,
	0
};

const u8 c_LcdLineMax[] = {
	LCD_FONT_ASCII_5X8_LINE_MAX,
	LCD_FONT_ASCII_6X12_LINE_MAX,
	LCD_FONT_ASCII_8X16_LINE_MAX,
	LCD_FONT_ASCII_12X24_LINE_MAX,
	LCD_FONT_CHINESE_12X12_LINE_MAX,
	LCD_FONT_CHINESE_16X16_LINE_MAX,
	0
};

const u8 c_LcdColumn[] = {
	LCD_FONT_ASCII_5X8_COLUMN,
	LCD_FONT_ASCII_6X12_COLUMNE,
	LCD_FONT_ASCII_8X16_COLUMN,
	LCD_FONT_ASCII_12X24_COLUMN,
	LCD_FONT_CHINESE_12X12_COLUMN,
	LCD_FONT_CHINESE_16X16_COLUMN,
	0
};

const u8 c_LcdRow[] = {
	LCD_FONT_ASCII_5X8_ROW,
	LCD_FONT_ASCII_6X12_ROW,
	LCD_FONT_ASCII_8X16_ROW,
	LCD_FONT_ASCII_12X24_ROW,
	LCD_FONT_CHINESE_12X12_ROW,
	LCD_FONT_CHINESE_16X16_ROW,
	0
};

const u8 c_LcdBit[] = {
	LCD_FONT_ASCII_5X8_BIT,
	LCD_FONT_ASCII_6X12_BIT,
	LCD_FONT_ASCII_8X16_BIT,
	LCD_FONT_ASCII_12X24_BIT,
	LCD_FONT_CHINESE_12X12_BIT,
	LCD_FONT_CHINESE_16X16_BIT,
	0
};

const u8 *c_LcdFontDot[] = {
	ASCII_5_8,
	NULL,
	ASCII_8_16,
	ASCII_12_24,
	NULL,
	chinese_16_16,
	NULL
};

const u8 *c_LcdWaiting[] = {
	gImage_Waiting1,
	gImage_Waiting2,
	gImage_Waiting3,
	gImage_Waiting4,
	gImage_Waiting5,
	NULL
};

const u8 LcdPara[]={
	0xe2, 	 //reset lcd
	0xa2,   //A0 95 fps fram rate; enable partial disp	
	0xA0,   //ADC seg镜像选择 0xa0正常，0xA1左右镜像
	0xc8,	 //com output scan direction，com镜像选择 0xc0正常，0xC8上下镜像
	0xa6,
	0x2c,
	0x2e,
	0x2F,
	0xf8,   // bias rate 9
	0x00,
	0x24,
	0x81,	 //set bias	voltage/potentionmeter PM
	0x12,   //电压调整寄存器低位 范围：0x00-0x3f ，显示文字色度
	0x40,    //电压调整寄存器高位 范围：0x21-0x27 ，值越大，背景越浓（底影越浓）
	0xaf,
};

/*
****************************************************************************************************
** 名    称 ：Spi_WriteData()
** 功    能 ：模拟SPI发送数据
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
#ifndef LCD_INTERFACE_SPI
void Spi_WriteData(u8 val)
{
    u8 i,j;
	
    j = 0x80;
	
    for(i=0; i<8; i++)
    {
        if(0 == (val&j))
        {
            LCD_MOSI_L();
        }
        else
        {
            LCD_MOSI_H();
        }
		
        LCD_SCK_H();
		
        j >>= 1;
		
        LCD_SCK_L();
    }
}
#endif
/*
****************************************************************************************************
** 名    称 ：LCD_Send_Cmd()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Send_Cmd(u8 command)
{
	#ifdef LCD_INTERFACE_SPI
    Spi_Enable(SPI_LCD_CS);        	//LCD CS = 0
    LCD_A0_L();			//A0 = 0
    Spi_Send_Byte(command);
    Spi_Disable(SPI_LCD_CS);        	//LCD CS = 1
	#else
    Spi_Open();        	//LCD CS = 0
    LCD_A0_L();			//A0 = 0
    Spi_WriteData(command);
    Spi_Close();        	//LCD CS = 1
    #endif
}

/*
****************************************************************************************************
** 名    称 ：LCD_Send_Data()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Send_Data(u8 data)
{
	#ifdef LCD_INTERFACE_SPI
    Spi_Enable(SPI_LCD_CS);        	//LCD CS = 0
    LCD_A0_H();			//A0 = 0
    Spi_Send_Byte(data);
    Spi_Disable(SPI_LCD_CS);        	//LCD CS = 1
	#else
    Spi_Open();    	//LCD CS = 0
    LCD_A0_H();	//A0 = 1
    Spi_WriteData(data);
    Spi_Close();		//LCD CS = 1    
    #endif
}

/*
****************************************************************************************************
** 名    称 ：LCD_Ram_SetAddr()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Ram_SetAddr(u8 page, u8 col)
{
	  u8 tempVal;
	  
	  //"发送页地址"
	  tempVal = (u8)(0x0f & page);
	  tempVal |= __PAGE_ADDR;
	  LCD_Send_Cmd(tempVal);
	  
	  //"发送列地址高字节"
	  tempVal = 0x0f & (col >> 4);
	  tempVal |= __COL_ADDR_HI;
	  LCD_Send_Cmd(tempVal);
	  
	  //"发送列地址低字节"
	  tempVal = 0x0f & col;
	  tempVal |= __COL_ADDR_LO;
	  LCD_Send_Cmd(tempVal);
}

/*
****************************************************************************************************
** 名    称 ：LCD_Refresh()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Refresh(void)
{
    u16 x,y;

	for(y=0;y<(MAX_DOT_ROW/8);y++)
	{
		LCD_Ram_SetAddr(y, 0x00); 
		
		for(x=0;x<MAX_DOT_COL;x++)
		{
			LCD_Send_Data(s_DisplayBuf[y][x]);
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：LCD_FillLine()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 LCD_FillLine(u8 t_aFont,u8 t_aLine,u8 t_cData)
{
	u16 i,x,y;

	if(t_aFont > LCD_FONT_CHINESE_16X16)
	{
		return 1;
	}
	
	if(t_aLine >= c_LcdLineMax[t_aFont])
	{
		return 2;
	}
	
	for(i=0;i<c_LcdRow[t_aFont];i++)
	{
		y = i * c_LcdBit[t_aFont]+t_aLine * c_LcdLine[t_aFont];
		
		for(x=0;x<MAX_DOT_COL;x++)
		{
			LCD_SetXYDot(t_aFont,x,y,t_cData);
		}
	}

	return 0;
}

/*
****************************************************************************************************
** 名    称 ：LCD_FillScreen()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_FillScreen(u8 t_cData,u8 t_cPara)
{
	u16 i,j;
	
	for(i=0;i<(MAX_DOT_ROW/8);i++)
	{		
		for(j=0;j<MAX_DOT_COL;j++)
		{
			s_DisplayBuf[i][j] = t_cData;
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：LCD_FillRow()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_FillRow(u8 t_aFont,u8 t_aLine, u8 t_aOffset,u8 t_aColumn, char *t_pcDisp, u8 t_aAtr)
{
	LCDPARA uLcdPara;

	memset(&uLcdPara,0x00,sizeof(uLcdPara));

	uLcdPara.m_aLine = t_aLine;
	uLcdPara.m_aLineOffset = t_aOffset;
	uLcdPara.m_aColumn = t_aColumn;

	if(t_aAtr & LCD_CLEAR_LINE)
	{
		uLcdPara.m_aClear = LCD_CLEAR_LINE;
	}
	
	uLcdPara.m_aAutoAlign = t_aAtr & LCD_ALIGN_AUTO;
	uLcdPara.m_aAutoWrap = t_aAtr & LCD_WRAP_AUTO;
	
	uLcdPara.m_aFont = t_aFont;

	if(t_aAtr & LCD_ALIGN_CENTER)
	{
		uLcdPara.m_aAlign = LCD_ALIGN_CENTER;
	}
	else if(t_aAtr & LCD_ALIGN_RIGHT)
	{
		uLcdPara.m_aAlign = LCD_ALIGN_RIGHT;
	}
	else
	{
		uLcdPara.m_aAlign = LCD_ALIGN_LEFT;
	}
	
	uLcdPara.m_aStyle = t_aAtr & LCD_STYLE_BACKCOLOR;
	
	uLcdPara.m_pcStr = t_pcDisp;

	LCD_LightTime(LCD_BLingh_ON,0);

	LCD_DisplayRow(&uLcdPara);
	
}

/*
****************************************************************************************************
** 名    称 ：LCD_FillPictrue()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_FillPictrue(u8 cType)
{
	u8 x;
	u8 y;
	const u8 *pt;

	if(cType == LOGO_SEVENBLOCK)
	{
		pt = gImage_Logo_Sevenblock;
	}
	else if(cType == LOGO_PSIWALLET)
	{
		pt = gImage_Logo_PsiWallet;
	}
	else
	{
		return;
	}
	
	for(y=0;y<(MAX_DOT_ROW/8);y++)
	{		
		for(x=0;x<MAX_DOT_COL;x++)
		{
			s_DisplayBuf[y][x] = pt[y*128+x];
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：LCD_FillIcon()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_FillIcon(u8 cType,u8 t_cPara)
{
	u8 x;
	const u8 *pt;
	u8 aStartX;
	u8 aCount;

	if(cType <= ICON_BATTERY5)
	{
		aStartX = MAX_DOT_COL - 16;
		aCount = 16;
		
		pt = &gImage_Battery[cType*16];
		
	}
	else if(cType == ICON_BLUETOOTH)
	{
		aStartX = 4;
		aCount = 8;
		
		pt = gImage_Bluetooth;
	}
	else if(cType == ICON_WALLET)
	{
		aStartX = 16+4;
		aCount = 16;
		
		pt = gImage_Wallet;
	}
	else
	{
		return;
	}

	if(t_cPara)
	{
		for(x=0;x<aCount;x++)
		{
			s_DisplayBuf[0][aStartX+x] = pt[x];
		}
	}
	else
	{
		for(x=0;x<aCount;x++)
		{
			s_DisplayBuf[0][aStartX+x] = 0x00;
		}
	}

	LCD_Refresh();
	
}

/*
****************************************************************************************************
** 名    称 ：LCD_Waiting()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Waiting(void)
{
	static u8 aState = 0;
	u8 x;
	u8 y;
	
	if(ValSystemInfo.m_Waiting)
	{
		if(aState >= 5)
		{
			aState = 0;
		}

		if(c_LcdWaiting[aState] != NULL)
		{
			for(y=1;y<(MAX_DOT_ROW/8);y++)
			{		
				for(x=0;x<MAX_DOT_COL;x++)
				{
					s_DisplayBuf[y][x] = c_LcdWaiting[aState][(y-1)*128+x];
				}
			}
		}

		aState++;

		LCD_Refresh();
	}
}

/*
****************************************************************************************************
** 名    称 ：LCD_LightTime()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_LightTime(u8 cFlag,u32 t_nDelay)
{
	if(cFlag == LCD_BLingh_OFF)
	{
		LCD_BLight_L();
	}
	else if(cFlag == LCD_BLingh_ON)
	{
		LCD_BLight_H();

		ValSystemInfo.cLinghtTime = t_nDelay;
	}
}
/*
****************************************************************************************************
** 名    称 ：LCD_BackLightAdj()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_BackLightAdj(u8 t_cType)
{
	static u8 cBackLight = 0x3F;
	char aDisp[17];
	
	if(t_cType)
	{
		if(cBackLight < (0x3F-5))
		{
			cBackLight+=5;
		}
	}
	else
	{
		if(cBackLight > 5)
		{
			cBackLight-=5;
		}
	}
	
	LCD_Send_Cmd(0x81);
	LCD_Send_Cmd(cBackLight);

	memset(aDisp,0x00,sizeof(aDisp));
	
	sprintf(aDisp,"%x",cBackLight);

	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, aDisp, LCD_ALIGN_CENTER|LCD_CLEAR_LINE);
	
}
/*
****************************************************************************************************
** 名    称 ：LCD_Close()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_Close(void)
{		
	LCD_nRST_L();
	LCD_DelayMs(1);
	LCD_nRST_H();
	//LCD_nRST_L();
	//LCD_A0_L();
	//LCD_BLight_L();
	//Spi_Close();
	//LCD_MOSI_L();
	//LCD_SCK_L();
}

/*
****************************************************************************************************
** 名    称 ：LCD_init()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void LCD_init(void)
{
	char aDisp[17];
	char aTemp[4];
	u8 cCount = 0;
	
	#ifndef LCD_INTERFACE_SPI
	GPIO_Init(GPIOA, SPI_MOSI, OUTPUT);
	GPIO_Init(GPIOA, SPI_SCK, OUTPUT);
	GPIO_Init(GPIOA, SPI_CS, OUTPUT);
	GPIO_Init(GPIOA, LCD_A0, OUTPUT);
	#endif
	
	GPIO_Init(GPIOA, LCD_BLight, OUTPUT);
	GPIO_Init(GPIOA, SPI_nRST, OUTPUT);
	
	LCD_nRST_L();
	LCD_DelayUs(100);
	LCD_nRST_H();
	
	LCD_LightTime(LCD_BLingh_ON,0);

	#if(LCD_TYPE == LCD_TYPE_LCD)
	LCD_Send_Cmd(__DISPLAY_OFF);             //close LDE display
	LCD_Send_Cmd(__DISPLAY_RESET);           //LCD soft reset
	LCD_DelayUs(100);
	
	LCD_Send_Cmd(__BIAS);
	LCD_Send_Cmd(0xa1);
	LCD_Send_Cmd(0xc8);
	LCD_Send_Cmd(0xa4);
	LCD_Send_Cmd(0x40);
	LCD_Send_Cmd(0x24);
	LCD_Send_Cmd(__CONT_CTRL);
	LCD_Send_Cmd(0x1F);		//对比度 00~3F
	LCD_Send_Cmd(0x2c);
	LCD_DelayUs(100);
	LCD_Send_Cmd(0x2e);
	LCD_DelayUs(100);
	LCD_Send_Cmd(__NEGATIVE_LEVEL);       //
	LCD_DelayUs(100);
	LCD_Send_Cmd(0x00);
	LCD_DelayUs(50);
	
	LCD_Send_Cmd(__DISPLAY_ON);
	#else
	LCD_Send_Cmd(__DISPLAY_OFF);             //close LDE display	
	LCD_Send_Cmd(0xD5);
	LCD_Send_Cmd(0x80);
	LCD_Send_Cmd(0xa8);
	LCD_Send_Cmd(0x3F);
	LCD_Send_Cmd(0xD3);
	LCD_Send_Cmd(0x00);
	LCD_Send_Cmd(0x40);
	LCD_Send_Cmd(0x8D);
	LCD_Send_Cmd(0x14);
	LCD_Send_Cmd(0xA1);
	LCD_Send_Cmd(0xC8);
	LCD_Send_Cmd(0xDA);
	LCD_Send_Cmd(0x12);	
	LCD_Send_Cmd(0x81);
	LCD_Send_Cmd(0x3F);
	
	LCD_Send_Cmd(0xD9);
	LCD_Send_Cmd(0xF1);	
	LCD_Send_Cmd(0xDB);
	LCD_Send_Cmd(0x40);	
	LCD_Send_Cmd(0xA4);
	LCD_Send_Cmd(0xA6);	
	
	LCD_Send_Cmd(__DISPLAY_ON);
	#endif
	
	LCD_DelayUs(100);
	
	LCD_FillScreen(0x00,0);

	memcpy(aTemp,(u8 *)VERSION_SOFT,3);
	memset(aDisp,0x00,sizeof(aDisp));

	aDisp[cCount++] = 'V';
	
	if(aTemp[0] >= 10)
	{
		aDisp[cCount++] = (aTemp[0]>>4) + '0';
		aDisp[cCount++] = (aTemp[0]&0x0F) + '0';
	}
	else
	{
		aDisp[cCount++] = aTemp[0] + '0';
	}

	aDisp[cCount++] = '.';
	
	if(aTemp[1] >= 10)
	{
		aDisp[cCount++] = (aTemp[1]>>4) + '0';
		aDisp[cCount++] = (aTemp[1]&0x0F) + '0';
	}
	else
	{
		aDisp[cCount++] = aTemp[1] + '0';
	}

	aDisp[cCount++] = '.';

	if(aTemp[2] >= 10)
	{
		aDisp[cCount++] = (aTemp[2]>>4) + '0';
		aDisp[cCount++] = (aTemp[2]&0x0F) + '0';
	}
	else
	{
		aDisp[cCount++] = aTemp[2] + '0';
	}
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,8, 0, aDisp, LCD_ALIGN_CENTER);
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0, "Loading...", LCD_ALIGN_CENTER);

	LCD_Progress(0,100);
	
}

/****************************************End Of File************************************************
***************************************************************************************************/

