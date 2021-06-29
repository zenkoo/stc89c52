#ifndef SPINCOMMONH
#define  SPINCOMMONH
#define inter_num	5
#define NULL		0x00
#define FSOC		12 //��Ƭ��Ƶ��12MHZ
#define true		1
#define false		0
#define BUFF_MAX_SIZE	32
typedef unsigned char  uchar;   
typedef unsigned int   uint; 
typedef bit bool;   
typedef enum{
 GPIO0=0,
 GPIO1,
 GPIO2,
 GPIO3,
}GPIOS;
 /*
 P30 RXD 
 P31 TXD
 P32 out_int0
 P33 out_int1
 P34 timer0	  //�ⲿ�������
 P35 timer1	  //�ⲿ�������
 */
typedef enum{
 out_int0=0, 
 timer0,
 out_int1,
 timer1,
 serial,
}INTER_LIST;

typedef enum{
 low=0,
 high,
}INTER_PRORITY;

typedef enum{
   low_vol=0,
   down_eage,
}TRIGER_TYPES;
enum{
 disable=0,
 enable,
};
typedef enum{
   spin_ok,
   allocate_timer_fail,
   allocate_counter_fail,
}SPIN_STATUS;

typedef struct buff_t{
	uchar buff[BUFF_MAX_SIZE];	//���ջ�����
	uchar len;
}buff_t;

//�ٽ��� ע�⣬Ŀǰ�ٽ���ֻ�д�����Ҫʹ�ã�����������
//#define critical_area_enter()				spin_interupt_close(serial)
//#define critical_area_exit()				spin_interupt_open(serial,high)

#define critical_area_enter()				spin_interupt_disable()
#define critical_area_exit()				spin_interupt_enable()

typedef uint sem_t;		//�������ź��������𻺳�����full or empty
#define sem_init(sem_name,value)		sem_name = value
#define sem_wait(sem_name)				while(!sem_name);	\
										critical_area_enter();\
										sem_name-=1;		\
										critical_area_exit()

#define sem_post(sem_name)				critical_area_enter();\
										sem_name+=1;	   	\
										critical_area_exit()
static int sem_try_wait(sem_t sem_name){
	if(!sem_name){
		sem_name-=1;
		return 0;
	}
	else		
		return -1;
}

typedef uint mutex_t;  //���𻺳����Ķ�д����
#define mutex_init(mutex_name)			mutex_name = 1
#define mutex_lock(mutex_name)			while(!mutex_name);	\
										mutex_name=0	
#define mutex_unlock(mutex_name)		mutex_name=1

#endif