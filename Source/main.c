/****************************************************************************
* File name    :	main.c
* Function     :	the main application of the testing
* Author       : 	
* Date         :	2015/07/1
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
//#include "..\Source\SYglobal.h"

#include "../block/app/App_Macro.h"

/****************************************************************************
 * Subroutine:	main
 * Function:	the main function
 * Input:		None
 * Output:		
 * Description:	 
 * Date:		2015/07/1
 * ModifyRecord:
 * *************************************************************************/ 
 int main()
{  
    SCM_SetSysClk(SYS_CLK_96M);

	App_Main_Run();

	//¸´Î»
	
	while(1);
}


