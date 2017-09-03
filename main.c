/*
 * GccApplication1.c
 *
 * Created: 20/02/2016 09:23:40
 * Author : angel
 */ 
#include "header.h"
#include "motors.h"
#include "directional_motors.h"

volatile char received_command[30];							//command to be executed 
volatile char parse_index;										//save the last command executed with success 
char str_read_last(void);

int test = 0;

//used to read the received_command array
char str_read(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index];
	if(received_command[parse_index]!='\0')
	parse_index ++;
	
	return temp_parse;
}//end function

//to read a character that as been redden 
char str_read_last(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index-1];
	return temp_parse;
} //end function 

int main(void)
{
	serial_configuration();		//configuration of serial port
	config_ultra();
	init_motors();
	init_directional();
	
	sei();
	
	while (1)
	{
		//serial_write('1');
		//nao faço nada
	}
		
}


/*copyright RIBEIRO_ANGELO*/

