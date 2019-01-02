/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_ComPro.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define App_ComPro_D
#include "App_Macro.h"

static u16 s_rDataLen;
static u16 s_rBufLen;

/*
****************************************************************************************************
** 名    称 ：App_ComPro_SetResErr()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_SetResErr(SENDCOMData *t_puSendData,u16 t_rResponse)
{
	t_puSendData->m_aLen[0] = 0x00;
	t_puSendData->m_aLen[1] = 0x02;
	t_puSendData->m_aData[0] = (u8)(t_rResponse>>8);
	t_puSendData->m_aData[1] = (u8)t_rResponse;
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_SetResSuccess()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_SetResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen)
{
	t_rDataLen +=2;
	t_puSendData->m_aLen[0] = (u8)(t_rDataLen>>8);
	t_puSendData->m_aLen[1] = (u8)t_rDataLen;
	t_puSendData->m_aData[0] = (u8)(COMPRO_RESPONSE_SUCCESS>>8);
	t_puSendData->m_aData[1] = (u8)COMPRO_RESPONSE_SUCCESS;

	DDEBUGHEX("COMPRO_RESPONSE_SUCCESS = ",t_puSendData->m_aData,2);
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_EncryptResSuccess()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_EncryptResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen,u8 *t_paRand)
{
	u16 rLen = 0;
	
	t_rDataLen +=2;
	t_puSendData->m_aData[0] = (u8)(COMPRO_RESPONSE_SUCCESS>>8);
	t_puSendData->m_aData[1] = (u8)COMPRO_RESPONSE_SUCCESS;


	//MAC
	t_rDataLen +=4;

	rLen = ((t_rDataLen+7)/8)*8;

	t_puSendData->m_aLen[0] = (u8)(rLen>>8);
	t_puSendData->m_aLen[1] = (u8)rLen;

	//MAC
	App_Key_MAKCrypt(&t_puSendData->m_aType,t_rDataLen+4,&t_puSendData->m_aData[t_rDataLen-4],t_paRand);
	
	//TSK
	App_Key_TSKCrypt(KEY_ENCRYPT,t_puSendData->m_aData,rLen,t_paRand);

}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_MacResSuccess()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_MacResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen,u8 *t_paRand)
{
	t_rDataLen +=2;
	t_puSendData->m_aData[0] = (u8)(COMPRO_RESPONSE_SUCCESS>>8);
	t_puSendData->m_aData[1] = (u8)COMPRO_RESPONSE_SUCCESS;
	
	//MAC
	t_rDataLen +=4;

	t_puSendData->m_aLen[0] = (u8)(t_rDataLen>>8);
	t_puSendData->m_aLen[1] = (u8)t_rDataLen;
	
	//MAC
	App_Key_MAKCrypt(&t_puSendData->m_aType,t_rDataLen,&t_puSendData->m_aData[t_rDataLen-4],t_paRand);
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_CmdDeal()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_CmdDeal(STRPROTOCOL *t_puProtocolData,RECCOMData *t_puReceiveData)
{
	SENDCOMData uSendData;

	APP_Macro_PrintfStart();

	memset((u8 *)&uSendData,0x00,sizeof(uSendData));

	uSendData.m_aType = t_puReceiveData->m_aType;
	uSendData.m_aCMD = t_puReceiveData->m_aCMD;

	if(App_Main_GetSysState() == SysState_PowerOff)
	{
		App_ComPro_SetResErr(&uSendData,COMPRO_RESPONSE_POWEROFF);

		App_ComPro_SendData(t_puProtocolData,&uSendData);
		
		memset((u8 *)&uSendData,0x00,sizeof(uSendData));
		
		return;
	}
	
	if(App_Main_OnWaitting(&uSendData) == 0)
	{
		switch(t_puReceiveData->m_aType)
		{
			case COMPRO_TYPE_HANDSHAKE:
			{
				App_HandShake_CmdDeal(t_puReceiveData,&uSendData);
			}
			break;

			case COMPRO_TYPE_MANAGE:
			{
				App_Manage_CmdDeal(t_puReceiveData,&uSendData);
			}
			break;

			case COMPRO_TYPE_DEVICE:
			{
				App_Device_CmdDeal(t_puReceiveData,&uSendData);
			}
			break;

			case COMPRO_TYPE_WALLET:
			{
				App_Wallet_CmdDeal(t_puReceiveData,&uSendData);
			}
			break;
			
			case COMPRO_TYPE_DOWNLOAD:
			{
				App_DownLoad_CmdDeal(t_puReceiveData,&uSendData);
			}
			break;
			
			default:
				App_ComPro_SetResErr(&uSendData,COMPRO_RESPONSE_TYPE_ERR);
			break;
		}
		
		if(COMPRO_RESPONSE_WAITTING == App_Math_ValUint16(uSendData.m_aData))
		{
			memcpy(&g_SendDataBac.m_aSTX,&t_puProtocolData->m_aSTX,8);

			g_SendDataBac.m_aInterface = t_puProtocolData->m_aInterface;
			g_SendDataBac.m_aType = uSendData.m_aType;
			g_SendDataBac.m_aCMD = uSendData.m_aCMD;

			memset((u8 *)&uSendData,0x00,sizeof(uSendData));
			
			return;
		}
	}
	
	App_ComPro_SendData(t_puProtocolData,&uSendData);
	
	memset((u8 *)&uSendData,0x00,sizeof(uSendData));
	
	APP_Macro_PrintfEnd();
	
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_PCDownLoad()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_PCDownLoad(RECCOMData *t_puReceiveData)
{
	u32 iRet;

	iRet = App_DownProtocol_Deal(t_puReceiveData->m_aData,App_Math_ValUint16(t_puReceiveData->m_aLen));
	
	if(iRet != 0 && iRet != 0x80)
	{
		API_CB_FillRowRam(LCD_FONT_ASCII_8X16,LCD_LINE4,0 , 0, "Fail", LCD_ALIGN_CENTER|LCD_CLEAR_LINE);
		API_CB_LcdRefresh();
		//App_Main_SetOperateTimeOut(3*2);
	}
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_ComDeal()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_ComDeal(void)
{
	STRPROTOCOL uProtocolData;
	RECCOMData uReceiveData;
	
	if(!App_ComPro_DataDeal(&uProtocolData,&uReceiveData))
	{
		return;
	}

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
	}

	if(uProtocolData.m_aProtocolType == PROTOCOL_STATE_DEFAULT)
	{
		//DDEBUGHEX("uReceiveData",(u8 *)&uReceiveData,App_Math_ValUint16(uReceiveData.m_aLen)+4);

		if(App_Main_GetSysState() == SysState_OpenKeyPad)
		{
			App_Main_EnterWork();
		}
		
		App_ComPro_CmdDeal(&uProtocolData,&uReceiveData);
		
	}
	else if(uProtocolData.m_aProtocolType == PROTOCOL_STATE_5555AAAA)
	{
		App_ComPro_PCDownLoad(&uReceiveData);
	}
	
	memset((u8 *)&uProtocolData,0x00,sizeof(uProtocolData));

	ValSystemInfo.cPowerOffTime = 0;
	
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_SendResponseData()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void App_ComPro_SendResponseData(u8 *t_paData,u16 t_rDataLen,u16 t_rResponse,u8 t_MacType)
{
	SENDCOMData uSendData;
	STRPROTOCOL uProtocolData;
	u8 aRand[16];
	
	APP_Macro_PrintfStart();

	memset((u8 *)&uSendData,0x00,sizeof(uSendData));
	memset((u8 *)&uProtocolData,0x00,sizeof(uProtocolData));

	uProtocolData.m_aInterface = g_SendDataBac.m_aInterface;

	memcpy(&uProtocolData.m_aSTX,&g_SendDataBac.m_aSTX,8);
	
	uSendData.m_aType = g_SendDataBac.m_aType;
	uSendData.m_aCMD = g_SendDataBac.m_aCMD;

	if(t_MacType)
	{
		if(App_Manage_FillRand(aRand,16,1))
		{
			App_ComPro_SetResErr(&uSendData,COMPRO_RESPONSE_NEED_RAND);
		}
		else
		{
			if(t_rDataLen)
			{
				memcpy(&uSendData.m_aData[2],t_paData,t_rDataLen);
			}
			
			App_ComPro_EncryptResSuccess(&uSendData,t_rDataLen,aRand);
		}
	}
	else 
	{
		if(t_rDataLen)
		{
			memcpy(&uSendData.m_aData[2],t_paData,t_rDataLen);
			
			App_ComPro_SetResSuccess(&uSendData,t_rDataLen);
		}
		else
		{
			App_ComPro_SetResErr(&uSendData,t_rResponse);
		}
	}
	
	App_ComPro_SendData(&uProtocolData,&uSendData);

	memset(&uProtocolData,0x00,sizeof(uProtocolData));
	memset(&uSendData,0x00,sizeof(uSendData));
	memset((u8 *)&g_SendDataBac,0x00,sizeof(SENDCOMDataBac));
	
	App_Main_EnterWork();
	
	APP_Macro_PrintfEnd();
	
}

/*
****************************************************************************************************
** 名    称 ：App_ComPro_SaveSendDataBac()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 App_ComPro_SaveSendDataBac(u8 *t_paData,u16 t_rDataLen)
{
	u16 rLen = 0;
	
	if(t_rDataLen > COMPRO_DATA_MAX_SEND)
	{
		return 1;
	}

	memset((u8 *)&g_SendDataBac,0x00,sizeof(SENDCOMDataBac));

	g_SendDataBac.m_aLen = t_rDataLen;
	
	memcpy(g_SendDataBac.m_aData,t_paData,g_SendDataBac.m_aLen);

	g_SendDataBac.m_aCrc16 = App_Crc16_Get(g_SendDataBac.m_aData,g_SendDataBac.m_aLen);

	rLen = ((g_SendDataBac.m_aLen+7)/8)*8;
	
	if(App_Key_TDKCrypt(KEY_ENCRYPT,g_SendDataBac.m_aData,rLen))
	{
		memset((u8 *)&g_SendDataBac,0x00,sizeof(SENDCOMDataBac));
				
		return 2;
	}

	return 0;
}

/****************************************End Of File************************************************
***************************************************************************************************/


