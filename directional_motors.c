/*
 * motors.c
 *
 * Created: 24/06/2016 19:22:45
 *  Author: angel
 */ 

#include "directional_motors.h"

/*input parameters*/

/*input parameters*/
volatile float P_terms;
volatile float I_terms;
volatile float D_terms;

/*operational parameters*/
volatile float sum_es;						/*error integration*/
volatile float errors;								/*to calculate the error*/
volatile int us;									/*output*/
volatile int out_puts = 160;

static int u_sat_ups = 200;							/*upper saturation limit*/
static int u_sat_downs = 0;						/*down saturation limit*/
static int sat_sum_ups = 20;					/*uper sum saturation*/
static int sat_sum_downs = 0;					/*down up satoration*/
static float Ks=1;
static float Kps=2;
static float Kis=4;
static float Yrps=15;							/*output ref*/


void init_directional(void)
{
	/* timer configuration*/
	TCCR3A |= (1<<WGM30) | (1<<COM3A1) | (1<<COM3B1);	/*fast pwm 8 bists compare output on A*/
	TCCR3B |= (1<<WGM32); 					/*FOC0A: Force Output Compare A*/
	TIMSK3 |= (1<<OCIE3A) | (1<<OCIE3B);								/*Output Compare Match A Interrupt Enable*/
	TCCR3B |= (1<<CS30);
	DDRE = 0xFF;
	DDRH = 0xFF;
}


void pid_directional(float y)
{
	errors = Yrps - y;
	P_terms = Kps * errors;
	sum_es = sum_es+errors;
	if((int)sum_es > sat_sum_downs)    sum_es = (float)sat_sum_ups;
	if((int)sum_es < sat_sum_downs)  sum_es = (float)sat_sum_downs;
	I_terms = Kis * sum_es;
	us = (int)(Ks * (P_terms + I_terms));
	if(us>u_sat_ups)  us=u_sat_ups;
	if(us<u_sat_downs)  us=u_sat_downs;
	out_puts = us;
	print_int(out_puts);
	serial_write(0x20);
}

ISR(TIMER3_COMPA_vect)
{
	if( (out_puts >= 124) && (out_puts <= 155) )  /*right pin 5 pwm*/
	{
		OCR3A = 255;			/*to make 250 at mmaximum*/
		DDRE |= (1<<PE3);			/*PB5 as output to pwm*/
		PORTH |= (1 << PORTH3);		/*enable bridge pin 23*/
		OCR3B = 0;					/*stop pwm reverse*/
		//DDRE &= ~(1<<PE4);			/*change to input*/
	}
	
	if( (out_puts > 155) || (out_puts < 70)  ) /*car stopped*/
	{
		PORTH &= ~(1<<PORTH3);		/*disable bridge*/
		OCR3A = 0;					/*stop pwm front*/
		//DDRE &= ~(1<<PB3);			/*change to input*/
		OCR3B = 0;					/*stop pwm reverse*/
		//DDRE &= ~(1<<PE4);			/*change to input*/
	}
}

ISR(TIMER3_COMPB_vect)
{
	if( (out_puts >= 70) && (out_puts <=100) ) /*left pin 2 pwm*/
	{
		DDRE |= (1<<PB4);				/*PB6 as output*/
		OCR3B = 255;			/*make 250 at maximum*/
		PORTH |= (1<<PORTH3);			/*enable bridge*/
		OCR3A = 0;					/*stop pwm front*/
	//	DDRE &= ~(1<<PB3);			/*change to input*/
	}
}