#include"spin_uart.h"
#include"spin_timer.h"
#include"spin_interupt.h"
#include<stdio.h>
#include<string.h>
volatile sem_t uart_send; //��������ź���
volatile buff_t idata recv_buff; //����buff

static const uint boaud_rate=2400;
static const uint over_time_us=13;//ÿover_time_us����ʱ��1���1��

uint spin_uart_init(){
	sem_init(uart_send,1);
	spin_timer_for_uart(timer1,over_time_us);

   //SCON=0xF0;//��ʽ3�����ͨѶ���������
   //SCON = 0x50;		//8λ����,�ɱ䲨����
   SCON=0xD0;//��ʽ3,������� ֹͣλ1.5λ
   PCON|=0x80;//52��Ƭ��֧�֡�ʹ�ܺ󣬴���ʵ�ʲ����ʼӱ�
   spin_interupt_open(serial);	//�������ж�
   return boaud_rate*2;
}
static uchar spin_write_byte(uchar c){
	sem_wait(uart_send);    //�ȴ��������
	critical_area_enter();  //�ٽ���Դ SBUF
	SBUF=c;
	critical_area_exit();
	return c;	
}
uint spin_write_uart(const char idata *buff,uint n){
	uint i = 0;		
	for(;i<n;i++){
		spin_write_byte(*buff);
		buff++;
	}
	return i;
}
//����0�������
static uchar spin_read_byte(){
	 char ret;
	 if(recv_buff.len<=0)return 0;
	 critical_area_enter();
	 ret = recv_buff.buff[--recv_buff.len];
	 critical_area_exit();
	 return ret;
}

uint spin_read_uart(char idata *buff){
	char c = 0;
	uint i = 0;
	uint j = 0;
	uint end = 0;
	while( (c=spin_read_byte()) !=0){
	  buff[i++] = c;
	}
	if(i<2){buff[i]='\0';return i;} 
//	//reverse array
	end = i-1;
	for(j=0;j<i/2;j++){
		c = buff[j];
		buff[j] = buff[end-j];
		buff[end-j] = c;
	}
	buff[i]='\0';
	return i;//�����ĳ���
}
