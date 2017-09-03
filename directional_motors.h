/*
 * motors.h
 *
 * Created: 24/06/2016 19:23:01
 *  Author: angel
 */ 


#ifndef DIRECTIONAL_MOTORS_H_
#define DIRECTIONAL_MOTORS_H_
#include "header.h"

	/*
	– Variável de referência, 
	– Variável controlada,
	– Variável de erro,
	– Variável de comando,
	– Valores de saturação da variável de comando,
	– Período de amostragem,						H
	– Ganho proporcional,							KP
	– Ganho integral,								KI
	– Ganho derivativo (Td)							
	– Ganho do filtro (Tf),						freq de corte 1/Tf
		*/

void init_directional(void);	/*initiation of the parameters*/
void pid_directional(float y);

#endif /* MOTORS_H_ */