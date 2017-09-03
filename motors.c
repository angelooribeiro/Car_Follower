/*
 * motors.c
 *
 * Created: 24/06/2016 19:22:45
 *  Author: angel
 */ 

#include "motors.h"

/*input parameters*/
volatile float P_term;
volatile float I_term;
volatile float D_term;

/*operational parameters*/
volatile float sum_e;						/*error integration*/
volatile float error;								/*to calculate the error*/
volatile int u;									/*output*/
volatile int out_put = 160;

static int u_sat_up = 200;							/*upper saturation limit*/
static int u_sat_down = 0;						/*down saturation limit*/
static int sat_sum_up = 40;					/*uper sum saturation*/
static int sat_sum_down = 0;					/*down up satoration*/
static float K=1;
static float Kp=2;
static float Ki=4;
static float Yrp=15;							/*output ref*/


void pid_front(float y)
{
	error = Yrp - y;
	P_term = Kp * error;
	sum_e = sum_e+error;
	if((int)sum_e > sat_sum_down)    sum_e = (float)sat_sum_up;
	if((int)sum_e < sat_sum_down)  sum_e = (float)sat_sum_down;
	I_term = Ki * sum_e;
	u = (int)(K * (P_term + I_term));
	if(u>u_sat_up)  u=u_sat_up;
	if(u<u_sat_down)  u=u_sat_down;
	out_put = u;
}


void init_motors()
{
	/* timer configuration*/
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);	/*fast pwm 8 bists compare output on A*/
	TCCR1B |= (1<<WGM12);					/*FOC0A: Force Output Compare A*/
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);								/*Output Compare Match A Interrupt Enable*/
	TCCR1B |= (1<<CS10);
}

ISR(TIMER1_COMPA_vect)
{	
	if( out_put <= 160 )  /*front* pin 11 pwm*/
	{
		OCR1A =  300 - out_put;		/*to make 250 at mmaximum*/
		DDRB |= (1<<PB5);			/*PB5 as output to pwm*/
		PORTB |= (1 << PORTB7);		/*enable bridge*/
		OCR1B = 0;					/*stop pwm reverse*/
		DDRB &= ~(1<<PB6);			/*change to input*/
	}
	
	//if( (out_put > 160) && (out_put < 165) ) /*car stopped*/
	//{
	//	PORTB &= ~(1<<PORTB7);		/*disable bridge*/
	//	OCR1A = 0;					/*stop pwm front*/
	//	DDRB &= ~(1<<PB5);			/*change to input*/
	//	OCR1B = 0;					/*stop pwm reverse*/
	//	DDRB &= ~(1<<PB6);			/*change to input*/
	//}
}

ISR(TIMER1_COMPB_vect)
{
	if(out_put > 160) /*reverse pin 12 pwm*/
	{
		DDRB |= (1<<PB6);				/*PB6 as output*/
		OCR1B = out_put;			/*make 250 at maximum*/
		PORTB |= (1<<PORTB7);			/*enable bridge pin 28*/
		OCR1A = 0;					/*stop pwm front*/
		DDRB &= ~(1<<PB5);			/*change to input*/
	}
}