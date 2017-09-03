/*
 * Sonar.c
 *
 * Created: 21/06/2016 18:26:12
 *  Author: angel
 */ 
#include "Sonar.h"

#define delay for(int i=0; i<10; i++);

/*flags to validate the read operation*/
volatile char flag_right = 0;
volatile char flag_center = 0;
volatile char flag_left = 0;
/*used to indicate the right task*/
volatile int timer_tick = 0;

void config_ultra()
{
	/*config timer0*/
	TCCR0B |= (1 << CS00) | (1 << CS01);
	TIMSK0 |= (1 << TOIE0);		/*interrupt on overflow*/
	
	/*config external itnerrupts*/
	DDRA = 0xFF;
	DDRD = 0;
	EICRA |= (1 << ISC11);			/*ei1 on rising edge*/ /*20*/
	EIMSK |= (1 << INT1);			/*ei1 enable*/
	EIMSK |= (1 << INT2);			/*ei2 enable*/
	EICRA |= (1 << ISC21);			/*ei2 on rising edge*/ /*21*/
	EICRA |= (1 << ISC31);			/*ei3 on rising edge*/ /*22*/
	EIMSK |= (1 << INT3);			/*ei3 enable*/
}

void print_int(int value)
{
	unsigned char string[10]= "0";		/*string to convert to dec*/
	int i = 0;

	do {       /* generate digits in reverse order */
		string[i++] = value % 10;  /* get next digit */
		value /= 10;			/* delete it */
	} while (value);

	while(i > 0){       /* send digits in reverse order */
		i--;
		serial_write(string[i]+48);
	}
}

/*calculates the distance from the timer*/
float calculate_distance(void)
{
	return (float) ( (float)TCNT5 * 0.005f);	/*convert to cm*/
}

/*##########################################valores seguros até +-35cm ##########################################*/
ISR(TIMER0_OVF_vect)
{
	/*the read operation takes at the maximum 40ms*/
	timer_tick ++;
	/*each cycle = 1ms*/
	if(timer_tick==5) {
		/*PORTA 22*/
		PORTA |= (1 << PORTA0 ); /*trigger*/
		delay;					/*at least 10 ys*/
		PORTA &= ~(1 << PORTA0 );
		TCNT5 = 0;
		TCCR5B |= (1<<CS51);	/*fclock/8 start*/
		flag_center = 0;
		flag_right = 0;
		flag_left = 1;
	}
	
	if(timer_tick==10) {
		/*PORTA 24*/
		PORTA |= (1 << PORTA2 );
		delay;
		PORTA &= ~(1 << PORTA2 );
		TCNT5 = 0;
		TCCR5B |= (1<<CS51);	/*fclock/8*/
		flag_center = 1;
		flag_right = 0;
		flag_left = 0;
	} 
	
	if(timer_tick==15) {
		/*PORTA 26*/
		PORTA |= (1 << PORTA4 );
		delay;
		PORTA &= ~(1 << PORTA4 );
		TCNT5 = 0;
		TCCR5B |= (1<<CS51);	/*fclock/8*/
		flag_center = 0;
		flag_right = 1;
		flag_left = 0;
		timer_tick = 0;
	}
} /*end isr timer0*/

ISR(INT1_vect)		/* to right sensor*/
{
	if(flag_left == 1)
	{
		TCCR5B &= ~(1<<CS51);	/*fclock/8*/
		pid_directional(calculate_distance());
		flag_left = 0;
	}
	EIFR |= (1 <<INTF1);	/*clear flag*/
}

ISR(INT2_vect)		/* to central sensor*/
{
	if(flag_center == 1)
	{
		TCCR5B &= ~(1<<CS51);	/*fclock/8*/
		pid_front(calculate_distance());
		flag_center = 0;
	}
	EIFR |= (1 <<INTF2);
}

ISR(INT3_vect)		/*to left sensor*/
{
	if(flag_right == 1)
	{
		TCCR5B &= ~(1<<CS51);	/*fclock/8*/
		pid_directional(-calculate_distance());
		flag_right = 0;
	}
	EIFR |= (1 <<INTF3);
}