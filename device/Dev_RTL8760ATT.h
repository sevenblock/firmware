/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_RTL8760ATT.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_RTL8760ATT_H
#define Dev_RTL8760ATT_H

#ifndef Dev_RTL8760ATT_D
#define EX_Dev_RTL8760ATT		extern
#else
#define EX_Dev_RTL8760ATT
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/

#define IOCAP_DISPLAY_ONLY  					0x00  	/**< only a Display present, no Keyboard or Yes/No Keys */
#define IOCAP_DISPLAY_YES_NO				0x01   /**< Display and Yes/No Keys present */
#define IOCAP_KEYBOARD_ONLY				0x02	/**< only a Keyboard present, no Display */
#define IOCAP_NO_IO									0x03   	/**< no input/output capabilities */
#define IOCAP_KEYBOARD_DISPLAY			0x04    /**< BLE: Keyboard and Display present */

#define CMD_PKT									0x01
#define ACL_PKT									0x02
#define EVT_PKT									0x04
#define HCI_CMD_HDR_LEN				4
#define HCI_ACL_HDR_LEN				5
#define HCI_EVT_HDR_LEN				3

#define MAX_COMMAND_LEN			(5 + 6 + 640 + 1)
#define MAX_NUM_OF_BUFF			8
#define TP_TX_PACKET_LEN				512

typedef enum t_hci_le_cmd
{
    HCI_LE_CMD_WRITE_LOCAL_NAME = 0x0002,
    HCI_LE_CMD_WRITE_LOCAL_BDADDR = 0x0004,
    HCI_LE_CMD_READ_LOCAL_BDADDR = 0x0005,
    HCI_LE_CMD_WRITE_ADVI = 0x0006,
    HCI_LE_CMD_WRITE_ADV_MODE = 0x0008,
    HCI_LE_CMD_READ_REMOTE_DEVICE_NAME = 0x00a0,
    HCI_LE_CMD_WRITE_ADV_DATA = 0x00a4,
    HCI_LE_CMD_WRITE_SCAN_RSP_DATA = 0x00a5,
    HCI_LE_CMD_ADV_ENABLE  = 0x00a6,
    HCI_LE_CMD_ALL_PAIRED_DEVICE_INFO = 0x00b0,
    HCI_LE_CMD_DISCONNECT_DEVICE = 0x00b6,
    HCI_LE_CMD_SEND_GATT_DATA = 0x00b7,
    HCI_LE_CMD_CONN_INTERVAL_UPDATE = 0x00ba,
    HCI_LE_CMD_READ_RSSI_VALUE = 0x00bb,
    HCI_LE_CMD_SET_BOND_AUTHEN_FLAG = 0x00bc,
    HCI_LE_CMD_PAIR = 0x00be,
    HCI_LE_CMD_SET_DIS_PARAM = 0x00d0,
} T_HCI_LE_CMD;

#define HCI_CMD_READ_BT_STATUS			0x00b3

/* Command cmd */
#define HCI_CMD_VERSION_REQ			0x0001
#define HCI_CMD_ERASE_PAIRED_DEVICE             0x00a1
#define HCI_CMD_PASSKEY_ENTRY_RSP               0x00a2
#define HCI_CMD_USER_CONFIRM_RSP                0x00a3
#define HCI_CMD_DEL_PAIRED_DEVICE               0x00a8
#define HCI_CMD_WRITE_PIN_CODE                  0x00a9
#define HCI_CMD_KEY_REQ_NEG_REPLY               0x00b1
#define HCI_CMD_LINK_KEY_RSP                    0x00b2
#define HCI_CMD_WRITE_PAIR_MODE                 0x00b5
#define HCI_CMD_SET_BAUDRATE                    0x00b8
#define HCI_CMD_RESET                           0x00b9
#define HCI_CMD_SET_FLOW_CTL                    0x00bd
#define HCI_CMD_SET_LOG_MASK			0x00bf

/* BR/EDR Commands */
#define HCI_LEGACY_CMD_DISCONNECT_DEVICE	0x0030
#define HCI_LEGACY_CMD_READ_RSSI_VALUE          0x0032
#define HCI_LEGACY_CMD_WRITE_DEVICE_NAME        0x0033
#define HCI_LEGACY_CMD_SEND_SPP_DATA            0x0034
#define HCI_LEGACY_CMD_CONNECT_DEVICE           0x0036
#define HCI_LEGACY_CMD_SET_PAGE_SCAN_PARA       0x0037
#define HCI_LEGACY_CMD_READ_REMOTE_DEVICE_NAME  0x0038
#define HCI_LEGACY_CMD_SET_PISCAN               0x0039
#define HCI_LEGACY_CMD_SET_SDP_RECORD           0x0040
#define HCI_LEGACY_CMD_SET_DID			0x0041
#define HCI_LEGACY_CMD_SET_ERTM			0x0042
#define HCI_LEGACY_CMD_SET_INQUIRY_SCAN_PARA     0x0043
#define HCI_LEGACY_CMD_SET_SSP			0x0044

#define HCI_CMD_LPS_MODE                        0x0035

//SEND EA  REC EF PROFILE

/* Events */
#define HCI_EVT_CMD_COMPLETE                    0x0080
#define HCI_EVT_PASSKEY_ENTRY_REQ		0x0081
#define HCI_EVT_DISPLAY_YES_NO_REQ		0x0082
#define HCI_EVT_LE_CONNECTION_COMPLETE		0x0083
#define HCI_EVT_LE_DISCONN_COMPLETE		0x0084
#define HCI_EVT_RECV_GATT_DATA                  0x0085
#define HCI_EVT_LINK_KEY_NOTIFY                 0x0086
#define HCI_EVT_LINK_KEY_REQ                    0x0087
#define HCI_EVT_DEL_LINK_KEY                    0x0088
#define HCI_EVT_NUMBER_OF_COMP_EVT              0x0089
#define HCI_EVT_LE_ADV_STATE                    0x008a
#define HCI_EVT_LE_CONN_PARAMS_UPDATE		0x008b
#define HCI_EVT_LE_LOCAL_RANDOM_BDADDR_CHANGED	0x008c
#define HCI_EVT_LE_PASSKEY_NOTIF		0x008d
#define HCI_EVT_LE_AUTHEN_COMPLETE		0x008e
#define HCI_EVT_STACK_READY			0x00ff

#define HCI_EVT_RECV_SPP_DATA                   0x0050
#define HCI_EVT_SPP_DISCONN_COMPLETE            0x0051
#define HCI_EVT_SPP_CONNECTION_COMPLETE		0x0052
#define HCI_EVT_LEGACY_AUTHEN_COMPLETE		0x0053

#define HCI_EVT_SCAN_RESULT			0x008f

#define LE_RAND_ADDR_STATIC		0xC0
#define LE_RAND_ADDR_RESOLVABLE     	0x40
#define LE_RAND_ADDR_MASK               	0xC0

/****************************************功能定义***************************************************
***************************************************************************************************/
EX_Dev_RTL8760ATT u8 Dev_RTL8760ATT_CheckSum(u8 *data, u16 len);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_SendCmd(u16 opcode, u8 *data, u16 dlen);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_ReadSwVersion(void);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_ReadBtStatus(u8 *rbuf, u16 *rlen);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_WriteDeviceName(u8 t_aType, const char *t_pcBtName);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_SetIoCap(u8 iocap);
EX_Dev_RTL8760ATT void Dev_RTL8760ATT_KeyNotify(uint8_t *buff, u16 len);
EX_Dev_RTL8760ATT void Dev_RTL8760ATT_KeyReq(uint8_t *buff, u16 len);
EX_Dev_RTL8760ATT s32 Dev_RTL8760ATT_Reset(void);
#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

