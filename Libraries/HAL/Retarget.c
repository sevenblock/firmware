/*******************************************************************************
* File name    :	Serial.c
* Function     :	The low level routines for retarget printf function
* Author       : 	coins
* Date         :	2015/05/28
* Version      :    v1.0
* Description  :	
* ModifyRecord :
*******************************************************************************/

/*******************************************************************************
* Include Header Files
*******************************************************************************/
#include <stdio.h>


/*******************************************************************************
* Local Macro Definition
*******************************************************************************/

/*******************************************************************************
* Local Type Definition
*******************************************************************************/


/*******************************************************************************
* Local static Variable Declaration
*******************************************************************************/


/*******************************************************************************
* Global Variable Declaration
*******************************************************************************/
extern int  sendchar(int ch);  /* in Serial.c */
extern int  getkey(void);      /* in Serial.c */
extern void ExitSimulate(void);


#if(defined(__CC_ARM ))
//#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };
#endif

FILE __stdout;
FILE __stdin;

/*******************************************************************************
* Local Functon Declaration
*******************************************************************************/


/*******************************************************************************
* Functon
*******************************************************************************/

int fputc(int ch, FILE *f) {
	return (sendchar(ch));
}


int fgetc(FILE *f) {
	return (sendchar(getkey()));
}


int ferror(FILE *f) {
  	/* Your implementation of ferror */
	return EOF;
}


void _ttywrch(int ch) {
	sendchar (ch);
}


void _sys_exit(int return_code) {
	ExitSimulate();
	while (1);    /* endless loop */
}
