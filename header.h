/*
 * heder.h
 *
 * Created: 23/02/2016 15:16:14
 *  Author: angel
 */ 


#ifndef HEDER_H_
#define HEDER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"
#include "parse.h"
#include "number_conv.h" 
#include "Sonar.h"

extern volatile char received_command[30];							//command to be executed
extern volatile char parse_index;
extern char command_read();											//read the command to be executed 

#endif /* HEDER_H_ */ 