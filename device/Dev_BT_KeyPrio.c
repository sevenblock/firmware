/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_BT_KeyPrio.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_BT_KeyPrio_D
#include "..\block\app\App_Macro.h"

T_BOND_PRIORITY bt_bond_sort;
T_BOND_PRIORITY *p_bond_sort;
uint8_t bt_max_bond_num = 0;

void bond_priority_queue_init(uint8_t max_bond_num)
{
	if (!flash_load_bond_priority(&bt_bond_sort))
	{
		DDEBUG("bond_num 0x%02x", bt_bond_sort.bond_num);
		DDEBUGHEX("bond_idx = ",bt_bond_sort.bond_idx,sizeof(bt_bond_sort.bond_idx));
	}

	bt_max_bond_num = max_bond_num;
}

void bond_priority_queue_clear(void)
{
	bt_bond_sort.bond_num = 0;
	
	flash_save_bond_priority(&bt_bond_sort);
}

uint8_t *bond_priority_get_table(uint8_t *num)
{
	*num =bt_bond_sort.bond_num;
	
	return bt_bond_sort.bond_idx;
}

static uint8_t bond_priority_queue_find(uint8_t idx)
{
	uint8_t i = 0;

	p_bond_sort = &bt_bond_sort;

	for (i = 0; i < p_bond_sort->bond_num; i++)
	{
		if (p_bond_sort->bond_idx[i] == idx)
		{
			return i;
		}
	}

	/* pr_err("%s: failed, idx %d", __func__, idx); */

	return 0xff;
}

bool bond_priority_queue_delete(uint8_t idx)
{
	uint8_t res = 0;
	uint8_t i;

	p_bond_sort = &bt_bond_sort;

	if (p_bond_sort->bond_num == 0) 
	{
		return false;
	}

	res = bond_priority_queue_find(idx);
	
	if (res != 0xff) 
	{
		for (i = res; i < p_bond_sort->bond_num; i++) 
		{
			p_bond_sort->bond_idx[i] = p_bond_sort->bond_idx[i + 1];
		}
		
		p_bond_sort->bond_num--;

		flash_save_bond_priority(p_bond_sort);
		
		return true;

	} 
	else 
	{
		return false;
	}
}

bool bond_priority_queue_add(uint8_t idx)
{
	uint8_t res = 0;
	uint8_t max_num;

	p_bond_sort = &bt_bond_sort;
	max_num = bt_max_bond_num;

	res = bond_priority_queue_find(idx);
	
	if (res != 0xff)
	{
		return bond_set_high_priority(idx);
	}

	if (p_bond_sort->bond_num == max_num)
	{
		return false;
	}

	p_bond_sort->bond_idx[p_bond_sort->bond_num] = idx;
	p_bond_sort->bond_num++;

	flash_save_bond_priority(p_bond_sort);

	return true;
}

bool bond_set_high_priority(uint8_t idx)
{
	uint8_t res = 0;
	uint8_t i;

	p_bond_sort = &bt_bond_sort;

	res = bond_priority_queue_find(idx);
	
	if (res == 0xff) 
	{
		return false;
	}

	/* If it is already the highest priority, nothing to do */
	if (res == (p_bond_sort->bond_num - 1))
	{
		return true;
	}

	for (i = res; i < p_bond_sort->bond_num; i++) 
	{
		p_bond_sort->bond_idx[i] = p_bond_sort->bond_idx[i + 1];
	}
	
	p_bond_sort->bond_idx[p_bond_sort->bond_num - 1] = idx;

	flash_save_bond_priority(p_bond_sort);

	return true;
}

bool bond_set_low_priority(uint8_t idx)
{
	uint8_t res = 0;
	uint8_t i;

	p_bond_sort = &bt_bond_sort;

	res = bond_priority_queue_find(idx);
	
	if (res == 0xff) 
	{
		return false;
	}

	if (res == 0)
	{
		return true;
	}

	for (i = res; i > 0; i--) 
	{
		p_bond_sort->bond_idx[i] = p_bond_sort->bond_idx[i - 1];
	}
	
	p_bond_sort->bond_idx[0] = idx;

	flash_save_bond_priority(p_bond_sort);

	return true;
}

uint8_t bond_get_low_priority(void)
{
	uint8_t idx;

	p_bond_sort = &bt_bond_sort;

	if (p_bond_sort->bond_num == 0) 
	{
		return 0xff;
	}

	idx = p_bond_sort->bond_idx[0];

	return idx;
}

uint8_t bond_get_high_priority(void)
{
	uint8_t idx;

	p_bond_sort = &bt_bond_sort;

	if (p_bond_sort->bond_num == 0) 
	{
		return 0xff;
	}

	idx = p_bond_sort->bond_idx[p_bond_sort->bond_num - 1];

	return idx;
}
