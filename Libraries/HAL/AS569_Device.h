/***************************************************************************
* File name    :	AS569_Device.h
* Function     :	Header of AS569 device to include all register struct
* 					and data type header
* Author       : 	coins
* Date         :	2015/09/23
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
#ifndef AS569_DEVICE_H_
#define AS569_DEVICE_H_

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/* locate variable at ABS_ADDR */
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
	#define	__AT(ABS_ADDR)	__attribute__((at(ABS_ADDR)))	
#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
	#define	__AT(ABS_ADDR)	@ (ABS_ADDR)					
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif

/* non-initialization variable keyword */
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
	#define	__NO_INIT
#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
	#define	__NO_INIT	__no_init					
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif


/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT           0       /*!< cm0ikmcu does not provide a MPU present or not       */
#define __NVIC_PRIO_BITS        2       /*!< cm0ikmcu Supports 2 Bits for the Priority Levels     */
#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used         */

/* HFROSC output clock frequency definition */
#define	F_HFROSC_0		12000000UL		/* Grade = 0 */
#define	F_HFROSC_1		48000000UL		/* Grade = 1 */
#define	F_HFROSC_2		96000000UL		/* Grade = 2 */
#define	F_HFROSC_3		144000000UL		/* Grade = 3 */
#define	F_HFROSC		F_HFROSC_2		/* HFROSC default output frequency */

/* MFROSC output clock frequency definition */
#define	F_MFROSC		4000000UL		

/* LFROSC output clock frequency definition */
#define	F_LFROSC		32000UL	

/* External RTC clock frequency definition */
#define F_EXT_RTC		32768UL

/* External clock frequency definition */
#define	F_XTAL			12000000UL		

//--------------------------------------------------------------------------
// define the peripheral ID
//--------------------------------------------------------------------------
#define	ID_SCM		0
#define	ID_GPIO		1
#define	ID_T0		2
#define	ID_SEA		3
#define	ID_AEA		4
#define	ID_SPI0		5
#define	ID_TRNG		6
#define	ID_USB		7
#define	ID_T1		8
#define	ID_T2		9
#define	ID_SSI		10
#define	ID_UART0	11
#define	ID_I2C0		12
#define	ID_SWPS		13
#define	ID_FD12M	14
#define	ID_ISO7816	15
#define	ID_SCI		16
#define	ID_DMA		17
#define	ID_VPWM		18
#define	ID_SDSI		19
#define	ID_AOSS		20
#define	ID_EFC		21
#define	ID_UART1	22
#define	ID_UART2	23
#define	ID_T3		24
#define	ID_PUF		25
#define	ID_SDI		26
#define	ID_SPI1		27
#define	ID_T4		28
#define	ID_T5		29
#define	ID_T6		30
#define	ID_T7		31
#define	ID_SQI		32
#define	ID_CRC		33
#define	ID_HASH		34
#define	ID_CACHE	35

#define	ID_APSI		36

// -------------------------------------------------------------------------
// define the memory-map-register base address
#define	MMR_BASE		0x40000000
#define	APB_BASE		MMR_BASE
#define	AHB_BASE		(MMR_BASE + 0x10000000)
#define	APB_SPACE		0x4000
#define	AHB_SPACE		0x10000

#define	APB_PERI_BASE(id)	(APB_BASE + APB_SPACE * (id))	
#define	AHB_PERI_BASE(id)	(AHB_BASE + AHB_SPACE * (id))		

// APB
#define	SCM_BASE		APB_PERI_BASE(ID_SCM)	// 0x40000000
#define	GPIO_BASE		APB_PERI_BASE(ID_GPIO)	// 0x40004000
#define	T0_BASE			APB_PERI_BASE(ID_T0) 	// 0x40008000
//#define	SEA_BASE		APB_PERI_BASE(ID_SEA)	// 0x4000C000
//#define	AEA_BASE		APB_PERI_BASE(ID_AEA)  	// 0x40010000
#define	SPI0_BASE		APB_PERI_BASE(ID_SPI0) 	// 0x40014000
//#define	TRNG_BASE		APB_PERI_BASE(ID_TRNG)	// 0x40018000
											 	// 0x4001C000
#define	T1_BASE			APB_PERI_BASE(ID_T1) 	// 0x40020000
#define	T2_BASE			APB_PERI_BASE(ID_T2)  	// 0x40024000
#define	SSI_BASE		APB_PERI_BASE(ID_SSI) 	// 0x40028000
#define	UART0_BASE		APB_PERI_BASE(ID_UART0)	// 0x4002C000
#define	I2C0_BASE		APB_PERI_BASE(ID_I2C0)	// 0x40030000
#define	SWPS_BASE		APB_PERI_BASE(ID_SWPS)	// 0x40034000
#define	FD12M_BASE		APB_PERI_BASE(ID_FD12M)	// 0x40038000
#define	ISO7816_BASE	APB_PERI_BASE(ID_ISO7816)//0x4003C000
#define	SCI_BASE		APB_PERI_BASE(ID_SCI)	// 0x40040000
												// 0x40044000
#define	VPWM_BASE		APB_PERI_BASE(ID_VPWM)	// 0x40048000
 												// 0x4004C000 
											 	// 0x40050000
#define	EFC_BASE		APB_PERI_BASE(ID_EFC) 	// 0x40054000
#define	UART1_BASE		APB_PERI_BASE(ID_UART1)	// 0x40058000
#define	UART2_BASE		APB_PERI_BASE(ID_UART2)	// 0x4005C000
#define	T3_BASE			APB_PERI_BASE(ID_T3) 	// 0x40060000
#define	PUF_BASE		APB_PERI_BASE(ID_PUF)	// 0x40064000
												// 0x40068000
#define	SPI1_BASE		APB_PERI_BASE(ID_SPI1)	// 0x4006C000
#define	T4_BASE			APB_PERI_BASE(ID_T4) 	// 0x40070000
#define	T5_BASE			APB_PERI_BASE(ID_T5)	// 0x40074000
#define	T6_BASE			APB_PERI_BASE(ID_T6)	// 0x40078000
#define	T7_BASE			APB_PERI_BASE(ID_T7)	// 0x4007C000
#define	SQI_BASE		APB_PERI_BASE(ID_SQI)	// 0x40080000
#define	CRC_BASE		APB_PERI_BASE(ID_CRC)	// 0x40084000
#define	HASH_BASE		APB_PERI_BASE(ID_HASH)	// 0x40088000
#define	CACHE_BASE		APB_PERI_BASE(ID_CACHE)	// 0x4008C000

#define	APSI_BASE		APB_PERI_BASE(ID_APSI) 	// 0x40090000
// AHB
#define	SDSI_BASE		AHB_PERI_BASE(0)		// 0x50000000	
#define	USB_BASE		AHB_PERI_BASE(1)		// 0x50010000
#define	SDI_BASE		AHB_PERI_BASE(2) 		// 0x50020000
#define	DMA_BASE		AHB_PERI_BASE(3) 		// 0x50030000
#define	AOSS_BASE		AHB_PERI_BASE(4) 		// 0x50040000	


/*General Return Code */
#define RT_OK            0x00 //success
#define RT_FAIL          0x01  //fail
#define RT_COMMAND_ERR   0x02  //command error
#define RT_PARAM_ERR     0x03  //param error
#define RT_OVERTIME      0x04  //over time
#define RT_ECC_ERR       0x05  //ecc error
#define RT_WRITE_ERR     0x06  //write flash err
#define RT_READ_ERR      0x07  //read flash err
#define RT_DATA_EXIST    0x08  //data is exist
#define RT_DATA_NOT_EXIST 0x09

#define RT_REPOWER		 0xE0 //repower

// define BOOL value
#ifndef FALSE
#define FALSE			(0)
#endif

#ifndef TRUE
#define TRUE				(1)
#endif

#define false 	0
#define true  	1

#ifndef NULL
#define NULL 	0
#endif


//the function of GPIO to be configured
//the value from 0 to 15, but not all mode are used
#ifndef _GPIO_MODE_
#define _GPIO_MODE_
#define INPUT     			0
#define FUNC1				1
#define FUNC2				2
#define FUNC3				3
#define FUNC4				4
#define	FUNC5				5
#define OUTPUT    			8
#define	INT_RISE_EDGE		12
#define INT_FALL_EDGE		13

// Define the pheriphals' MUX 
#define MUX_SPI         FUNC1
#define MUX_I2C         FUNC3
#define MUX_UART0       FUNC1
#define MUX_UART1       FUNC2
#define MUX_UART2       FUNC1
#define MUX_SQI         FUNC1
#define	MUX_TMR			FUNC4
#define MUX_SWD        	FUNC1
#define MUX_SSI         FUNC2
#define	MUX_SCI			FUNC1
#define	MUX_ISO7816		FUNC1
#define	MUX_SDI			FUNC4
#define	MUX_SDSI		FUNC4
#define	MUX_VPWM		FUNC3
#define MUX_CLKOUT      FUNC2
#define	MUX_SDIN		FUNC1
#define	MUX_APSI		FUNC5
#endif	
	

/**************************************************************************
* Global Type Definition
***************************************************************************/
/* Firmware Library General Type */
//------- BOOL type definition --------------
#ifndef	_BOOL_TYPE_
#define	_BOOL_TYPE_
typedef	unsigned char BOOL;
//typedef	unsigned char bool;
#endif

//------- UINT8 type definition --------------
#ifndef	_UINT8_TYPE_
#define _UINT8_TYPE_
typedef unsigned char UINT8;
#endif

//------- UINT16 type definition --------------
#ifndef	_UINT16_TYPE_
#define _UINT16_TYPE_
typedef unsigned short UINT16;
#endif

//------- UINT32 type definition --------------
#ifndef	_UINT32_TYPE_
#define _UINT32_TYPE_
typedef unsigned long UINT32;
#endif

//------- UINT64 type definition --------------
#ifndef	_UINT64_TYPE_
#define _UINT64_TYPE_
typedef unsigned long long UINT64;
#endif

//-------- INT8 type definition --------------
#ifndef	_INT8_TYPE_
#define _INT8_TYPE_
typedef signed char  INT8;
#endif

//-------- INT16 type definition --------------
#ifndef	_INT16_TYPE_
#define _INT16_TYPE_
typedef signed short INT16;
#endif

//-------- INT32 type definition --------------
#ifndef	_INT32_TYPE_
#define _INT32_TYPE_
typedef signed long INT32;
#endif

//------- INT64 type definition --------------
#ifndef	_INT64_TYPE_
#define _INT64_TYPE_
typedef signed long long INT64;
#endif

//------- VUINT8 type definition --------------
#ifndef	_VUINT8_TYPE_
#define _VUINT8_TYPE_
typedef volatile unsigned char VUINT8;
#endif

//------- VUINT16 type definition --------------
#ifndef	_VUINT16_TYPE_
#define _VUINT16_TYPE_
typedef volatile unsigned short VUINT16;
#endif

//------- VUINT32 type definition --------------
#ifndef	_VUINT32_TYPE_
#define _VUINT32_TYPE_
typedef volatile unsigned long VUINT32;
#endif

//------- VINT8 type definition --------------
#ifndef	_VINT8_TYPE_
#define _VINT8_TYPE_
typedef volatile signed char VINT8;
#endif

//------- VINT16 type definition --------------
#ifndef	_VINT16_TYPE_
#define _VINT16_TYPE_
typedef volatile signed short VINT16;
#endif

//------- VUINT32 type definition --------------
#ifndef	_VINT32_TYPE_
#define _VINT32_TYPE_
typedef volatile signed long VINT32;
#endif


/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NonMaskableInt_IRQn     = -14,    /*!< 2 Non Maskable Interrupt                             */
	HardFault_IRQn	      	= -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
	SVCall_IRQn             = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
	PendSV_IRQn             = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
	SysTick_IRQn            = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  CM0IKMCU Cortex-M0 specific Interrupt Numbers ********************************************/
	SCM_IRQn                = ID_SCM,      /*!< SCM Interrupt                                       */
	                                  /*!< maximum of 32 Interrupts are possible                */
	GPIO_IRQn         		= ID_GPIO,
	T0_IRQn         		= ID_T0,
	SEA_IRQn         		= ID_SEA,
	AEA_IRQn         		= ID_AEA,
	SPI0_IRQn      			= ID_SPI0,
	TRNG_IRQn         		= ID_TRNG,
	USB_IRQn         		= ID_USB,
	T1_IRQn         		= ID_T1,
	T2_IRQn         		= ID_T2,
	SSI_IRQn         		= ID_SSI,
	UART0_IRQn         		= ID_UART0,
	I2C0_IRQn        		= ID_I2C0, 
	SWPS_IRQn         		= ID_SWPS,
	FD12M_IRQn         		= ID_FD12M,	
	ISO7816_IRQn         	= ID_ISO7816,
	SCI_IRQn         		= ID_SCI,
	DMA_IRQn         		= ID_DMA,
	VPWM_IRQn         		= ID_VPWM,
	SDSI_IRQn         		= ID_SDSI,
	AOSS_IRQn         		= ID_AOSS,
	EFC_IRQn         		= ID_EFC,
	UART1_IRQn         		= ID_UART1,
	UART2_IRQn         		= ID_UART2,
	T3_IRQn         		= ID_T3,
	PUF_IRQn         		= ID_PUF,
	SDI_IRQn         		= ID_SDI,
	SPI1_IRQn         		= ID_SPI1,
	T4_IRQn         		= ID_T4,
	T5_IRQn         		= ID_T5,
	T6_IRQn         		= ID_T6,
	T7_IRQn         		= ID_T7,
} IRQn_Type;

#define	APSI_IRQn		SEA_IRQn		

/*
 * ==========================================================================
 * ---------- Perpheral Registers Definition --------------------------------
 * ==========================================================================
 */

/*
*************************** SCM ******************************************
*/
// System Clock Control Register bit field
struct	SCM_SYSCLKCTRL_BITS	{
	UINT32	FclkSrc:1;		//0	
	UINT32	Clk32kSel:1;	//1
	UINT32	FclkSel:2;		//3:2
	UINT32  RoscClkSel:1;	//4
	UINT32	ExtClkSel:1;	//5
	UINT32	Rsvd1:17;		//22:6
	UINT32	ClkSqiEn:1;		//23
	UINT32	UsbClk48mEn:1;	//24
	UINT32	UsbClk48mSel:2;	//26:25
	UINT32	Rsvd2:5;		//31:27
};
union	SCM_SYSCLKCTRL_REG	{
	UINT32	all;
	struct	SCM_SYSCLKCTRL_BITS	bit;
};

// System Reset Control Register bit field
struct	SCM_SYSRSTCTRL_BITS	{
	UINT32	Pdr33:1;
	UINT32	Vdd33Vd:1;
	UINT32 	Rsvd1:6;
	UINT32	Vdd18Vd:1;
	UINT32 	Pdr18:1;
	UINT32	Rsvd2:6;
	UINT32	Vdd11Vd:1;
	UINT32	Rsvd3:9;
	UINT32	SramErr:1;	
	UINT32	Rsvd5:5; 
};
union	SCM_SYSRSTCTRL_REG	{
	UINT32	all;
	struct	SCM_SYSRSTCTRL_BITS	bit;
};

// System Reset Status Register bit field
struct	SCM_SYSRSTSTATUS_BITS	{
	UINT32	Pdr33:1;
	UINT32	Vdd33Vd:1;
	UINT32	Wdt:1;
	UINT32	Soft:1;
	UINT32	Pdr18:1;
	UINT32 	Rsvd1:3;
	UINT32	Vdd18Vd:1;
	UINT32	Rsvd3:7;
	UINT32	Vdd11Vd:1;
	UINT32	Rsvd4:15;	 
};
union	SCM_SYSRSTSTATUS_REG	{
	UINT32	all;
	struct	SCM_SYSRSTSTATUS_BITS	bit;
};

// High Frequency ROSC Control Register bit field
struct	SCM_HFROSCCTRL_BITS	{
	UINT32 	HFROscCRE:1;
	UINT32 	Rsvd1:7;  			
	UINT32	HFROscTrim:10;
	UINT32	HFROscGrade:2;
	UINT32	Rsvd2:12;
};
union	SCM_HFROSCCTRL_REG	{
	UINT32	all;
	struct	SCM_HFROSCCTRL_BITS	bit;
};

// High Frequency ROSC Trim Value Control Register bit field
struct	SCM_HFROSCTRIMVAL_BITS	{
	UINT32 	TrimHLimit:10;
	UINT32 	Rsvd1:6;  			
	UINT32	TrimLLimit:10;
	UINT32	Rsvd2:6;
};
union	SCM_HFROSCTRIMVAL_REG	{
	UINT32	all;
	struct	SCM_HFROSCTRIMVAL_BITS	bit;
};

// Analog Module Power Enable Control Register bit field
struct	SCM_AMODENCTRL_BITS	{
	UINT32	Hfrosc:1;
	UINT32	Mfrosc:1;
	UINT32	Pdr33:1;
	UINT32	Pdr18:1;
	UINT32	SdsiRam:1;
	UINT32	Rom:1;
	UINT32	TrngRing:1;
	UINT32	Avr:1;
	UINT32	Xtal:1;
	UINT32	Rsvd:23; 
};
union	SCM_AMODENCTRL_REG	{
	UINT32	all;
	struct	SCM_AMODENCTRL_BITS	bit;
};

// Clock Output Control Register bit field
struct	SCM_CLKOUTCTRL_BITS	{
	UINT32	ClkOutDiv:8;		
	UINT32	ClkOutEn:1;
	UINT32	ClkOutSel:3;
	UINT32	Rsvd1:20;
};
union	SCM_CLKOUTCTRL_REG	{
	UINT32	all;
	struct 	SCM_CLKOUTCTRL_BITS	bit;
};

// System Self-Destruct Control Register bit field	 
struct	SCM_SYSSDCTRL_BITS	{
	UINT32	Rsvd1:1;
	UINT32	Vdd33Vd:1;
	UINT32 	Rsvd2:6;
	UINT32	Vdd18Vd:1;	
	UINT32	Rsvd3:7;
	UINT32	Vdd11Vd:1;
	UINT32	Rsvd4:9;
	UINT32	SramErr:1;
	UINT32	RomErr:1;	
	UINT32	Rsvd7:4; 
};
union	SCM_SYSSDCTRL_REG	{
	UINT32	all;
	struct 	SCM_SYSSDCTRL_BITS	bit;
};

// USB PHY	 
struct	SCM_USBPHYCTRL_BITS	{
	UINT32	Pupdpen:1;		// DP Pull-up enable
	UINT32	Pupdnen:1;		// DN Pull-up enable
	UINT32	Pdndpen:1;		// DP Pull-down enable
	UINT32	Pdndnen:1;		// DN Pull-down enable
	UINT32	Pdrvp:1;		
	UINT32	Pdrvm:1;
	UINT32	Pdrcv:1;
	UINT32	UsbUmode:1;
	UINT32	UsbSuspend:1;
	UINT32 	Rsvd1:23;
};
union	SCM_USBPHYCTRL_REG	{
	UINT32	all;
	struct 	SCM_USBPHYCTRL_BITS	bit;
};

// UART Clock Source Selection Register	bit field
struct 	SCM_UARTCLKSEL_BITS {
	UINT32	Uart0ClkSel:3;		//2:0
	UINT32	Rsvd0:5;			//7:3
	UINT32	Uart1ClkSel:3;		//10:8
	UINT32	Rsvd1:5;			//15:11
	UINT32	Uart2ClkSel:3;		//18:16
	UINT32	Rsvd2:5;			//23:19	
	UINT32	Rsvd3:8;			//31:24
};
union	SCM_UARTCLKSEL_REG	{
   	UINT32	all;
	struct 	SCM_UARTCLKSEL_BITS	bit;
};

// I2C Clock Source Selection Register	bit field
struct 	SCM_I2CCLKSEL_BITS {
	UINT32	I2c0ClkSel:2;		//1:0
	UINT32	Rsvd0:30;			//31:2
};
union	SCM_I2CCLKSEL_REG	{
   	UINT32	all;
	struct 	SCM_I2CCLKSEL_BITS	bit;
};

// I2C Clock Source Selection Register	bit field
struct 	SCM_ANALOGTRIM_BITS {
	UINT32	Pdr33ThSel:3;		//2:0
	UINT32	Rsvd0:5;			//7:3
	UINT32	Pdr18ThSel:1;		//8
	UINT32	Rsvd1:7;			//15:9
	UINT32	AvrHd18ThSel:2;		//17:16
	UINT32	AvrLd18ThSel:2;		//19:18
	UINT32	AvrHd11ThSel:2;		//21:20
	UINT32	AvrLd11ThSel:2;		//23:22
	UINT32	AvrHdHvThSel:2;		//25:24
	UINT32	Rsvd2:6;			//31:26
};
union	SCM_ANALOGTRIM_REG	{
   	UINT32	all;
	struct 	SCM_ANALOGTRIM_BITS	bit;
};


// ROSC Trimming Control Register
struct	SCM_TRIM_CTRL_BITS	{
	UINT32	TrimStart:1;		//0
	UINT32	TrimRefClkSel:3;	//3:1
	UINT32	Rsvd0:4;			//7:4
	UINT32	TrimStable:1;		//8
	UINT32	Rsvd:23;			//31:9
};
union	SCM_TRIM_CTRL_REG	{
   	UINT32	all;
	struct 	SCM_TRIM_CTRL_BITS	bit;
};

// SRAM Control Register 0 bit field
struct	SCM_SRAMCTRL0_BITS	{
	UINT32	Ret0:1;				//0
	UINT32	Psd0:1;				//1
	UINT32	Nap0:1;				//2
	UINT32	Mse0:1;				//3
	UINT32	Ms0:4;				//7:4
	UINT32	Ret1:1;				//8
	UINT32	Psd1:1;				//9
	UINT32	Nap1:1;				//10
	UINT32	Mse1:1;				//11
	UINT32	Ms1:4;				//15:12
	UINT32	Ret2:1;				//16
	UINT32	Psd2:1;				//17
	UINT32	Nap2:1;				//18
	UINT32	Mse2:1;				//19
	UINT32	Ms2:4;				//23:20
	UINT32	Ret3:1;				//24
	UINT32	Psd3:1;				//25
	UINT32	Nap3:1;				//26
	UINT32	Mse3:1;				//27
	UINT32	Ms3:4;				//31:28
};
union	SCM_SRAMCTRL0_REG	{
   	UINT32	all;
	struct 	SCM_SRAMCTRL0_BITS	bit;
};

// SRAM Control Register 1 bit field
struct	SCM_SRAMCTRL1_BITS	{
	UINT32	Ret4:1;				//0
	UINT32	Psd4:1;				//1
	UINT32	Nap4:1;				//2
	UINT32	Mse4:1;				//3
	UINT32	Ms4:4;				//7:4
	UINT32	Ret5:1;				//8
	UINT32	Psd5:1;				//9
	UINT32	Nap5:1;				//10
	UINT32	Mse5:1;				//11
	UINT32	Ms5:4;				//15:12
	UINT32	RetUsb:1;			//16
	UINT32	PsdUsb:1;			//17
	UINT32	NapUsb:1;			//18
	UINT32	MseUsb:1;			//19
	UINT32	MsUsb:4;			//23:20
	UINT32	RetCache:1;			//24
	UINT32	PsdCache:1;			//25
	UINT32	NapCache:1;			//26
	UINT32	MseCache:1;			//27
	UINT32	MsCache:4;			//31:28
};
union	SCM_SRAMCTRL1_REG	{
   	UINT32	all;
	struct 	SCM_SRAMCTRL1_BITS	bit;
};

// ROM Control Register bit field
struct	SCM_ROMCTRL_BITS	{
	UINT32	Am:1;				//0
	UINT32	Cebm:1;				//1
	UINT32	PowerDown:1;		//2
	UINT32	Rsvd:29;			//3
};
union	SCM_ROMCTRL_REG	{
   	UINT32	all;
	struct 	SCM_ROMCTRL_BITS	bit;
};

// ROM Control Register bit field
struct	SCM_DSMCTRL_BITS	{
	UINT32	Rsvd0:16;			//15:0
	UINT32	AutoFclkSwitch:1;	//16
	UINT32	AutoHFRoscPowSw:1;	//17
	UINT32	AutoMFRoscPowSw:1;	//18
	UINT32	AutoXtalPowSw:1;	//19
	UINT32	EfSlm:1;			//20
	UINT32	EfPdm:1;			//21
	UINT32	Rsvd1:9;			//30:22
	UINT32	Rsvd2:1;			//31
};
union	SCM_DSMCTRL_REG	{
   	UINT32	all;
	struct 	SCM_DSMCTRL_BITS	bit;
};



// System Control Moudle Registers
struct	SCM_REGS{
	UINT32	SOCID;									//0x00
	UINT32	MODCLKCTRL0;							//0x04
	UINT32	MODRSTCTRL0;	 						//0x08
	union	SCM_SYSCLKCTRL_REG		SYSCLKCTRL;		//0x0C
	union	SCM_SYSRSTCTRL_REG		SYSRSTCTRL;		//0x10
	union	SCM_HFROSCCTRL_REG		HFROSCCTRL;		//0x14
	union	SCM_HFROSCTRIMVAL_REG	HFROSCTRIMVAL;	//0x18
	union	SCM_AMODENCTRL_REG		AMODENCTRL;		//0x1C
	UINT32	ROM_EXE_INHIBIT;						//0x20
	UINT32	MEMREMAP;								//0x24
	union	SCM_CLKOUTCTRL_REG 		CLKOUTCTRL;		//0x28
	union	SCM_SYSRSTSTATUS_REG	SYSRSTSTATUS;	//0x2C
	UINT32	CACHEBASE0;								//0x30
	UINT32	CACHETOP0;								//0x34
	UINT32	CACHEBASE1;								//0x38
	UINT32	CACHETOP1;								//0x3C
	union	SCM_SYSSDCTRL_REG		SYSSDCTRL;		//0x40
	UINT32	Rsvd2[1];								//0x44
	UINT32	REGWP0;									//0x48
	union	SCM_USBPHYCTRL_REG  	USBPHYCTRL;		//0x4C
	UINT32 	SRAM_ADDRKEY;							//0x50
	UINT32 	SRAM_DATAKEY;							//0x54
	UINT32 	SRAM_EXE_INHIBIT;						//0x58
	UINT32 	REGWP1;									//0x5C						
	UINT32 	MODCLKCTRL1;							//0x60
	UINT32 	MODRSTCTRL1;	   						//0x64
	union 	SCM_UARTCLKSEL_REG		UARTCLKSEL;		//0x68
	union 	SCM_I2CCLKSEL_REG		I2CCLKSEL;		//0x6C
	UINT32 	KEY_ACCESS_INHIBIT;						//0x70
	UINT32 	Rsvd5[2];								//0x74 - 0x7B
	union 	SCM_ANALOGTRIM_REG		ANALOGTRIM;		//0x7C
	union	SCM_TRIM_CTRL_REG		TRIM_CTRL;		//0x80
	UINT32	TRIM_STD_MIN;							//0x84
	UINT32	TRIM_STD_MAX;							//0x88	
	UINT32	TRIM_THRESHOLD_MIN;						//0x8C
	UINT32	TRIM_THRESHOLD_MAX;						//0x90
	union	SCM_SRAMCTRL0_REG		SRAMCTRL0;		//0x94
	union	SCM_SRAMCTRL1_REG		SRAMCTRL1;		//0x98
	union	SCM_DSMCTRL_REG			DSMCTRL;		//0x9C
	UINT32	Rsvd6[12];								//0xA0 - 0xCF
	UINT32 	GPREGx[12];								//0xD0 - 0xFF
};

/*
**************************** GPIO ******************************************
*/
// GPIOA DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPADATA_BITS	{
	UINT32	GPA0:1;
	UINT32	GPA1:1;
	UINT32	GPA2:1;
	UINT32	GPA3:1;
	UINT32	GPA4:1;
	UINT32	GPA5:1;
	UINT32	GPA6:1;
	UINT32	GPA7:1;
	UINT32	GPA8:1;
	UINT32	GPA9:1;
	UINT32	GPA10:1;
	UINT32	GPA11:1;
	UINT32	GPA12:1;
	UINT32	GPA13:1;
	UINT32	GPA14:1;
	UINT32	GPA15:1;
	UINT32	GPA16:1;
	UINT32	GPA17:1;
	UINT32	GPA18:1;
	UINT32	GPA19:1;
	UINT32	GPA20:1;
	UINT32	GPA21:1;
	UINT32	GPA22:1;
	UINT32	GPA23:1;
	UINT32	GPA24:1;
	UINT32	GPA25:1;
	UINT32	GPA26:1;
	UINT32	GPA27:1;
	UINT32	GPA28:1;
	UINT32	GPA29:1;
	UINT32	GPA30:1;
	UINT32	GPA31:1;
};
union	GPIO_GPADATA_REG	{
	UINT32	all;
	struct	GPIO_GPADATA_BITS	bit;
};

// GPIOA USE0 Bits Definition
struct	GPIO_GPAUSE0_BITS	{	// bits	description
	UINT32	GPA0:4;				//	3:0		Use of GPIOA0
	UINT32	GPA1:4;				//	7:4		Use of GPIOA1
	UINT32	GPA2:4;				//	11:8	Use of GPIOA2
	UINT32	GPA3:4;				//	15:12	Use of GPIOA3
	UINT32	GPA4:4;				//	19:16	Use of GPIOA4
	UINT32	GPA5:4;				//	23:20	Use of GPIOA5
	UINT32	GPA6:4;				//	27:24	Use of GPIOA6
	UINT32	GPA7:4;				//	31:28	Use of GPIOA7
};
union 	GPIO_GPAUSE0_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE0_BITS	bit;
};

//GPIOA USE1 Bits Definition
struct	GPIO_GPAUSE1_BITS	{	// bits	description
	UINT32	GPA8:4;				//	3:0		Use of GPIOA8
	UINT32	GPA9:4;				//	7:4		Use of GPIOA9
	UINT32	GPA10:4;			//	11:8	Use of GPIOA10
	UINT32	GPA11:4;			//	15:12	Use of GPIOA11
	UINT32	GPA12:4;			//	19:16	Use of GPIOA12
	UINT32	GPA13:4;			//	23:20	Use of GPIOA13
	UINT32	GPA14:4;			//	27:24	Use of GPIOA14
	UINT32	GPA15:4;			//	31:28	Use of GPIOA15
};
union 	GPIO_GPAUSE1_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE1_BITS	bit;
};

//GPIOA USE2 Bits Definition
struct	GPIO_GPAUSE2_BITS	{	// bits	description
	UINT32	GPA16:4;			//	3:0		Use of GPIOA16
	UINT32	GPA17:4;			//	7:4		Use of GPIOA17
	UINT32	GPA18:4;			//	11:8	Use of GPIOA18
	UINT32	GPA19:4;			//	15:12	Use of GPIOA19
	UINT32	GPA20:4;			//	19:16	Use of GPIOA20
	UINT32	GPA21:4;			//	23:20	Use of GPIOA21
	UINT32	GPA22:4;			//	27:24	Use of GPIOA22
	UINT32	GPA23:4;			//	31:28	Use of GPIOA23
};
union 	GPIO_GPAUSE2_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE2_BITS	bit;
};

//GPIOA USE3 Bits Definition
struct	GPIO_GPAUSE3_BITS	{	// bits	description
	UINT32	GPA24:4;			//	3:0		Use of GPIOA24
	UINT32	GPA25:4;			//	7:4		Use of GPIOA25
	UINT32	GPA26:4;			//	11:8	Use of GPIOA26
	UINT32	GPA27:4;			//	15:12	Use of GPIOA27
	UINT32	GPA28:4;			//	19:16	Use of GPIOA28
	UINT32	GPA29:4;			//	23:20	Use of GPIOA29
	UINT32	GPA30:4;			//	27:24	Use of GPIOA30
	UINT32	GPA31:4;			//	31:28	Use of GPIOA31
};
union 	GPIO_GPAUSE3_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE3_BITS	bit;
};

//-------------- GPIOB --------------------------
//GPIOB DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPBDATA_BITS	{
	UINT32	GPB0:1;
	UINT32	GPB1:1;
	UINT32	GPB2:1;
	UINT32	GPB3:1;
	UINT32	GPB4:1;
	UINT32	GPB5:1;
	UINT32	GPB6:1;
	UINT32	GPB7:1;
	UINT32	GPB8:1;
	UINT32	GPB9:1;
	UINT32	GPB10:1;
	UINT32	GPB11:1;
	UINT32	GPB12:1;
	UINT32	GPB13:1;
	UINT32	GPB14:1;
	UINT32	GPB15:1;
	UINT32	GPB16:1;
	UINT32	GPB17:1;
	UINT32	GPB18:1;
	UINT32	GPB19:1;
	UINT32	GPB20:1;
	UINT32	GPB21:1;
	UINT32	GPB22:1;
	UINT32	GPB23:1;
	UINT32	GPB24:1;
	UINT32	GPB25:1;
	UINT32	GPB26:1;
	UINT32	GPB27:1;
	UINT32	GPB28:1;
	UINT32	GPB29:1;
	UINT32	GPB30:1;
	UINT32	GPB31:1;
};
union	GPIO_GPBDATA_REG	{
	UINT32	all;
	struct	GPIO_GPBDATA_BITS	bit;
};

//GPIOB USE0 Bits Definition
struct	GPIO_GPBUSE0_BITS	{	// bits	description
	UINT32	GPB0:4;				//	3:0		Use of GPIOB0
	UINT32	GPB1:4;				//	7:4		Use of GPIOB1
	UINT32	GPB2:4;				//	11:8	Use of GPIOB2
	UINT32	GPB3:4;				//	15:12	Use of GPIOB3
	UINT32	GPB4:4;				//	19:16	Use of GPIOB4
	UINT32	GPB5:4;				//	23:20	Use of GPIOB5
	UINT32	GPB6:4;				//	27:24	Use of GPIOB6
	UINT32	GPB7:4;				//	31:28	Use of GPIOB7
};
union 	GPIO_GPBUSE0_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE0_BITS	bit;
};

//GPIOB USE1 Bits Definition
struct	GPIO_GPBUSE1_BITS	{	// bits	description
	UINT32	GPB8:4;				//	3:0		Use of GPIOB8
	UINT32	GPB9:4;				//	7:4		Use of GPIOB9
	UINT32	GPB10:4;			//	11:8	Use of GPIOB10
	UINT32	GPB11:4;			//	15:12	Use of GPIOB11
	UINT32	GPB12:4;			//	19:16	Use of GPIOB12
	UINT32	GPB13:4;			//	23:20	Use of GPIOB13
	UINT32	GPB14:4;			//	27:24	Use of GPIOB14
	UINT32	GPB15:4;			//	31:28	Use of GPIOB15
};
union 	GPIO_GPBUSE1_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE1_BITS	bit;
};

//GPIOB USE2 Bits Definition
struct	GPIO_GPBUSE2_BITS	{	// bits	description
	UINT32	GPB16:4;			//	3:0		Use of GPIOB16
	UINT32	GPB17:4;			//	7:4		Use of GPIOB17
	UINT32	GPB18:4;			//	11:8	Use of GPIOB18
	UINT32	GPB19:4;			//	15:12	Use of GPIOB19
	UINT32	GPB20:4;			//	19:16	Use of GPIOB20
	UINT32	GPB21:4;			//	23:20	Use of GPIOB21
	UINT32	GPB22:4;			//	27:24	Use of GPIOB22
	UINT32	GPB23:4;			//	31:28	Use of GPIOB23
};
union 	GPIO_GPBUSE2_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE2_BITS	bit;
};

//GPIOB USE3 Bits Definition
struct	GPIO_GPBUSE3_BITS	{	// bits	description
	UINT32	GPB24:4;			//	3:0		Use of GPIOB24
	UINT32	GPB25:4;			//	7:4		Use of GPIOB25
	UINT32	GPB26:4;			//	11:8	Use of GPIOB26
	UINT32	GPB27:4;			//	15:12	Use of GPIOB27
	UINT32	GPB28:4;			//	19:16	Use of GPIOB28
	UINT32	GPB29:4;			//	23:20	Use of GPIOB29
	UINT32	GPB30:4;			//	27:24	Use of GPIOB30
	UINT32	GPB31:4;			//	31:28	Use of GPIOB31
};
union 	GPIO_GPBUSE3_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE3_BITS	bit;
};

// GPIOA DRIVE0 Bits Definition
struct	GPIO_GPADRIVE0_BITS	{	// bits	description
	UINT32	GPA0:2;				//	1:0		Use of GPIOA0
	UINT32	GPA1:2;				//	3:2		Use of GPIOA1
	UINT32	GPA2:2;				//	5:4		Use of GPIOA2
	UINT32	GPA3:2;				//	7:6		Use of GPIOA3
	UINT32	GPA4:2;				//	9:8		Use of GPIOA4
	UINT32	GPA5:2;				//	11:10	Use of GPIOA5
	UINT32	GPA6:2;				//	13:12	Use of GPIOA6
	UINT32	GPA7:2;				//	15:14	Use of GPIOA7
	UINT32	GPA8:2;				//	17:16	Use of GPIOA8
	UINT32	GPA9:2;				//	19:18	Use of GPIOA9
	UINT32	GPA10:2;			//	21:20	Use of GPIOA10
	UINT32	GPA11:2;			//	23:22	Use of GPIOA11
	UINT32	GPA12:2;			//	25:24	Use of GPIOA12
	UINT32	GPA13:2;			//	27:26	Use of GPIOA13
	UINT32	GPA14:2;			//	29:28	Use of GPIOA14
	UINT32	GPA15:2;			//	31:30	Use of GPIOA15
};
union 	GPIO_GPADRIVE0_REG	{
	UINT32	all;
	struct	GPIO_GPADRIVE0_BITS	bit;
};

// GPIOA DRIVE1 Bits Definition
struct	GPIO_GPADRIVE1_BITS	{	// bits	description
	UINT32	GPA16:2;			//	1:0		Use of GPIOA16
	UINT32	GPA17:2;			//	3:2		Use of GPIOA17
	UINT32	GPA18:2;			//	5:4		Use of GPIOA18
	UINT32	GPA19:2;			//	7:6		Use of GPIOA19
	UINT32	GPA20:2;			//	9:8		Use of GPIOA20
	UINT32	GPA21:2;			//	11:10	Use of GPIOA21
	UINT32	GPA22:2;			//	13:12	Use of GPIOA22
	UINT32	GPA23:2;			//	15:14	Use of GPIOA23
	UINT32	GPA24:2;			//	17:16	Use of GPIOA24
	UINT32	GPA25:2;			//	19:18	Use of GPIOA25
	UINT32	GPA26:2;			//	21:20	Use of GPIOA26
	UINT32	GPA27:2;			//	23:22	Use of GPIOA27
	UINT32	GPA28:2;			//	25:24	Use of GPIOA28
	UINT32	GPA29:2;			//	27:26	Use of GPIOA29
	UINT32	GPA30:2;			//	29:28	Use of GPIOA30
	UINT32	GPA31:2;			//	31:30	Use of GPIOA31
};
union 	GPIO_GPADRIVE1_REG	{
	UINT32	all;
	struct	GPIO_GPADRIVE1_BITS	bit;
};

// GPIOB DRIVE0 Bits Definition
struct	GPIO_GPBDRIVE0_BITS	{	// bits	description
	UINT32	GPB0:2;				//	1:0		Use of GPIOB0
	UINT32	GPB1:2;				//	3:2		Use of GPIOB1
	UINT32	GPB2:2;				//	5:4		Use of GPIOB2
	UINT32	GPB3:2;				//	7:6		Use of GPIOB3
	UINT32	GPB4:2;				//	9:8		Use of GPIOB4
	UINT32	GPB5:2;				//	11:10	Use of GPIOB5
	UINT32	GPB6:2;				//	13:12	Use of GPIOB6
	UINT32	GPB7:2;				//	15:14	Use of GPIOB7
	UINT32	GPB8:2;				//	17:16	Use of GPIOB8
	UINT32	GPB9:2;				//	19:18	Use of GPIOB9
	UINT32	GPB10:2;			//	21:20	Use of GPIOB10
	UINT32	GPB11:2;			//	23:22	Use of GPIOB11
	UINT32	GPB12:2;			//	25:24	Use of GPIOB12
	UINT32	GPB13:2;			//	27:26	Use of GPIOB13
	UINT32	GPB14:2;			//	29:28	Use of GPIOB14
	UINT32	GPB15:2;			//	31:30	Use of GPIOB15
};
union 	GPIO_GPBDRIVE0_REG	{
	UINT32	all;
	struct	GPIO_GPBDRIVE0_BITS	bit;
};

// GPIOB DRIVE1 Bits Definition
struct	GPIO_GPBDRIVE1_BITS	{	// bits	description
	UINT32	GPB16:2;			//	1:0		Use of GPIOB16
	UINT32	GPB17:2;			//	3:2		Use of GPIOB17
	UINT32	GPB18:2;			//	5:4		Use of GPIOB18
	UINT32	GPB19:2;			//	7:6		Use of GPIOB19
	UINT32	GPB20:2;			//	9:8		Use of GPIOB20
	UINT32	GPB21:2;			//	11:10	Use of GPIOB21
	UINT32	GPB22:2;			//	13:12	Use of GPIOB22
	UINT32	GPB23:2;			//	15:14	Use of GPIOB23
	UINT32	GPB24:2;			//	17:16	Use of GPIOB24
	UINT32	GPB25:2;			//	19:18	Use of GPIOB25
	UINT32	GPB26:2;			//	21:20	Use of GPIOB26
	UINT32	GPB27:2;			//	23:22	Use of GPIOB27
	UINT32	GPB28:2;			//	25:24	Use of GPIOB28
	UINT32	GPB29:2;			//	27:26	Use of GPIOB29
	UINT32	GPB30:2;			//	29:28	Use of GPIOB30
	UINT32	GPB31:2;			//	31:30	Use of GPIOB31
};
union 	GPIO_GPBDRIVE1_REG	{
	UINT32	all;
	struct	GPIO_GPBDRIVE1_BITS	bit;
};

// GPIO Registers
struct	GPIO_REGS	{
	//-------- GPIOA --------------
	union 	GPIO_GPAUSE0_REG	GPAUSE0;	// GPIOA use0 register
	union 	GPIO_GPAUSE1_REG	GPAUSE1;	// GPIOA use1 register
	union 	GPIO_GPAUSE2_REG	GPAUSE2;	// GPIOA use2 register
	union 	GPIO_GPAUSE3_REG	GPAUSE3;	// GPIOA use3 register
	union 	GPIO_GPADATA_REG	GPADATA;	// GPIOA data register
	union 	GPIO_GPADATA_REG	GPASET;		// GPIOA data set register
	union 	GPIO_GPADATA_REG 	GPACLR;		// GPIOA data clear register
	union 	GPIO_GPADATA_REG	GPATOG;		// GPIOA data toggle register
	union 	GPIO_GPADATA_REG	GPAINT;		// GPIOA interrupt flag and clear register
	union 	GPIO_GPADATA_REG	GPAPS;		// GPIOA Pull select
	//union 	GPIO_GPADRIVE0_REG	GPADRIVE0;	//GPIOA Dirve control	register
	//union	GPIO_GPADRIVE1_REG	GPADRIVE1; 
	//union 	GPIO_GPADATA_REG	GPAOD;		//
	
	UINT32	Rsvd1[6];
	//--------- GPIOB -------------
	union 	GPIO_GPBUSE0_REG	GPBUSE0;	// GPIOB use0 register
	UINT32	Rsvd2[3];
	union 	GPIO_GPBDATA_REG	GPBDATA;	// GPIOB data register
	union 	GPIO_GPBDATA_REG	GPBSET;		// GPIOB data set register
	union 	GPIO_GPBDATA_REG 	GPBCLR;		// GPIOB data clear register
	union 	GPIO_GPBDATA_REG	GPBTOG;		// GPIOB data toggle register
	union 	GPIO_GPBDATA_REG	GPBINT;		// GPIOB interrupt flag and clear register
	union 	GPIO_GPBDATA_REG	GPBPS;		// GPIOB Pull select
	//union 	GPIO_GPBDRIVE0_REG 	GPBDRIVE0;	//GPIOB Dirve control register
	//UINT32	Rsvd3;
	//union 	GPIO_GPBDATA_REG	GPBOD;		//
	UINT32	Rsvd4[6];

	//--------- GPIOC -------------
	UINT32 	RsvdC[16];
	//---------- GPIOD -----------
	UINT32 	RsvdD[16];
	//---------- GPIOE -----------
	UINT32 	RsvdE[16];
};

/*
*************************** TMR ******************************************
*/
// Define bits of TIMER_CONTROL
#define	CONTROL_WKEY	(0x67<<24)
#define CONTROL_WUFLAG	(0x55<<16)

#define	CONTROL_CLKSEL	5
#define CONTROL_EN		4
#define CONTROL_IP		3
#define CONTROL_WDT		2
#define CONTROL_IE		0
// Timer Control Register Bit Definition
struct	TIMER_CONTROL_BITS	{// bits	description
	UINT32	Ie:1;			
	UINT32	Rsvd0:1;			
	UINT32	Wdt:1;		
	UINT32	Ip:1;		
	UINT32	En:1;				
	UINT32	ClkSel:4;		
	UINT32	Rsvd1:7;
	UINT32  Wuflag:8;
	UINT32	Wkey:8;		
};
union 	TIMER_CONTROL_REG	{
	UINT32	all;
	struct	TIMER_CONTROL_BITS	bit;
};

struct	TIMER_CAPCTRL_BITS	{// bits	description
	UINT32	CapIntEn:1;			
	UINT32	CapInt:1;			
	UINT32	CapEn:1;		
	UINT32	CapFifoLevel:4;		
	UINT32	CapFifoOverflow:1;		
	UINT32	CapRiseEdge:1;		
	UINT32	CapFallEdge:1;		
	UINT32	Rsvd2:22;	
};
union 	TIMER_CAPCTRL_REG	{
	UINT32	all;
	struct	TIMER_CAPCTRL_BITS	bit;
};

// Timer Registers
struct TMR_REGS	{
	UINT32	COUNT;
	union	TIMER_CONTROL_REG CONTROL;
	UINT32	LIMIT;
	union	TIMER_CAPCTRL_REG CAPCTRL;
	UINT32	CAPVAL;
};

/*
*************************** UART ******************************************
*/
// UART Interrupt Enable Register Bit Definition
#define	UART_IER_ETBFI		0	// Enable Received Data Available Interrupt
#define	UART_IER_ETBEI		1	// Enable Transmit Holding Register Empty Interrupt
#define	UART_IER_ELSI		2	// Enable Receiver Line Status Interrupt
#define	UART_IER_EDSSI		3	// Enable Modem Status Interrupt
#define	UART_IER_PTIME		7	// Programmable THRE Interrupt Mode Enable
struct	UART_IER_BITS	{// bits	description
	UINT32	ETBFI:1;	//  0	Enable Received Data Available Interrupt					
	UINT32	ETBEI:1;	//  1	Enable Transmit Holding Register Empty Interrupt			
	UINT32	ELSI:1;		//  2	Enable Receiver Line Status Interrupt	
	UINT32	EDSSI:1;	//  3	Enable Modem Status Interrupt				
	UINT32	Rsvd1:3;	//	6:4	Reserved		
	UINT32	PTIME:1;	//	7	Programmable THRE Interrupt Mode Enable		
	UINT32	Rsvd2:24;	
};
union 	UART_IER_REG	{
	UINT32	all;
	struct	UART_IER_BITS	bit;
};

// UART Interrupt Indentity Register Bit Definition
#define	UART_IIR_IID		0	// Interrupt ID
#define	UART_IIR_FIFOSE		6	// FIFOs Enable
struct	UART_IIR_BITS	{// bits	description
	UINT32	IID:4;		//  3:0	Interrupt ID								
	UINT32	Rsvd1:2;	//	5:4	Reserved		
	UINT32	FIFOSE:2;	//	7:6	FIFOs Enable		
	UINT32	Rsvd2:24;	
};
union 	UART_IIR_REG	{
	UINT32	all;
	struct	UART_IIR_BITS	bit;
};

// UART FIFO Control Register Bit Definition
#define	UART_FCR_FIFOE		0	// FIFO Enable
#define	UART_FCR_RFIFOR		1	// RCVR FIFO Reset
#define	UART_FCR_XFIFOR		2	// XMIT FIFO Reset
#define	UART_FCR_DMAM		3	// DMA Mode
#define	UART_FCR_TET		4	// TX Empty Trigger
#define	UART_FCR_RT			6	// RCVR Trigger
struct	UART_FCR_BITS	{// bits	description
	UINT32	FIFOE:1;	//  0	FIFO Enable								
	UINT32	RFIFOR:1;	//	1	RCVR FIFO Reset		
	UINT32	XFIFOR:1;	//	2	XMIT FIFO Reset
	UINT32	DMAM:1;		//	3	DMA Mode
	UINT32	TET:2;		// 	5:4	TX Empty Trigger
	UINT32	RT:2;		//	7:6	RCVR Trigger		
	UINT32	Rsvd1:24;	
};
union 	UART_FCR_REG	{
	UINT32	all;
	struct	UART_FCR_BITS	bit;
};

// UART Line Control Register Bit Definition
#define	UART_LCR_DLS		0	// Data Length Select
#define	UART_LCR_STOP		2	// Number of stop bits
#define	UART_LCR_PEN		3	// Parity Enable
#define	UART_LCR_EPS		4	// Even Parity Selset
#define	UART_LCR_SP			5	// Stick Parity
#define	UART_LCR_BC			6	// Break Control Bit
#define	UART_LCR_DLAB		7	// Divisor Latch Access Bit
struct	UART_LCR_BITS	{// bits	description
	UINT32	DLS:2;		//  1:0	Data Length Select								
	UINT32	STOP:1;		//	2	Number of stop bits		
	UINT32	PEN:1;		//	3	Parity Enable
	UINT32	EPS:1;		//	4	Even Parity Selset
	UINT32	SP:1;		// 	5	Stick Parity
	UINT32	BC:1;		//	6	Break Control Bit
	UINT32	DLAB:1;		//	7	Divisor Latch Access Bit		
	UINT32	Rsvd1:24;	
};
union 	UART_LCR_REG	{
	UINT32	all;
	struct	UART_LCR_BITS	bit;
};

// UART Modem Control Register Bit Definition
#define	UART_MCR_DTR		0	// Data Terminal Ready
#define	UART_MCR_RTS		1	// Request to Send
#define	UART_MCR_OUT1		2	// OUT1
#define	UART_MCR_OUT2		3	// OUT2
#define	UART_MCR_LB			4	// LoopBack Bit
#define	UART_MCR_AFCE		5	// Auto Flow Control Enable
#define	UART_MCR_SIRE		6	// SIR Mode Enable
struct	UART_MCR_BITS	{// bits	description
	UINT32	DTR:1;		//  0	Data Terminal Ready								
	UINT32	RTS:1;		//	1	Request to Send	
	UINT32	OUT1:1;		//	2	OUT1
	UINT32	OUT2:1;		//	3	OUT2
	UINT32	LB:1;		// 	4	LoopBack Bit
	UINT32	AFCE:1;		//	5	Auto Flow Control Enable
	UINT32	SIRE:1;		//	6	SIR Mode Enable		
	UINT32	Rsvd1:25;	
};
union 	UART_MCR_REG	{
	UINT32	all;
	struct	UART_MCR_BITS	bit;
};

// UART Line Status Register Bit Definition
#define	UART_LSR_DR			0	// Data Ready bit
#define	UART_LSR_OE			1	// Overrun error bit
#define	UART_LSR_PE			2	// Parity Error bit
#define	UART_LSR_FE			3	// Framing Error bit
#define	UART_LSR_BI			4	// Break Interrupt bit
#define	UART_LSR_THRE		5	// Transmit Holding Register Empty bit
#define	UART_LSR_TEMT		6	// Transmitter Empty bit
#define	UART_LSR_RFE		7	// Receiver FIFO Error bit
struct	UART_LSR_BITS	{// bits	description
	UINT32	DR:1;		//  0	Data Ready bit								
	UINT32	OE:1;		//	1	Overrun error bit	
	UINT32	PE:1;		//	2	Parity Error bit
	UINT32	FE:1;		//	3	Framing Error bit
	UINT32	BI:1;		// 	4	Break Interrupt bit
	UINT32	THRE:1;		//	5	Transmit Holding Register Empty bit
	UINT32	TEMT:1;		//	6	Transmitter Empty bit	
	UINT32	RFE:1;		// 	7	Receiver FIFO Error bit	
	UINT32	Rsvd1:24;	
};
union 	UART_LSR_REG	{
	UINT32	all;
	struct	UART_LSR_BITS	bit;
};

// UART Modem Status Register Bit Definition
#define	UART_MSR_DCTS		0	// Delta Clear to Send
#define	UART_MSR_DDSR		1	// Delta Data Set Ready
#define	UART_MSR_TERI		2	// Trailing Edge of Ring Indicator
#define	UART_MSR_DDCD		3	// Delta Data Carrier Detect
#define	UART_MSR_CTS		4	// Clear to Send
#define	UART_MSR_DSR		5	// Data Set Ready
#define	UART_MSR_RI			6	// Ring Indicator
#define	UART_MSR_DCD		7	// Data Carrier Detect
struct	UART_MSR_BITS	{// bits	description
	UINT32	DCTS:1;		//  0	Delta Clear to Send								
	UINT32	DDSR:1;		//	1	Delta Data Set Ready	
	UINT32	TERI:1;		//	2	Trailing Edge of Ring Indicator
	UINT32	DDCD:1;		//	3	Delta Data Carrier Detect
	UINT32	CTS:1;		// 	4	Clear to Send
	UINT32	DSR:1;		//	5	Data Set Ready
	UINT32	RI:1;		//	6	Ring Indicator	
	UINT32	DCD:1;		// 	7	Data Carrier Detect
	UINT32	Rsvd1:24;	
};
union 	UART_MSR_REG	{
	UINT32	all;
	struct	UART_MSR_BITS	bit;
};

// UART Status Register Bit Definition
#define	UART_USR_BUSY		0	// UART busy
#define	UART_USR_TFNF		1	// Transmit FIFO Not Full
#define	UART_USR_TFE		2	// Transmit FIFO Empty
#define	UART_USR_RFNE		3	// Receive FIFO Not Empty
#define	UART_USR_RFF		4	// eceive FIFO Full
struct	UART_USR_BITS	{// bits	description
	UINT32	BUSY:1;		//  0	UART busy								
	UINT32	TFNF:1;		//	1	Transmit FIFO Not Full	
	UINT32	TFE:1;		//	2	Transmit FIFO Empty
	UINT32	RFNE:1;		//	3	Receive FIFO Not Empty
	UINT32	RFF:1;		// 	4	Receive FIFO Full
	UINT32	Rsvd1:27;	
};
union 	UART_USR_REG	{
	UINT32	all;
	struct	UART_USR_BITS	bit;
};

// UART Software Reset Register Bit Definition
#define	UART_SRR_UR			0	// UART Reset
#define	UART_SRR_RFR		1	// RCVR FIFO Reset
#define	UART_SRR_XFR		2	// XMIT FIFO Reset
struct	UART_SRR_BITS	{// bits	description
	UINT32	UR:1;		//  0	UART Reset								
	UINT32	RFR:1;		//	1	RCVR FIFO Reset	
	UINT32	XFR:1;		//	2	XMIT FIFO Reset
	UINT32	Rsvd1:29;	
};
union 	UART_SRR_REG	{
	UINT32	all;
	struct	UART_SRR_BITS	bit;
};

// UART Registers
struct	UART_REGS		{
	UINT32	RBR_THR_DLL;		// 0x00 UART Receive Buffer Register/Transmit Holding Register
	UINT32	IER_DLH;			// 0x04 UART Interrupt Enable Register/Divisor Latch High
	UINT32	IIR_FCR;			// 0x08 UART Divisor Latch Low
	union UART_LCR_REG	LCR;	// 0x0C	UART Line Control Register
	union UART_MCR_REG	MCR;	// 0x10	UART Modem Control Register
	union UART_LSR_REG	LSR;	// 0x14	UART Line Status Register
	union UART_MSR_REG	MSR;	// 0x18	UART Modem Status Register
	UINT32	SCR;				// 0x1C	UART Scratchpad Register
	UINT32	LPDLL;				// 0x20	UART Low Power Divisor Low Register (SIR only)
	UINT32	LPDLH;				// 0x24	UART Low Power Divisor High Register (SIR only)
	UINT32	RSVD1[2];			// 0x28 - 0x2C	Reserved
	UINT32	SRBR_STHR[16];		// 0x30 - 0x6C	UART Shadow Receive Buffer Register (as RBR)
								// 0x30 - 0x6C	UART Shadow Transmit Holding Register (as THR)
	UINT32	FAR;				// 0x70	UART FIFO Access Register (for testing)
	UINT32	TFR;				// 0x74 UART Transmit FIFO Read (for testing)
	UINT32	RFW;				// 0x78	UART Receive FIFO Write (for testing)
	union UART_USR_REG	USR;	// 0x7C	UART Status Register
	UINT32	TFL;				// 0x80	UART Transmit FIFO Level
	UINT32	RFL;				// 0x84	UART Receive FIFO Level
	union UART_SRR_REG	SRR;  	// 0x88	UART Software Reset Register (shadow)
	UINT32	SRTS;				// 0x8C	UART Shadow Request to Send (as MCR[1])
	UINT32	SBCR;				// 0x90	UART Shadow Break Control Register (as LCR[6])
   	UINT32	SDMAM;				// 0x94	UART Shadow DMA Mode (as FCR[3])
	UINT32	SFE;				// 0x98	UART Shadow FIFO Enable (as FCR[0])
	UINT32	SRT;				// 0x9C	UART Shadow RCVR Trigger (as FCR[7:6])
	UINT32	STET;				// 0xA0	UART Shadow TX Empty Trigger (as FCR[5:4])
	UINT32	HTX;				// 0xA4	UART Halt TX (for testing)
	UINT32	DMASA;				// 0xA8	UART DMA Software Acknowledge
	UINT32 	RSVD[19];			// 0xAC - 0xF0 Reserved
	UINT32	CPR;				// 0xF4	UART Component Parameter Register
	UINT32	UCV;				// 0xF8	UART Component Version
	UINT32	CTR;				// 0xFC	UART Component Type Register
};


/*
*************************** I2C ******************************************
*/
// I2C Registers
#define	MASTER_MODE		1     
#define SPEED			2 
#define BIT_ADDR_SLAVE	3 
#define BIT_ADDR_MASTER	4
#define RESTART_EN		5
#define SLAVE_DIS		6
#define STOP_DET_IfADDR	7
#define TX_EMPTY_CTRL	8
#define RX_FIFO_FULL_HLD_CTRL	9

// I2C CON Bits Definition
struct	I2C_CON_BITS{   //bits	 description
	UINT32 MasterMode	:1;     //0    
    UINT32 Speed		:2;     //2:1
    UINT32 BitAddrSlave	:1;     //3 slave only ,1:10-bit,0:7-bit
	UINT32 BitAddrMaster:1;		//4
	UINT32 RestartEn	:1;		//5
	UINT32 SlaveDis		:1;		//6
	UINT32 StopDetIfAddressed	:1;	//7 slave mode only
	UINT32 TxEmptyCtrl	:1;		//8
	UINT32 RxFifoFullHldCtrl	:1;	//9
	UINT32 StopDetIfMasterActive:1;
	UINT32 Rsvd			:21;	//31:11
};
union	I2C_CON_REG{
	UINT32	all;
	struct I2C_CON_BITS	bit;
};

struct	I2C_TAR_BITS{   //bits	 description
	UINT32 IcTar		:10;	//9:0    
	UINT32 GCorStart	:1;		//10
	UINT32 Special		:1;		//11
	UINT32 BitAddrMaster:1;		//12	
	UINT32 Rsvd			:19;	//31:13
};
union	I2C_TAR_REG{
	UINT32	all;
	struct I2C_TAR_BITS	bit;
};

struct	I2C_DATA_CMD_BITS{   //bits	 description
	UINT32 Dat			:8;		//7:0    
	UINT32 Cmd			:1;		//8
	UINT32 Stop			:1;		//9
	UINT32 Restart		:1;		//10	
	UINT32 Rsvd			:21;	//31:11	//19
};
union	I2C_DATA_CMD_REG{
	UINT32	all;
	struct I2C_DATA_CMD_BITS	bit;
};

struct	I2C_INTR_STAT_BITS{   //bits	 description
	UINT32 RxUnder		:1;		//0    
	UINT32 RxOver		:1;		//1
	UINT32 RxFull		:1;		//2
	UINT32 TxOver		:1;		//3
	UINT32 TxEmpty		:1;		//4
	UINT32 RdReq		:1;		//5
	UINT32 TxAbrt		:1;		//6
	UINT32 RxDone		:1;		//7
	UINT32 Activity		:1;		//8
	UINT32 StopDet		:1;		//9
	UINT32 StartDet		:1;		//10
	UINT32 GenCall		:1;		//11
	UINT32 RestartDet	:1;		//12
	UINT32 MstOnHold	:1;		//13	
	UINT32 Rsvd			:18;	//31:14
};
union	I2C_INTR_STAT_REG{
	UINT32	all;
	struct I2C_INTR_STAT_BITS	bit;
};

struct	I2C_ENABLE_BITS{   //bits	 description
	UINT32 Enable		:1;		//0    
	UINT32 Abort		:1;		//1
	UINT32 Rsvd			:30;	//31:2
};
union	I2C_ENABLE_REG{
	UINT32	all;
	struct I2C_ENABLE_BITS	bit;
};

struct	I2C_STATUS_BITS{   //bits	 description
	UINT32 Activity		:1;		//0    
	UINT32 Tfnf			:1;		//1
	UINT32 Tfe			:1;		//2
	UINT32 Rfne			:1;		//3
	UINT32 Rff			:1;		//4
	UINT32 MstActivity	:1;		//5
	UINT32 SlvActivity	:1;		//6
	UINT32 MstTxEmpty	:1;		//7
	UINT32 MstRxFull	:1;		//8
	UINT32 SlvTxEmpty	:1;		//9
	UINT32 SlvRxFull	:1;		//10
	UINT32 Rsvd			:21;	//31:11
};
union	I2C_STATUS_REG{
	UINT32	all;
	struct I2C_STATUS_BITS	bit;
};

struct	I2C_SDA_HOLD_BITS{   //bits	 description
	UINT32 SdaTxHold	:16;	//15:0    
	UINT32 SdaRxHold	:8;		//23:16
	UINT32 Rsvd			:8;		//31:24
};
union	I2C_SDA_HOLD_REG{
	UINT32	all;
	struct I2C_SDA_HOLD_BITS	bit;
};

struct	I2C_DMA_CR_BITS{   //bits	 description
	UINT32 RdMae	:1;		//0    
	UINT32 TdMae	:1;		//1
	UINT32 Rsvd		:30;		//31:2
};
union	I2C_DMA_CR_REG{
	UINT32	all;
	struct I2C_DMA_CR_BITS	bit;
};

struct	I2C_ENABLE_STATUS_BITS{   //bits	 description
	UINT32 IcEn				:1;		//0    
	UINT32 SlvDisWhileBusy	:1;		//1
	UINT32 SlvRxDataLost	:1;		//2
	UINT32 Rsvd				:29;	//31:3
};
union	I2C_ENABLE_STATUS_REG{
	UINT32	all;
	struct I2C_ENABLE_STATUS_BITS	bit;
};

// I2C registers
struct	I2C_REGS{
	union	I2C_CON_REG		CON;	//0x00	  
	union	I2C_TAR_REG		TAR;	//0x04	  
	UINT32	SAR;	//0x08		10 bits
	UINT32	HS_MADDR;	//0x0c	3 bits
	union 	I2C_DATA_CMD_REG	DATA_CMD;	//0x10
	UINT32	SS_SCL_HCNT;	//0x14	 16 bits
	UINT32	SS_SCL_LCNT;	//0x18	 16 bits
	UINT32	FS_SCL_HCNT;	//0x1C	 16 bits
	UINT32	FS_SCL_LCNT;	//0x20	 16 bits
	UINT32	HS_SCL_HCNT;	//0x24	 16 bits
	UINT32	HS_SCL_LCNT;	//0x28	 16 bits
	
	union 	I2C_INTR_STAT_REG	INTR_STAT;	//0x2C
	union 	I2C_INTR_STAT_REG	INTR_MASK;	//0x30
	union 	I2C_INTR_STAT_REG	RAW_INTR_STAT;//0x34
	UINT32	RX_TL;	//0x38	8 bits
	UINT32	TX_TL;	//0x3C	8 bits
	UINT32	CLR_INTR;	//0x40	1 bit
	UINT32	CLR_RX_UNDER;	//0x44	1 bit
	UINT32	CLR_RX_OVER;	//0x48	1 bit
	UINT32	CLR_TX_OVER;	//0x4C	1 bit
	UINT32	CLR_RD_REQ;	//0x50	1 bit
	UINT32	CLR_TX_ABRT;	//0x54	1 bit	
	UINT32	CLR_RX_DONE;	//0x58	1 bit
	UINT32	CLR_ACTIVITY;	//0x5C	1 bit
	UINT32	CLR_STOP_DET;	//0x60	1 bit
	UINT32	CLR_START_DET;	//0x64	1 bit
	UINT32	CLR_GEN_CALL;	//0x68	1 bit
	union 	I2C_ENABLE_REG	ENABLE;	//0x6C
	union 	I2C_STATUS_REG	STATUS;	//0x70
	UINT32	TXFLR;	//0x74
	UINT32	RXFLR;	//0x78
	union 	I2C_SDA_HOLD_REG	SDA_HOLD;	//0x7C
	UINT32	TX_ABRT_SOURCE;	//0x80
	UINT32	SLV_DATA_NACK_ONLY;	//0x84	1 bit
	union 	I2C_DMA_CR_REG	DMA_CR;	//0x88
	UINT32	DMA_TDLR;	//0x8C
	UINT32	DMA_RDLR;	//0x90
	UINT32	SDA_SETUP;	//0x94	8 bits
	UINT32	ACK_GENERAL_CALL;	//0x98	1bit
	union 	I2C_ENABLE_STATUS_REG	ENABLE_STATUS;	//0x9C
	UINT32	FS_SPKLEN;	//0xA0	8 bits
	UINT32	HS_SPKLEN;	//0xA4	8 bits
	UINT32	CLR_RESTART_DET;	//0xA8	1 bit
	UINT32	Rsvd[18];	//0xAC-0xF3
	UINT32	COMP_PARAM_1;	//0xF4	32 bits
	UINT32	COMP_VERSION;	//0xF8	32 bits
	UINT32	COMP_TYPE;	//0xFC	32 bits
};

/*
*************************** AOSS ******************************************
*/
// AOSS Registers
#pragma pack(push)
#pragma pack(1)

struct	PORT_BITS{
	UINT8	GPA0:1;									
	UINT8	GPA1:1;		
	UINT8	GPA2:1;		
	UINT8	GPA3:1;
	UINT8	GPA4:1;									
	UINT8	GPA5:1;		
	UINT8	GPA6:1;		
	UINT8	GPA7:1;
	UINT8	GPA8:1;									
	UINT8	GPA9:1;		
	UINT8	GPA10:1;		
	UINT8	GPA11:1;
	UINT8	GPA12:1;									
	UINT8	GPA13:1;		
	UINT8	GPA14:1;		
	UINT8	GPA15:1;
	UINT8	GPA16:1;									
	UINT8	GPA17:1;		
	UINT8	GPA18:1;		
	UINT8	GPA19:1;
	UINT8	GPA20:1;									
	UINT8	GPA21:1;		
	UINT8	GPA22:1;		
	UINT8	GPA23:1;
	UINT8	GPA24:1;									
	UINT8	GPA25:1;		
	UINT8	GPA26:1;		
	UINT8	GPA27:1;
	UINT8	GPA28:1;									
	UINT8	GPA29:1;		
	UINT8	GPA30:1;		
	UINT8	GPA31:1;
	UINT8	GPB0:1;									
	UINT8	GPB1:1;		
	UINT8	GPB2:1;		
	UINT8	GPB3:1;
	UINT8	GPB4:1;									
	UINT8	GPB5:1;		
	UINT8	GPB6:1;		
	UINT8	GPB7:1;	
};
union	PORT_REG{
	UINT8	all[5];
	struct	PORT_BITS	bit;
};

struct	PMU_CFG_BITS {
	UINT8	AossClkSel:1;	//32K时钟选择									
	UINT8	RtcWakeEn:1;	//RTC唤醒使能
	UINT8	BkRamRetEn:1;	//备份ram进入低功耗使能		
	UINT8	LdoDis:1;		//LDO-H,低功耗模式，关闭功能使能
	UINT8	VddaIoDis:1;	//低功耗模式，模拟IO电源关闭使能									
	UINT8	LdoStepEn:1;	//低功耗模式,LDO-L电压调低功能使能		
	UINT8	VrefDis:1;
	UINT8	ExtSleepEn:1;
//	UINT8	Rsvd0:2;		
};
union 	PMU_CFG_REG	{
	UINT8	all;
	struct	PMU_CFG_BITS	bit;
};

struct	AVR_LP_CTRL_BITS {
	UINT8	Vd11RstEn:1;										
	UINT8	VdHvRstEn:1;	
	UINT8	Vd11SdEn:1;	
	UINT8	VdHvSdEn:1;	
	UINT8	Rsvd0:4;		
};
union 	AVR_LP_CTRL_REG	{
	UINT8	all;
	struct	AVR_LP_CTRL_BITS	bit;
};

struct	BKRAM_ADDR_KEY1_BITS {
	UINT8	KeyH:2;										
	UINT8	Rsvd0:1;	
	UINT8	MSE:1;	
	UINT8	MS:4;			
};
union 	BKRAM_ADDR_KEY1_REG	{
	UINT8	all;
	struct	BKRAM_ADDR_KEY1_BITS	bit;
};

struct	RTC_INTE_BITS {
	UINT8	SecIntEn:1;										
	UINT8	HSecIntEn:1;	
	UINT8	MinIntEn:1;	
	UINT8	SecInt:1;
	UINT8	HsecInt:1;
	UINT8	MinInt:1;
	UINT8	Rsvd0:1;			
	UINT8	GlobalInt:1;
};
union 	RTC_INTE_REG	{
	UINT8	all;
	struct	RTC_INTE_BITS	bit;
};

struct	ANMOD_CFG_BITS {
	UINT8	VrefTrim:3;										
	UINT8	Ldo5033Sel:3;	
	UINT8	AvrLpHd11Sel:2;	
	UINT8	VrefPd:1;
	UINT8	Ldo3318Sel:3;
	UINT8	Ldo3318Pd:1;
	UINT8	Ldo3311Sel:3;
	UINT8	Ldo3311Pd:1;
	UINT8	Osc32kSel:3;
	UINT8	Osc32kEn:1;
	UINT8	Ldo3311LpSel:3;	  ////Ldo3311LpSel [2:1]为LDO3311_LP输出电压档位选择,4档
//	UINT8	Rsvd0:1;		  ////Ldo3311LpSel [2:0]为LDO3311_LP_V5输出电压档位选择,8档
	UINT8	AvrLpLd11Sel:2;
	UINT8	AvrLpHdHvSel:2;
	UINT8	AvrLpLdHvSel:2;
	UINT8	AvrLpPd:1;
	UINT8	LFRoscEn:1;
	UINT8	Vdda18PowEn:1;
	UINT8	VddaIOPowEn:1;
	UINT8	UsbPhyPowEn:1;
	UINT8	I2cMcsSel:3;
	UINT8	Rsvd1:2;
};
union 	ANMOD_CFG_REG	{
	UINT8	all[5];
	struct	ANMOD_CFG_BITS	bit;
};

struct	SHIELD_CTRL_BITS{
	UINT8	TbusDiv:4;
	UINT8	Active:1;
	UINT8	RstEn:1;
	UINT8	SdEn:1;
	UINT8	Rsvd0:1;
};
union	SHIELD_CTRL_REG{
	UINT8	all;
	struct	SHIELD_CTRL_BITS	bit;
};

struct	DECAP_SMODECTRL0_BITS{
	UINT8	D0RstEn:1;
	UINT8	D0SdEn:1;
	UINT8	D1RstEn:1;
	UINT8	D1SdEn:1;
	UINT8	D2RstEn:1;
	UINT8	D2SdEn:1;
	UINT8	D3RstEn:1;
	UINT8	D3SdEn:1;
};
union	DECAP_SMODECTRL0_REG{
	UINT8	all;
	struct	DECAP_SMODECTRL0_BITS	bit;
};

struct	DECAP_SMODECTRL1_BITS{
	UINT8	D4RstEn:1;
	UINT8	D4SdEn:1;
	UINT8	D5RstEn:1;
	UINT8	D6SdEn:1;
	UINT8	Rsvd0:4;
};
union	DECAP_SMODECTRL1_REG{
	UINT8	all;
	struct	DECAP_SMODECTRL1_BITS	bit;
};

struct	DECAP_SMODECTRL2_BITS{
	UINT8	D0SmActive:1;
	UINT8	D1SmActive:1;
	UINT8	D2SmActive:1;
	UINT8	D3SmActive:1;
	UINT8	D4SmActive:1;
	UINT8	D5SmActive:1;
	UINT8	Rsvd0:2;
};
union	DECAP_SMODECTRL2_REG{
	UINT8	all;
	struct	DECAP_SMODECTRL2_BITS	bit;
};


struct	DECAP_DMODECTRL0_BITS{
	UINT8	D0RstEn:1;
	UINT8	D0SdEn:1;
	UINT8	D1RstEn:1;
	UINT8	D1SdEn:1;	
	UINT8	D2RstEn:1;
	UINT8	D2SdEn:1;
	UINT8	Rsvd0:2;
};
union	DECAP_DMODECTRL0_REG{
	UINT8	all;
	struct	DECAP_DMODECTRL0_BITS	bit;
};

struct	DECAP_DMODECTRL1_BITS{
	UINT8	D0DmActive:1;
	UINT8	D1DmActive:1;
	UINT8	D2DmActive:1;
	UINT8	Rsvd0:5;
};
union	DECAP_DMODECTRL1_REG{
	UINT8	all;
	struct	DECAP_DMODECTRL1_BITS	bit;
};


struct	ADC_CTRL_BITS{
	UINT8	Resetn:1;
	UINT8	Soc:1;
	UINT8	BvdDivEnSel:1;
	UINT8	Rsvd2:5;
};
union	ADC_CTRL_REG{
	UINT8	all;
	struct	ADC_CTRL_BITS	bit;
};

struct	ADC_INTEN_BITS{
	UINT8	Eoc:1;
	UINT8	Ch0AutoErr:1;
	UINT8	Ch1AutoErr:1;
	UINT8	Rsvd1:5;
};
union	ADC_INTEN_REG{
	UINT8	all;
	struct	ADC_INTEN_BITS	bit;
};

struct	ADC_INTFLAG_BITS{
	UINT8	Eoc:1;
	UINT8	Ch0AutoErr:1;
	UINT8	Ch1AutoErr:1;
	UINT8	Rsvd1:5;
};
union	ADC_INTFLAG_REG{
	UINT8	all;
	struct	ADC_INTFLAG_BITS	bit;
};

struct	ADC_AUTO_CTRL_BITS{
	UINT8	Ch0AutoEn:1;
	UINT8	Ch1AutoEn:1;
	UINT8	Ch0RstEn:1;
	UINT8	Ch1RstEn:1;
	UINT8	Ch0SdEn:1;
	UINT8	Ch1SdEn:1;
	UINT8	Rsvd1:2;
};
union	ADC_AUTO_CTRL_REG{
	UINT8	all;
	struct	ADC_AUTO_CTRL_BITS	bit;
};

struct	FD32K_CTRL_BITS{
	UINT8	FdEn:1;
	UINT8	RstEn:1;
	UINT8	SdEn:1;
	UINT8	FdInt:1;
	UINT8	FdIntEn:1;
	UINT8	Rsvd1:3;
};
union	FD32K_CTRL_REG{
	UINT8	all;
	struct	FD32K_CTRL_BITS	bit;
};

struct	DECAP_SMODEINT_BITS{
	UINT8	D0:1;
	UINT8	D1:1;
	UINT8	D2:1;
	UINT8	D3:1;
	UINT8	D4:1;
	UINT8	D5:1;
	UINT8	Rsvd1:2;
};
union	DECAP_SMODEINT_REG{
	UINT8	all;
	struct	DECAP_SMODEINT_BITS	bit;
};

struct	DECAP_DMODEINT_BITS{
	UINT8	D0:1;
	UINT8	D1:1;
	UINT8	D2:1;
	UINT8	Rsvd1:5;
};
union	DECAP_DMODEINT_REG{
	UINT8	all;
	struct	DECAP_DMODEINT_BITS	bit;
};

struct	AOSS_REGS		{
	union PORT_REG 		PD_PINS;	//0x0~0x04
	UINT8 PD_FLT;					//0x05
	union PORT_REG		WAK_PINS;	//0x06~0x0A	
	UINT8 WAK_FLT;					//0x0B
	union PMU_CFG_REG	PMU_CFG;	//0x0C
	UINT8 RST_MARK;					//0x0D
	union AVR_LP_CTRL_REG	AVR_LP_CTRL;//0x0E
	UINT8 RSVD0;					//0x0F
	UINT8 PMU_RST_WAIT;				//0x10
	UINT8 PMU_ISO_WAIT;				//0x11
	UINT8 PMU_POFF_WAIT;			//0x12
	UINT8 PMU_BKRAM_RET_WAIT;		//0x13
	UINT8 PMU_LDO_STEP_WAIT;		//0x14
	UINT8 PMU_LDO_WAIT;				//0x15
	UINT8 PMU_PON_WAIT;				//0x16
	UINT8 PMU_CLK_WAIT;				//0x17
	UINT8 PMU_POWER_WAIT;			//0x18
	UINT8 PMU_EPD_DLY;				//0x19
	UINT8 RSVD1[6];					//0x1A~0x1F
	UINT8 PMU_PD_CMD;				//0x20
	UINT8 PMU_LDO_STEP_TAR;			//0x21
	UINT8 RSVD2[2];					//0x22~0x23
	UINT8 BKRAM_ADDR_KEY0;			//0x24
	union BKRAM_ADDR_KEY1_REG	BKRAM_ADDR_KEY1;//0x25
	UINT8 BKRAM_DATA_KEY;			//0x26
	UINT8 RSVD3[9];					//0x27~0x2F
	UINT8 RTC_LOAD[4];				//0x30~0x33
	UINT8 RTC_CTRL;					//0x34
	union RTC_INTE_REG	RTC_INTE;	//0x35
	UINT8 RSVD4[6];					//0x36~0x3B
	UINT8 RTC_CCVR[4];				//0x3C~0x3F
	UINT8 RTC_WCVR[4];				//0x40~0x43
	UINT8 RTC_HSEC_CNT[2];			//0x44~0x45
	UINT8 RSVD6[5];					//0x46~0x4A
	union PORT_REG		GPIO_TAKE;	//0x4B~0x4F
	union PORT_REG		GPIO_REN;	//0x50~0x54
	union PORT_REG		GPIO_OEN;	//0x55~0x59
	union PORT_REG		GPIO_DOUT;	//0x5A~0x5E
	UINT8 RSVD7[17];	
	union ANMOD_CFG_REG	ANMOD_CFG;	//0x70~0x74

	UINT8 RSVD9[11];	
	//DECAP/ACTIVESHIELD	
	union SHIELD_CTRL_REG		SHIELD_CTRL;		//0x80 		
	UINT8 SHIELD_INITVAL;
	UINT8 SHIELD_INT;
	UINT8 SHIELD_INTEN;
	union DECAP_SMODECTRL0_REG	DECAP_SMODECTRL0;	
	union DECAP_SMODECTRL1_REG	DECAP_SMODECTRL1;
	union DECAP_SMODECTRL2_REG	DECAP_SMODECTRL2;
	union DECAP_SMODEINT_REG	DECAP_SMODEINT;
	union DECAP_SMODEINT_REG	DECAP_SMODEINTEN;	
	union DECAP_DMODECTRL0_REG	DECAP_DMODECTRL0;
	union DECAP_DMODECTRL1_REG	DECAP_DMODECTRL1;
	union DECAP_DMODEINT_REG	DECAP_DMODEINT;
	union DECAP_DMODEINT_REG	DECAP_DMODEINTEN;	
	UINT8 RSVD10[3];
	//ADC	
	union ADC_CTRL_REG		ADC_CTRL;		//0x90					
	UINT8 RSTDEL;
	UINT8 SOCDEL;
	union ADC_INTEN_REG		INTEN;
	union ADC_INTFLAG_REG	INTFLAG;
	UINT8 CH0_INTERVAL_B0;
	UINT8 CH0_INTERVAL_B1;
	UINT8 CH1_INTERVAL_B0;
	UINT8 CH1_INTERVAL_B1;
	UINT8 CH0_LOW;
	UINT8 CH0_HIGH;
	UINT8 CH1_LOW;
	UINT8 CH1_HIGH;
	union ADC_AUTO_CTRL_REG	AUTO_CTRL;
	UINT8 CH_SEL;
	UINT8 DOUT;

	//FD32K	
	union FD32K_CTRL_REG	FD32K_CTRL;	//0xA0
	UINT8 MAX_B0;
	UINT8 MAX_B1;
	UINT8 MIN_B0;
	UINT8 MIN_B1;
	UINT8 FOUT_B0;
	UINT8 FOUT_B1;
};

#pragma pack(pop)

/*
*************************** USART ******************************************
*/
// USART Current Status Register Bit Definitions
struct	USART_CSTAT_BITS	{	//	bits	description
	UINT32	TxEpt:1;			//  0		Transmitter empty
	UINT32	RxAvl:1;			//	1		Receive available byte data message
	UINT32	TxFull:1;			//	2		Transmitter FIFO full status
	UINT32	Rsvd1:9;			//	11:3	Reserved
	UINT32	Si2cDa:1;			//	12		Data/address (I2C slave only)
	UINT32	Si2cRw:1;			// 	13		Read/Write info. (I2C slave only)
	UINT32	Si2cNack:1;			//	14		I2C acknowledge (I2C slave only)
	UINT32	Si2cBusy:1;			//  15		I2C bus busy (I2C slave only)
	UINT32	Rsvd2:16;			//	31:16	Reserved
};
union	USART_CSTAT_REG		{
	UINT32	all;
	struct	USART_CSTAT_BITS	bit;
};

// USART Interrupt Status Register Bit Definitions
struct	USART_INTSTAT_BITS	{	//	bits	dedcription
	UINT32	TxIntf:1;			//	0		Transmitter FIFO empty interrupt flag
	UINT32	RxIntf:1;			//	1		Receiver data available interrupt flag
	UINT32	UnderRunIntf:1;		//	2		SPI slave transmitter underrun flag (SPI slave only)
	UINT32	RxOerrIntf:1;		//  3		Receiver overrun error interrupt flag
	UINT32	RxPerrIntf:1;		//	4		Parity check error interrupt flag (UART only)
	UINT32	RxFerrIntf:1;		//  5		Framing error interrupt flag (UART only)
	UINT32	RxBrkIntf:1;		//  6		Break condition interrupt flag (UART only)
	UINT32	NackIntf:1;			//  7		No acknowledge interupt flag (I2C master only)
	UINT32	RtsIntf:1;			//	8		Request to send data flag (I2C slave transmitter only)
	UINT32	Rsvd:23;			//  31:9	Reserved
};
union	USART_INTSTAT_REG	{
	UINT32	all;
	struct	USART_INTSTAT_BITS	bit;
};

// USART Interrupt Enable Register Bits Definitions
struct	USART_INTEN_BITS	{	//	bits	description
	UINT32	TxiEn:1;			//	0		Transmitter FIFO empty interrupt enable
	UINT32	RxiEn:1;			//	1		Receiver data available interrupt enable
	UINT32	UnderRuniEn:1;		//	2		SPI slave transmitter underrun enable (SPI slave only)
	UINT32	RxOerriIEn:1;		//  3		Receiver overrun error interrupt enable
	UINT32	RxPerriEn:1;		//	4		Parity check error interrupt enable (UART only)
	UINT32	RxFerriEn:1;		//  5		Framing error interrupt enable (UART only)
	UINT32	RxBrkiEn:1;			//  6		Break condition interrupt enable (UART only)
	UINT32	NackiEn:1;			//  7		No acknowledge interupt enable (I2C master only)
	UINT32	RtsiEn:1;			//	8		Request to send data enable (I2C slave transmitter only)
	UINT32	Rsvd:23;			//  31:9	Reserved
};
union	USART_INTEN_REG	{
	UINT32	all;
	struct	USART_INTEN_BITS	bit;
};

// USART Interrupt Clear Register Bit Definition
struct	USART_INTCLR_BITS{	//	bits	description
	UINT32	TxiClr:1;		//	0		Transmitter FIFO empty interrupt clear
	UINT32	RxiClr:1;		//	1		Receiver data available interrupt clear
	UINT32	UnderRunClr:1;	//	2		SPI slave transmitter underrun clear (SPI slave only)
	UINT32	RxOerrClr:1;	//  3		Receiver overrun error interrupt clear
	UINT32	RxPerrClr:1;	//	4		Parity check error interrupt clear (UART only)
	UINT32	RxFerrClr:1;	//  5		Framing error interrupt clear (UART only)
	UINT32	RxBrkClr:1;		//  6		Break condition interrupt clear (UART only)
	UINT32	NackiClr:1;		//  7		No acknowledge interupt clear (I2C master only)
	UINT32	RtsiClr:1;		//	8		Request to send data clear (I2C slave transmitter only)
	UINT32	Rsvd:23;		//  31:9	Reserved
};
union	USART_INTCLR_REG{
	UINT32	all;
	struct	USART_INTCLR_BITS	bit;
};

// USART Global Control Register Bits Definition
struct	USART_GCTL_BITS	{	// bits	description
	UINT32	Usam:2;			//	1:0		USART mode select
	UINT32	Mm:1;			//  2		Master mode (SPI or I2C)
	UINT32	TxEn:1;			//  3		Transmit enable
	UINT32	RxEn:1;			//  4		Receive enable
	UINT32	Rxtlf:2;		//  6:5		Receive FIFO trigger level flag
	UINT32  Txtlf:2;		//  8:7		Transmit FIFO trigger level flag
	UINT32	Rsvd:23;		//	31:9	Reserved
};
union	USART_GCTL_REG	{
	UINT32	all;
	struct	USART_GCTL_BITS	bit;
};

// USART Common Control Register Bits Definition
struct	USART_CCTL_BITS	{	// bits	description
	UINT32	Ckph:1;			//	0		Clock phase select (SPI only)
	UINT32	Ckpl:1;			//	1		Clock polarity select (SPI only)
	UINT32	Lsbfe:1;		//	2		LSB first enable (SPI only)
	UINT32	SpiLen:1;		//	3		SPI character length (SPI only)
	UINT32	PEn:1;			//	4		Parity enable (UART only)
	UINT32	PSel:1;			//	5		Parity select (UART only)
	UINT32	Spb:1;			//  6		Stop bit select (UART only)
	UINT32	Char:2;			//	8:7		UART character length (UART only)
	UINT32	Brk:1;			//  9		UART break control (UART only)
	UINT32	I2cXa:1;		//	10		I2C extend addressing (I2C only)
	UINT32	I2cRstt:1;		//	11		Repeat start (I2C master only)
	UINT32	I2cStp:1;		//  12		STOP bit (I2C master only)
	UINT32	I2cStt:1;		//  13		Start bit (I2C master only)
	UINT32	NackDt:1;		//	14		Acknowledge data bit (I2C master receive only)
	UINT32	Rsvd:17;		//	31:15	Reserved
};
union	USART_CCTL_REG	{
	UINT32	all;
	struct	USART_CCTL_BITS	bit;
};

// USART Registers
struct	USART_REGS		{
	UINT32	TXREG;							// USART transmit data register
	UINT32	RXREG;							// USART receive data register
	union 	USART_CSTAT_REG		CSTAT;		// USART current status register
	union 	USART_INTSTAT_REG	INTSTAT;	// USART interrupt status register
	union 	USART_INTEN_REG		INTEN;		// USART interrupt enable register
	union 	USART_INTCLR_REG	INTCLR;		// USART interrupt clear register
	union 	USART_GCTL_REG		GCTL;		// USART global control register
	union 	USART_CCTL_REG		CCTL;		// USART common control register
	UINT32	SPBREG;							// USART baud rate generator register
	UINT32	SI2CADD;						// Addrss value in I2C slave mode
};

/*
*************************** USB ******************************************
*/
//#pragma pack(push) 		//保存对齐状态 
#pragma pack(1)
//----- USB FADDR Register Bit Definition-----
struct USB_FADDR_BITS	{	//	bits	description
	UINT8 	Addr:7;			//	Address of usb
	UINT8 	Update:1;		//	Set when FAddr is written. Cleared when the new address takes effect (at the end of the current transfer).	  
};

union USB_FADDR_REG		{	//reset is 0x00
	UINT8 	all;
	struct 	USB_FADDR_BITS bit;
};

//---- USB POWER Register Bit Definition -------
struct USB_POWER_BITS	{	//	bits	description
	UINT8 	EnSuspendM:1;	//Set by the CPU to enable the SUSPENDM signal.
	UINT8 	SuspendMode:1;	//	This read-only bit is set when Suspend mode is entered. It is cleared when the CPU reads the
	                     	//interrupt register, or sets the Resume bit of this register
	UINT8 	Resume:1; 		//Set by the CPU to generate Resume signaling when the function is in Suspend mode. The CPU
	              			//should clear this bit after 10 ms (a maximum of 15 ms) to end Resume signaling.  
	UINT8 	Reset:1;		//This read-only bit is set when Reset signaling has been detected on the bus
	
	UINT8 	Rev:3;
	UINT8 	ISOUpdate:1; 
};
union USB_POWER_REG		{	//reset is 0x20
	UINT8 	all;
	struct	USB_POWER_BITS bit;
};

//----- USB Interrupt IN Register Bit Definition-----
struct USB_INTRIN_BITS	{	//	bits	description
	UINT16	EP0:1;  	    //	0 		EP0 interrupt
	UINT16	EP1In:1;   	    //	1 		EP1 In interrupt
	UINT16	EP2In:1;		//	1		EP2 In interrupt
	UINT16	EP3In:1;   	   	//	1 		EP3 In interrupt
	UINT16	EP4In:1;	 	//	1		EP4 In interrupt
	UINT16	Rev:11;
};
union USB_INTRIN_REG	{	//reset is 0x0000
	UINT16 	all;
	struct 	USB_INTRIN_BITS bit;
};

//------ USB Interrupt OUT Register Bit Definition------
struct USB_INTROUT_BITS	{	//	bits	description
	UINT16 	RESERVED:1;  	//	0 	 
	UINT16 	EP1Out:1;   	//	1 		EP1 Out interrupt
	UINT16 	EP2Out:1;		//	1		EP2 Out interrupt
	UINT16 	EP3Out:1;   	//	1 		EP3 Out interrupt
	UINT16 	EP4Out:1;		//	1		EP4 Out interrupt
	UINT16 	REV:11;
};
union USB_INTROUT_REG	{	//reset is 0x0000
	UINT16 	all;
	struct 	USB_INTROUT_BITS bit;
};

//---- USB Interrupt enable IN Register Bit Definition------
struct USB_INTRINE_BITS	{	//	bits	description
	UINT16 	EP0:1;  	    //	0 		EP0 interrupt
	UINT16 	EP1In:1;   	    //	1 		EP1 In interrupt
	UINT16 	EP2In:1;		//	1		EP2 In interrupt
	UINT16 	EP3In:1;   	    //	1 		EP3 In interrupt
	UINT16 	EP4In:1;	 	//	1		EP4 In interrupt
	UINT16 	Rev:11;
};
union USB_INTRINE_REG	{	//reset is 0xffff
	UINT16 	all;
	struct 	USB_INTRINE_BITS bit;
};
//----- USB Interrupt enable OUT Register Bit Definition-----
struct USB_INTROUTE_BITS{	//	bits	description
	UINT16 	RESERVED:1;  	//	0 	 
	UINT16 	EP1Out:1;   	//	1 		EP1 Out interrupt
	UINT16 	EP2Out:1;		//	1		EP2 Out interrupt
	UINT16 	EP3Out:1;   	//	1 		EP3 Out interrupt
	UINT16 	EP4Out:1; 		//	1		EP4 Out interrupt
	UINT16 	Rev:11;
};
union USB_INTROUTE_REG	{	//reset is 0xffff
	UINT16 	all;
	struct 	USB_INTROUTE_BITS bit;
};

//---- USB Interrupt Register Bit Definition------- 
struct USB_INTRUSB_BITS	{	//bits	description
	UINT8 	Suspend:1;		//Set when suspend signaling is detected on the bus.
	UINT8 	Resume:1; 		//Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
	UINT8 	Reset:1;		//Set when reset signaling is detected on the bus.
	UINT8 	SOF:1;			//Set at the start of each frame      
	UINT8 	Reserved:4;                       
};
union USB_INTRUSB_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INTRUSB_BITS bit;
};
//----- USB Interrupt Enable Register Bit Definition -----
struct USB_INTRUSBE_BITS{	//	bits	description
	UINT8 	Suspend:1;		//Set when suspend signaling is detected on the bus.
	UINT8 	Resume:1; 		//Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
	UINT8 	Reset:1;		//Set when reset signaling is detected on the bus.
	UINT8 	SOF:1;			//Set at the start of each frame      
	UINT8 	Reserved:4;                       
};
union USB_INTRUSBE_REG	{	//reset is 0x06
	UINT8 	all;
	struct	USB_INTRUSBE_BITS bit;
};

//----- USB Frame Register Bit Definition------- 
struct USB_FRAME_BITS	{	//bits	description
	UINT16 	FrameNumber:11;	     
	UINT16 	Reserved:5;                      
};
union USB_FRAME_REG		{	//reset is 0x0000
	UINT16 	all;
	struct	USB_FRAME_BITS bit;
};

//----- USB Index Register Bit Definition --------
struct USB_INDEX_BITS	{	//bits	description
	UINT8 	EPIndex:4;	     
	UINT8 	Reserved:4;                     
                                              
};
union USB_INDEX_REG{		//reset is 0x0000
	UINT8 	all;
	struct	USB_INDEX_BITS bit;
};
// ----USB control and status bits for Endpoint 0 Register Bit Definition------
struct USB_CSR0_BITS	{		//	bits	description
	UINT8 	OutPktRdy:1;		//0 This bit is set when a data packet has been received.
	UINT8 	InPktRdy:1;			//1 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	SentStall:1;		//2 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	UINT8 	DataEnd:1;			//3 The CPU sets this bit:
	                        		//1. When setting InPktRdy for the last data packet.
	                        		//2. When clearing OutPktRdy after unloading the last data packet.
	                        		//3. When setting InPktRdy for a zero length data packet.
	                        		//It is cleared automatically.
	UINT8 	SetupEnd:1;			//4 This bit will be set when a control transaction ends before the DataEnd bit has been set.
	                        		//An interrupt will be generated and the FIFO flushed at this time.
	UINT8 	SendStall:1;		//5 The CPU writes a 1 to this bit to terminate the current transaction. The STALL handshake
	                        		//will be transmitted and then this bit will be cleared automatically.
	UINT8 	ServicedOutPktRdy:1;//6 The CPU writes a 1 to this bit to clear the OutPktRdy bit. It is cleared automatically.
	UINT8 	ServicedSetupEnd:1;	//7 The CPU writes a 1 to this bit to clear the SetupEnd bit. It is cleared automatically.
	     
};
union USB_CSR0_REG		{	//reset is 0x00
	UINT8 	all;
	struct	USB_CSR0_BITS bit;
};
//---- USB Count of EP0 FIFO Register Bit Definition------ 
struct USB_COUNT0_BITS	{	//bits	description
	UINT8	Count:7;	     
	UINT8	Reserved:1;                                                                  
};
union USB_COUNT0_REG	{	//reset is 0x0000
	UINT8 	all;
	struct	USB_COUNT0_BITS bit;
};

//------- USB provides control and status bits for IN transactions Register Bit Definition
struct USB_INCSRL_BITS	{	//bits	description
	UINT8 	InPktRdy:1;		//0 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	FIFONotEmpty:1;	//1 This bit is set when there is at least 1 packet in the IN FIFO.
	UINT8 	UnderRun:1;		//2 In ISO mode,
	UINT8 	FlushFIFO:1;	//3 TThe CPU writes a 1 to this bit to flush the next packet to be transmitted from the endpoint IN FIFO.
	UINT8 	SendStall:1;	//4 The CPU writes a 1 to this bit to issue a STALL handshake to an IN token.
	UINT8 	SentStall:1;	//5 This bit is set when a STALL handshake is transmitted.
	UINT8 	ClrDataTog:1;	//6 The CPU writes a 1 to this bit to reset the endpoint IN data toggle to 0.
	UINT8 	IncompTx:1;		//7 used in ISO mode
};
union USB_INCSRL_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INCSRL_BITS bit;
};

//------- USB provides additional control for IN transactions Register Bit Definition
struct USB_INCSRU_BITS	{	//bits	description
	UINT8 	Reserved1:1;	//0 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	DPktBufDis:1;	//1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled
	UINT8 	Reserved2:1;	//2 The CPU sets this bit to select DMA Request Mode 1 and clears it to select DMA Request Mode 0.
	UINT8 	FrcDataTog:1;	//3 sets this bit to force the endpoint’s IN data toggle to switch after each data packet is sent
	UINT8 	Reserved3:1;	//4 sets this bit to enable the DMA request for the IN endpoint.
	UINT8 	Mode:1;			//5 sets this bit to enable the endpoint direction as IN, and clears it to enable the endpoint direction as OUT.
	UINT8 	ISO:1;			//6  used in ISO mode
	UINT8 	AutoSet:1;		//7 If the CPU sets this bit, InPktRdy will be automatically set when data of the maximum packet size
	                  		//(as set in InMaxP) is loaded into the IN FIFO. If a packet of less than the maximum packet size is
	                   		//loaded, InPktRdy will have to be set manually. Note:	     
};
union USB_INCSRU_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INCSRU_BITS bit;
};
// -----USB Count of EPx OUT Max amount of data that can be transferred through the selected IN endpoint in a single
//operation. Register Bit Definition 
struct USB_OUTMAXP_BITS	{	//bits	description
	UINT8 	MaxAmount:3;	     
	UINT8 	Reserved:5;                                              
};
union USB_OUTMAXP_REG	{	//reset is 0x0000
	UINT8 	all;
	struct	USB_OUTMAXP_BITS bit;
};

//------- USB provides control and status bits for Out transactions Register Bit Definition
struct USB_OUTCSRL_BITS	{	//bits	description
	UINT8 	OutPktRdy:1;	//0 This bit is set when a data packet has been received.
	UINT8 	FIFOFull:1;		//1 This bit is set when no more packets can be loaded into the OUT FIFO.
	UINT8 	OverRun:1;		//2 This bit is set if an OUT packet arrives while FIFOFull is set
	UINT8 	DataError:1;	//3 This bit is set at the same time that OutPktRdy is set if the data packet has a CRC error.
	UINT8 	FlushFIFO:1;	//4 writes a 1 to this bit to flush the next packet to be read from the endpoint OUT FIFO.
	UINT8 	SendStall:1;	//5 writes a 1 to this bit to issue a STALL handshake to a DATA packet.
	UINT8 	SentStall:1;	//6 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	UINT8 	ClrDataTog:1;	//7 writes a 1 to this bit to reset the endpoint data toggle to 0.	     
};
union USB_OUTCSRL_REG{		//reset is 0x00
	UINT8 	all;
	struct	USB_OUTCSRL_BITS bit;
};

//----- USB provides additional control for Out transactions Register Bit Definition
struct USB_OUTCSRU_BITS	{		//bits	description
	UINT8 	Reserved1:1;		//0 This bit is set in a high-bandwidth Isochronous transfer if the packet in the OUT FIFO is incomplete
	UINT8 	DPktBufDis:1;		//1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled.
	UINT8 	Reserved2:1;		//2
	UINT8 	Reserved3:1;		//3 Two modes of DMA Request operation are supported:
	UINT8 	DisNYETPIDError:1;	//4 sets this bit to disable the sending of NYET handshakes.
	UINT8 	Reserved4:1;		//5 sets this bit to enable the DMA request for the OUT endpoint.
	UINT8 	ISO:1;				//6 used in ISO mode
	UINT8 	AutoClear:1;		//7 sets this bit then the OutPktRdy bit will be automatically cleared when a packet of
	                    		//OutMaxP bytes has been unloaded from the OUT FIFO.
};
union USB_OUTCSRU_REG	{		//reset is 0x00
	UINT8 	all;
	struct	USB_OUTCSRU_BITS bit;
};
 
union USB_STAS_REG{    
    union 	USB_CSR0_REG CSR0;
    union 	USB_INCSRL_REG INCSRL;
};

//=========== USB Registers=====================
struct	USB_REGS		{
	union	USB_FADDR_REG		FADDR;		//8
	union	USB_POWER_REG 		POWER;		//8
	union	USB_INTRIN_REG		INTRIN;		//16
	union	USB_INTROUT_REG		INTROUT;	//16
	union	USB_INTRUSB_REG		INTRUSB;	//8	  
	union	USB_INTRINE_REG		INTRINE;	//16	
	union	USB_INTROUTE_REG	INTROUTE;	//16
	union	USB_INTRUSBE_REG	INTRUSBE;	//8
	UINT16	FRAME;							//16//union  USB_FRAME_REG    FRAME;
    UINT8 	INDEX;							//8//union  USB_INDEX_REG     INDEX;
    UINT8 	RESERVED1; 						//8
    UINT8 	INMAXP;  						//8//union USB_INMAXP_REG    INMAXP;	
    union 	USB_STAS_REG		STAS;		//8
	union 	USB_INCSRU_REG		INCSRU;		//8
    UINT8 	OUTMAXP;						//8 
	union 	USB_OUTCSRL_REG     OUTCSRL;	//8	
	union 	USB_OUTCSRU_REG     OUTCSRU;	//8
	UINT16 	OUTCOUNT;						//16
	UINT16 	RESERVED2[4];
	UINT32 	FIFO_EP0;
	UINT32 	FIFO_EP1;
	UINT32 	FIFO_EP2;
	UINT32 	FIFO_EP3;
	UINT32 	FIFO_EP4;
};
#pragma pack()

/*
*************************** SSI ******************************************
*/
// SSI Control Register 0 Bit Definition
struct	SSI_CTRLR0_BITS	{	//	bits	description
	UINT32	Dfs:4;  		//	3:0		Data Frame Size
	UINT32	Frf:2;			//	5:4		Frame Format
	UINT32	Scph:1;			// 	6		Serial Clock Phase
	UINT32	Scpol:1;		//	7		Serial Clock Polarity
	UINT32	Tmod:2;			// 	9:8		Transfer Mode
	UINT32	SlvOe:1;		// 	10		Slave Output Enable
	UINT32	Srl:1;			//	11		Shift Register Loop
	UINT32	Cfs:4;			//	15:12	Control Frame Size
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR0_REG	{
	UINT32	all;
	struct	SSI_CTRLR0_BITS	bit;
};

// SSI Control Register 1 Bit Definition
struct	SSI_CTRLR1_BITS	{	//	bits	description
	UINT32	Ndf:16;  		//	15:0	Number of Data Frame
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR1_REG	{
	UINT32	all;
	struct	SSI_CTRLR1_BITS	bit;
};

// SSI Microwire Control Register Bit Definition
struct	SSI_MWCR_BITS	{	//	bits	description
	UINT32	MwMod:1;  		//	0		Microwire Transfer Mode
	UINT32	Mod:1;  		//	1		Microwire Control
	UINT32	Mhs:1;  		//	2		Microwire Handshaking
	UINT32	Rsvd:29; 		//	31:3 	Reserved
};
union	SSI_MWCR_REG	{
	UINT32	all;
	struct	SSI_MWCR_BITS	bit;
};

// SSI Status Register Bit Definition
struct	SSI_SR_BITS		{	//	bits	description
	UINT32	Busy:1;  		//	0		SSI Busy Flag
	UINT32	Tfnf:1;			//	1		Transmit FIFO Not Full
	UINT32	Tfe:1;			// 	2		Transmit FIFO Empty
	UINT32	Rfne:1;			//	3		Receive FIFO Not Empty
	UINT32	Rff:1;			// 	4		Receive FIFO Full
	UINT32	Txe:1;			// 	5		Transmission Error
	UINT32	Dcol:1;			//	6		Data Collision Error
	UINT32	Rsvd:25; 		//	31:7 	Reserved
};
union	SSI_SR_REG		{
	UINT32	all;
	struct	SSI_SR_BITS	bit;
};

// SSI Interrupt Register Bit Definition
struct	SSI_INT_BITS	{	//	bits	description
	UINT32	Txe:1;  		//	0		Transmit FIFO Empty 
	UINT32	Txo:1;			//	1		Transmit FIFO Overflow
	UINT32	Rxu:1;			// 	2		Receive FIFO Underflow
	UINT32	Rxo:1;			//	3		Receive FIFO Overflow
	UINT32	Rxf:1;			// 	4		Receive FIFO Full
	UINT32	Mst:1;			// 	5		Transmission Error
	UINT32	Rsvd:26; 		//	31:6 	Reserved
};
union	SSI_INT_REG		{
	UINT32	all;
	struct	SSI_INT_BITS	bit;
};

// SSI DMA Control Register Bit Definition
struct	SSI_DMACR_BITS	{	//	bits	description
	UINT32	Rdmae:1;  		//	0		Receive DMA Enable
	UINT32	Tdmae:1;  		//	1		Transmit DMA Enable
	UINT32	Rsvd:30; 		//	31:2 	Reserved
};
union	SSI_DMACR_REG	{
	UINT32	all;
	struct	SSI_DMACR_BITS	bit;
};

// SSI regs
struct	SSI_REGS		{
	union 	SSI_CTRLR0_REG	CTRLR0;		// Control Register 0
	union 	SSI_CTRLR1_REG	CTRLR1;		// Control Register 1
	UINT32	SSIENR;
	union 	SSI_MWCR_REG	MWCR;

	UINT32 	SER;
	UINT32	BAUDR;
	UINT32	TXFTLR;
	UINT32	RXFTLR;

	UINT32	TXFLR;	
	UINT32	RXFLR;
	union 	SSI_SR_REG 		SR;
	union 	SSI_INT_REG		IMR;

	union 	SSI_INT_REG		ISR;
	union 	SSI_INT_REG		RISR;
	UINT32	TXOICR;
	UINT32	RXOICR;

	UINT32	RXUICR;
	UINT32	MSTICR;
	UINT32	ICR;
	union 	SSI_DMACR_REG	DMACR;

	UINT32 	DMATDLR;
	UINT32	DMARDLR;
	UINT32	IDR;
	UINT32	COMP_VERSION;

	UINT32	DR;
	UINT32	RSVD1[35];

	UINT32	RX_SAMPLE_DLY;
	UINT32	RSVD2[3];
};

/*
*************************** SWPS ******************************************
*/
#define	SWPS_INT_TX_DONE	(1<<0)
#define	SWPS_INT_RX_SOF		(1<<8)
#define	SWPS_INT_RX_DONE	(1<<9)
#define	SWPS_INT_RESUME		(1<<24)
#define	SWPS_INT_SUSPEND	(1<<25)
#define	SWPS_INT_DEACTIVE	(1<<26)
#define	SWPS_INT_CLF_START	(1<<27)

// SWPS Interrupt (Enable) Register Bit Definition
struct	SWPS_INT_BITS		{	//	bits	description
	UINT32	TxDone:1;  			//	0		transmit over
	UINT32	Rsvd0:1;			//	1		reserved
	UINT32	TxFifoNotFull:1;	// 	2		transmit FIFO not full
	UINT32	Rsvd1:5;
	UINT32	RxSof:1;			// 	8		SOF received
	UINT32	RxDone:1;			// 	9		receive over
	UINT32	Rsvd2:3;			//	12:10	reserved
	UINT32	RxFifoNotEmpty:1;	//	13		receive FIFO not empty
	UINT32	Rsvd3:10;			// 	23:14	reserved
	UINT32	Resume:1;			// 	24		Resume received
	UINT32	Suspend:1;			//	25		Suspend received
	UINT32	Deactive:1;			//	26		Deactive received
	UINT32	ClfStart:1;			// 	27		
	UINT32	Rsvd:4; 			//	31:28 	Reserved
};
union	SWPS_INT_REG		{
	UINT32	all;
	struct	SWPS_INT_BITS	bit;
};

// SWPS Status Register Bit Definition
struct	SWPS_STATUS_BITS	{	//	bits	description
	UINT32	TxFifoEmpty:1;  			
	UINT32	TxFifoFull:1;				
	UINT32	RxFifoEmpty:1;			
	UINT32	RxFifoFull:1;
	UINT32	Rsvd0:4;				
	UINT32	TxInhibit:1;		
	UINT32	TxFifoUnderFlow:1;	
	UINT32	TxFrameErr:1;		
	UINT32	Rsvd1:5;			
	UINT32	RxFrameErr:1;		
	UINT32	RxCrcErr:1;			
	UINT32	RxEofNotFoundErr:1;	
	UINT32	RxFifoOverFlow:1;			
	UINT32	Rsvd2:12; 			
};
union	SWPS_STATUS_REG		{
	UINT32	all;
	struct	SWPS_STATUS_BITS	bit;
};

// SWPS Control Register Bit Definition
struct	SWPS_CTRL_BITS		{	//	bits	description
	UINT32	TxEn:1;  			//	0		transmit enable
	UINT32	Rsvd1:3;			//	3:1		reserved
	UINT32	TxResumeBit:3;		// 	6:4		the bit 1 befor transmit SOF(reserved)	
	UINT32	TxFifoClr:1;		//	7		Tx FIFO clear
	UINT32	RxEn:1;				// 	8		receive	enable
	UINT32	SuspendIdleBit:3;	// 11:9	Suspend idle bit number
	UINT32	Rsvd2:3;			// 14:12	Reserved
	UINT32	RxFifoClr:1;		//	15		Rx FIFO clear
	UINT32	Rsvd:16; 			//	31:13 	Reserved
};
union	SWPS_CTRL_REG		{
	UINT32	all;
	struct	SWPS_CTRL_BITS	bit;
};

// SWPS Length Register Bit Definition
struct	SWPS_LEN_BITS		{	//	bits	description
	UINT32	TxLen:8;  			//	7:0		transmit length
	UINT32	RxLen:8;			//	15:8	receive length
	UINT32	Rsvd:16; 			//	31:16 	Reserved
};
union	SWPS_LEN_REG		{
	UINT32	all;
	struct	SWPS_LEN_BITS	bit;
};

// SWPS S1 Period Register Bit Definition
struct	SWPS_S1_PERIOD_BITS	{	//	bits	description
	UINT32	MaxHighTime:16;  	//	9:0		S1 maximum high level duration time
	UINT32	MaxLowTime:16;		//	25:16	S1 maximum low level duration time
};
union	SWPS_S1_PERIOD_REG	{
	UINT32	all;
	struct	SWPS_S1_PERIOD_BITS	bit;
};

// SWPS regs
struct	SWPS_REGS			{
	union 	SWPS_INT_REG		INT;
	union 	SWPS_INT_REG		IE;
	union 	SWPS_STATUS_REG		STATUS;
	union 	SWPS_CTRL_REG		CTRL;
	union 	SWPS_LEN_REG 		LEN;
	UINT32	TX_FIFO;
	UINT32	RX_FIFO;
	UINT32	SUSPEND_TM;						// 13bit
	UINT32	DEACTIVATED_TM;					// 14bit
	UINT32	INHIBIT_TM;	   					// 23bit
	union 	SWPS_S1_PERIOD_REG	S1_PERIOD; 	// 11bit
};

/*
*************************** FD12M ******************************************
*/

struct	FD12M_CTRL_BITS	{
	UINT32	FdEn:1;
	UINT32	Rsvd1:7;
	UINT32	RstEn:1; 
	UINT32	SdEn:1;
	UINT32	Rsvd2:6; 
	UINT32	IntEn:1;
	UINT32	Rsvd3:7; 
	UINT32	IntFlag:1;
	UINT32	Rsvd4:7; 
};

union	FD12M_CTRL_REG		{
	UINT32	all;
	struct	FD12M_CTRL_BITS	bit;
};

// FD12M regs
struct	FD12M_REGS			{
	union 	FD12M_CTRL_REG		CTRL;
	UINT32 	MAX;
	UINT32 	MIN;
	UINT32 	FDOUT;
};

/*
*************************** ISO7816 ******************************************
*/
struct	ISO7816_CON0_BITS	{	// bits		description
	UINT32  CLKOE:1;            //  0       clock output enable (master)
	UINT32  INV:1;              //  1       inverse convention
	UINT32  Rsvd_1:2;           //  3:2     Reserved
	UINT32  RBRST:1;            //  4       write 1 to reset rx buffer     
	UINT32  TR:1;               //  5       0:Rx mode; 1:Tx mode
	UINT32  Rsvd_2:1;           //  6       Reserved
	UINT32  MOD:1;              //  7       0:hardware;1:direct I/O
	UINT32  Rsvd_3:24;          //  31:8 Reserved
};

union	ISO7816_CON0_REG	{
	UINT32	all;
	struct	ISO7816_CON0_BITS  bit;
};

//struct	ISO7816_CON1_BITS	{   // bits            description
//	UINT32  TRY:3;              // 2:0             auto tx times
//	UINT32  RRY:3;              // 5:3             auto rx times			
//	UINT32  DIV:7;              // 12:6            divider for clock output
//	UINT32  Rsvd_1:3;           // 15:13           Reserved
//	UINT32  EGT:9;              // 24:16           extra guard time for tx	
//	UINT32  Rsvd_2:7;           // 31:25           Reserved
//};
struct	ISO7816_CON1_BITS	{   // bits            description
	UINT32  TRY:3;              // 2:0             auto tx times
	UINT32  RRY:3;              // 5:3             auto rx times			
	UINT32  DIV:7;              // 12:6            divider for clock output
	UINT32  Rsvd_1:3;           // 15:13           Reserved
	UINT32  EGT:9;              // 24:16           extra guard time for tx	
	UINT32  ReTxWidth:1;		// 25			   T0,retx width
	UINT32  Rsvd_2:6;           // 31:26           Reserved
};

union	ISO7816_CON1_REG   {
	UINT32	all;
	struct	ISO7816_CON1_BITS  bit;
};

//struct	ISO7816_CON2_BITS	{ 	//  bits 	description
//	UINT32 	WES0:1; 			//	0      	error signal width
//	UINT32 	Rsvd_1:1;			//	1       Reserved
//  	UINT32  TPS:1;				// 	2   	T=0 or T=1 protocal
// 	UINT32	Rsvd_2:4; 			//	6:3  	Reserved
//	UINT32	SBIT:1; 			// 	7  		I/O negedge flag
//	UINT32	Rsvd_3:24; 			// 	31:8 	Reserved
//};
struct	ISO7816_CON2_BITS	{ 	//  bits 	description
	UINT32 	WES0:2; 			//	1:0      	error signal width
  	UINT32  TPS:1;				// 	2   	T=0 or T=1 protocal
 	UINT32	Rsvd_2:4; 			//	6:3  	Reserved
	UINT32	SBIT:1; 			// 	7  		I/O negedge flag
	UINT32	Rsvd_3:24; 			// 	31:8 	Reserved
};

union	ISO7816_CON2_REG	{
	UINT32	all;
    struct	ISO7816_CON2_BITS  bit;
};

struct	ISO7816_STS_BITS	{	// bits    	description
	UINT32	TBE:1;				//	0       tx buffer empty flag
	UINT32	RBF:1;              //  1     	rx buffer full flag
	UINT32	RBNE:1;             //	2      	rx buffer is not empty
	UINT32	PE:1;               //	3       parity error flag
	UINT32	OE:1;               //	4       rx buffer overrun error flag
	UINT32	WAIT:1;             // 	5   	1:register access is not finished
	UINT32	Rsvd_1:26;          // 	31:6 	Reserved
};

union	ISO7816_STS_REG		{
	UINT32	all;
	struct 	ISO7816_STS_BITS	bit;
};

struct	ISO7816_BRC_BITS	{	//	bits    	description
 	UINT32 	DI:4;             	//	3:0     baud rate parameter: D
    UINT32	FI:4;               //	7:4  	baud rate parameter: F
    UINT32	Rsvd_1:24;          //	31:8 	Reserved
};

union	ISO7816_BRC_REG		{
	UINT32	all;
	struct 	ISO7816_BRC_BITS	bit;
};

struct	ISO7816_BUF_BITS	{ 	//	bits    	description
	UINT32 	DATA:8;           	//	7:0     tx or rx buffer
	UINT32  Rsvd_1:24;        	//	31:8 	Reserved
};

union	ISO7816_BUF_REG		{
	UINT32	all;
	struct 	ISO7816_BUF_BITS	bits;
};

struct	ISO7816_DIO_BITS	{	//	bits  	description
	UINT32	DIO:1;            	//	0      	data input/output
	UINT32	ENO:1;             	//	1       direct output enable
	UINT32 	Rsvd_1:30;         	//	31:2 	Reserved
};

union	ISO7816_DIO_REG 	{
	UINT32	all;
	struct 	ISO7816_DIO_BITS 	bit;
};

struct	ISO7816_MSK_BITS 	{	// bits  	description
	UINT32	MTBE:1; 			//	0      	mask for TBE
	UINT32	MRBF:1;            	// 	1      	mask for RBF
	UINT32	MRBNE:1;            //	2      	mask for RBNE
	UINT32	MPE:1;           	// 	3       mask for PE
	UINT32	MOE:1;             	// 	4      	mask for OE
	UINT32	MSBIT:1;          	// 	5      	mask for SBIT
	UINT32	Rsvd_2:26;         	// 	31:6 	Reserved
};

union	ISO7816_MSK_REG     {
	UINT32	all;
	struct 	ISO7816_MSK_BITS 	bit;
};

struct	ISO7816_TCON_BITS   { 	// bits    	description
	UINT32	TS:1;             	// 	0      	ETU counter start
	UINT32	TMOD:2;           	//	2:1    	ETU counter mode selection
	UINT32	TF:1;              	// 	3      	ETU counter overflow flag
	UINT32	Rsvd_1:28;        	// 	31:4 	Reserved 
};

union	ISO7816_TCON_REG   	{
	UINT32	all;
	struct ISO7816_TCON_BITS  bit;
};

struct	ISO7816_TMSK_BITS   {	// bits    	description
	UINT32	MTF:1;            	//	0    	mask for TF
    UINT32	Rsvd_1:31;      	//	31:1 	Reserved
};

union	ISO7816_TMSK_REG    {
	UINT32 	all;	
	struct 	ISO7816_TMSK_BITS  bit; 
};

struct	ISO7816_PROC_BITS   {	// bits    	description	  
	UINT32	DATA:8;       		//	7:0   	auto tx value	
    UINT32	Rsvd_1:24;         	//	31:8 	Reserved	
};

union	ISO7816_PROC_REG  	{
	UINT32     all;	
	struct	ISO7816_PROC_BITS  bit;	 
};

struct	ISO7816_REGS{
	union	ISO7816_CON0_REG	CON0;	 //00
    union	ISO7816_CON1_REG   	CON1;	 //04
    union	ISO7816_CON2_REG   	CON2;	 //08
    union	ISO7816_STS_REG    	STS;	 //0C
    union	ISO7816_BRC_REG    	BRC;	 //10
    union	ISO7816_BUF_REG    	BUF;	 //14
    union	ISO7816_DIO_REG    	DIO;	 //18
    union	ISO7816_MSK_REG    	MSK;	 //1C
    union	ISO7816_TCON_REG   	TCON0;	 //20
    UINT32   	TDAT0;					 //24
    UINT32   	TRLD0;					 //28
    union 	ISO7816_TMSK_REG   	TMSK0;	 //2C
    union 	ISO7816_PROC_REG   	PROC0;	 //30
	union	ISO7816_TCON_REG   	TCON1;	 //34
    UINT32   	TDAT1;					 //38
    UINT32   	TRLD1;					 //3C
    union 	ISO7816_TMSK_REG   	TMSK1;	 //40
	UINT32	COUNT0;						 //44
	UINT32	COUNT1;						 //48
};

/*
*************************** SCI ******************************************
*/
// SCI Data Register Bit Definition
struct	SCI_DATA_BITS	{//	bits	description
	UINT32	Data:8;  	//	7:0		read or write data
	UINT32	ParityErr:1;//	8		parity error flag
	UINT32	Rsvd:23; 	//	31:9 	Reserved
};
union	SCI_DATA_REG	{
	UINT32	all;
	struct	SCI_DATA_BITS	bit;
};

// SCI Control Register 0 Bit Definition
struct SCI_CR0_BITS 	{	//	bits	description
	UINT32  Sense :1;		//	0
	UINT32  Order :1;		// 	1
	UINT32  TxParity:1;		//	2
	UINT32  TxNak:1;		//	3
	UINT32  RxParity:1;		//	4
	UINT32  RxNak:1;		//	5
	UINT32  ClkDis:1;		//	6
	UINT32  ClkVal :1;		//	7
	UINT32  Rsvd:24;
};
union SCI_CR0_REG {
	UINT32 	all;
	struct 	SCI_CR0_BITS bit;
};

// SCI Control Register 1 Bit Definition
struct SCI_CR1_BITS {
	UINT32  ATRDEn :1;
	UINT32  BlkEn :1;
	UINT32  Mode:1;
	UINT32  Clkz1:1;
	UINT32  BgtEn:1;
	UINT32  ExDbnce:1;
	UINT32  SyncCard:1;
	UINT32	CardSocket:1;
	UINT32  Rsvd:24;
};
union SCI_CR1_REG {
	UINT32 	all;
	struct 	SCI_CR1_BITS bit;
};

// SCI Control Register 2 Bit Definition
struct SCI_CR2_BITS {
	UINT32  StartUp :1;
	UINT32  Finish :1;
	UINT32  WrReset:1;
	UINT32  Rsvd:29;
};
union SCI_CR2_REG {
	UINT32 	all;
	struct 	SCI_CR2_BITS bit;
};

// SCI Transmit and Receive Tide Register Bit Definition
struct SCI_TIDE_BITS {
	UINT32  TxTide :4;
	UINT32  RxTide :4;
	UINT32  Rsvd:24;
};
union SCI_TIDE_REG {
	UINT32 	all;
	struct 	SCI_TIDE_BITS bit;
};

// SCI Transmit and Receive Retry Register Bit Definition
struct SCI_RETRY_BITS {
	UINT32  TxRetry :3;
	UINT32  RxRetry :3;
	UINT32  Rsvd:26;
};
union SCI_RETRY_REG 	{
	UINT32 all;
	struct SCI_RETRY_BITS bit;
};

// SCI Transmit FIFO Count Register Bit Definition
struct SCI_FIFOSTATUS_BITS {
	UINT32  TxFF :1;
	UINT32  TxFE :1;
	UINT32  RxFF :1;
	UINT32  RxFE :1;
	UINT32  Rsvd:28;
};
union SCI_FIFOSTATUS_REG	{
	UINT32 	all;
	struct 	SCI_FIFOSTATUS_BITS bit;
};

// SCI Interrupt Mask/Status/Masked Status Register Bit Definition
struct SCI_INT_BITS 	{
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  RxTide :1;
	UINT32  TxTide :1;
	UINT32  Rsvd:17;
};
union SCI_INT_REG	{
	UINT32 	all;
	struct 	SCI_INT_BITS bit;
};

//SCI Interrupt Clear Register Bit Definition
struct SCI_ICR_BITS		{
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  Rsvd:19;
};
union SCI_ICR_REG		{
	UINT32 	all;
	struct 	SCI_ICR_BITS bit;
};

// SCI Synchronous Card Activation Control Register Bit Definition
struct SCI_SYNCACT_BITS	{
	UINT32  Power :1;
	UINT32  CRERst :1;
	UINT32  ClkEn :1;
	UINT32  DataEn :1;
	UINT32  FCB :1;
	UINT32  ClkoutEn :1;
	UINT32	ExClkEn	:1;
	UINT32  Clkout :1;
	UINT32  DataOutEn :1;
	UINT32  ExDataEn:1;
	UINT32  Present :1;
	UINT32  Rsvd:21;
};
union SCI_SYNCACT_REG	{
	UINT32 	all;
	struct 	SCI_SYNCACT_BITS bit;
};

// SCI Synchronous Transmit Clock and Data Register Bit Definition
struct SCI_SYNCTX_BITS	{
	UINT32  WData :1;
	UINT32  WClk :1;
	UINT32  WDataEn :1;
	UINT32  WClkEn :1;
	UINT32  WRst :1;
	UINT32  WFCB :1;
	UINT32  Rsvd:26;
};
union SCI_SYNCTX_REG	{
	UINT32 	all;
	struct 	SCI_SYNCTX_BITS bit;
};

// SCI Synchronous Receive Clock and Data register Bit Definition
struct SCI_SYNCRX_BITS {
	UINT32  RData :1;
	UINT32  RClk :1;
	UINT32  Rsvd:30;
};
union SCI_SYNCRX_REG	{
	UINT32 	all;
	struct 	SCI_SYNCRX_BITS bit;
};

//SCI regs
struct	SCI_REGS	{
	union 	SCI_DATA_REG	DATA;
	union 	SCI_CR0_REG	CR0;
	union 	SCI_CR1_REG	CR1;
	union 	SCI_CR2_REG	CR2;
	UINT32	CLKICC;
	UINT32  VALUE;
	UINT32	BAUD;
	union 	SCI_TIDE_REG	TIDE;
	UINT32 	Rsvd1;		// Reserved for DMA control
	UINT32  STABLE;
	UINT32	ATIME;
	UINT32	DTIME;
	UINT32 	ATRSTIME;
	UINT32	ATRDTIME;
	UINT32  STOPTIME;
	UINT32	STARTTIME;
	union 	SCI_RETRY_REG	RETRY;
	UINT32	CHTIMELS;
	UINT32	CHTIMEMS;
	UINT32  BLKTIMELS;
	UINT32  BLKTIMEMS;
	UINT32  CHGUARD;
	UINT32	BLKGUARD;
	UINT32  RXTIME;
	union 	SCI_FIFOSTATUS_REG 	FIFOSTATUS;
	UINT32  TXCOUNT;
    UINT32	RXCOUNT;
    union 	SCI_INT_REG			IMSC;
    union 	SCI_INT_REG   		RIS;
    union 	SCI_INT_REG   		MIS;
    union 	SCI_ICR_REG   		ICR;
    union 	SCI_SYNCACT_REG  	SYNCACT;
    union 	SCI_SYNCTX_REG    	SYNCTX;
    union 	SCI_SYNCRX_REG		SYNCRX;
};

/*
*************************** VPWM ******************************************
*/
// VPWM Timer Register Bit Definition
struct	VPWM_TIMER_BITS	{	//	bits	     description
   UINT32 	FsPeriod:16;	//	15:0      	frequency
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_TIMER_REG	{
	UINT32	all;
	struct 	VPWM_TIMER_BITS	bit;
};

// VPWM config Register Bit Definition
struct	VPWM_CFG_BITS	{   //	bits	     description
   UINT32 	En:1;    		//	0
   UINT32 	DutySelect:1;   //	1
   UINT32 	Sample_b:1;    	//	2
   UINT32 	RLshift:1;    	//	3
   UINT32 	ShiftBits:4;    //	7:4
   UINT32 	FuncSelect:1;   //	8
   UINT32 	Empty:1;        //	9
   UINT32 	Full:1;         //	10
   UINT32 	Rsvd1:21;		//	31:11
};
union	VPWM_CFG_REG	{
	UINT32 	all;
	struct 	VPWM_CFG_BITS bit;
};

// VPWM period Register Bit Definition
struct	VPWM_PR_BITS{   	//	bits	     description
   UINT32 	Period:16;    	//	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_PR_REG	{
	UINT32	all;
	struct 	VPWM_PR_BITS	bit;
};

// VPWM duty Register Bit Definition
struct	VPWM_DR_BITS{   	//	bits	     description
   UINT32 	Duty:16;      	//	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_DR_REG	{
	UINT32	all;
	struct 	VPWM_DR_BITS	bit;
};

// VPWM int Register Bit Definition
struct	VPWM_INTCTRL_BITS{   //	bits	     description
   UINT32 	NoFullEn:1;      	//	0
   UINT32 	EmptyEn:1;      	//	1
   UINT32 	NoFullInt:1;      	//	2
   UINT32 	EmptyInt:1;      	//	3
   UINT32 	Rsvd1:28;			//	31:4
};
union	VPWM_INTCTRL{
	UINT32	all;
	struct 	VPWM_INTCTRL_BITS	bit;
};

// VPWM FIFO Register Bit Definition
struct	VPWM_FIFO_BITS{   //	bits	     description
   UINT32 	FIFO:16;      //	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_FIFO_REG	{
	UINT32	all;
	struct 	VPWM_FIFO_BITS	bit;
};

// VPWM Registers
struct VPWM_REGS	{
	union   VPWM_TIMER_REG			TIMER;
	union   VPWM_CFG_REG			CFG;
	union   VPWM_PR_REG				PR;
	union   VPWM_DR_REG				DR;
	union   VPWM_INTCTRL			INTCTRL;
	union   VPWM_FIFO_REG			FIFO;
};

/*
*************************** EFC ******************************************
*/
// EFC Time Configuration Register 0
struct 	EFC_TC0_BITS	{
	UINT32	T_pgs:8;	// 7:0		NVSTR to program set up time
	UINT32	T_nvh1:8;	// 15:8		NVSTR hold time for mass erase 
	UINT32	T_nvh:8;	// 23:16	NVSTR hold time
	UINT32	T_nvs:8;	// 31:24	PROG/ERASE to NVSTR set up time
};
union	EFC_TC0_REG{
	UINT32	all;
	struct 	EFC_TC0_BITS	bit;
};

// EFC Time Configuration Register 1
struct 	EFC_TC1_BITS	{
	UINT32	T_me:8;		// 7:0		Mass erase time
	UINT32	T_erase:8;	// 15:8		Smart Erase time of each pulse
	UINT32	T_rcv:8;	// 23:16	Recovery time
	UINT32	T_prog:8;	// 31:24	Smart Program time of each pulse
};
union	EFC_TC1_REG{
	UINT32	all;
	struct 	EFC_TC1_BITS	bit;
};

// EFC Time Configuration Register 2
struct 	EFC_TC2_BITS	{
	UINT32	T_rec:4;	// 3:0 		Recall time
	UINT32	Rsvd0:8;	// 11:4	  	Reserved
	UINT32	T_rs:4;		// 15:12	Address/Control signals to RECALL setup time
	UINT32	T_pgh:4;	// 19:16  	Program hold time
	UINT32	T_adh:4;	// 23:20  	Address/data hold time
	UINT32	Rsvd1:8;	// 31:24	Reserved
};
union	EFC_TC2_REG{
	UINT32	all;
	struct 	EFC_TC2_BITS	bit;
};

// EFC Time Configuration Register 3
struct 	EFC_TC3_BITS	{
	UINT32	T_acc:8;	// 7:0 		Access time
	UINT32	T_accv:8;	// 15:8		Access time with EV=1 or PV=1
	UINT32	DoNoWait:1;	// 16		DOUT not latched
	UINT32	Rsvd0:3;	// 19:17	Reserved
	UINT32	T_ads:4;	// 23:20 	Address/data set up time
	UINT32	Rsvd1:8;	// 31:24 	Reserved
};
union	EFC_TC3_REG{
	UINT32	all;
	struct 	EFC_TC3_BITS	bit;
};

// EFC Status Register
struct 	EFC_STS_BITS	{
	UINT32	Busy:1;		// 0 		Busy
	UINT32	Buf_empty:1;// 1		buffer empty
	UINT32	Buf_ptr:4;	// 5:2	  	buffer pointer
	UINT32	Rsvd0:2;	// 7:6		Reserved
	UINT32	Test_en:1;	// 8		Test enable flag
	UINT32	Jtag_en:1;	// 9		Debug interface enable flag	
	UINT32	Key_en:1;	// 10		Key region enable flag
	UINT32	Exe_en:1;	// 11		Execute only region enable flag
	UINT32	Rsvd1:12;	// 23:12	Reserved
	UINT32	Err_num:8;	// 31:24   	Error word number when erase verify
};
union	EFC_STS_REG{
	UINT32	all;
	struct 	EFC_STS_BITS	bit;
};

// EFC Page Erase Address Register
struct 	EFC_PEA_BITS	{
	UINT32	Pe_addr:21;	// 20:0		Page erase address
	UINT32	Rev0:11;	// 31:21  	Reserved
};
union	EFC_PEA_REG{
	UINT32	all;
	struct 	EFC_PEA_BITS	bit;
};

// EFC Control Register
#define	EFC_CTRL_ENCRYPT	(1UL<<1)
#define	EFC_CTRL_BUF_RST	(1UL<<4)
#define	EFC_CTRL_HW_MR		(1UL<<23)
#define	EFC_CTRL_EV			(1UL<<24)
#define	EFC_CTRL_PV			(1UL<<25)
#define	EFC_CTRL_PDM		(1UL<<26)
#define	EFC_CTRL_SLM		(1UL<<27)
#define	EFC_CTRL_MAS1		(1UL<<29)
#define	EFC_CTRL_SMW_EN		(1UL<<30)
#define	EFC_CTRL_SMW_EVPV	(1UL<<31)
struct 	EFC_CTRL_BITS	{
	UINT32	Recall:1;	// 0		Recall	
	UINT32	Encrypt:1;	// 1 		Encrypt enable
	UINT32	Pre_Read:1;	// 2		Pre-read start bit
	UINT32	EXTC:1;		// 3		external recall for soft
	UINT32	Buf_rst:1;	// 4  		Buffer reset
	UINT32	Recall_in_PR:1;	// 5   	for soft recall
	UINT32	Biasini_in_PR:1;//6	  	for soft recall	
	UINT32	Rsvd1:1;	// 7	 	Reserved
	UINT32	IFREN1:1;	// 8		
	UINT32	PWRDY:1;	// 9
	UINT32	WMV:3;		// 12:10
	UINT32	WHV:4;		// 16:13
	UINT32 	WIPGM_th:6;	// 22:17	WIPPGM setting threshold
	UINT32	HW_MR:1;	// 23		Hardware margin	read enable
	UINT32	EV:1;		// 24  		EV
	UINT32	PV:1;		// 25	 	PV
	UINT32	PDM:1;		// 26  		PDM
	UINT32	SLM:1;		// 27  		SLM
	UINT32	Rsvd3:1;	// 28 		Reserved
	UINT32	MAS1:1;		// 29  		Mass erase enable
	UINT32	SMW_en:1;	// 30		Smart Write enable
	UINT32	SMW_EVPV:1;	// 31 		Smart EV/PV control		
};
union	EFC_CTRL_REG{
	UINT32	all;
	struct 	EFC_CTRL_BITS	bit;
};

// EFC Write Enalbe Register
struct 	EFC_WEN_BITS	{
	UINT32	We_addr:21;	// 20:0		Write address
	UINT32	Rsvd0:10;	// 30:21 	Reserved	
	UINT32	Wen:1;		// 31 		Write enable
};
union	EFC_WEN_REG{
	UINT32	all;
	struct 	EFC_WEN_BITS	bit;
};

// EFC Interrupt Enalbe/Status Register
struct 	EFC_INT_BITS	{
	UINT32	ReadErr:1;	// 0		Read error
	UINT32	WriteErr:1;	// 1		Write error
	UINT32 	EraseErr:1;	// 2		Erase error
	UINT32	RomErr:1;	// 3		ROM error	
	UINT32	DmaErr:1;	// 4		DMA MPU error
	UINT32	Rsvd0:3; 	// 7:5
	UINT32	EccErr:1; 	// 8	 	ECC error
	UINT32	Rsvd:23;	// 31:9		Reserved
};
union	EFC_INT_REG{
	UINT32	all;
	struct 	EFC_INT_BITS	bit;
};

// EFC Smart Write Control Register
struct 	EFC_SMW_BITS	{
	UINT32	N_sme:8;	// 7:0		Smart erase maximum retry times
	UINT32	N_smp:8;	// 15:8		Smart Program maximum retry times
	UINT32 	SME_Fail:1;	// 16		Smart erase result
	UINT32	SMP_Fail:1;	// 17		Smart program result
	UINT32	Use_sep_WHV:1;//18	 	Sequence for WHV/WMV
	UINT32	Rsvd:5;		// 23:19	Reserved
	UINT32	SMW_cnt:8;	// 31:24 
};
union	EFC_SMW_REG{
	UINT32	all;
	struct 	EFC_SMW_BITS	bit;
};


#define	EFC_ECC3_INFO_EN		(1UL<<31)
#define	EFC_ECC3_MAIN_TOP_EN	(1UL<<30)
#define	EFC_ECC3_MAIN_LOW_EN	(1UL<<29)
// EFC ECC Register 3
struct 	EFC_ECC3_BITS	{
	UINT32	Rsvd:29;		// 28:0
	UINT32	EccMainLowEn:1;	// 29
	UINT32	EccMainTopEn:1;	// 30
	UINT32	EccInfoEn:1;	// 31	
};
union	EFC_ECC3_REG{
	UINT32	all;
	struct 	EFC_ECC3_BITS	bit;
};

// EFC Registers
struct EFC_REGS	{
	
	union	EFC_TC0_REG		TC0;	//0x00
	union	EFC_TC1_REG		TC1;	//0x04
	union	EFC_TC2_REG		TC2;	//0x08
	union	EFC_TC3_REG		TC3;	//0x0C
	union	EFC_STS_REG		STS;	//0x10
	union	EFC_PEA_REG		PEA;	//0x14
	union	EFC_CTRL_REG	CTRL;	//0x18
	union	EFC_WEN_REG		WEN;	//0x1C
	UINT32	DATA;				//0x20 Data area start address
	UINT32	KEY0;	   			//0x24 
	UINT32  KEY1;      			//0x28	
	UINT32  CHK0;    				//0x2C//check accessing code 
	UINT32  CHK1;  				//0x30  //check accessing code 
	union   EFC_INT_REG  INTEN;	  		//0x34
	union   EFC_INT_REG  INT;	  		//0x38
	union	EFC_SMW_REG  SMW;	  		//0x3C
	UINT32  EVRF;	  		// 0x40
	UINT32	ECC0;			// 0x44
	UINT32	ECC1;			// 0x48
	UINT32	ECC2;			// 0x4C
	UINT32	RSVD1[2];		// 0x50 0x54
	UINT32	PREAD;			// 0x58
	UINT32	RSVD2[2];		// 0x5C 0x60
	union	EFC_ECC3_REG	ECC3; 	// 0x64
	UINT32	WHV_ARRAY;		// 0x68
	UINT32	WMV_ARRAY; 		// 0x6C
};

/*
*************************** SQI ******************************************
*/
//--------------- Define Bits for Registers -----------------------------
// define bits of INST or INST_READ
#define	MODE_INST			0
#define MODE_ADDR			2
#define MODE_DUMMY			4
#define	MODE_WAIT			6
#define	MODE_DATA			8
#define NUM_ADDR			12
#define	NUM_DUMMY			16
#define NUM_WAIT			20
#define	NUM_INST			23
#define INST_DATA			24

// define bits of INST_SET
#define	START				0
#define	EN_SPEC				4
#define	EN_CHANNEL			8
#define	EN_ENCRYPT			12
#define CS_LOW				13

// define bits of SPEC
#define	REG2				0
#define	REG1				8

// define bits of CLKGEN
#define	CLK_DIV				0
#define	EN_DIV				7
// SQI INST Bits Definition
struct	SQI_INST_BITS{     //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_REG{
	UINT32	all;
	struct	SQI_INST_BITS	bit;
};

// SQI INST_SET Bits Definition
struct	SQI_INST_SET_BITS{//bits	 description
	UINT32	Start  :1;     //0       start output flag
    UINT32	Rsvd1  :3;     //3:1     Reserved
    UINT32	Spec   :1;     //4       spec flag
    UINT32	Rsvd2  :3;     //7:5     Reserved
    UINT32	Channel:1;     //8       control the channel data flag
    UINT32  Rsvd3  :3;     //11:9    Reserved
    UINT32  Encrypt:1;     //12      encrypt flash data
    UINT32  CsLow  :1;     //13      Hold the chip select
	UINT32	BurstEn:1;	   //14
    UINT32	Rsvd4  :17;    //31:15   Reserved
};
union	SQI_INST_SET_REG{
	UINT32	all;
	struct	SQI_INST_SET_BITS	bit;
};

// SQI SPEC Bits Definition
struct	SQI_SPEC_BITS{     //bits	 description
	UINT32 	Addr      :8;   //7:0
    UINT32 	Reg2      :8;   //15:8
    UINT32 	Reg1      :8;   //23:16
    UINT32 	Rsvd      :8;   //31:24    Reserved
};
union	SQI_SPEC_REG{
	UINT32	all;
	struct	SQI_SPEC_BITS	bit;
};

// SQI CLKGEN Bits Definition
struct	SQI_CLKGEN_BITS{   //bits	 description
	UINT32 	ClkDiv  :7;     //6:0     spi CLOCK div
    UINT32 	Divide  :1;     //7
    UINT32 	Rsvd    :24;    //31:9    Reserved
};
union	SQI_CLKGEN_REG{
	UINT32	all;
	struct	SQI_CLKGEN_BITS	bit;
};

// SQI DATA_ADDR Bits Definition
struct	SQI_DATA_ADDR_BITS{   //bits	 description
	UINT32 	IdData  :8;        //7:0      read flash ID
    UINT32 	Rsvd    :24;       //31:8     Reserved
};
union	SQI_DATA_ADDR_REG{
	UINT32	all;
	struct	SQI_DATA_ADDR_BITS	bit;
};

// SQI INST_READ Bits Definition
struct	SQI_INST_READ_BITS{   //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_READ_REG{
	UINT32	all;
	struct	SQI_INST_READ_BITS  bit;
};

// SQI registers
struct	SQI_REGS{
	union 	SQI_INST_REG        INST;
	union 	SQI_INST_SET_REG    INST_SET;
	union 	SQI_SPEC_REG        SPEC;
	union 	SQI_CLKGEN_REG      CLKGEN;
	union 	SQI_DATA_ADDR_REG   DATA_ADDR;
	union 	SQI_INST_READ_REG   INST_READ;
	UINT32 	ENCRYPT;
	UINT32 	CHECK;
	UINT32 	DATAPRO;
	UINT32 	DATAOPEN;
};

/*
*************************** CRC ******************************************
*/
// CRC control register Bit Definitions
struct	CRC_CTRL_BITS	{//	bits	description
	UINT32	ArithSel:1;		//  0		Algorithm Selection
	UINT32	DatRev:1;		//	1		Input data reverse
	UINT32	ResRev:1;		//	2		Output Descending
	UINT32	Rsvd1:29;		//	31:3	Reserved
};
union	CRC_CTRL_REG{
	UINT32	all;
	struct	CRC_CTRL_BITS	bit;
};

// USART Registers
struct	CRC_REGS	{
	union 	CRC_CTRL_REG	CTRL;		//CRC control register					
	UINT32	DATA;			//CRC data registers
	UINT32	INITVAL;		//CRC initial value register			
	UINT32	RESULT;			//CRC results output register
};

///*
//*************************** TRNG ******************************************
//*/

/*
*************************** DMA ******************************************
*/
// define DMA Channels
#define	 DMA_CHN0 	0
#define	 DMA_CHN1	1
#define	 DMA_CHN2	2
#define	 DMA_CHN3	3

#define	 MASK_DMA_CHN0 			(1<<DMA_CHN0)
#define	 MASK_DMA_CHN1 			(1<<DMA_CHN1)
#define	 MASK_DMA_CHN2 			(1<<DMA_CHN2)
#define	 MASK_DMA_CHN3			(1<<DMA_CHN3)

#define	 DMA_CHANNEL0_INT 		(1<<0)
#define	 DMA_CHANNEL1_INT 		(1<<1)
#define	 DMA_CHANNEL2_INT 		(1<<2)
#define	 DMA_CHANNEL3_INT		(1<<3)
#define	 DMA_CHANNEL_INT_ALL		0xF
// Channel Registers
// DMA Control Register for Channel x Bits Definition
// CHANNEL REGISTER
struct	DMA_CHN_LLP_REG_BITS	{		//LLP REGISTER
	UINT32	Lms: 2;
	UINT32	Loc: 30;
};
union 	DMA_CHN_LLP_REG	{
	UINT32	all;
	struct DMA_CHN_LLP_REG_BITS	bit;
};

// DMA Control Register for Channel x Bits Definition
#define	DMA_INT_EN			0
#define	DMA_DST_TR_WIDTH	1
#define	DMA_SRC_TR_WIDTH	4
#define	DMA_DINC			7
#define	DMA_SINC			9
#define	DMA_DEST_MSIZE		11
#define	DMA_SRC_MSIZE		14
#define	DMA_SRC_GATHER_EN	17
#define	DMA_DST_SCATTER_EN	18
#define	DMA_TT_FC			20
#define	DMA_DMS				23
#define	DMA_SMS				25
#define	DMA_LLP_DST_EN		27
#define	DMA_LLP_SRC_EN		28

struct	DMA_CHN_CTL_REG_L_BITS	{	// bits		description
	UINT32	IntEn: 1;			   	//	0		Interrupt Enable Bit		
	UINT32	DstTrWidth:	3; 			// 	3:1		Destination Transfer Width
	UINT32	SrcTrWidth:	3; 	 		//	6:4		Source Transfer Width
	UINT32	Dinc:	2; 	   			//	8:7		Destination Address Increment
	UINT32	Sinc:	2; 			   	//	10:9	Source Address Increment
	UINT32 	DestMsize:	3;	  		//	13:11	Destination Burst Transfer Length
	UINT32	SrcMsize:	3; 		  	//	16:14	Source Burst Transfer Length
	UINT32	SrcGatherEn:	1; 		//	17		Source Gather Enable Bit
	UINT32	DstScatterEn:	1; 	 	// 	18		Destination Scatter Enable Bit
	UINT32	Rsvd0:	1; 			  	//	19		Reserved
	UINT32	TtFc:	3;				//	22:20	Transfer Type and Flow Control
	UINT32	Dms:	2;				//	24:23	Destination Master Select
	UINT32	Sms:	2; 				//	26:25	Source Master Select
	UINT32	LlpDstEn:	1;	 		//	27		block chaining is enable on the destination side
	UINT32	LlpSrcEn:	1;			//	28		block chaining is enable on the source side
	UINT32	Rsvd1:	3; 				// 	31:29	Reserved
};

union 	DMA_CHN_CTL_REG_L	{
	UINT32	all;
	struct	DMA_CHN_CTL_REG_L_BITS	bit;
};

#define	DMA_BLOCK_TS		0
#define	DMA_DONE			1
struct	DMA_CHN_CTL_REG_H_BITS	{	// bits		description
	UINT32	BlockTs:12;		   		//	11:0	Block Transfer Size
	UINT32	Done:	1;	   			//	12		Done bit
	UINT32	Rsvd0:	19;	  			//	31:13	Reserved
};
union 	DMA_CHN_CTL_REG_H	{
	UINT32	all;
	struct	DMA_CHN_CTL_REG_H_BITS	bit;
};

// DMA Configuration Register for Channel x Bits Definition
#define	DMA_CH_PRIOR		5
#define	DMA_CH_SUSP			8
#define	DMA_FIFO_EMPTY		9
#define	DMA_HS_SEL_DST		10
#define	DMA_HS_SEL_SRC		11
#define	DMA_LOCK_CH_L		12
#define	DMA_LOCK_B_L		14
#define	DMA_LOCK_CH			16
#define	DMA_LOCK_B			17
#define	DMA_DST_HS_POL		18
#define	DMA_SRC_HS_POL		19
#define	DMA_MAX_ABRST		20
#define	DMA_RELOAD_SRC		30
#define	DMA_RELOAD_DST		31
struct	DMA_CHN_CFG_REG_L_BITS{	// bits		description
	UINT32	Rsvd:		5; 		//	4:0		Reserved
	UINT32	ChPrior:	3;	   	//	7:5		Channel priority
	UINT32	ChSusp:	 	1;		//	8		Channel Suspend
	UINT32	FifoEmpty:	1;	 	//	9		Indicates if there is data left in the channel FIFO
	UINT32	HsSelDst:	1;		//	10		Destination Software or Hardware Handshaking Select
	UINT32	HsSelSrc:	1;	 	//	11		Source Software or Hardware Handshaking Select
	UINT32	LockChL:	2; 		//	13:12	Channel Lock Level
	UINT32	LockBL:		2;	 	//	15:14	Bus Lock Level
	UINT32	LockCh:		1;	 	//	16		Channel Lock Bit
	UINT32	LockB:		1;	   	//	17		Bus Lock Bit
	UINT32	DstHsPol:	1; 		//	18		Destination Handshaking Interface Polarity
	UINT32	SrcHsPol:	1;	   	//	19		Source Handshaking Interface Polarity
	UINT32	MaxAbrst:	10;	 	//	29:20	Maximum AMBA Burst Length
	UINT32	ReloadSrc:	1;		// 	30		Automatic Source Reload
	UINT32	ReloadDst:	1;		//	31		Automatic Destination Reload	
};
union 	DMA_CHN_CFG_REG_L	{
	UINT32	all;
	struct	DMA_CHN_CFG_REG_L_BITS	bit;
};

#define	DMA_FCMODE			0
#define	DMA_FIFO_MODE		1
#define	DMA_PROTCTL			2
#define	DMA_DS_UPD_EN		5
#define	DMA_SS_UPD_EN		6
#define	DMA_SRC_PER			7
#define	DMA_DEST_PER		11
struct	DMA_CHN_CFG_REG_H_BITS{	// bits		description
	UINT32	Fcmode:		1;	 	//	0		Flow Control Mode
	UINT32	FifoMode:	1;	  	//	1		FFIO Mode Select
	UINT32	ProtCtl:	3; 		//	4:2		Protection Control
	UINT32	DsUpdEn:	1;	 	//	5		Destination Status Update Enable
	UINT32	SsUpdEn:	1; 		//	6		Source Status Update Enable
	UINT32	SrcPer:		4;		//	10:7	Assigns a hardware handshaking interface to the soure
	UINT32	DestPer:	4;		//	14:11	Assigns a hardware handshaking interface to the destination
	UINT32	Rsvd:	    17;		//	31:15	Reserved	
};
union 	DMA_CHN_CFG_REG_H	{
	UINT32	all;
	struct	DMA_CHN_CFG_REG_H_BITS	bit;
};

// DMA Source Gather Register for Channel x Bits Definition
struct	DMA_CHN_SGR_REG_BITS{	// bits		description
	UINT32	Sgi:	20;	 		// 	19:0	Source gather interval
	UINT32	Sgc:	12;	  		// 	31:20	Source gather count
};
union 	DMA_CHN_SGR_REG	{
	UINT32	all;
	struct	DMA_CHN_SGR_REG_BITS	bit;
};

// DMA Destination Scatter Register for Channel x Bits Definition
struct	DMA_CHN_DST_REG_BITS{	// bits		description
	UINT32	Dsi:	20;			// 	19:0	Destination scatter interval
	UINT32	Dsc:	12;	  		// 	31:20	Destination scatter count
};
union 	DMA_CHN_DST_REG	{
	UINT32	all;
	struct	DMA_CHN_DST_REG_BITS	bit;
};

// DMA Channel Registers struct
struct DMA_CH_REG {
	UINT32	SAR;
	UINT32	RSVD0;
	UINT32	DAR;
	UINT32	RSVD1;
	union 	DMA_CHN_LLP_REG  LLP;
	UINT32	RSVD2;
	union 	DMA_CHN_CTL_REG_L  CTL_L;
	union 	DMA_CHN_CTL_REG_H  CTL_H;
	UINT32	SSTAT;
	UINT32	RSVD3;
	UINT32	DSTAT;
	UINT32	RSVD4;
	UINT32	SSTATAR;
	UINT32	RSVD5;
	UINT32	DSTATAR;
	UINT32	RSVD6;
	union 	DMA_CHN_CFG_REG_L	CFG_L;
	union 	DMA_CHN_CFG_REG_H	CFG_H;	
	union	DMA_CHN_SGR_REG SGR;
	UINT32	RSVD7;
	union	DMA_CHN_DST_REG DSR;
	UINT32	RSVD8;
};

// Interrupt Registers
// DMA Raw Status / Status / Clear Register Bits Definition
struct DMA_INT_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Revd:28;
};
union DMA_INT_REG {
	UINT32	all;
	struct	DMA_INT_REG_BITS	bit;
};

// DMA Mask Register Bits Definition
struct DMA_CHN_INT_MASK_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;	
};
union DMA_CHN_INT_MASK_REG {
	UINT32	all;
	struct	DMA_CHN_INT_MASK_REG_BITS	bit;
};

// DMA Combined Interrupt Status Register Bits Definition
struct DMA_COMB_INT_STATUS_BITS{	// bits		description
	UINT32	Tfr:	1; 						
	UINT32	Block:	1;
	UINT32	Srct:	1;
	UINT32	Dstt:	1;
	UINT32	Err:	1;
	UINT32	Rsvd:	27;
};
union DMA_COMB_INT_STATUS_REG {
	UINT32	all;
	struct	DMA_COMB_INT_STATUS_BITS	bit;
};

// Software Handshaking Registers
// DMA Source Software Transaction Register Bits Definition
struct DMA_REQ_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;	
};
union DMA_REQ_REG {
	UINT32	all;
	struct	DMA_REQ_REG_BITS	bit;
};

// Miscellaneous Registers
// DMA Configuration Register Bits Definition
struct DMA_CFG_REG_BITS{		  
	UINT32	DmaEn:	1;
	UINT32	Rsvd:	31;
};
union DMA_CFG_REG {
	UINT32	all;
	struct	DMA_CFG_REG_BITS	bit;
};

// DMA Enable Register Bits Definition
struct DMA_CHN_EN_REG_BITS{	  
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;
};
union DMA_CH_EN_REG {
	UINT32	all;
	struct	DMA_CHN_EN_REG_BITS bit;
};


// DMA Component Parameters Register Bits Definition
struct DMA_CH_PARAMS_REG_BITS{	  
	UINT32	Dtw:3;
	UINT32	Stw:3;
	UINT32	StatDst:1;
	UINT32	StatSrc:1;
	UINT32	DstScaEn:1;
	UINT32	SrcGatEn:1;
	UINT32	LockEn:	1;
	UINT32	MultiBlkEn:1;
	UINT32	CtlWbEn:1;
	UINT32	HcLlp:1;
	UINT32	Fc:2;
	UINT32	MaxMultSize:3;
	UINT32	Dms:3;
	UINT32	Lms:3;
	UINT32	Sms:3;
	UINT32	FifoDepth:3;
	UINT32	Rsvd:1;
};
union DMA_CH_PARAMS_REG {
	UINT32	all;
	struct	DMA_CH_PARAMS_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MULTI_BLK_TYPE_REG_BITS{	  
	UINT32	Ch0:4;
	UINT32	Ch1:4;
	UINT32	Ch2:4;
	UINT32	Ch3:4;
	UINT32	Ch4:4;
	UINT32	Ch5:4;
	UINT32	Ch6:4;
	UINT32	Ch7:4;
};
union DMA_MULTI_BLK_TYPE_REG {
	UINT32	all;
	struct	DMA_MULTI_BLK_TYPE_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MAX_BLK_SIZE_REG_BITS{	  
	UINT32	Ch0:4;
	UINT32	Ch1:4;
	UINT32	Ch2:4;
	UINT32	Ch3:4;
	UINT32	Ch4:4;
	UINT32	Ch5:4;
	UINT32	Ch6:4;
	UINT32	Ch7:4;
};
union DMA_MAX_BLK_SIZE_REG {
	UINT32	all;
	struct	DMA_MAX_BLK_SIZE_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MISC_PARAMS_REG_BITS{	  
	UINT32	BigEndian:	1;
	UINT32	IntrIo:	2;
	UINT32	Mabrst:	1;
	UINT32	Rsvd0:	4;
	UINT32	NumChannels:	3;
	UINT32	NumMasterInt:	2;
	UINT32	ShdataWidth:	2;
	UINT32	M1HdataWidth:	2;
	UINT32	M2HdataWidth:	2;
	UINT32	M3HdataWidth:	2;
	UINT32	M4HdataWidth:	2;
	UINT32	NumHsInt:	5;
	UINT32	AddEnCodedParams:	1;
	UINT32	StaticEndianSelect:	1;
	UINT32	Rsvd1:	2;
};
union DMA_MISC_PARAMS_REG {
	UINT32	all;
	struct	DMA_MISC_PARAMS_REG_BITS bit;
};

// DMA Registers 
struct	DMA_REGS{
	//1.Channel Register
	struct  DMA_CH_REG CH[4];						//0x000-0x158
	struct  DMA_CH_REG CH_RSVD[4];					//0x160-0x2b8

   	//2.Interrupt Register
	union	DMA_INT_REG	  RAW_TFR;	   				//0x2c0
	UINT32	RSVD1;
	union	DMA_INT_REG	  RAW_BLOCK;				//0x2c8
	UINT32	RSVD2;
	union	DMA_INT_REG   RAW_SRC_TRAN;				//0x2d0
	UINT32	RSVD3;
	union	DMA_INT_REG   RAW_DST_TRAN;				//0x2d8
	UINT32	RSVD4;
	union	DMA_INT_REG	  RAW_ERR;					//0x2e0
	UINT32	RSVD5;

	union	DMA_INT_REG	  STATUS_TFR;				//0x2e8
	UINT32	RSVD6;
	union	DMA_INT_REG	  STATUS_BLOCK;   			//0x2f0
	UINT32	RSVD7;
	union	DMA_INT_REG	  STATUS_SRC_TRAN; 			//0x2f8
	UINT32	RSVD8;
	union	DMA_INT_REG	  STATUS_DST_TRAN; 			//0x300
	UINT32	RSVD9;
	union	DMA_INT_REG	  STSTUS_ERR;	   			//0x308
	UINT32	RSVD10;

	union	DMA_CHN_INT_MASK_REG	MASK_TFR;		//0x310
	UINT32	RSVD11;
	union	DMA_CHN_INT_MASK_REG	MASK_BLOCK;	   	//0x318
	UINT32	RSVD12;
	union	DMA_CHN_INT_MASK_REG	MASK_SRC_TRAN; 	//0x320
	UINT32	RSVD13;
	union	DMA_CHN_INT_MASK_REG	MASK_DST_TRAN;	//0x328
	UINT32	RSVD14;
	union	DMA_CHN_INT_MASK_REG	MASK_ERR;		//0x330
	UINT32	RSVD15;

	union	DMA_INT_REG 	CLEAR_TFR;	   			//0x338
	UINT32	RSVD16;
	union	DMA_INT_REG 	CLEAR_BLOCK;			//0x340
	UINT32	RSVD17;
	union	DMA_INT_REG 	CLEAR_SRC_TRAN; 		//0x348
	UINT32	RSVD18;
	union	DMA_INT_REG 	CLEAR_DST_TRAN; 		//0x350
	UINT32	RSVD19;
	union	DMA_INT_REG 	CLEAR_ERR;	   			//0x358
	UINT32	RSVD20;

	union	DMA_COMB_INT_STATUS_REG	INT_STATUS;	  	//0x360
	UINT32	RSVD21;

	//3.Software Handshaking Registers
	union	DMA_REQ_REG  	REQ_SRC_REG;	 		//0x368
	UINT32	RSVD22;
	union	DMA_REQ_REG 	REQ_DST_REG;	 		//0x370
	UINT32	RSVD23;
	union	DMA_REQ_REG 	SGL_REQ_SRC_REG; 		//0x378
	UINT32	RSVD24;
	union	DMA_REQ_REG 	SGL_REQ_DST_REG; 		//0x380
	UINT32	RSVD25;
	union	DMA_REQ_REG 	LST_SRC_REG;	 		//0x388
	UINT32	RSVD26;
	union	DMA_REQ_REG 	LST_DST_REG;	 		//0x390
	UINT32	RSVD27;

	//4.Miscellaneous Registers
	union	DMA_CFG_REG		CFG_REG;	  //0x398
	UINT32	RSVD28;
	union	DMA_CH_EN_REG	CH_EN_REG;	  //0x3a0
	UINT32	RSVD29;
	UINT32	ID_REG;	  	//0x3a8
	UINT32	RSVD30;
	UINT32	TEST_REG;	//0x3b0
	UINT32	RSVD31;
	UINT32	RSVD32[4];

  // Parameters Register
  	UINT32	RSVD33;	  	// 0x3c8
	union 	DMA_CH_PARAMS_REG		PARAMS_CH7;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH6;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH5;	
	union 	DMA_CH_PARAMS_REG		PARAMS_CH4;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH3;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH2;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH1;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH0;
	union 	DMA_MULTI_BLK_TYPE_REG	PARAMS_MULTI_BLK_TYPE;
	union 	DMA_MAX_BLK_SIZE_REG	MAX_BLK_SIZE;
	union 	DMA_MISC_PARAMS_REG		MISC_PARAMS;
};


/*
*************************** SDSI ******************************************
*/
#define	SDSI_BUFF_BASE_ADDR				0x20008000	// the SDSI read/write buffer base address
#define	SDSI_BUFF_BANK_SIZE				512		
#define	SDSI_BUFF_BANK_ADDR(BANK_NUM)	(SDSI_BUFF_BASE_ADDR + SDSI_BUFF_BANK_SIZE*BANK_NUM)
#define	SDSI_BUFF_BANK(BANK_NUM)		(void *)SDSI_BUFF_BANK_ADDR(BANK_NUM)

#define	SDSI_ADMA2_DESC_TBL_ADDR		0x2000CC00	// the SDSI ADMA2 descriptor table

// Define the ADMA2 Descriptor Table Attibute field
#define	SDSI_ADMA2_DT_VALID_BIT		0
#define	SDSI_ADMA2_DT_VALID			(1<<SDSI_ADMA2_DT_VALID_BIT)

#define	SDSI_ADMA2_DT_END_BIT		1
#define	SDSI_ADMA2_DT_END			(1<<SDSI_ADMA2_DT_END_BIT)

#define	SDSI_ADMA2_DT_INT_BIT		2
#define	SDSI_ADMA2_DT_INT			(1<<SDSI_ADMA2_DT_INT_BIT)

#define	SDSI_ADMA2_DT_ACT_BITS		4
#define	SDSI_ADMA2_DT_ACT_NOP		(0<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_RSV		(1<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_TRAN		(2<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_LINK		(3<<SDSI_ADMA2_DT_ACT_BITS)

// Define the SD SDIO interrupt bit
#define	SDSI_SD_SDIO_INT_SOFT_RST				(1<<0)
#define	SDSI_SD_SDIO_INT_VOLT_SWITCH_CMD		(1<<1)
#define	SDSI_SD_SDIO_INT_CMD19_RD_STRT			(1<<2)
#define	SDSI_SD_SDIO_INT_CMD19_RD_TRANS_OVER	(1<<3)
#define	SDSI_SD_SDIO_INT_FN0_WR_START			(1<<4)
#define	SDSI_SD_SDIO_INT_FN0_WR_TRN_OVER		(1<<5)
#define	SDSI_SD_SDIO_INT_FN0_RD_START			(1<<6)
#define	SDSI_SD_SDIO_INT_FN0_RD_TRN_OVER		(1<<7)
#define	SDSI_SD_SDIO_INT_FN0_RD_TRN_ERR			(1<<8)
#define	SDSI_SD_SDIO_INT_FN0_ADMA_END			(1<<9)
#define	SDSI_SD_SDIO_INT_FN0_ADMA				(1<<10)
#define	SDSI_SD_SDIO_INT_FN0_ADMA_ERR			(1<<11)
#define	SDSI_SD_SDIO_INT_ALL					(0x0FFF)

// Define the Mem interrupt bit
#define	SDSI_MEM_INT_PASSWORD_UPDATE			(1<<0)
#define	SDSI_MEM_INT_ERASE						(1<<1)
#define	SDSI_MEM_INT_PRE_ERASE					(1<<2)
#define	SDSI_MEM_INT_SD_RD_START				(1<<3)
#define	SDSI_MEM_INT_SD_WR_START				(1<<4)
#define	SDSI_MEM_INT_WR_OVER					(1<<5)
#define	SDSI_MEM_INT_RD_OVER					(1<<6)
#define	SDSI_MEM_INT_SEC_ERASE_PULSE			(1<<7)
#define	SDSI_MEM_INT_CHGE_SEC_WR_PULSE			(1<<8)
#define	SDSI_MEM_INT_ADMA_END					(1<<9)
#define	SDSI_MEM_INT_UPDATE_CSD					(1<<10)
#define	SDSI_MEM_INT_CMD20						(1<<11)
#define	SDSI_MEM_INT_ADMA						(1<<12)
#define	SDSI_MEM_INT_ADMA_ERR					(1<<13)
#define	SDSI_MEM_INT_CMD6_SWITCH				(1<<14)
#define	SDSI_MEM_INT_XPC						(1<<15)
#define	SDSI_MEM_INT_ALL						(0xFFFF)

// Define the Fn1 interrupt bit
#define	SDSI_FN1_INT_WR_OVER					(1<<0)
#define	SDSI_FN1_INT_RD_OVER					(1<<1)
#define	SDSI_FN1_INT_RD_ERR						(1<<2)
#define	SDSI_FN1_INT_RST						(1<<3)
#define	SDSI_FN1_INT_SD_HOST_FN1_MSG_RDY		(1<<4)
#define	SDSI_FN1_INT_ACK_TO_ARM					(1<<5)
#define	SDSI_FN1_INT_SDIO_RD_START				(1<<6)
#define	SDSI_FN1_INT_SDIO_WR_START				(1<<7)
#define	SDSI_FN1_INT_ADMA_END					(1<<8)
#define	SDSI_FN1_INT_SUSPEND					(1<<9)
#define	SDSI_FN1_INT_RESUME						(1<<10)
#define	SDSI_FN1_INT_ADMA						(1<<11)
#define	SDSI_FN1_INT_ADMA_ERR					(1<<12)
#define	SDSI_FN1_INT_FN1_EN						(1<<13)
#define	SDSI_FN1_INT_ALL						(0x3FFF)

// Define the Card Type
#define	MEM_CARD	1
#define	IO_CARD		2
#define	COMBO_CARD	3
//Clock Wake-up Definition
struct	SDSI_CLK_WAKEUP_BITS {
	UINT32	AutoClkEn:1;  		//	0		
	UINT32	ManualClkEn:1;
	UINT32	Rsvd1:6;
	UINT32	Rsvd2:24;
};
union	SDSI_CLK_WAKEUP_REG {
	UINT32	all;
	struct SDSI_CLK_WAKEUP_BITS bit;
};

//ESW CCCR Reg Definition
struct SDSI_ESW_CCCR_BITS {
	UINT32 CCCRRev:4;
	UINT32 SDIORev:4;
	UINT32 SDRev:4;
	UINT32 SCSI:1;
	UINT32 SDC:1;
	UINT32 SMB:1;
	UINT32 SRW:1;
	UINT32 SBS:1;
	UINT32 S4MI:1;
	UINT32 LSC:1;
	UINT32 BLS:1;
	UINT32 SMPC:1;
	UINT32 SHS:1;
	UINT32 MemPre:2;
	UINT32 CmdIndex:6;
	UINT32 Rsvd:2;
};
union 	SDSI_ESW_CCCR_REG {
	UINT32 all;
	struct SDSI_ESW_CCCR_BITS bit;
};

// ESW Card Ready
struct 	SDSI_ESW_CARD_READY_BITS {
	UINT32 	CardRdy:1;
	UINT32  Rsvd:31;
};
union SDSI_ESW_CARD_READY_REG {
	UINT32 all;
	struct SDSI_ESW_CARD_READY_BITS bit;
};

// ESW Fun Ready
struct SDSI_ESW_FUN_READY_BITS {
	UINT32 FunRdy:1;
	UINT32 Rsvd:31;	
};
union SDSI_ESW_FUN_READY_REG {
	UINT32 all;
	struct SDSI_ESW_FUN_READY_BITS  bit;
};

// SD SDIO Interrupt Enable/Status Register
struct SDSI_SD_SDIO_INT_BITS {
	UINT32 SoftRst:1;      
	UINT32 VoltSwitchCmd:1;
	UINT32 Cmd19RdStrt:1;
	UINT32 Cmd19RdTransOver:1;
	UINT32 Fn0WrStart:1;
	UINT32 Fn0WrTrnOver:1;
	UINT32 Fn0RdStart:1;
	UINT32 Fn0RdTrnOver:1;
	UINT32 Fn0RdTrnErr:1;
	UINT32 Fn0AdmaEnd:1;
	UINT32 Fn0Adma:1;
	UINT32 Fn0AdmaErr:1;	
	UINT32 Rsvd:20;
};
union SDSI_SD_SDIO_INT_REG {
	UINT32 all;
	struct SDSI_SD_SDIO_INT_BITS bit;
};


// Soft reset valid
struct SDSI_SOFT_RESET_VALID_BITS {
	UINT32 	ahbsoft_valid:1;
	UINT32  Rsvd:31;	
};
union SDSI_SOFT_RESET_VALID_REG {
	UINT32 all;
	struct SDSI_SOFT_RESET_VALID_BITS bit;
};

// AHB MEM Interrupt Enable/Status Register
struct SDSI_MEM_INT_BITS {
	UINT32 PasswordUpdate:1;	  
	UINT32 Erase:1;
	UINT32 PreErase:1;
	UINT32 SdRdStart:1;
	UINT32 SdWrStart:1;      
	UINT32 MemWriteOver:1;
	UINT32 MemReadOver:1;
	UINT32 SecErasePulse:1;
	UINT32 ChgeSecWrPulse:1;
	UINT32 AdmaEnd:1;
	UINT32 UpdateCSD:1;
	UINT32 Cmd20:1;
	UINT32 Adma:1;
	UINT32 AdmaErr:1;
	UINT32 Cmd6Switch:1;
	UINT32 Xpc:1;		   
	UINT32 Rsvd:16;
};
union SDSI_MEM_INT_REG {
	UINT32 all;
	struct SDSI_MEM_INT_BITS bit;
};

//Global Interrupt Status/Enable Register
struct SDSI_GLOBAL_INT_BITS {
	UINT32 IntFrmFn0:1;	  
	UINT32 IntFrmMem:1;
	UINT32 IntFrmFn1:1;
	UINT32 Rsvd:29;
};
union SDSI_GLOBAL_INT_REG {
	UINT32 all;
	struct SDSI_GLOBAL_INT_BITS bit;
};

// IO Access mode	
struct SDSI_IO_ACCESS_MODE_BITS {
	UINT32 SSDR50:1;	  
	UINT32 SSDR104:1;
	UINT32 SDDR50:1;
	UINT32 SDTA:1;
	UINT32 SDTC:1;
	UINT32 SDTD:1;
	UINT32 SAI:1;		  
	UINT32 Rsvd:25;
};
union SDSI_IO_ACCESS_MODE_REG {
	UINT32 all;
	struct SDSI_IO_ACCESS_MODE_BITS bit;
};

// UHS Support
struct SDSI_UHS_SUPPORT_BITS {
	UINT32 UHSSupport:1;	  
	UINT32 DDRDlySelect:1;
	UINT32 CardVoltAccepted:1;
	UINT32 SdClkLineSwitched:1;
	UINT32 SdCmdLineSwitched:1;
	UINT32 Rsvd:27;
};
union SDSI_UHS_SUPPORT_REG {
	UINT32 all;
	struct SDSI_UHS_SUPPORT_BITS bit;
};


// Power control	   0x40
struct SDSI_PWR_CTRL_BITS {
	UINT32 EMPC:1;	  //BIT0
	UINT32 EPS:1;
	UINT32 Rsvd:30;
};
union SDSI_PWR_CTRL_REG {
	UINT32 all;
	struct SDSI_PWR_CTRL_BITS bit;
};

// Power state	 0x44
struct SDSI_PWR_STATE_BITS {
	UINT32 PwrStateFun1:1;	  //BIT0
	UINT32 Rsvd:31;
};
union SDSI_PWR_STATE_REG {
	UINT32 all;
	struct SDSI_PWR_STATE_BITS bit;
};
//Reserved	0x048 - 0x0FF

///***************************************************************************
//* Function1 Reg Global Type Definition
//* USE ARM Access Offset ESW Base Address + Offset
//****************************************************************************/

//AHB Interrupt Enable/Status Register (0x 118)
struct SDSI_AHB_INT_BITS {
	UINT32 Fn1WrOver:1;
	UINT32 Fn1RdOver:1;
	UINT32 Fn1RdErr:1;
	UINT32 Fun1Rst:1;
	UINT32 SdHostFn1MsgRdy:1;		  
	UINT32 Fn1AckToARM:1;
	UINT32 Fn1SdioRdStart:1;		  
	UINT32 Fn1SdioWrStart:1;
	UINT32 AdmaEnd:1;		  
	UINT32 Fn1Suspend:1;
	UINT32 Resume:1;		  
	UINT32 Adma:1;
	UINT32 AdmaErr:1;
	UINT32 Fun1En:1;		  
	UINT32 Rsvd:18;
};
union SDSI_AHB_INT_REG {
	UINT32 all;
	struct SDSI_AHB_INT_BITS bit;
};

//ESW FBR1 Register (0x 11c)
struct SDSI_ESW_FBR1_BITS {
	UINT32 IODeviceCode1:4;
	UINT32 FunCsaSup:1;
	UINT32 ExtendedIODeviceCode1:8;
	UINT32 SPS:1;		  
	UINT32 Rsvd:18;
};
union SDSI_ESW_FBR1_REG {
	UINT32 all;
	struct SDSI_ESW_FBR1_BITS bit;
};

//Reserved 0x130-0x13F

///***************************************************************************
//* SD Memory Register Address Mapping Global Type Definition
//****************************************************************************/
//
// MEM OCR Register (0x 200)
struct SDSI_MEM_OCR_BITS {
	UINT32 MemOCR:24;
	UINT32 Rsvd:6;	
	UINT32 CCS:1;
	UINT32 CardEccDisabled:1;
};
union SDSI_MEM_OCR_REG {
	UINT32 all;
	struct SDSI_MEM_OCR_BITS bit;
};

//Block size register (0x 244)
struct SDSI_BLOCK_SIZE_BITS {
	UINT32 BlockSize:12;
	UINT32 SinMulBlk:1;
	UINT32 Rsvd:19;	
};
union SDSI_BLOCK_SIZE_REG {
	UINT32 all;
	struct SDSI_BLOCK_SIZE_BITS bit;
};


//SD memory program Register (0x 25c)
struct SDSI_SD_MEM_PROGRAM_BITS {
	UINT32 WrProgDone:1;
	UINT32 Rsvd1:4;
	UINT32 AkeWpFail:1;
	UINT32 AkeError:1;
	UINT32 OutOfRange:1;
	UINT32 EccFail:1;
	UINT32 EraseParam:1;
	UINT32 Rsvd2:22;	
};
union SDSI_SD_MEM_PROGRAM_REG {
	UINT32 all;
	struct SDSI_SD_MEM_PROGRAM_BITS bit;
};

// Access Mode Register (0x 270)
struct SDSI_ACCESS_MODE_BITS {
	UINT32 SDR12:1;
	UINT32 SDR25:1;
	UINT32 SDR50:1;
	UINT32 SDR104:1;
	UINT32 DDR50:1;
	UINT32 DefaultCmdSys:1;
	UINT32 eC:1;
	UINT32 OTP:1;
	UINT32 ASSD:1;
	UINT32 eSD:1;
	UINT32 VendorSpec:1;
	UINT32 TypeB:1;
	UINT32 TypeA:1;
	UINT32 TypeC:1;
	UINT32 TypeD:1;
	UINT32 CurrLimit200:1;
	UINT32 CurrLimit400:1;
	UINT32 CurrLimit600:1;
	UINT32 CurrLimit800:1;
	UINT32 Rsvd:13;	
};
union SDSI_ACCESS_MODE_REG {
	UINT32 all;
	struct SDSI_ACCESS_MODE_BITS bit;
};

//SD Card Type (0x 280)
struct SDSI_SD_CARD_TYPE_BITS {
	UINT32 CardType:16; 
	UINT32 SpeedClass:8;
	UINT32 PerformanceMove:8;	
};
union SDSI_SD_CARD_TYPE_REG {
	UINT32 all;
	struct SDSI_SD_CARD_TYPE_BITS bit;
};

//SD Erase (0x 284)
struct SDSI_SD_ERASE_BITS {
	UINT32 EraseSize:16; 
	UINT32 EraseTimeout:6;
	UINT32 EraseOffset:2;
	UINT32 Rsvd:8;		
};
union SDSI_SD_ERASE_REG {
	UINT32 all;
	struct SDSI_SD_ERASE_BITS bit;
};

//CMD6_MODE (0x 288)
struct SDSI_CMD6_MODE_BITS {
	UINT32 AccessMode:4; 
	UINT32 CommandSystem:4;
	UINT32 DriverStrength:4;
	UINT32 CurrentLimit:4;
	UINT32 Rsvd:16;		
};
union SDSI_CMD6_MODE_REG {
	UINT32 all;
	struct SDSI_CMD6_MODE_BITS bit;
};


// SDSI registers
struct	SDSI_REGS 	{
	// FUN0 Registers
	union SDSI_CLK_WAKEUP_REG			CLK_WAKEUP;			// 0x00							   
	union SDSI_ESW_CCCR_REG				ESW_CCCR;			// 0x04
	UINT32 								ADMA_SYS_ADDR;		// 0x08
	union SDSI_ESW_CARD_READY_REG    	ESW_CARD_READY;		// 0x0c		READ ONLY
	union SDSI_ESW_FUN_READY_REG		ESW_FUN_READY;		// 0x10           
	union SDSI_SD_SDIO_INT_REG			SD_SDIO_INT_EN;		// 0x14
	union SDSI_SD_SDIO_INT_REG			SD_SDIO_INT_STATUS;	// 0x18
	union SDSI_SOFT_RESET_VALID_REG		SOFT_RESET_VALID;	// 0x1c
	union SDSI_MEM_INT_REG				MEM_INT_EN;			// 0x20
	union SDSI_MEM_INT_REG				MEM_INT_STUTAS;		// 0x24
	union SDSI_GLOBAL_INT_REG			GLOBAL_INT_EN;  	// 0x28
	union SDSI_GLOBAL_INT_REG			GLOBAL_INT_STATUS;	// 0x2c
	UINT32								CSA_PTR;			// 0x30		READ ONLY	24 bit
	union SDSI_IO_ACCESS_MODE_REG		IO_ACCESS_MODE;		// 0x34
	union SDSI_UHS_SUPPORT_REG			UHS_SUPPORT;		// 0x38
	UINT32 								CLK_DELAY_TIMER;	// 0x3c		32 bit
	union SDSI_PWR_CTRL_REG 		  	POWER_CONTROL;		// 0x40		READ ONLY
	union SDSI_PWR_STATE_REG			POWER_STATE;		// 0x44		READ ONLY
	UINT32 								SDSI_ADMA_GRANT;
	UINT32 RSVD0[45];				  						// 0x48 - 0xFF

	// FUN1 registers
	UINT32 								DATA_PORT;			// 0x100   	32 bit
	UINT32								IO_OCR;		 		// 0x104	24 bit	
	UINT32 RSVD1;						 					// 0x108
	UINT32								AHB_XFER_CNT;	  	// 0x10c 	21 bit
	UINT32						 		SDIO_XFER_CNT;	  	// 0x110	READ ONLY 	21 bit
	union SDSI_AHB_INT_REG				AHB_INT_STATUS;		// 0x114
	union SDSI_AHB_INT_REG				AHB_INT_EN;			// 0x118
	union SDSI_ESW_FBR1_REG				ESW_FBR1;	  		// 0x11c
	UINT32								ESW_IOR1;	  		// 0x120  	1 bit
	UINT32 								SD_HOST_GPR;		// 0x124	READ ONLY	32 bit
	UINT32 								ARM_GPR;			// 0x128	WRITE ONLY	32 bit
	UINT32								FUN1_RD_DATA_RDY;	// 0x12c   	1 bit
	UINT32 RSVD2[52];	   									// 0x130 - 0x1FF

	// SD Memory register
	union  SDSI_MEM_OCR_REG				MEM_OCR;			// 0x200
	UINT32 								LAST_FETCH_ADDR;	// 0x204	READ ONLY	32 bit
	UINT32 								CID0;				// 0x208   	32 bit
	UINT32 								CID1;				// 0x20c  	32 bit
	UINT32 								CID2;				// 0x210
	UINT32 								CID3;				// 0x214
	UINT32 								CSD0;				// 0x218
	UINT32 								CSD1;				// 0x21c
	UINT32 								CSD2;				// 0x220
	UINT32 								CSD3;				// 0x224
	UINT32 								SCR0;				// 0x228
	UINT32 								SCR1;				// 0x22c
	UINT32								PWD_LEN;			// 0x230 	8 bit
	UINT32 								PWD0;				// 0x234
	UINT32 								PWD1;				// 0x238
	UINT32 								PWD2;				// 0x23c
	UINT32 								PWD3;				// 0x240
	union  SDSI_BLOCK_SIZE_REG			BL0CK_SIZE; 		// 0x244 	READ ONLY
	UINT32 								ARGUMENT;			// 0x248	READ ONLY
	UINT32								NUM_WR_BLKS; 		// 0x24c   	10 bit
	UINT32 								ERASE_START_ADDR;	// 0x250 	READ ONLY	
	UINT32 								ERASE_END_ADDR;		// 0x254	READ ONLY
	UINT32								PRE_ERASE_BLK_CNT; 	// 0x258	READ ONLY	23 bit
	union  SDSI_SD_MEM_PROGRAM_REG		SD_MEM_PROGRAM;		// 0x25c
	UINT32								UNIT_CNT;	 		// 0x260 	10 bit
	UINT32								WRBLKCNT; 			// 0x264   	16 bit
	UINT32  							RDBLKCNT; 			// 0x268 	16 bit  
	UINT32								SECURE_REG;	 		// 0x26c  	9 bit
	union  SDSI_ACCESS_MODE_REG			ACCESS_MODE;	 	// 0x270 	
	UINT32								AU_SIZE;	 		// 0x274 	4 bit
	UINT32								MAX_CURR;			// 0x278   	16 bit
	UINT32 								PROTECTED_AREA_SIZE;// 0x27c   	32 bit
	union  SDSI_SD_CARD_TYPE_REG		SD_CARD_TYPE;		// 0x280
	union  SDSI_SD_ERASE_REG			SD_ERASE;		 	// 0x284
	union  SDSI_CMD6_MODE_REG			CMD6_MODE;			// 0x288
};

// SDSI ADMA2 Descriptor Table
struct	ADMA2_DESC_TBL{
	UINT16 ATTRIBUTE;
	UINT16 LENGTH;
	UINT32 ADDRESS;
};

/*
*************************** SDI ******************************************
*/
// SDI Status Register Bits Definition
struct	SDI_STATUS_BITS	{	// bits		description
	UINT32	DataTranOver:1;	// 0		Data transfer over 
	UINT32	RespRecOver:1;	// 1		Response receive over
	UINT32	Timeout:1;		// 2		Timeout
	UINT32	BusReady:1;		// 3		Bus Ready
	UINT32	DatCrcErr:1;	// 4		Data read CRC error
	UINT32	Rsvd3:1;		// 5		Reserved
	UINT32	RespCrcErr:1;	// 6		Write high to Busy low
	UINT32	Rsvd4:1;		// 7		Reserved
	UINT32	Rsvd5:24;		// 31:8		Reserved
};
union 	SDI_STATUS_REG	{
	UINT32	all;
	struct SDI_STATUS_BITS	bit;
};

//SDI Control Register Bits Definition
struct	SDI_CTRL_BITS	{	// bits		description
	UINT32	ClkEn:1;		// 0		Reset 
	UINT32	NxtDatStart:1;	// 1		next data block read/write operation start
	UINT32	Rsvd1:1;		// 2		Reserved
	UINT32	CmdSendStart:1;	// 3		command send start
	UINT32	BufBankSel:5;	// 8:4		data buffer RAM bank select
	UINT32	Rsvd2:7;		// 15:9		Reserved
	UINT32	Rsvd3:16;		// 31:16	Reserved
};
union 	SDI_CTRL_REG	{
	UINT32	all;
	struct SDI_CTRL_BITS	bit;
};

//SDI CRC Status Token Register Bits Definition (write block CRC Response)
struct	SDI_CRC_TOKEN_BITS	{	// bits		description
	UINT32	EndBit:1;			// 0		End bit 
	UINT32	Status:3;			// 3:1		CRC status
	UINT32	Rsvd2:4;			// 7:4		Reserved
	UINT32	Rsvd3:24;			// 31:8		Reserved
};
union 	SDI_CRC_TOKEN_REG	{
	UINT32	all;
	struct SDI_CRC_TOKEN_BITS	bit;
};

//SDI Clock Divider Register Bits Definition
struct	SDI_CLK_DIV_BITS	{	// bits		description
	UINT32	Div:8;			// 7:0			Reserved 
	UINT32	Rsvd1:24;		// 31:8			Reserved
};
union 	SDI_CLK_DIV_REG	{
	UINT32	all;
	struct SDI_CLK_DIV_BITS	bit;
};

//SDI Interrupt Register Bits Definition
struct	SDI_INT_BITS	{	// bits		description
	UINT32	DataTranOverInt:1;	// 0		Data transfer over 
	UINT32	Rsvd1:7;
	UINT32	RespRecOverInt:1;	// 8		command response over
	UINT32	Rsvd2:7;
	UINT32	TimeoutInt:1;		// 16		time out
	UINT32	Rsvd3:7;
	UINT32	Rsvd4:8;
};
union 	SDI_INT_REG	{
	UINT32	all;
	struct SDI_INT_BITS	bit;
};


// SDI Registers
struct	SDI_REGS	{
	union 	SDI_STATUS_REG	STATUS;			// SDI Status Register
	union	SDI_CTRL_REG	CTRL;			// SDI Control Register
	UINT32	TIMEOUT;						// SDI Command timerout Register
	union 	SDI_CRC_TOKEN_REG	CRC_TOKEN;	// SDI CRC Status Register
	union 	SDI_CLK_DIV_REG	CLK_DIVIDER;	// SDI Clock Divider Register
	union 	SDI_INT_REG		INT_FLAG;		// SDI Interrupt Flag Register
	union 	SDI_INT_REG		INT_EN;			// SDI Interrupt Enable Register
	UINT32	BLK_LEN;						// Block Length
	UINT32	CMD_BUF0;						// Command Buffer
	UINT32	CMD_BUF1;
	UINT32	RSP_BUF0;						// Response Buffer
	UINT32	RSP_BUF1;
	UINT32	RSP_BUF2;
	UINT32	RSP_BUF3;
};

#define	SDI_DAT_BUF		(UINT8 *)0x20008000
#define	SDI_DAT_BUF_SIZE	(16*1024)

/*--------------------------------------------Hash------------------------------------------------*/

/*
*************************** Sea ******************************************
*/

/*
*************************** Cache ******************************************
*/
// Cache Control Register Bits Definition
struct	CACHE_CTRL_BITS	{	// bits		description
	UINT32	Rsvd0:24;		// 23:0		Reserved
	UINT32	Flush:1;		// 24		flush bit 
	UINT32	Bypass:1;		// 25		bypass bit
	UINT32	Rsvd1:6;		// 31:26		Reserved
};
union 	CACHE_CTRL_REG	{
	UINT32	all;
	struct CACHE_CTRL_BITS	bit;
};

// Cache Line Size Configuration Register Bits Definition (read only)
struct	CACHE_LNSIZECFG_BITS	{	// bits		description
	UINT32	Rsvd0:24;		// 23:0		Reserved
	UINT32	CacheLine:2;	// 1:0		Cache line size  
	UINT32	Rsvd1:6;		// 31:26		Reserved
};
union 	CACHE_LNSIZECFG_REG	{
	UINT32	all;
	struct CACHE_LNSIZECFG_BITS	bit;
};

// Cache Associativity Configuration Register Bits Definition (read only)
struct	CACHE_ASSOCCFG_BITS	{	// bits		description
	UINT32	Rsvd0:24;		// 23:0		Reserved
	UINT32	Associativity:2;	// 1:0		Cache Associativity  
	UINT32	Rsvd1:6;		// 31:26		Reserved
};
union 	CACHE_ASSOCCFG_REG	{
	UINT32	all;
	struct CACHE_ASSOCCFG_BITS	bit;
};

// Cache Registers
struct	CACHE_REGS	{
	union 	CACHE_CTRL_REG	CTRL;			// Cache Control Register Bits Definition
	union	CACHE_LNSIZECFG_REG	LNSIZECFG;	// Cache Line Size Configuration Register Bits Definition (read only)
	union 	CACHE_ASSOCCFG_REG	ASSOCCFG;	// Cache Associativity Configuration Register Bits Definition (read only)
};


/*
*************************** APSI ******************************************
*/

// Config Register Bits Definition
struct	APSI_CFG_BITS	{	// bits		description
	UINT32	RegEdcEn:1;		// 0		Register access Error dectect enable
	UINT32	MemEdcMode:1;	// 1		memory access Error dectect mode
	UINT32	MemEdcEn:1;		// 2		memory access Error dectect enable 
	UINT32	Rsvd1:1;		// 3		Reserved
	UINT32	CrcEndian:1;	// 4		CRC dectect mode endian
	UINT32	Rsvd:27;		// 31:5		Reserved
};
union 	APSI_CFG_REG	{
	UINT32	all;
	struct APSI_CFG_BITS	bit;
};

// Control Register Bits Definition
struct	APSI_CTRL_BITS	{	// bits		description
	UINT32	ClearBusy:1;	// 0		clear busy 
	UINT32	SeaMux:1;		// 1		SEA Mux 
	UINT32	Rsvd:30;		// 31:2		Reserved
};
union 	APSI_CTRL_REG	{
	UINT32	all;
	struct APSI_CTRL_BITS	bit;
};

// Status Register Bits Definition
struct	APSI_STA_BITS	{	// bits		description
	UINT32	Reset:1;		// 0		reset by APSI access
	UINT32	SeaRun:1;		// 1		SEA running 
	UINT32	SemRcv:1;		// 2		semphore received
	UINT32	Rsvd:29;		// 31:3		Reserved
};
union 	APSI_STA_REG	{
	UINT32	all;
	struct APSI_STA_BITS	bit;
};

// Interrupt enable Register Bits Definition
struct	APSI_INTEN_BITS	{	// bits		description
	UINT32	Reset:1;		// 0		reset by APSI access
	UINT32	Rsvd1:1;		// 1		Reserved 
	UINT32	SemRcv:1;		// 2		semphore received
	UINT32	Rsvd:29;		// 31:3		Reserved
};
union 	APSI_INTEN_REG	{
	UINT32	all;
	struct APSI_INTEN_BITS	bit;
};

// APSI Registers
struct	APSI_REGS	{
	union 	APSI_CFG_REG	CFG;			// Config Register
	union 	APSI_CTRL_REG	CTRL;			// Control Register
	UINT32	SEM_IN;
	UINT32	SEM_OUT;
	union 	APSI_STA_REG	STA;			// Status Register
	union 	APSI_INTEN_REG	INTEN;			// Interrupt enable Register
	UINT32	GPR0;
	UINT32 	GPR1;
	UINT32	GPR2;
	UINT32 	GPR3;
};

/***************************************************************************
* Include Header Files
***************************************************************************/
// all peripheral header files


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
extern volatile struct SCM_REGS ScmRegs;
extern volatile struct GPIO_REGS GpioRegs;
extern volatile struct TMR_REGS T0Regs;


extern volatile struct USART_REGS Spi0Regs;

extern volatile struct USB_REGS UsbRegs;
extern volatile struct TMR_REGS T1Regs;
extern volatile struct TMR_REGS T2Regs;
extern volatile struct SSI_REGS SsiRegs;
extern volatile struct UART_REGS Uart0Regs;
extern volatile struct I2C_REGS I2C0Regs;
extern volatile struct SWPS_REGS SwpsRegs;
extern volatile struct FD12M_REGS Fd12mRegs;
extern volatile struct AOSS_REGS AossRegs;
extern volatile struct ISO7816_REGS Iso7816Regs;
extern volatile struct SCI_REGS SciRegs;
extern volatile struct DMA_REGS DmaRegs;
extern volatile struct VPWM_REGS VpwmRegs;
extern volatile struct SDSI_REGS SdsiRegs;

extern volatile struct EFC_REGS EfcRegs;
extern volatile struct UART_REGS Uart1Regs;
extern volatile struct UART_REGS Uart2Regs;
extern volatile struct TMR_REGS T3Regs;

extern volatile struct SDI_REGS SdiRegs;
extern volatile struct USART_REGS Spi1Regs;
extern volatile struct TMR_REGS T4Regs;
extern volatile struct TMR_REGS T5Regs;
extern volatile struct TMR_REGS T6Regs;
extern volatile struct TMR_REGS T7Regs;
extern volatile struct SQI_REGS SqiRegs;
extern volatile struct CRC_REGS CrcRegs;
extern volatile struct CACHE_REGS CacheRegs;
extern volatile struct APSI_REGS ApsiRegs;
// SDSI ADMA2 Descriptor Table
extern volatile struct ADMA2_DESC_TBL SdsiADMA2DescTbl[32];


#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
__NO_INIT volatile struct SCM_REGS ScmRegs 				__AT(SCM_BASE);   
__NO_INIT volatile struct GPIO_REGS GpioRegs			__AT(GPIO_BASE); 
__NO_INIT volatile struct TMR_REGS T0Regs				__AT(T1_BASE);    
                                            
                                            
__NO_INIT volatile struct USART_REGS Spi0Regs			__AT(SPI0_BASE);	

__NO_INIT volatile struct USB_REGS UsbRegs				__AT(USB_BASE);
__NO_INIT volatile struct TMR_REGS T1Regs				__AT(T1_BASE);
__NO_INIT volatile struct TMR_REGS T2Regs				__AT(T2_BASE);
__NO_INIT volatile struct SSI_REGS SsiRegs				__AT(SSI_BASE);
__NO_INIT volatile struct UART_REGS Uart0Regs			__AT(UART0_BASE);
__NO_INIT volatile struct I2C_REGS I2C0Regs				__AT(I2C0_BASE);
__NO_INIT volatile struct SWPS_REGS SwpsRegs			__AT(SWPS_BASE);
__NO_INIT volatile struct FD12M_REGS Fd12mRegs			__AT(FD12M_BASE);
__NO_INIT volatile struct AOSS_REGS AossRegs			__AT(AOSS_BASE);
__NO_INIT volatile struct ISO7816_REGS Iso7816Regs		__AT(ISO7816_BASE);
__NO_INIT volatile struct SCI_REGS SciRegs				__AT(SCI_BASE);
__NO_INIT volatile struct DMA_REGS DmaRegs				__AT(DMA_BASE);
__NO_INIT volatile struct VPWM_REGS VpwmRegs			__AT(VPWM_BASE);
__NO_INIT volatile struct SDSI_REGS SdsiRegs			__AT(SDSI_BASE);

__NO_INIT volatile struct EFC_REGS EfcRegs				__AT(EFC_BASE);
__NO_INIT volatile struct UART_REGS Uart1Regs			__AT(UART1_BASE);
__NO_INIT volatile struct UART_REGS Uart2Regs			__AT(UART2_BASE);
__NO_INIT volatile struct TMR_REGS T3Regs				__AT(T3_BASE);

__NO_INIT volatile struct SDI_REGS SdiRegs				__AT(SDI_BASE);
__NO_INIT volatile struct USART_REGS Spi1Regs			__AT(SPI1_BASE);
__NO_INIT volatile struct TMR_REGS T4Regs				__AT(T4_BASE);
__NO_INIT volatile struct TMR_REGS T5Regs				__AT(T5_BASE);
__NO_INIT volatile struct TMR_REGS T6Regs				__AT(T6_BASE);
__NO_INIT volatile struct TMR_REGS T7Regs				__AT(T7_BASE);
__NO_INIT volatile struct SQI_REGS SqiRegs				__AT(SQI_BASE);
__NO_INIT volatile struct CRC_REGS CrcRegs				__AT(CRC_BASE);
__NO_INIT volatile struct CACHE_REGS CacheRegs			__AT(CACHE_BASE);

__NO_INIT volatile struct APSI_REGS ApsiRegs			__AT(APSI_BASE);

// SDSI ADMA2 Descriptor Table
__NO_INIT volatile struct ADMA2_DESC_TBL SdsiADMA2DescTbl[32]	__AT(SDSI_ADMA2_DESC_TBL_ADDR);

#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif



/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*AS569_DEVICE_H_*/
