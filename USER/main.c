/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����3.5.0�汾���Ĺ���ģ�塣         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "includes.h"

static OS_STK start_task_stk[START_TASK_STK_SIZE];
/* 
 * ��������main
 * ����  : ������
 * ����  ����
 * ���  : ��
 */
int main(void)
	{
  	 BSP_Init();
	 OSInit();
	 OSTaskCreate(TASK_Start,(void*)0,&start_task_stk[START_TASK_STK_SIZE - 1],START_TASK_PRIO	);
	 OSStart();
	 return 0;
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


