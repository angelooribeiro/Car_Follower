/*
 * motors.h
 *
 * Created: 24/06/2016 19:23:01
 *  Author: angel
 */ 


#ifndef MOTORS_H_
#define MOTORS_H_

#include "header.h"

	/*
	� Vari�vel de refer�ncia, 
	� Vari�vel controlada,
	� Vari�vel de erro,
	� Vari�vel de comando,
	� Valores de satura��o da vari�vel de comando,
	� Per�odo de amostragem,						H
	� Ganho proporcional,							KP
	� Ganho integral,								KI
	� Ganho derivativo (Td)							
	� Ganho do filtro (Tf),						freq de corte 1/Tf
		*/

void init_motors();	/*initiation of the parameters*/
void pid_front(float y);
void config_pwm(void);

#endif /* MOTORS_H_ */