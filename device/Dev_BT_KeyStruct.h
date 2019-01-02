/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_BT_KeyStruct.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_BT_KeyStruct_H
#define Dev_BT_KeyStruct_H

#ifndef Dev_BT_KeyStruct_D
#define EX_Dev_BT_KeyStruct		extern
#else
#define EX_Dev_BT_KeyStruct
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/

#define MAX_STORE_NUM						10
#define LE_KEY_LEN								28
#define BREDR_KEY_LEN						16

typedef enum t_bt_remote_addr_type 
{
    BTIF_REMOTE_ADDR_LE_PUBLIC          = 0x00,
    BTIF_REMOTE_ADDR_LE_RANDOM          = 0x01,
    BTIF_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02,
    BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03,
    BTIF_REMOTE_ADDR_CLASSIC            = 0x10,
    BTIF_REMOTE_ADDR_ANY                = 0x20,
    BTIF_REMOTE_ADDR_LE_ANONYMOUS       = 0xff,
} T_BTIF_REMOTE_ADDR_TYPE;

typedef enum t_bt_link_key_type 
{
    BTIF_LINK_KEY_COMBINATION       = 0x00,     /**< BT2.0 link key */
    BTIF_LINK_KEY_UNAUTHEN          = 0x04,     /**< SSP generated link key without MITM protection */
    BTIF_LINK_KEY_AUTHEN            = 0x05,     /**< SSP generated link key with MITM protection */
    BTIF_LINK_KEY_LE_LOCAL_LTK      = 0x11,     /**< BLE Long Term Key Blob */
    BTIF_LINK_KEY_LE_REMOTE_LTK     = 0x12,     /**< BLE Long Term Key Blob */
    BTIF_LINK_KEY_LE_LOCAL_IRK      = 0x13,     /**< BLE Identity Resolving Key */
    BTIF_LINK_KEY_LE_REMOTE_IRK     = 0x14,     /**< BLE Identity Resolving Key */
    BTIF_LINK_KEY_LE_LOCAL_CSRK     = 0x15,     /**< BLE Local CSR Key */
    BTIF_LINK_KEY_LE_REMOTE_CSRK    = 0x16,     /**< BLE Remote CSR Key */
    BTIF_LINK_KEY_UNAUTHEN_P256     = 0x17,     /**< security connection */
    BTIF_LINK_KEY_AUTHEN_P256       = 0x18,     /**< security connection */
    BTIF_LINK_KEY_REQ_BR            = 0x80,     /**< only used to request a BR linkkey */
    BTIF_LINK_KEY_DELETE            = 0xFF      /**< Link key is no longer valid and deleted */
} T_BTIF_LINK_KEY_TYPE;

typedef enum t_dev_type 
{
	LEGACY_BT_DEV,
	LE_BT_DEV
} T_DEV_TYPE;

typedef struct t_bond_priority 
{
	uint8_t bond_num;
	uint8_t bond_idx[7];
} T_BOND_PRIORITY;

#define BOND_FLAG_PAIRED						(1 << 7)
#define BOND_FLAG_IRK								(1 << 6)
#define BOND_FLAG_IDENTITY_ADDR		(1 << 5)
#define BOND_FLAG_ADDR_BREDR			0x01
#define BOND_FLAG_ADDR_LE_PUBLIC	0x02
#define BOND_FLAG_ADDR_LE_RANDOM	0x03

typedef struct t_remote_bd 
{
	uint8_t remote_bd[6];
	uint8_t bond_flag;
	uint8_t key_type;
} T_REMOTE_BD;

typedef struct t_link_key 
{
	uint8_t key[16];
} T_LINK_KEY;

typedef struct t_ltk 
{
	uint8_t key[28];
} T_LTK;

typedef struct t_irk 
{
	uint8_t key[28];
} T_REMOTE_IRK;

typedef struct t_key_common
{
	uint8_t key[28];
} T_KEY_COMMON;

/* Different from btif. The info is defined by app. */
typedef struct bt_auth_info_head 
{
	uint8_t bd_addr[6];
	uint8_t remote_addr_type;
	uint8_t key_type;
} T_BT_AUTH_INFO_HEAD;

/* Very similar to T_BTIF_AUTHEN_KEY_REQ_IND */
typedef struct t_bt_auth_key_req_ind 
{
	uint8_t bd_addr[6];
	uint8_t remote_addr_type;
	uint8_t key_type;
} T_BT_AUTH_KEY_REQ_IND;

/****************************************功能定义***************************************************
***************************************************************************************************/


#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

