//#include <reg51.h>	   //���ļ��ж�����51��һЩ���⹦�ܼĴ���
#include<reg52.h>
#include<string.h>
#include<stdio.h>
#include"spin_common.h"
#include"spin_userstart.h"
#include"spin_interupt.h"
#include"spin_timer.h"
#include"spin_exinterupt.h"
#include"spin_gpio.h"
#include"spin_uart.h"
#include"spin_watchdog.h"
#include"spin_pwrmgr.h"
#include"one_os.h"
void eint0_user(void){
	spin_set_gpio_bit_value(GPIO2,0,0);
}
void timer0_user(void){
	static uint islive=0;
	islive=~islive;
	spin_set_gpio_bit_value(GPIO2,1,islive);
}
void eint1_user(void){
	spin_set_gpio_bit_value(GPIO2,2,0);
}
void timer1_user(void){
	spin_set_gpio_bit_value(GPIO2,3,0);
}
void uart_user(void){ //���յ�1���ַ��ͽ�������
	static uint recv_fre=0;
	recv_fre=~recv_fre;
	spin_set_gpio_bit_value(GPIO2,4,recv_fre);
}

void hardware_init(){
    //ע���û��жϺ���
   inter_vector_function[out_int0]=(uchar *)eint0_user;
   inter_vector_function[timer0]=(uchar *)timer0_user;
   inter_vector_function[out_int1]=(uchar *)eint1_user;
   inter_vector_function[timer1]=(uchar *)timer1_user;
   inter_vector_function[serial]=(uchar *)uart_user;
   //Ӳ����ʼ������
   spin_set_work_register_group(0);
   spin_interupt_init();
   spin_interupt_enable();
   spin_uart_init();
   spin_watchdog_enable();
   spin_watchdog_overtime(64,FSOC); //�������ʱ��ms
   spin_exint0_start(down_eage);
   spin_exint1_start(low_vol);
   //���ڲ���
   spin_write_uart("ok\r\n",4);//���յ������ݻ�д
   //��ʱ����ز���
   //spin_counter_time(50);//����50�Σ��ﵽ���жϡ��ʹ��ڹ���timer1
   //spin_timer_sec(1);  //��ʱ1s
   spin_sysTick(TaskRemarks);//����ʱ������ʼϵͳ,ʱ��250us
}
void main(void){
   hardware_init();
   while(1){
   		TaskProcess();
   }
}

