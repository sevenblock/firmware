/***************************************************************************************
* File name    :	Gpio.h
* Function     :	Header of Gpio.c
* Author       : 	Footman
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#ifndef	_GPIO_PORT_
#define	_GPIO_PORT_
#define GPIOA	0
#define GPIOB	1
#define	GPIO_OUT_OF_PORT(bPort)		((bPort!=GPIOA)&&(bPort!=GPIOB))
#endif

#define DRIVE_4MA	0
#define DRIVE_8MA	1
#define DRIVE_12MA	2
#define DRIVE_16MA	3

// Specified the bit of GPIO Port control
#ifndef	_BIT_
#define _BIT_
#define	BIT0			(1<<0)
#define	BIT1			(1<<1)
#define	BIT2			(1<<2)
#define	BIT3			(1<<3)
#define	BIT4			(1<<4)
#define	BIT5			(1<<5)
#define	BIT6			(1<<6)
#define	BIT7			(1<<7)
#define	BIT8			(1<<8)
#define	BIT9			(1<<9)
#define	BIT10			(1<<10)
#define	BIT11			(1<<11)
#define	BIT12			(1<<12)
#define	BIT13			(1<<13)
#define	BIT14			(1<<14)
#define	BIT15			(1<<15)
#define	BIT16			(1<<16)
#define	BIT17			(1<<17)
#define	BIT18			(1<<18)
#define	BIT19			(1<<19)
#define	BIT20			(1<<20)
#define	BIT21			(1<<21)
#define	BIT22			(1<<22)
#define	BIT23			(1<<23)
#define	BIT24			(1<<24)
#define	BIT25			(1<<25)
#define	BIT26			(1<<26)
#define	BIT27			(1<<27)
#define	BIT28			(1<<28)
#define	BIT29			(1<<29)
#define	BIT30			(1<<30)
#define	BIT31			(1<<31)
#define BIT_ALL			(0xffffffff)
#endif

#define GpioInit		GPIO_Init
#define GpioBitIn		GPIO_BitIn
#define	GpioBitOut		GPIO_BitOut
#define	GpioBitTog		GPIO_BitTog
#define	GpioBitSet		GPIO_BitSet
#define	GpioBitClr		GPIO_BitClr
#define	GpioBitIntClr	GPIO_BitIntClr
#define	GpioPortIn		GPIO_PortIn
#define	GpioPortIntStat	GPIO_PortIntStat
#define	GpioPortOut		GPIO_PortOut
#define	GpioPortTog		GPIO_PortTog
#define	GpioPortSet		GPIO_PortSet
#define	GpioPortClr		GPIO_PortClr
#define	GpioPortIntClr	GPIO_PortIntClr

/**************************************************************************
* Global Type Definition
***************************************************************************/



/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 GPIO_Init(UINT8 nPort, UINT8 nBitNum, UINT8 nMode);
UINT8 GPIO_BitIn(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitOut(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal);
UINT8 GPIO_BitTog(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitSet(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitClr(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitIntClr(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_GetBitUse(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitPullSet(UINT8 nPort, UINT8 nBitNum);
UINT8 GPIO_BitPullClr(UINT8 nPort, UINT8 nBitNum);
//UINT8 GPIO_BitODSet(UINT8 nPort, UINT8 nBitNum);
//UINT8 GPIO_BitODClr(UINT8 nPort, UINT8 nBitNum);
//UINT8 GPIO_BitDriveConfig(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal);

UINT32 GPIO_PortIn(UINT8 nPort);
UINT32 GPIO_PortIntStat(UINT8 nPort);
UINT8 GPIO_PortOut(UINT8 nPort, UINT32 nPortVal);
UINT8 GPIO_PortTog(UINT8 nPort, UINT32 nPortVal);
UINT8 GPIO_PortSet(UINT8 nPort, UINT32 nPortVal);
UINT8 GPIO_PortClr(UINT8 nPort, UINT32 nPortVal);
UINT8 GPIO_PortIntClr(UINT8 nPort, UINT32 nSpecBits);

#endif /*GPIO_H_*/
