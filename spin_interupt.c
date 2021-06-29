#include"spin_interupt.h"
#include<stdlib.h>
#include<string.h>
uint* inter_vector_function[inter_num]={NULL,NULL,NULL,NULL,NULL};	//�����û���ʼ��
static uchar* inter_src_list[inter_num];
static uchar* inter_prority[inter_num];

//static void *callback_data;
void spin_interupt_init(){
	//�ж�Դ���ص�ַ
	inter_src_list[out_int0]=0xA8;
	inter_src_list[timer0]=0xA9;
	inter_src_list[out_int1]=0xAA;
	inter_src_list[timer1]=0xAB;
	inter_src_list[serial]=0xAC;
	//�ж����ȼ����õ�ַ
	inter_prority[out_int0]=0xB8;
	inter_prority[timer0]=0xB9;
	inter_prority[out_int1]=0xBA;
	inter_prority[timer1]=0xBB;
	inter_prority[serial]=0xBC;
}

void spin_interupt_enable(){
	   EA=1;
}
void spin_interupt_disable(){
	   EA=0;
}
void spin_interupt_open(INTER_LIST num,INTER_PRORITY prority){
		IE |= 0x01<<num;
	   //*inter_prority[num] = prority;
	   //*inter_src_list[num] = enable;//��λѰַ������д��1bit
}
void spin_interupt_close(INTER_LIST num){
		IE &= (~(0x01<<num));
	   //*inter_src_list[num] = disable;//��λѰַ������д��1bit
}
void spin_interupt_set_params(INTER_LIST num,uchar params){

}

void intersvr0() interrupt 0{
	   if(inter_vector_function[0]!=NULL)
	   		((interupt_callback)inter_vector_function[0])();
}

void intersvr1(void) interrupt 1{
		if(inter_vector_function[1]!=NULL)
	   		((interupt_callback)inter_vector_function[1])();
}

void intersvr2(void) interrupt 2{
		if(inter_vector_function[2]!=NULL)
	  		 ((interupt_callback)inter_vector_function[2])();
}
void intersvr3(void) interrupt 3{
		if(inter_vector_function[3]!=NULL)
	   		((interupt_callback)inter_vector_function[3])();
}

extern volatile sem_t uart_send;		//�����ź���
extern volatile buff_t idata recv_buff; 	//����buff
void intersvr4(void) interrupt 4{
		if(RI==1){ //�����ж�
			critical_area_enter();
			recv_buff.buff[recv_buff.len++] = SBUF;
			critical_area_exit();
			RI=0;
		}
		if(TI==1){//�����ж�
			TI=0;
			sem_post(uart_send);
		}	
}
