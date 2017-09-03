//serial port comunication defenition
//USING USART0
#include "serial.h"

extern volatile char received_command[];
extern void parse();
extern volatile char parse_index;


//serial port configuration
void serial_configuration()
{
	//Baud rate 9600, fosc=16M
	UBRR0H = (103 >> 8);
	UBRR0L = 103;
	//RESTAR ALL FLAGS
	UCSR0A = 0X00;
	//enable transmission and reception
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0);
	//USART asynchronous mode, 8 bits, 1 stop bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
	//initiation of auxiliary variables
	istart = 0;
	iend  = 0;
	ostart = 0;
	oend = 0;
}
//end function

//process the received data
void serial_receive()
{
		unsigned char data = UDR0;					//save received char
		
		//<BCKSP>, clear last received
		if(data == 8)									//#########################done and tested#########################
		{
			if(iend != istart)						//if exist data in buffer								
				istart =  (istart-1);	//returns back one
		}
		
		//<ESC>, clear all received, istart goes to the same position as iend
		if(data == 27)									//#####################done and tested#########################
		{	
			if(iend != istart)						//if exist data in buffer								
				istart = (iend & input_mask);			//jumps over all received data
		}
		
		if(data == 13)		//execute command			//######################done and tested#############################
		{	
			unsigned char i = 0;
			while( istart != ( iend & input_mask) )
			{
				iend = (iend+1) & input_mask;
				received_command[i] = inbuf[iend];
				i++;
			}
			received_command[i] = '\0';				//end character
			iend++;									//jumps over 13
			parse_index = 0;
			parse();
		}
		
		//stores the received byte
		if( (data!=8) && (data!=27) && (data!='$') && (data!='?') )
		{
			istart = (istart+1)&input_mask;			//actualize istart index
			inbuf[istart] = data;					//put in buffer the received data
		}
}
//end function

//interrupt service routine on reading
ISR(USART0_RX_vect)
{	
	serial_receive();
}
//end function

//write for serial port					
void serial_write(char data)				
{
	while(!(1<<UDRIE0));	//waits for free space

	ostart = ( (ostart+1) & output_mask);			//actualization of the position
	outbuf[ostart] = data;		//stores data in the buffer						
		
	UCSR0B |= (1<<UDRIE0);						//enable sending
}
//end function

//interrupt service routine on sending
ISR(USART0_UDRE_vect)
{
	if( ostart == oend)						//if all is transmitted 
		UCSR0B &= ~(1<<UDRIE0);				// Disable UDRE interrupt
	
	if ( ostart != oend )						//if there something to transmit
	{
		oend = ( oend+ 1 ) & output_mask;	//actualization of index
		UDR0 = outbuf[oend];				//send data		
	}
}
//end function

//prints a buffer/string
void serial_writestr( char* data )
{
	while((*data) != '\0' )
	{
		serial_write(*data);
		data++;
	}
}//end function

//prints an error message
void serial_error()
{
	serial_writestr("Error\n");
}//end function



/*copyright RIBEIRO_ANGELO*/