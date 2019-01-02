/***************************************************************************
* File name    :	Scm.h
* Function     :	Header of System Control Module
* Author       : 	coins
* Date         :	2012/07/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SCM_H_
#define SCM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

// define the default system clock frequency
#define		F_DEF_SYSCLK	(F_HFROSC/2)

// define the system tick stclk clock frequency
#define		F_SYST_STCLK	(F_LFROSC/4)

// define the clock source for SysTick
#define		SYST_STCLK	0	// the STCLK for SysTick
#define		SYST_SCLK	1	// the SCLK for SysTick  
// define the work mode	for SysTick
#define		SYST_CNT	0	// SysTick count but no interrupt
#define		SYST_INT	1  	// SysTick count and interrupt when down to 0

/* SysTick constants */
#define SYSTICK_ENABLE		0      			/* Config-Bit to start or stop the SysTick Timer                         */
#define SYSTICK_TICKINT		1      			/* Config-Bit to enable or disable the SysTick interrupt                 */
#define SYSTICK_CLKSOURCE	2      			/* Clocksource has the offset 2 in SysTick Control and Status Register   */
#define SYSTICK_MAXCOUNT	((1<<24) -1)	/* SysTick MaxCount*/	

// Define the analog module
#define	AN_HFROSC			(1L<<0)
#define	AN_MFROSC			(1L<<1)
#define	AN_PDR33			(1L<<2)
#define	AN_PDR18			(1L<<3)
#define	AN_SDSI_RAM			(1L<<4)
#define	AN_ROM				(1L<<5)
#define	AN_TRNG_RING		(1L<<6)
#define	AN_AVR				(1L<<7)
#define	AN_XTAL				(1L<<8)

//Destruct type
#define	DESTRUCT_ROM		(1L<<27)
#define	DESTRUCT_SRAM		(1L<<26)
#define	DESTRUCT_VDD11VD	(1L<<16)
#define	DESTRUCT_VDD18VD	(1L<<8)
#define	DESTRUCT_VDD33VD	(1L<<1)	
//Rst type
#define	RST_SRAM_ERR	(1L<<26)
#define	RST_VDD11VD		(1L<<16)
#define	RST_PDR18		((1L<<9)|(1L<<4))
#define	RST_VDD18VD		(1L<<8)
#define	RST_SOFT		(1L<<3)
#define	RST_WDT			(1L<<2)
#define	RST_VDD33VD		(1L<<1)
#define	RST_PDR33		(1L)


// define auto power down when deepsleep
#define	DSM_PD_HFROSC	(1L<<17)
#define	DSM_PD_MFROSC	(1L<<18)
#define	DSM_PD_XTAL		(1L<<19)


// for compatible with previous functions
#define	SCM_HFRoscCRE()		SCM_HFRoscAutoTrim(AUTO_TRIM_REF_CLK_USB, 1000, SCM_GetHFRoscClk())
#define	SCM_UsbPhySet()		SCM_UsbPhyWork();
#define	SCM_EnableUsbPuRes()	SCM_UsbPhyDpResSet(USB_PHY_RES_TYPE_PU)	
#define	SCM_DisableUsbPuRes()	SCM_UsbPhyDpResSet(USB_PHY_RES_TYPE_NONE)			


/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the system clock for setting
// HFROSC_Grade[1:0] << 6 + ExtClkSel<<5 + RoscClkSel<<4 +  FclkSel[1:0]<<2 + Clk32kSel<<1 + FclkSrc
typedef	enum	{
	SYS_CLK_HFROSC_0_DIV8 = 0x00,
	SYS_CLK_HFROSC_0_DIV4 = 0x04,
  	SYS_CLK_HFROSC_0_DIV2 = 0x08,
	SYS_CLK_HFROSC_0_DIV1 = 0x0C,
	SYS_CLK_HFROSC_1_DIV8 = 0x40,
	SYS_CLK_HFROSC_1_DIV4 = 0x44,
  	SYS_CLK_HFROSC_1_DIV2 = 0x48,
	SYS_CLK_HFROSC_1_DIV1 = 0x4C,
	SYS_CLK_HFROSC_2_DIV8 = 0x80,
	SYS_CLK_HFROSC_2_DIV4 = 0x84,
  	SYS_CLK_HFROSC_2_DIV2 = 0x88,
	SYS_CLK_HFROSC_2_DIV1 = 0x8C,
	SYS_CLK_HFROSC_3_DIV8 = 0xC0,
	SYS_CLK_HFROSC_3_DIV4 = 0xC4,
  	SYS_CLK_HFROSC_3_DIV2 = 0xC8,
	SYS_CLK_HFROSC_3_DIV1 = 0xCC,
	SYS_CLK_MFROSC_DIV8 = 0x10,
	SYS_CLK_MFROSC_DIV4 = 0x14,
  	SYS_CLK_MFROSC_DIV2 = 0x18,
	SYS_CLK_MFROSC_DIV1 = 0x1C,
	SYS_CLK_XTAL_DIV8 = 0x20,
	SYS_CLK_XTAL_DIV4 = 0x24,
  	SYS_CLK_XTAL_DIV2 = 0x28,
	SYS_CLK_XTAL_DIV1 = 0x2C,
	SYS_CLK_RTC = 0x03,
	SYS_CLK_LFROSC = 0x01,
}SYS_CLK;

#define		SYS_CLK_144M	SYS_CLK_HFROSC_3_DIV1
#define		SYS_CLK_96M		SYS_CLK_HFROSC_2_DIV1
#define		SYS_CLK_72M		SYS_CLK_HFROSC_3_DIV2
#define		SYS_CLK_48M		SYS_CLK_HFROSC_2_DIV2
#define		SYS_CLK_36M		SYS_CLK_HFROSC_3_DIV4
#define		SYS_CLK_24M		SYS_CLK_HFROSC_2_DIV4
#define		SYS_CLK_18M		SYS_CLK_HFROSC_3_DIV8
#define		SYS_CLK_12M		SYS_CLK_HFROSC_2_DIV8
#define		SYS_CLK_6M		SYS_CLK_HFROSC_0_DIV2


// define the clock output source
typedef	enum {
	CLKOUT_SRC_FCLK = 0,
	CLKOUT_SRC_HFROSC = 1,
	CLKOUT_SRC_MFROSC = 2,
	CLKOUT_SRC_LFROSC = 3,
	CLKOUT_SRC_XTAL = 4,
	CLKOUT_SRC_RTC = 5,		
}CLKOUT_SRC;

// define the clock output pin
typedef	enum {
	CLKOUT_PIN_GPA0 = 0,
	CLKOUT_PIN_GPB7 = 1,	
}CLKOUT_PIN;


// define the UART clock source
typedef	enum {
	UART_CLK_SRC_FCLK = 0,
	UART_CLK_SRC_XTAL = 1,	
	UART_CLK_SRC_HFROSC_DIV1 = 4,
	UART_CLK_SRC_HFROSC_DIV2 = 5,
	UART_CLK_SRC_HFROSC_DIV4 = 6,
	UART_CLK_SRC_HFROSC_DIV8 = 7,
}UART_CLK_SRC;

// define the I2C clock source
typedef	enum {
	I2C_CLK_SRC_FCLK = 0,
	I2C_CLK_SRC_HFROSC = 1,
	I2C_CLK_SRC_MFROSC = 2,
	I2C_CLK_SRC_XTAL = 3,	
}I2C_CLK_SRC;

// define the HFROSC auto trimming reference clock
typedef	enum {
	AUTO_TRIM_REF_CLK_USB = 0,
	AUTO_TRIM_REF_CLK_XTAL = 1,
	AUTO_TRIM_REF_CLK_RTC = 2,
	AUTO_TRIM_REF_CLK_UART0 = 3,	
	AUTO_TRIM_REF_CLK_UART1 = 4,
	AUTO_TRIM_REF_CLK_UART2 = 5,
}AUTO_TRIM_REF_CLK;


// define the PDR33 threshold
typedef	enum {
	PDR33_TH_1V60 = 0,
	PDR33_TH_2V08 = 1,
	PDR33_TH_2V20 = 2,
	PDR33_TH_2V44 = 3,	
	PDR33_TH_2V68 = 4,
	PDR33_TH_2V80 = 5,
	PDR33_TH_2V92 = 6,
	PDR33_TH_3V02 = 7,
}PDR33_TH;

// define the PDR18 threshold
typedef	enum {
	PDR18_TH_1V45 = 0,
	PDR18_TH_1V55 = 1,
}PDR18_TH;

// define the AVR HD18 threshold
typedef	enum {
	AVR_HD18_TH_2V05 = 0,
	AVR_HD18_TH_2V10 = 1,
	AVR_HD18_TH_2V15 = 2,
	AVR_HD18_TH_2V20 = 3,
}AVR_HD18_TH;

// define the AVR LD18 threshold
typedef	enum {
	AVR_LD18_TH_1V50 = 0,
	AVR_LD18_TH_1V55 = 1,
	AVR_LD18_TH_1V60 = 2,
	AVR_LD18_TH_1V65 = 3,
}AVR_LD18_TH;

// define the AVR HD11 threshold
typedef	enum {
	AVR_HD11_TH_1V25 = 0,
	AVR_HD11_TH_1V30 = 1,
	AVR_HD11_TH_1V35 = 2,
	AVR_HD11_TH_1V40 = 3,
}AVR_HD11_TH;

// define the AVR LD11 threshold
typedef	enum {
	AVR_LD11_TH_0V85 = 0,
	AVR_LD11_TH_0V90 = 1,
	AVR_LD11_TH_0V95 = 2,
	AVR_LD11_TH_1V00 = 3,
}AVR_LD11_TH;

// define the AVR HDHV threshold
typedef	enum {
	AVR_HDHV_TH_3V70 = 0,
	AVR_HDHV_TH_3V80 = 1,
	AVR_HDHV_TH_3V90 = 2,
	AVR_HDHV_TH_4V00 = 3,
}AVR_HDHV_TH;


// define the vector table memory map type
typedef enum	{
	VT_MEM_ROM 		= 0,
	VT_MEM_SRAM 	= 1,
	VT_MEM_FLASH 	= 2,
}VT_MEM;

// define the USB 48MHz clock divider
typedef	enum {
	USB_CLK_DIV1 = 2,
	USB_CLK_DIV2 = 0,
	USB_CLK_DIV3 = 1,
}USB_CLK_DIV;

// define the USB PHY resistor type
typedef	enum {
	USB_PHY_RES_TYPE_NONE = 0,
	USB_PHY_RES_TYPE_PU = 1,
	USB_PHY_RES_TYPE_PD = 4,
	USB_PHY_RES_TYPE_BOTH = 5,
}USB_PHY_RES_TYPE;


// define the Flash mode when deepsleep
typedef	enum {
	DSM_EF_STANDBY = 0,		// stand by
	DSM_EF_SLEEP = 1, 		// sleep
	DSM_EF_PWRDN = 2,	  	// power down
}DSM_EF_TYPE;




/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
// Miscellaneous
__INLINE UINT32 SCM_GetSOCID(void);
void _delay_ms(UINT32 ms);
void _delay_us(UINT32 us);

// Clock Control
UINT8 SCM_SetSysClk(SYS_CLK Clk);
void SCM_SetSysClkLowFreq(void);
UINT32 SCM_GetSysClk(void);
void SCM_DisableModClk(UINT8 ModId);
void SCM_EnableModClk(UINT8 ModId);
__INLINE void SCM_EnableSqiClk(void);
__INLINE void SCM_DisableSqiClk(void);
void SCM_SetClkout(CLKOUT_SRC mSrc, CLKOUT_PIN mPin, UINT8 nDiv);
__INLINE void SCM_EnableClkout(void);
__INLINE void SCM_DisableClkout(void);
UINT8 SCM_SetUartClk(UINT8 UARTx, UART_CLK_SRC mSrc);
UINT32 SCM_GetUartClk(UINT8 UARTx);
UINT8 SCM_SetI2cClk(UINT8 I2Cx, I2C_CLK_SRC mSrc);
UINT32 SCM_GetI2cClk(UINT8 I2Cx);

UINT32 SCM_GetHFRoscClk(void);
UINT8 SCM_LoadHFRoscTrim(void);
UINT8 SCM_HFRoscAutoTrim(AUTO_TRIM_REF_CLK mRefSrc, UINT32 nRefFreq, UINT32 nTargetFreq);
__INLINE void SCM_HFRoscAutoTrimDisable(void);
UINT8 SCM_SetHFRoscTrimLimit(UINT16 nLLimit, UINT16 nHLimit);
void SCM_HFRoscCRE1(UINT8 nType);

// Reset Control
__INLINE void SCM_EnableRst(UINT32 RstType);
__INLINE void SCM_DisableRst(UINT32 RstType);
__INLINE UINT32 SCM_GetRstStatus(void);
__INLINE void SCM_ClrRstSta(UINT32 RstType);
void SCM_KeepModRst(UINT8 ModId);
void SCM_ReleaseModRst(UINT8 ModId);

__INLINE void SCM_DisableDestruct(UINT32 Destructtype);
__INLINE void SCM_EnableDestruct(UINT32 Destructtype);

// Analog control
void SCM_AnModPowerEnable(UINT32 nAnMods);
void SCM_AnModPowerDisable(UINT32 nAnMods);
void SCM_SetPdr33Th(PDR33_TH mTh);
void SCM_SetPdr18Th(PDR18_TH mTh);
void SCM_SetArvTh(AVR_HD18_TH mHD18Th, AVR_LD18_TH mLD18Th, AVR_HD11_TH mHD11Th, AVR_LD11_TH mLD1LTh, AVR_HDHV_TH mHDHVTh);


// MPU
UINT8 SCM_RegWrEn(void);
__INLINE void SCM_VectReMapTo(VT_MEM mem);
__INLINE void SCM_SetCacheRegion0(UINT32 nStartAddr, UINT32 nEndAddr);
__INLINE void SCM_SetCacheRegion1(UINT32 nStartAddr, UINT32 nEndAddr);
__INLINE void SCM_EnableCache(void);
__INLINE void SCM_DisableCache(void);
__INLINE void SCM_FlushCache(void);
__INLINE void SCM_DisableSramExe(void);
__INLINE void SCM_DisableKeySramAcc(void);
__INLINE void SCM_EnableKeySramAcc(void);


// USB Phy
__INLINE void SCM_EnableUsbClk(void);
__INLINE void SCM_DisableUsbClk(void);
__INLINE void SCM_SetUsbClk(USB_CLK_DIV nDiv);
__INLINE void SCM_UsbPhySuspend(void);
__INLINE void SCM_UsbPhyWork(void);
__INLINE void SCM_UsbPhyDpResSet(USB_PHY_RES_TYPE nResType);
__INLINE void SCM_UsbPhyDnResSet(USB_PHY_RES_TYPE nResType);


// System Tick
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode);
__INLINE UINT32 SYST_GetCnt(void);
__INLINE void SYST_Stop(void);
__INLINE void SYST_Continue(void);


// PMU
__INLINE void SCM_SysSleep(void);
__INLINE void SCM_SysDeepSleep(void);
void SCM_SetDeepSleepMode(DSM_EF_TYPE mEfType, UINT32 nPd, UINT8 nSlowClkSw);


/**************************************************************************
* Functon
***************************************************************************/



/***************************************************************************
 *************************** Miscellaneous API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_GetSOCID
 * Function:	to get SOC ID Code
 * Input:		None 
 * Output:		return SOC ID	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SCM_GetSOCID(void)
{
	return ScmRegs.SOCID;
}



/***************************************************************************
 *************************** Clock Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_EnableSqiClk
 * Function:	to enable SQI clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableSqiClk(void)
{
	ScmRegs.SYSCLKCTRL.all |= 1L<<23;
}

/****************************************************************************
 * Subroutine:	SCM_DisableSqiClk
 * Function:	to disable SQI clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableSqiClk(void)
{
	ScmRegs.SYSCLKCTRL.all &= ~(1L<<23);
}

/****************************************************************************
 * Subroutine:	SCM_EnableClkout
 * Function:	to enable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all |= 0x100;
}

/****************************************************************************
 * Subroutine:	SCM_DisableClkout
 * Function:	to disable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all &= (UINT32)(~0x100);
}

/****************************************************************************
 * Subroutine:	SCM_HFRoscAutoTrimDisable
 * Function:	to disable HFROSC auto trimming
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_HFRoscAutoTrimDisable(void)
{
	ScmRegs.TRIM_CTRL.bit.TrimStart = 0;
}



/***************************************************************************
 *************************** Reset Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_EnableRst
 * Function:	Enable Reset
 * Input:		UINT32 RstType --
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableRst(UINT32 RstType)
{
	ScmRegs.SYSRSTCTRL.all |= (RstType);
}

/****************************************************************************
 * Subroutine:	SCM_DisableRst
 * Function:	Disable Reset
 * Input:		UINT32 RstType --
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableRst(UINT32 RstType)
{
	ScmRegs.SYSRSTCTRL.all &= ~(RstType);
}
/****************************************************************************
 * Subroutine:	SCM_GetRstStatus
 * Function:	to get reset status 
 * Input:		None 
 * Output:		reset status	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SCM_GetRstStatus(void)
{
	return ScmRegs.SYSRSTSTATUS.all;
}
/****************************************************************************
 * Subroutine:	SCM_ClrRstSta
 * Function:	clear Reset Status
 * Input:		UINT32 RstType --
 * Output:		None	
 * Description:	 
 * Date:		2014.09.25
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_ClrRstSta(UINT32 RstType)
{
	ScmRegs.SYSRSTSTATUS.all = (RstType);
}


/****************************************************************************
 * Subroutine:	SCM_DisableDestruct
 * Function:	Disable self-destruct
 * Input:		UINT32 Destructtype  --
 * Output:		None
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableDestruct(UINT32 Destructtype)
{
	ScmRegs.SYSSDCTRL.all &= ~(Destructtype);
}

/****************************************************************************
 * Subroutine:	SCM_EnableDestruct
 * Function:	Enable self-destruct
 * Input:		UINT32 Destructtype  --
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableDestruct(UINT32 Destructtype)
{
	ScmRegs.SYSSDCTRL.all |= (Destructtype);
}



/***************************************************************************
 *************************** Analog Control API *****************************
 **************************************************************************/








/***************************************************************************
 *************************** MPU Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_VectReMapTo
 * Function:	to re-map vector to specified memory
 * Input:		VT_MEM mem -- to specify the memory type  
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_VectReMapTo(VT_MEM mem)
{
	ScmRegs.MEMREMAP = mem;
}

/****************************************************************************
 * Subroutine:	SCM_SetCacheRegion0
 * Function:	to set cacheable region 0 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.20
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetCacheRegion0(UINT32 nStartAddr, UINT32 nEndAddr)
{
	ScmRegs.CACHEBASE0 = nStartAddr;
	ScmRegs.CACHETOP0 = nEndAddr;
}

/****************************************************************************
 * Subroutine:	SCM_SetCacheRegion1
 * Function:	to set cacheable region 1 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.20
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetCacheRegion1(UINT32 nStartAddr, UINT32 nEndAddr)
{
	ScmRegs.CACHEBASE1 = nStartAddr;
	ScmRegs.CACHETOP1 = nEndAddr;
}

/****************************************************************************
 * Subroutine:	SCM_EnableCache
 * Function:	to enable cache
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.20
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableCache(void)
{
	CacheRegs.CTRL.bit.Bypass = 0;
}

/****************************************************************************
 * Subroutine:	SCM_DisableCache
 * Function:	to disable cache control and enter into bypass mode
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.20
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableCache(void)
{
	CacheRegs.CTRL.bit.Bypass = 1;
}

/****************************************************************************
 * Subroutine:	SCM_FlushCache
 * Function:	to flush the contents of the tag RAM 
 *				which invalidate the contents of cache RAM
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.20
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_FlushCache(void)
{
	CacheRegs.CTRL.bit.Flush = 1;
}


/****************************************************************************
 * Subroutine:	SCM_DisableSramExe
 * Function:	to disable SRAM execute function
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableSramExe(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.SRAM_EXE_INHIBIT = 0x45239876;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_DisableKeySramAcc
 * Function:	to disable key SRAM access
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableKeySramAcc(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.KEY_ACCESS_INHIBIT = 0x54239876;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_EnableKeySramAcc
 * Function:	to enable key SRAM access
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableKeySramAcc(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.KEY_ACCESS_INHIBIT = 0;
	ScmRegs.REGWP1 = 0;
}




/***************************************************************************
 *************************** USB Global Control API ************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_EnableUsbClk
 * Function:	to enable USB 48MHz clock (enabled by default)
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 1;	
}

/****************************************************************************
 * Subroutine:	SCM_DisableUsbClk
 * Function:	to disable USB 48MHz clock
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 0;	
}

/****************************************************************************
 * Subroutine:	SCM_SetUsbClk
 * Function:	to set USB 48MHz clock 
 * Input:		USB_CLK_DIV nDiv -- to set USB clock divider
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetUsbClk(USB_CLK_DIV nDiv)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mSel = nDiv;	
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhySuspend
 * Function:	to set USB PHY suspend (power down) 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_UsbPhySuspend(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbSuspend = 0;	
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhyWork
 * Function:	to set USB PHY work 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_UsbPhyWork(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbSuspend = 1;	
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhyDpResSet
 * Function:	to set USB PHY DP resistor 
 * Input:		USB_PHY_RES_TYPE nResType -- to specify the resistor type
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_UsbPhyDpResSet(USB_PHY_RES_TYPE nResType)
{
	ScmRegs.USBPHYCTRL.all = (ScmRegs.USBPHYCTRL.all&(~0x5UL)) | nResType; 	
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhyDnResSet
 * Function:	to set USB PHY DN resistor 
 * Input:		USB_PHY_RES_TYPE nResType -- to specify the resistor type
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_UsbPhyDnResSet(USB_PHY_RES_TYPE nResType)
{
	ScmRegs.USBPHYCTRL.all = (ScmRegs.USBPHYCTRL.all&(~0xAUL)) | (nResType<<1);	
}





/***************************************************************************
 *************************** System Tick API *******************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SYST_GetCnt
 * Function:	to get SysTick current count value
 * Input:		None
 * Output:		UINT32 -- the value of current count	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SYST_GetCnt(void)
{
	return SysTick->VAL;
}

/****************************************************************************
 * Subroutine:	SYST_Stop
 * Function:	to stop SysTick
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SYST_Stop(void)
{
	SysTick->CTRL &= ~(UINT32)(1<<SYSTICK_ENABLE);
}

/****************************************************************************
 * Subroutine:	SYST_Continue
 * Function:	to Continue SysTick count
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SYST_Continue(void)
{
	SysTick->CTRL |= (1<<SYSTICK_ENABLE);
}



/***************************************************************************
 ****************************** PMU API **********************************
 **************************************************************************/


/****************************************************************************
 * Subroutine:	SCM_SysSleep
 * Function:	to set system into sleep mode 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SysSleep(void)
{
	SCB->SCR = 0x0;
	__WFI();
}

/****************************************************************************
 * Subroutine:	SCM_SysDeepSleep
 * Function:	to set system into sleep mode 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.01.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SysDeepSleep(void)
{
	SCB->SCR = 0x4;
	__WFI();
}



#endif /*SCM_H_*/
