#ifndef SPINUARTH
#define SPINUARTH
#include <reg52.h>	   //���ļ��ж�����51��һЩ���⹦�ܼĴ���
#include"spin_common.h"

uint spin_uart_init();//���ز�����
uint spin_write_uart(const char idata *buff,uint n);
uint spin_read_uart(char idata *buff);
#endif