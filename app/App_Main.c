/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_Main.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define App_Main_D
#include "App_Macro.h"

/*
****************************************************************************************************
** 名    称 ：App_Main_InitVariable()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_InitVariable (void)
{
	memset((u8 *)&ValSystemInfo,0x00,sizeof(ValSystemInfo));
}

/*
****************************************************************************************************
** 名    称 ：App_Main_SetSysState()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_SetSysState(SysState t_aSysState)
{
	ValSystemInfo.cSysState = t_aSysState;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_GetSysState()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
SysState App_Main_GetSysState(void)
{
	return ValSystemInfo.cSysState;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_EnterWork()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_EnterWork(void)
{	
	APP_Macro_PrintfStart();
	
	ValSystemInfo.cSysState = SysState_OnWork;
	ValSystemInfo.cOperateTime = 0;
	ValSystemInfo.m_RunAgingTest = 0;
	ValSystemInfo.m_Waiting = 0;
    
    API_CB_ClearScreen(0x00,1);

	LCD_FillPictrue(ValSystemInfo.m_LogoType);

	API_CB_LcdRefresh();

	App_Main_BatteryCharging(1);
	
}

/*
****************************************************************************************************
** 名    称 ：App_Main_SetOperateTimeOut()
** 功    能 ：500ms
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_SetOperateTimeOut(u16 t_rTimeout)
{
	ValSystemInfo.cOperateTime = t_rTimeout;
}

/*
*********************************************************************************************************
** 函数名称 ：App_Main_CheckBattery()
** 函数功能 ：         
** 入口参数 ：无
** 出口参数 ：无
** 返回值   ：无
*********************************************************************************************************
*/
u8 App_Main_CheckBattery(void)
{
    u32 cResult=0;
	UINT8 nADVal;
	double dTV;

	//DDEBUG("\n**************Run ADC test************\n");
	
	AOSS_ADCTransfer();
	
	nADVal = AOSS_ADCGetData();
	
	AOSS_ADCAd2Tv(ADC_BVD, nADVal, &dTV);
	
	//DDEBUG("Input voltage AD value is %d, voltage is %f mV\n", nADVal, dTV);	
    
    cResult = (u32)(dTV*(10+36))/10;
    
    if(cResult > BatteryLever1)
    {
        return 1;
    }
    else if(cResult > BatteryLever2)
    {
        return 2;
    }
    else if(cResult > BatteryLever3)
    {
        return 3;
    }
    else if(cResult >BatteryLever4)
    {
        return 4;
    }
    else if(cResult >BatteryLever5)
    {
        return 5;
    }
	else
	{
		return 6;
	}
}

/*
*********************************************************************************************************
** 函数名称 ：App_Main_BatterDisplay()
** 函数功能 ：         
** 入口参数 ：无
** 出口参数 ：无
** 返回值   ：无
*********************************************************************************************************
*/
void App_Main_BatterDisplay(u8 cLever)
{   
	if(cLever == 0)
	{
		return;
	}
	
	cLever--;
	
	if(cLever > ICON_BATTERY5)
	{
		cLever = ICON_BATTERY5;
	}
	
	LCD_FillIcon(cLever,1);
	
}

/*
*********************************************************************************************************
** 函数名称 ：App_Main_BatteryCharging()
** 函数功能 ：1s         
** 入口参数 ：无
** 出口参数 ：无
** 返回值   ：无
*********************************************************************************************************
*/
void App_Main_BatteryCharging(u8 t_aRefresh)
{
	static u8 cLeverDisplay=0;
	static u8 cCounts = 0;
	static u8 cDispRefresh = 0;
	static u8 cTimes = 0;

	if(t_aRefresh == 1)
	{
		cTimes = 0xFF;
	}
	else if(t_aRefresh == 2)
	{
		cDispRefresh = App_Main_CheckBattery();
		return;
	}
	else if(++cCounts < 2)
	{
		return;
	}
	
	cCounts = 0;

	cLeverDisplay = App_Main_CheckBattery();

	if(cLeverDisplay >= 6)
	{
		API_CB_ClearScreen(0x00,1);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, "Low Battery!", LCD_ALIGN_CENTER);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, "Power Off", LCD_ALIGN_CENTER);
		API_CB_LcdRefresh();
		
		API_CB_Delayms(2000);

		Dev_KeyPad_Shutdown();
	}
}

/*
****************************************************************************************************
** 名    称 ：App_Main_GetLockTimeState()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_GetLockTimeState(void)
{
	if(ValSystemInfo.cSysState == SysState_Lock)
	{
		return 1;
	}

	return 0;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_OnTimer()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_OnTimer(void)
{
    if(s_rTimeOut10ms)
    {
        s_rTimeOut10ms = 0;
    }
    
    if(s_rTimeOut100ms)
    {
        s_rTimeOut100ms = 0;
		
        if(ValSystemInfo.cLinghtTime)
        {
            if(--ValSystemInfo.cLinghtTime==0)
            {
                LCD_LightTime(LCD_BLingh_OFF,0);
            }
        }
		
        if(ValSystemInfo.cCloseDev == Power_Ready)
        {
            ValSystemInfo.cTimeStep++;
			             
            if(ValSystemInfo.cTimeStep <= 1)
            {
				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0, "Power Off...", LCD_ALIGN_CENTER);
				API_CB_LcdRefresh();

				ValSystemInfo.cSysState = SysState_PowerOff;
            }

			LCD_Progress(ValSystemInfo.cTimeStep,10);
						
            if(ValSystemInfo.cTimeStep >= 10)
            {
                Dev_KeyPad_Shutdown();
            }
        }

		App_Main_SaveKeyWordSel();
				
    }
    
    if(s_rTimeOut500ms)
    {
        s_rTimeOut500ms = 0;

		ValSystemInfo.cPowerOffTime++;

		if(ValSystemInfo.cPowerOffTime >= (5*60*2))
		{
			ValSystemInfo.cPowerOffTime = 0;
			
			Dev_KeyPad_Shutdown();       //断电
		}
		
        if(ValSystemInfo.cDisplayTime)
        {
            if(--ValSystemInfo.cDisplayTime == 0)
            {
                App_Main_EnterWork();
            }
        }
		
        if(ValSystemInfo.cOperateTime)
        {
            if(--ValSystemInfo.cOperateTime == 0)
            {
				if(ValSystemInfo.cSysState == SysState_OpenKeyPad)
				{
					App_Key_ClearKbDigitListBackUp();
				}

				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0, "Timeout", LCD_ALIGN_CENTER);
				API_CB_LcdRefresh();
				
				ValSystemInfo.cDisplayTime = 6;
				ValSystemInfo.cSysState = SysState_Waiting;
            }
        }

		if(ValSystemInfo.cSysState == SysState_ReStart)
		{
			App_DownLoad_RunBoot(0);
		}

		App_Main_DiplayBtChar();

		App_Main_AgingTest();

		App_Main_BatteryCharging(0);

		App_Main_CheckLockTime();

    }
}

/*
****************************************************************************************************
** 名    称 ：App_Main_DisplaySysInfo()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_DisplaySysInfo(void)
{
	DEVICEINFO uDeviceInfo;
	char aDisp[16+1];
	u8 aTemp[6];

	API_CB_ClearScreen(0x00,1);
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, "SN", LCD_ALIGN_CENTER);
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, "MAC", LCD_ALIGN_CENTER);
	
	if(App_Manage_ReadDevInfo(&uDeviceInfo))
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, "NULL", LCD_ALIGN_CENTER);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0, "NULL", LCD_ALIGN_CENTER);
		API_CB_LcdRefresh();
		
		return;
	}
	else
	{
		APP_Math_BcdToAsc_2(uDeviceInfo.Sn,sizeof(uDeviceInfo.Sn)*2,aDisp);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, aDisp, LCD_ALIGN_CENTER);
	}

	memxcpy(aTemp,uDeviceInfo.BtMac,sizeof(aTemp));
	APP_Math_BcdToAsc_2(aTemp,sizeof(aTemp)*2,aDisp);
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0, aDisp, LCD_ALIGN_CENTER);
	API_CB_LcdRefresh();
	
}

/*
****************************************************************************************************
** 名    称 ：App_Main_AgingTest()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_AgingTest(void)
{	
	if(ValSystemInfo.m_RunAgingTest == 0x5A)
	{
		switch(ValSystemInfo.m_AgingTestStat)
		{
			case 0:
			{
				API_CB_ClearScreen(0xFF,1);
				API_CB_LcdRefresh();
			}
			break;

			case 2:
			{
				API_CB_ClearScreen(0x00,1);
				API_CB_LcdRefresh();
			}
			break;

			case 4:
			{
				u8 aTemp[4];
				
				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"LCD", LCD_ALIGN_LEFT);
				API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"√", LCD_ALIGN_RIGHT);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0,"BUTTON", LCD_ALIGN_LEFT);
				API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE2,0, 0,"√", LCD_ALIGN_RIGHT);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0,"FLASH", LCD_ALIGN_LEFT);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,"BLUETOOTH", LCD_ALIGN_LEFT);
								
				if(SF_Read(FLASH_BASE_TEST_ADDR,aTemp,4) != FALSE)
				{
					API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE3,0, 0,"√", LCD_ALIGN_RIGHT);
				}
				else
				{
					API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE3,0, 0,"×", LCD_ALIGN_RIGHT);
				}
				
				if(ValSystemInfo.m_BtState)
				{
					API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE4,0, 0,"√", LCD_ALIGN_RIGHT);
				}
				else
				{
					API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE4,0, 0,"×", LCD_ALIGN_RIGHT);
				}

				API_CB_LcdRefresh();
			}
			break;

			case 6:
			{
				u8 aDesKey[16];
				u8 aTemp[16];

				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0,"Encrypted data.", LCD_ALIGN_LEFT);
				API_CB_LcdRefresh();
				
				API_CB_GetRandom(aDesKey,16);
				API_CB_GetRandom(aTemp,16);

				API_CB_DESProcess(aDesKey,16,aTemp,16,DES_ENCRYPT);
			}
			break;

			default:
				break;
		}

		ValSystemInfo.cPowerOffTime = 0;
		ValSystemInfo.m_AgingTestStat++;

		if(ValSystemInfo.m_AgingTestStat >= 8)
		{
			ValSystemInfo.m_AgingTestStat = 0;
		}
	}
}
/*
****************************************************************************************************
** 名    称 ：App_Main_OnMenuSel()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_OnMenuSel(u8 t_aKeyPadVal)
{
	if(t_aKeyPadVal == Dev_KeyPad_Down)
	{
		ValSystemInfo.m_MenuSel++;

		if(ValSystemInfo.m_MenuSel > 2)
		{
			ValSystemInfo.m_MenuSel = 0;
		}

		API_CB_ClearScreen(0x00,0);
		
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, "1.Hardware Test", ValSystemInfo.m_MenuSel == 0 ? LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR : LCD_ALIGN_LEFT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, "2.Aging Test", ValSystemInfo.m_MenuSel == 1 ? LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR : LCD_ALIGN_LEFT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, "3.Quit", ValSystemInfo.m_MenuSel == 2 ? LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR : LCD_ALIGN_LEFT);
		API_CB_LcdRefresh();
	}
	else if(t_aKeyPadVal == Dev_KeyPad_Up)
	{
		ValSystemInfo.cSysState = SysState_HardTest;
		ValSystemInfo.cSysState += ValSystemInfo.m_MenuSel;

		if(ValSystemInfo.cSysState == SysState_HardTest)
		{
			API_CB_ClearScreen(0x00,1);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, "Hardware Test", LCD_ALIGN_CENTER);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0, "running the test?", LCD_ALIGN_CENTER);
			LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
			API_CB_LcdRefresh();

			ValSystemInfo.cSysState = SysState_Test_Lcd1;
		}
		else if(ValSystemInfo.cSysState == SysState_AgingTest)
		{
			API_CB_ClearScreen(0x00,1);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, "Aging Test", LCD_ALIGN_CENTER);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,8, 0, "running the test?", LCD_ALIGN_CENTER);
			LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
			API_CB_LcdRefresh();
			
			ValSystemInfo.cSysState = SysState_Test_Aging;
		}
		else if(ValSystemInfo.cSysState == SysState_Quit)
		{
			App_Main_EnterWork();
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：App_Main_TestFlash()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_TestFlash(u8 t_aKeyPadVal)
{
	u8 aTemp[4];
	s32 ret = 0;
	
	if(t_aKeyPadVal == Dev_KeyPad_Down)
	{
		do
		{
			memset(aTemp,0xFF,sizeof(aTemp));

			if(SF_Write(FLASH_BASE_TEST_ADDR/PC_DL_BLOCK_SIZE,0,aTemp,4,1) == FALSE)
			{
				ret = 1;
				break;
			}
			
			if(SF_Read(FLASH_BASE_TEST_ADDR,aTemp,4) == FALSE)
			{
				ret = 2;
				break;
			}

			if(memcmp(aTemp,"\xFF\xFF\xFF\xFF",4))
			{
				ret = 3;
				break;
			}

			memcpy(aTemp,"\x55\xAA\xAA\x55",4);

			if(SF_Write(FLASH_BASE_TEST_ADDR/PC_DL_BLOCK_SIZE,0,aTemp,4,1) == FALSE)
			{
				ret = 4;
				break;
			}
			
			if(SF_Read(FLASH_BASE_TEST_ADDR,aTemp,4) == FALSE)
			{
				ret = 5;
				break;
			}

			if(memcmp(aTemp,"\x55\xAA\xAA\x55",4))
			{
				ret = 6;
				break;
			}
		}while(0);

		API_CB_ClearScreen(0x00,1);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"LCD", LCD_ALIGN_LEFT);
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"√", LCD_ALIGN_RIGHT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0,"BUTTON", LCD_ALIGN_LEFT);
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE2,0, 0,"√", LCD_ALIGN_RIGHT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0,"FLASH", LCD_ALIGN_LEFT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,"BLUETOOTH", LCD_ALIGN_LEFT);

		DDEBUG("ret = %d",ret);

		if(ret == 0)
		{
			API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE3,0, 0,"√", LCD_ALIGN_RIGHT);
		}
		else
		{
			API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE3,0, 0,"×", LCD_ALIGN_RIGHT);
		}
		
		if(ValSystemInfo.m_BtState)
		{
			API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE4,0, 0,"√", LCD_ALIGN_RIGHT);
		}
		else
		{
			API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE4,0, 0,"×", LCD_ALIGN_RIGHT);
		}
		
		API_CB_LcdRefresh();
		
		ValSystemInfo.cSysState = SysState_Waiting;
		
	}
}
/*
****************************************************************************************************
** 名    称 ：App_Main_WaitForDevInitCommit()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_WaitForDevInitCommit(u8 t_aKeyPadVal)
{
	if(t_aKeyPadVal != Dev_KeyPad_Down)
	{
		return;
	}
	
	if(App_Key_TDKCrypt(KEY_DECRYPT,(u8 *)g_KeyWord,API_CB_KEY_WORD_MAX))
	{
		memset(g_KeyWord,0x00,sizeof(g_KeyWord));
		
		App_ComPro_SendResponseData(NULL,0,COMPRO_RESPONSE_CRYPT_ERR,0);
		
		return;
	}

	if(App_Math_ValUint16((u8 *)&g_KeyWord[API_CB_KEY_WORD_MAX]) != App_Crc16_Get((u8 *)g_KeyWord,API_CB_KEY_WORD_MAX))
	{
		memset(g_KeyWord,0x00,API_CB_KEY_WORD_MAX);
		
		App_ComPro_SendResponseData(NULL,0,COMPRO_RESPONSE_CRC_ERR,0);
		
		return;
	}
		
	if(App_Math_MnemonicDispByOne(&ValSystemInfo.m_WordOffSet,&ValSystemInfo.m_WordPage,g_KeyWord))
	{
		API_CB_ClearScreen(0x00,1);
		
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"\xA3\xA1", LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);		
		
		if(ValSystemInfo.m_WordTimes == 0)
		{
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE1,0, 20,"Please check the  seed.", LCD_ALIGN_LEFT|LCD_ALIGN_AUTO|LCD_WRAP_AUTO);
			LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 8,"Next > ", LCD_ALIGN_RIGHT);
			ValSystemInfo.m_WordOffSet = 0;
			ValSystemInfo.m_WordPage = 0;
			ValSystemInfo.m_WordTimes++;
		}
		else 
		{
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE1,0, 20,"Check completion", LCD_ALIGN_LEFT|LCD_ALIGN_AUTO|LCD_WRAP_AUTO);
			LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
			API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
			ValSystemInfo.cSysState = SysState_WaitForDevInitCommit2;
		}

		API_CB_LcdRefresh();
	}
	
	if(App_Key_TDKCrypt(KEY_ENCRYPT,(u8 *)g_KeyWord,API_CB_KEY_WORD_MAX))
	{
		memset(g_KeyWord,0x00,sizeof(g_KeyWord));
		
		App_ComPro_SendResponseData(NULL,0,COMPRO_RESPONSE_CRYPT_ERR,0);
		
		return;
	}
	
}
/*
****************************************************************************************************
** 名    称 ：App_Main_OnKeyPad()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_OnKeyPad(void)
{
	u8 aKeyPadVal;

	aKeyPadVal = Dev_KeyPad_ReadKeyPad();

	if(aKeyPadVal == 0)
	{
		return;
	}

	DDEBUG("Dev_KeyPad_ReadKeyPad = %d",aKeyPadVal);

	if(App_Main_GetLockTimeState())
	{
		return;
	}

	LCD_LightTime(LCD_BLingh_ON,0);

	ValSystemInfo.cPowerOffTime = 0;

	if(ValSystemInfo.cCloseDev == Power_Off)
	{
		u8 cTemp[2];

		Dev_KeyPad_PowerOn();

		cTemp[0] = ValSystemInfo.m_BtState;
		cTemp[1] = ValSystemInfo.m_BlePinComple;

		App_Main_InitVariable();

		App_Main_SetLogoType();

		ValSystemInfo.m_BtState = cTemp[0];
		ValSystemInfo.m_BlePinComple = cTemp[1];

		LCD_init();

		App_Main_EnterWork();

		return;
	}
	
	switch(ValSystemInfo.cSysState)
	{
		case SysState_OnWork:
		{
			if(aKeyPadVal == Dev_KeyPad_DownAndDown)
			{
				DDEBUG("Dev_KeyPad_DownAndDown");

				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, "1.Hardware Test", LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, "2.Aging Test", LCD_ALIGN_LEFT);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, "3.Quit", LCD_ALIGN_LEFT);
				API_CB_LcdRefresh();

				ValSystemInfo.cSysState = SysState_OnMenuSel;
				ValSystemInfo.m_MenuSel = 0;

				//bt_clear_all_keys();
			}
			else if(aKeyPadVal == Dev_KeyPad_Up)
			{
				ValSystemInfo.cSysState = SysState_SysInfo;
				
				App_Main_DisplaySysInfo();
			}
		}
		break;

		case SysState_ReStart:
		{
			if(aKeyPadVal == Dev_KeyPad_Up)
			{
				App_DownLoad_RunBoot(0);
			}
			else if(aKeyPadVal == Dev_KeyPad_Down)
			{
				App_Main_EnterWork();
			}
		}
		break;

		case SysState_WaitForBtCommit:
		{
			u8 aData[8];
			
			if(aKeyPadVal == Dev_KeyPad_Up)
			{				
				aData[7] = 0x00;
			}
			else if(aKeyPadVal == Dev_KeyPad_Down)
			{
				aData[7] = 0x01;
			}
			
			Dev_Bt_GetComfirmRsp(aData);
			
			Dev_Bt_ResponeCmd(HCI_CMD_USER_CONFIRM_RSP, aData, 8);

			App_Main_EnterWork();
		}
		break;
		
		case SysState_OnMenuSel:
		{
			App_Main_OnMenuSel(aKeyPadVal);
		}
		break;
		
		case SysState_Test_Aging:
		{
			if(aKeyPadVal == Dev_KeyPad_Up)
			{
				ValSystemInfo.m_RunAgingTest = 0x5A;
				ValSystemInfo.m_AgingTestStat = 0;
			}
			else if(aKeyPadVal == Dev_KeyPad_Down)
			{
				App_Main_EnterWork();
			}
		}
		break;
		
		case SysState_Test_Lcd1:
		{
			if(aKeyPadVal == Dev_KeyPad_Up)
			{
				API_CB_ClearScreen(0xFF,1);
				API_CB_LcdRefresh();
				ValSystemInfo.cSysState = SysState_Test_Lcd2;
			}
			else if(aKeyPadVal == Dev_KeyPad_Down)
			{
				App_Main_EnterWork();
			}
		}
		break;
		
		case SysState_Test_Lcd2:
		{
			API_CB_ClearScreen(0x00,1);
			API_CB_LcdRefresh();
			ValSystemInfo.cSysState = SysState_Test_Key1;
		}
		break;

		case SysState_Test_Key1:
		{
			API_CB_ClearScreen(0x00,1);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"press the button", LCD_ALIGN_CENTER);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,LCD_FONT_ASCII_8X16_LINE/2, 0,"<", LCD_ALIGN_CENTER);
			API_CB_LcdRefresh();
			ValSystemInfo.cSysState = SysState_Test_Key2;
		}
		break;

		case SysState_Test_Key2:
		{
			if(aKeyPadVal == Dev_KeyPad_Up)
			{
				API_CB_ClearScreen(0x00,1);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"press the button", LCD_ALIGN_CENTER);
				API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,LCD_FONT_ASCII_8X16_LINE/2, 0,">", LCD_ALIGN_CENTER);
				API_CB_LcdRefresh();
				ValSystemInfo.cSysState = SysState_Test_Flash;
			}
		}
		break;

		case SysState_Test_Flash:
		{
			App_Main_TestFlash(aKeyPadVal);
		}
		break;
		
		case SysState_WaitForDevInitCommit1:
		{
			App_Main_WaitForDevInitCommit(aKeyPadVal);
		}
		break;

		case SysState_WaitForDevInitCommit2:
		{
			App_Device_SendRespInit(aKeyPadVal);
		}
		break;

		case SysState_WaitForPinModifyCommit:
		{
			App_Device_SendRespPassWordModify(aKeyPadVal);
		}
		break;

		case SysState_WaitForSetFactoryCommit:
		{
			App_Device_SendRespSetFactory(aKeyPadVal);
		}
		break;
		
		case SysState_WaitForTradeCommit1:
		{
			App_Wallet_SendRespSign1(aKeyPadVal);
		}
		break;

		case SysState_WaitForTradeCommit2:
		{
			App_Wallet_SendRespSign2(aKeyPadVal);
		}
		break;

		case SysState_WaitForKeyWord:
		{
			if(aKeyPadVal == Dev_KeyPad_Down)
			{
				ValSystemInfo.m_KeyWordSelNum++;

				if(ValSystemInfo.m_KeyWordSelNum >= sizeof(c_cWordList))
				{
					ValSystemInfo.m_KeyWordSelNum = 0;
				}
			}
			else
			{
				if(ValSystemInfo.m_KeyWordSelNum)
				{
					ValSystemInfo.m_KeyWordSelNum--;
				}
				else
				{
					ValSystemInfo.m_KeyWordSelNum = sizeof(c_cWordList) -1;
				}
			}
			
			ValSystemInfo.m_KeyWordSelTime = 12;
			
			App_Main_DispSelectWord(ValSystemInfo.m_KeyWordSelNum,0,NULL);
		}
		break;

		case SysState_PowerOff:
		case SysState_WaitForDownLoad:
		case SysState_OpenKeyPad:
		break;
		
		default:
		{
			if(aKeyPadVal == Dev_KeyPad_Down)
			{
				App_Main_EnterWork();
			}
		}
		break;
	}
}

/*
****************************************************************************************************
** 名    称 ：App_Main_WaitingForConfirm()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_WaitingForConfirm(u32 t_nTimeOut,u8 t_aStyle)
{
	u8 aKeyPadVal;
	u32 nTimerID;

	if(t_aStyle == MAIN_DISP_STYLE_CHAR)
	{
		LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
		API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
	}
	else if(t_aStyle == MAIN_DISP_STYLE_NEXT)
	{
		LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
		API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 8,"Next > ", LCD_ALIGN_RIGHT);
	}
	else if(t_aStyle == MAIN_DISP_STYLE_ARROW)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"<", LCD_ALIGN_RIGHT);
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,">", LCD_ALIGN_RIGHT);
	}
	else if(t_aStyle == MAIN_DISP_STYLE_ONLYUP)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"<", LCD_ALIGN_RIGHT);
	}
	else if(t_aStyle == MAIN_DISP_STYLE_ONLYDOWN)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,">", LCD_ALIGN_RIGHT);
	}	
	else if(t_aStyle == MAIN_DISP_STYLE_TICK)
	{
		//×√
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"√", LCD_ALIGN_RIGHT);
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE4,0, 0,"×", LCD_ALIGN_RIGHT);
	}

	API_CB_LcdRefresh();
	
	nTimerID = Dev_Timer_GetTick();
	
	while(1)
	{
        if(t_nTimeOut && Dev_Timer_IsTimeOut(nTimerID, t_nTimeOut))
        {
			 DDEBUG("Dev_Timer_IsTimeOut");
            return 2;
        }
		
		aKeyPadVal = Dev_KeyPad_ReadKeyPad();

		if(aKeyPadVal)
		{
			LCD_LightTime(LCD_BLingh_ON,0);

			if(aKeyPadVal == Dev_KeyPad_Up)
			{
				if(t_aStyle == MAIN_DISP_STYLE_ONLYDOWN || t_aStyle == MAIN_DISP_STYLE_NEXT)
				{
					continue;
				}
				
				DDEBUG("aKeyPadVal == Dev_KeyPad_Up");
				return 0;
			}

			if(aKeyPadVal == Dev_KeyPad_Down)
			{
				if(t_aStyle == MAIN_DISP_STYLE_ONLYUP)
				{
					continue;
				}
				
				DDEBUG("aKeyPadVal == Dev_KeyPad_Down");
				return 1;
			}
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：App_Main_DisPlayTradeNum()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_DisPlayTradeNum(u8 *t_paCoinType,u8 *t_paNum)
{
	u8 aDisp[17];

	APP_Macro_PrintfStart();

	if(strlen((char *)t_paCoinType)> 16)
	{
		DDEBUG("strlen((char *)t_paCoinType)> 16");
		return 1;
	}
	
	if(strlen((char *)t_paNum)> 16)
	{
		DDEBUG("strlen((char *)t_paNum)> 16");
		return 2;
	}
	
	API_CB_ClearScreen(0x00,1);
	
	memset(aDisp,0x00,sizeof(aDisp));
	strcpy((char *)aDisp,(char *)t_paNum);
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, (char *)aDisp, LCD_ALIGN_CENTER);

	memset(aDisp,0x00,sizeof(aDisp));
	strcpy((char *)aDisp,(char *)t_paCoinType);
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, (char *)aDisp, LCD_ALIGN_CENTER);

	API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"\xA3\xA1", LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);	
	API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE1,0, 20,"Really send from  your wallet?", LCD_ALIGN_LEFT|LCD_ALIGN_AUTO|LCD_WRAP_AUTO);
	LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
	API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
	API_CB_LcdRefresh();
	
	return 0;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_DisPlayPubAddress()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_DisPlayPubAddress(u8 *t_paMsg,u16 t_rMsgLen)
{
	u8 aDisp[64];
	
	if(t_rMsgLen > DEVICE_COINADDRESS_MAXLEN || t_rMsgLen == 0)
	{
		return 1;
	}

	memset(aDisp,0x00,sizeof(aDisp));

	memcpy(aDisp,t_paMsg,t_rMsgLen);

	API_CB_ClearScreen(0x00,1);
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0, (char *)aDisp, LCD_ALIGN_LEFT|LCD_WRAP_AUTO);
	LCD_FillLine(LCD_FONT_ASCII_5X8,LCD_LINE7,0x10);
	API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE8,0, 0," Confirm <  Cancel > ", LCD_ALIGN_LEFT);
	API_CB_LcdRefresh();
	
	return 0;

}

/*
****************************************************************************************************
** 名    称 ：App_Math_MnemonicDisp()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
s32 App_Math_MnemonicDisp(char *t_paWord)
{
	int i,j,k,n;
	char p[API_CB_KEY_WORD_MAX+1],*pDisp;
	u32 nWordLen;
	char aNum[4];
	s32 iRet = 0;

	nWordLen = strlen(t_paWord);

	if(API_CB_KEY_WORD_MAX < nWordLen)
	{
		return 1;
	}

	strcpy(p,t_paWord);

	p[nWordLen] = ' ';
	p[nWordLen+1] = '\0';

	nWordLen++;

	pDisp = p;

	memset(aNum,0x00,sizeof(aNum));

	API_CB_ClearScreen(0x00,1);

	for(i=0,j=0;i<nWordLen;i++)
	{
		if(p[i] == ' ')
		{
			p[i] = '\0';

			//DDEBUG("i = %d , j = %d",i,j);

			//DDEBUG("pDisp= %s",pDisp);

			memset(aNum,0x00,sizeof(aNum));

			if((j+1)<10)
			{
				aNum[0] = (j+1)+'0';
				aNum[1] = '.';
				k=16+4;
			}
			else
			{
				aNum[0] = (j+1)/10+'0';
				aNum[1] = (j+1)%10+'0';
				aNum[2] = '.';
				k=16+4+8;
			}

			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, 0,aNum, LCD_ALIGN_LEFT);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, k,pDisp, LCD_ALIGN_LEFT);
			API_CB_LcdRefresh();
			
			p[i] = ' ';

			pDisp=&p[i]+1;

			j++;

			if(j%4 == 0)
			{
				if(j>4)
				{
					iRet = App_Main_WaitingForConfirm(0,MAIN_DISP_STYLE_ARROW);
				}
				else
				{
					iRet = App_Main_WaitingForConfirm(0,MAIN_DISP_STYLE_ONLYDOWN);
				}
				
				if(iRet == 0)
				{
					if(j>=8)
					{
						j-=8;

						if(j == 0)
						{
							i=0;
							pDisp = p;
						}
						else
						{
							for(i=0,n=0;i<nWordLen;i++)
							{
								if(p[i] == ' ')
								{
									n++;

									if(n>=j)
									{
										pDisp=&p[i]+1;
										break;
									}
								}
							}
						}
					}
					else
					{
						j=0;
						i=0;
						pDisp = p;
					}
				}
				else if(iRet == 3)
				{
					App_Main_EnterWork();
					return 2;
				}
				
				API_CB_ClearScreen(0x00,1);
			}
		}
	}

	//DDEBUG("p= %s",p);

	if(App_Main_WaitingForConfirm(0,MAIN_DISP_STYLE_NEXT) == 3)
	{
		App_Main_EnterWork();
		return 3;
	}

	pDisp = p;

	memset(aNum,0x00,sizeof(aNum));

	API_CB_ClearScreen(0x00,1);

	for(i=0,j=0;i<nWordLen;i++)
	{
		if(p[i] == ' ')
		{
			p[i] = '\0';

			//DDEBUG("i = %d , j = %d",i,j);

			//DDEBUG("pDisp= %s",pDisp);

			memset(aNum,0x00,sizeof(aNum));

			if((j+1)<10)
			{
				aNum[0] = (j+1)+'0';
				aNum[1] = '.';
				k=16+4;
			}
			else
			{
				aNum[0] = (j+1)/10+'0';
				aNum[1] = (j+1)%10+'0';
				aNum[2] = '.';
				k=16+4+8;
			}

			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, 0,aNum, LCD_ALIGN_LEFT);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, k,pDisp, LCD_ALIGN_LEFT);
			API_CB_LcdRefresh();
			
			p[i] = ' ';

			pDisp=&p[i]+1;

			j++;

			if(j%4 == 0)
			{
				if(j>4)
				{
					iRet = App_Main_WaitingForConfirm(0,MAIN_DISP_STYLE_ARROW);
				}
				else
				{
					iRet = App_Main_WaitingForConfirm(0,MAIN_DISP_STYLE_ONLYDOWN);
				}
				
				if(iRet == 0)
				{
					if(j>=8)
					{
						j-=8;

						if(j == 0)
						{
							i=0;
							pDisp = p;
						}
						else
						{
							for(i=0,n=0;i<nWordLen;i++)
							{
								if(p[i] == ' ')
								{
									n++;

									if(n>=j)
									{
										pDisp=&p[i]+1;
										break;
									}
								}
							}
						}
					}
					else
					{
						j=0;
						i=0;
						pDisp = p;
					}
				}
				else if(iRet == 3)
				{
					App_Main_EnterWork();
					return 4;
				}
				
				API_CB_ClearScreen(0x00,1);
			}
		}
	}

	if(App_Main_WaitingForConfirm(MAIN_TIMEOUT_DEFAULT,MAIN_DISP_STYLE_CHAR))
	{
		App_Main_EnterWork();
		return 5;
	}

	App_Main_EnterWork();
	
	return 0;
}

/*
****************************************************************************************************
** 名    称 ：App_Math_MnemonicDispOnce()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
s32 App_Math_MnemonicDispOnce(u8 *t_pnOffSet,u8 *t_pnPage,char *t_paWord)
{
	u8 i,j,k;
	char p[API_CB_KEY_WORD_MAX+1],*pDisp;
	u8 nWordLen;
	char aNum[4];
	
	if(API_CB_KEY_WORD_MAX < strlen(t_paWord))
	{
		return 1;
	}

	nWordLen = strlen(t_paWord);

	strcpy(p,t_paWord);

	p[nWordLen] = ' ';
	p[nWordLen+1] = '\0';

	nWordLen++;

	pDisp = &p[*t_pnOffSet];


	memset(aNum,0x00,sizeof(aNum));

	API_CB_ClearScreen(0x00,1);

	for(i=*t_pnOffSet,j=*t_pnPage;i<nWordLen;i++)
	{
		if(p[i] == ' ')
		{
			p[i] = '\0';

			memset(aNum,0x00,sizeof(aNum));

			if((j+1)<10)
			{
				aNum[0] = (j+1)+'0';
				aNum[1] = '.';
				k=16+4;
			}
			else
			{
				aNum[0] = (j+1)/10+'0';
				aNum[1] = (j+1)%10+'0';
				aNum[2] = '.';
				k=16+4+8;
			}

			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, 0,aNum, LCD_ALIGN_LEFT);
			API_CB_FillRowRam(LCD_FONT_ASCII_8X16,(j%4),0, k,pDisp, LCD_ALIGN_LEFT);
			API_CB_LcdRefresh();
			
			p[i] = ' ';

			pDisp=&p[i]+1;

			j++;

			if(j%4 == 0)
			{
				if(j>4)
				{
					API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE1,0, 0,"<", LCD_ALIGN_RIGHT);
					API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,">", LCD_ALIGN_RIGHT);
				}
				else
				{
					API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0,">", LCD_ALIGN_RIGHT);
				}

				API_CB_LcdRefresh();

				*t_pnOffSet = i+1;
				*t_pnPage = j;

				memset(p,0x00,sizeof(p));

				return 0;
			}
		}
	}

	memset(p,0x00,sizeof(p));
	
	return 1;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_DispSelectWord()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_DispSelectWord(u8 t_aOffset,u8 t_aLine1,char *t_pcSelectWord)
{
	char aDisp[17];
	char aTemp[2];

	if(t_aOffset > sizeof(c_cWordList))
	{
		return 1;
	}

	memset(aDisp,0x00,sizeof(aDisp));
	memset(aTemp,0x00,sizeof(aTemp));

	if(t_aLine1)
	{
		API_CB_ClearScreen(0x00,1);
		API_CB_FillRowRam(LCD_FONT_CHINESE_16X16,LCD_LINE1,0, 0,"\xA3\xA1", LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);
		API_CB_FillRowRam(LCD_FONT_ASCII_5X8,LCD_LINE1,0, 20, "Please enter the  word:", LCD_ALIGN_LEFT|LCD_ALIGN_AUTO|LCD_WRAP_AUTO);
	}

	if(t_pcSelectWord != NULL)
	{		
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE2,0, 0, t_pcSelectWord, LCD_ALIGN_CENTER|LCD_CLEAR_LINE);
	}
	
	memcpy((u8 *)aDisp,(u8 *)c_cWordList,(MAX_DOT_COL/LCD_FONT_ASCII_8X16_COLUMN));

	if(t_aOffset < 16)
	{
		aTemp[0] = aDisp[t_aOffset];
		aDisp[t_aOffset] = ' ';
	}
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 0, aDisp, LCD_ALIGN_LEFT|LCD_CLEAR_LINE);

	if(aTemp[0] != 0)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE3,0, 8*t_aOffset, aTemp, LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);
		memset(aTemp,0x00,sizeof(aTemp));
	}

	memset(aDisp,0x00,sizeof(aDisp));
	
	memcpy((u8 *)aDisp,&c_cWordList[(MAX_DOT_COL/LCD_FONT_ASCII_8X16_COLUMN)],sizeof(c_cWordList)-(MAX_DOT_COL/LCD_FONT_ASCII_8X16_COLUMN));

	if(t_aOffset >= 16)
	{
		t_aOffset -= 16;
		aTemp[0] = aDisp[t_aOffset];
		aDisp[t_aOffset] = ' ';
	}
	
	API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 0, aDisp, LCD_ALIGN_LEFT|LCD_CLEAR_LINE);

	if(aTemp[0] != 0)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0, 8*t_aOffset, aTemp, LCD_ALIGN_LEFT|LCD_STYLE_BACKCOLOR);
	}

	API_CB_LcdRefresh();

	return 0;
}	

/*
****************************************************************************************************
** 名    称 ：App_Main_FirmwareVer()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_FirmwareVer(void)
{
	#ifdef DEBUG_OPEN
	UINT16 Version;
	
    Version = FWLIB_GetVersion();	// Get Firmware Version

	DDEBUG("System Starting ...");
	DDEBUG("System Clock = %d Hz ",SCM_GetSysClk());
	DDEBUG("FWLIB_Version is V%d%d.%d%d",(Version>>12)&0xf,(Version>>8)&0xf,(Version>>4)&0xf,Version&0xf);
	#endif
}

/*
****************************************************************************************************
** 名    称 ：App_Main_OnWaitting()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_Main_OnWaitting(SENDCOMData *t_puSendData)
{
	if(ValSystemInfo.cSysState >= SysState_WaitForDevInitCommit1 && ValSystemInfo.cSysState <= SysState_WaitForKeyWord)
	{
		if(t_puSendData->m_aType != COMPRO_TYPE_MANAGE ||
			(t_puSendData->m_aCMD != COMPRO_CMD_MANAGE_CANCEL && t_puSendData->m_aCMD != COMPRO_CMD_MANAGE_GETRAND))
		{
			App_ComPro_SetResErr(t_puSendData,COMPRO_RESPONSE_WAITTING);
			DDEBUG("COMPRO_RESPONSE_WAITTING");
			
			return 1;
		}
	}

	return 0;
}

/*
****************************************************************************************************
** 名    称 ：App_Main_OnStart()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_OnStart(void)
{	
	App_Main_InitVariable();

	Dev_KeyPad_KeyPadInit();

	Dev_Uart_Init();

	App_Main_FirmwareVer();
	
	InitAlgSram(AlgSramBuf); 

	Trng_Init(0);

	SF_Init(DL_KEY_PAGE_START,DL_KEY_PAGE_END);

	EFC_ConfigCodeSz((DL_APP_Add_START+DL_APP_SIZE_MAX)/DL_KEY_PAGE_SIZE);
	SCM_SetCacheRegion0(0, DL_APP_Add_START);

	Spi_Init();
	
	Dev_Timer_Init();

	Dev_Adc_Init();

	App_Key_Check();

	App_Manage_CheckingDevInfo();

	__enable_irq();

	LCD_init();
		
	if(Dev_Bt_Init() == 0)
	{
		DDEBUG("ValSystemInfo.m_BtState = 1");
		ValSystemInfo.m_BtState = 1;
	}

	App_Main_SetLogoType();

	App_Main_BatteryCharging(2);
	
	App_Main_EnterWork();
		
}

/*
****************************************************************************************************
** 名    称 ：App_Main_Run()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_Main_Run(void)
{
	App_Main_OnStart();
	
	while( 1 )
	{ 		
		App_Main_OnTimer();

		App_Main_OnKeyPad();
		
		App_ComPro_ComDeal();
	}
}

/****************************************End Of File************************************************
***************************************************************************************************/
