#include"one_os.h"
#include"spin_common.h"
#include"spin_userstart.h"
#include"spin_interupt.h"
#include"spin_timer.h"
#include"spin_exinterupt.h"
#include"spin_gpio.h"
#include"spin_uart.h"
#include"spin_watchdog.h"
#include"spin_pwrmgr.h"
#include<string.h>
#include<stdio.h>

#define  TASK_MAX(task)   (sizeof(task)/sizeof(task[0]))
//��ʱι����������
void task1(){
	static uint recv_fre=0;
	recv_fre=~recv_fre;
	spin_set_gpio_bit_value(GPIO2,5,recv_fre);
	spin_watchdog_clear();
}

void task2(){
     static uchar idata recv[BUFF_MAX_SIZE];
	 uint ret = 0;
 	 ret = spin_read_uart(recv);
	 if(ret != 0)
	 	spin_write_uart(recv,ret);
}
//��ʱ�������ݵ�����
void task3(){
	 static uchar idata send[BUFF_MAX_SIZE/2];
	 static unsigned long sec=0;
	 sprintf(send,"time:%llu\r\n",sec++);
	 spin_write_uart(send,strlen(send));
}
static Tasks idata task[]=   
{   
    {0,1,4000,4000,task1}, //ι������	500ms
	{1,1,400,400,task2},//������		50ms
//    {2,1,8000,8000,task3},//д����		1000ms
};

void TaskHangup(unsigned char Task_Num)//����������������������������
{ 
	task[Task_Num].Run=0;
}

void TaskRecovery(unsigned char Task_Num)//����ָ������������������������
{
	task[Task_Num].Run=1;
}

void TaskRemarks(void) //���ڶ�ʱ���ж�����
{
	unsigned char i;
	for (i=0; i<TASK_MAX(task); i++)          
	{
		if (task[i].Timer)          
		{
			task[i].Timer--;        
			if (task[i].Timer == 0)  																														//������65500���ֵ��֤�����ʱ��Ҳ��ִ����������
			{
					task[i].Timer = task[i].ItvTime;
					task[i].Run = 1;           
			}
		}
	}
}

void TaskProcess(void)//�������while(1)ѭ������
{
    unsigned char  i;
	for (i=0; i<TASK_MAX(task); i++)          
    {         
		if (task[i].Run)     
        {
             task[i].Run = 0;     
			 task[i].TaskHook();  
        }
    }   
	idle_mode();//����͹���ģʽ		
}