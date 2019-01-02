/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_BT_KeyPrio.h
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_BT_KeyPrio_H
#define Dev_BT_KeyPrio_H

#ifndef Dev_BT_KeyPrio_D
#define EX_Dev_BT_KeyPrio		extern
#else
#define EX_Dev_BT_KeyPrio
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************�궨��****************************************************
***************************************************************************************************/


/****************************************���ܶ���***************************************************
***************************************************************************************************/
EX_Dev_BT_KeyPrio void bond_priority_queue_init(uint8_t max_bond_num);
EX_Dev_BT_KeyPrio bool bond_priority_queue_delete(uint8_t idx);
EX_Dev_BT_KeyPrio bool bond_priority_queue_add(uint8_t idx);
EX_Dev_BT_KeyPrio void bond_priority_queue_clear(void);
EX_Dev_BT_KeyPrio uint8_t *bond_priority_get_table(uint8_t *num);
EX_Dev_BT_KeyPrio bool bond_set_high_priority(uint8_t idx);
EX_Dev_BT_KeyPrio bool bond_set_low_priority(uint8_t idx);
EX_Dev_BT_KeyPrio uint8_t bond_get_low_priority(void);
EX_Dev_BT_KeyPrio uint8_t bond_get_high_priority(void);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/


