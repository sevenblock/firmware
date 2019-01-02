/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_RTL8760ATT.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_RTL8760ATT_D
#include "..\block\app\App_Macro.h"

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_CheckSum()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
#ifdef HCIUART_CHECKSUM
u8 Dev_RTL8760ATT_CheckSum(u8 *data, u16 len)
{
	u8 checksum = 0;

	while (len) 
	{
		checksum += *data;
		data++;
		len--;
	}

	return (0xff - checksum + 1);
}
#endif
/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_SendCmd()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_SendCmd(u16 opcode, u8 *data, u16 dlen)
{
	//u8 *buff;
	u16 len;
	u16 acl_plen;
	u16 cmd_plen;
	int ret;
	u16 i = 0;
	u8 buff[MAX_COMMAND_LEN];

#ifdef HCIUART_CHECKSUM
	len = 5 + 5 + dlen + 1;
#else
	len = 5 + 5 + dlen;
#endif

	if (len > MAX_COMMAND_LEN) 
	{
		return 1;
	}
	
	//buff = malloc(len);
	
	//if (!buff) 
	//{
	//	return 2;
	//}
	
#ifdef HCIUART_CHECKSUM
	acl_plen = 6 + dlen;
#else
	acl_plen = 5 + dlen;
#endif
	
	cmd_plen = dlen;

	buff[i++] = ACL_PKT;
	buff[i++] = 0x00; /* special acl handle */
	buff[i++] = 0x00;
	buff[i++] = acl_plen & 0xff;
	buff[i++] = (acl_plen >> 8) & 0xff;
	buff[i++] = 0x01;
	buff[i++] = opcode & 0xff;
	buff[i++] = (opcode >> 8) & 0xff;
	buff[i++] = cmd_plen & 0xff;
	buff[i++] = (cmd_plen >> 8) & 0xff;

	if (dlen && data) 
	{
		memcpy(buff + i, data, dlen);
		i += dlen;
	}

#ifdef HCIUART_CHECKSUM
	buff[i] =  Dev_RTL8760ATT_CheckSum(&buff[1] ,  i - 1);
	i++;
#endif
	
	if (i != len) 
	{
		//free(buff);
		return 3;
	}

	DDEBUGHEX("Dev_Bt_ComSend",buff,len);

	ret = Dev_Bt_ComSend(buff, len);
	
	//free(buff);

	return ret;
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_ReadSwVersion()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_ReadSwVersion(void)
{
	u8 cwbuf;
	u8 rbuf[16];
	u16 rlen;
	
	return Dev_Bt_SendCmd(HCI_CMD_VERSION_REQ, &cwbuf, 0, rbuf, &rlen);
}
/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_Reset()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_Reset(void)
{
	u8 cwbuf;
	u8 rbuf[16];
	u16 rlen;
	
	return Dev_Bt_SendCmd(HCI_CMD_RESET, &cwbuf, 0, rbuf, &rlen);
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_ReadBtStatus()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_ReadBtStatus(u8 *rbuf, u16 *rlen)
{	
	return Dev_Bt_SendCmd(HCI_CMD_READ_BT_STATUS, NULL, 0, rbuf, rlen);
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_WriteDeviceName()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_WriteDeviceName(u8 t_aType, const char *t_pcBtName)
{
	int ret;
	int len;
	u8 buff[40];

	len = strlen(t_pcBtName);

	if (len > 39)
	{
		len = 39;
	}
	
	memcpy(buff, (u8 *)t_pcBtName, len);
	
	buff[len] = '\0';

	if(t_aType & 0x01)
	{
		ret = Dev_Bt_SendCmd(HCI_LE_CMD_WRITE_LOCAL_NAME, buff, len + 1, NULL, NULL);  //ble

		if(ret)
		{
			return 1;
		}
	}
	
	if(t_aType & 0x02)
	{
		ret = Dev_Bt_SendCmd(HCI_LEGACY_CMD_WRITE_DEVICE_NAME, buff, len + 1, NULL, NULL); //spp

		if(ret)
		{
			return 2;
		}
	}

	return 0;
	
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_SetIoCap()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_RTL8760ATT_SetIoCap(u8 iocap)
{
	return Dev_Bt_SendCmd(HCI_CMD_WRITE_PAIR_MODE, &iocap, 1, NULL, NULL);
}
/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_KeyReqNegReply()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_RTL8760ATT_KeyReqNegReply(T_BT_AUTH_INFO_HEAD *head)
{
	uint8_t buff[2 + sizeof(*head)];
	int ret;

	buff[0] = 0x01;
	buff[1] = 0x00;
	memcpy(buff + 2, head, sizeof(*head));

	ret = Dev_Bt_ResponeCmd(HCI_CMD_KEY_REQ_NEG_REPLY, buff, sizeof(buff));

	if(ret)
	{
		//pr_err("send key req neg reply error");
	}
}
/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_KeyReqRsp()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_RTL8760ATT_KeyReqRsp(T_BT_AUTH_INFO_HEAD *head, uint8_t *key,uint8_t key_len)
{
	uint8_t buff[sizeof(*head) + LE_KEY_LEN];
	int ret;

	memcpy(buff, head, sizeof(*head));
	memcpy(buff + sizeof(*head), key, key_len);

	ret = Dev_Bt_ResponeCmd(HCI_CMD_LINK_KEY_RSP, buff, sizeof(*head) + key_len);

	if(ret)
	{
		//pr_err("send link key req rsp error");
	}
}
/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_All_ltk_Irk_PairsRsp()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_RTL8760ATT_All_ltk_Irk_PairsRsp(T_BT_AUTH_INFO_HEAD *head)
{
	bool ret;
	uint8_t *buff;
	uint8_t num_pair = 0;
	uint16_t len;
	uint8_t index[MAX_STORE_NUM];
	int r;

	ret = le_get_key_pair_num(&num_pair, index);
	
	if (!ret || !num_pair) 
	{
		Dev_RTL8760ATT_KeyReqNegReply(head);
		
		return;
	}

	/* num + head + (bdaddr + ltk + irk) * num */
	len = 1 + sizeof(*head) + (6 + LE_KEY_LEN * 2) * num_pair;

	buff = malloc(len);
	
	if (!buff) 
	{
		return;
	}

	ret = le_get_key_pairs(num_pair, index, buff + 1 + sizeof(*head));
	
	if (ret)
	{		
		buff[0] = num_pair;
		
		memcpy(buff + 1, head, sizeof(*head));

		r = Dev_Bt_ResponeCmd(HCI_LE_CMD_ALL_PAIRED_DEVICE_INFO, buff, len);

		if(r)
		{
			//pr_err("send ltk irk pairs error");
		}
	} 
	else
	{
		Dev_RTL8760ATT_KeyReqNegReply(head);
	}

	free(buff);
	
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_KeyNotify()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_RTL8760ATT_KeyNotify(uint8_t *buff, u16 len)
{
	T_BT_AUTH_INFO_HEAD *head;

	if (!buff || len < sizeof(T_BT_AUTH_INFO_HEAD) + BREDR_KEY_LEN)
	{
		return;
	}

	head = (T_BT_AUTH_INFO_HEAD *)buff;

	if (head->remote_addr_type <= BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY) 
	{
		if (head->key_type != BTIF_LINK_KEY_LE_REMOTE_IRK)
		{
			le_save_bond(head->bd_addr, head->remote_addr_type,buff + sizeof(*head), head->key_type);
		}
		else
		{
			le_save_irk(head->bd_addr, head->remote_addr_type,buff + sizeof(*head), head->key_type);
		}
	} 
	else 
	{
		legacy_save_bond(head->bd_addr, buff + sizeof(*head),head->key_type);
	}
}

/*
****************************************************************************************************
** 名    称 ：Dev_RTL8760ATT_KeyReq()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_RTL8760ATT_KeyReq(uint8_t *buff, u16 len)
{
	T_BT_AUTH_KEY_REQ_IND *ind;
	T_KEY_COMMON key;
	uint8_t key_type;
	bool res;

	ind = (T_BT_AUTH_KEY_REQ_IND *)buff;
	
	if (!buff || len < sizeof(*ind)) 
	{
		Dev_RTL8760ATT_KeyReqNegReply((T_BT_AUTH_INFO_HEAD *)ind);
		
		return;
	}

	if (ind->remote_addr_type <= BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY) 
	{
		res = le_get_bond(ind->bd_addr, key.key, &key_type);
		
		if (res) 
		{
			Dev_RTL8760ATT_KeyReqRsp((T_BT_AUTH_INFO_HEAD *)ind,key.key, 28);
			
			return;
		}

		/* no ltk but if the addr is rpa,  we should send
		 * all ltk and irk pairs to 8761att to do
		 * resolvable private addr resolution */
		if (ind->remote_addr_type == BTIF_REMOTE_ADDR_LE_RANDOM &&(ind->bd_addr[5] >> 6) == 0x01)
		{
			Dev_RTL8760ATT_All_ltk_Irk_PairsRsp((T_BT_AUTH_INFO_HEAD *)ind);
		}
		else
		{
			Dev_RTL8760ATT_KeyReqNegReply((T_BT_AUTH_INFO_HEAD *)ind);
		}

	}
	else 
	{
		res = legacy_get_bond(ind->bd_addr, key.key, &key_type);
		
		if (res)
		{
			Dev_RTL8760ATT_KeyReqRsp((T_BT_AUTH_INFO_HEAD *)ind,key.key, 16);
		}
		else
		{
			Dev_RTL8760ATT_KeyReqNegReply((T_BT_AUTH_INFO_HEAD *)ind);
		}
	}
}


