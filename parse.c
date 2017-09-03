/*
 * CFile1.c
 *
 * Created: 22/02/2016 22:13:04
 *  Author: angel
 */ 


#include "header.h"

volatile unsigned char digital_flag;
extern volatile char parse_index;
extern volatile unsigned char pwm_value;
extern volatile char received_comand[];					//command to be executed
extern char str_read(void);
extern void parse_read();
extern volatile int pwm_red;

void parse(void)
{
	digital_flag = 0;
	unsigned char temp = str_read();
	while(temp != '\0'){
		serial_write(temp);
		temp = str_read();
	}
	
} /*end function*/
