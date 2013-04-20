/*
Firmware for EnGarde, a gesture classifying touch sensor based on the
Pittsburg Disney Research Labs Touche-sensor.
(C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
	    
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "stdout.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

//Scan from 10MHz down to (10000KHz/64=)156KHz
//Affects the amount of data passed back!
#define MAXDIV 32


#define PWMOUT PB1 //OC1A
#define ANAIN PC0 //adc0


//Initializes oc1a to output a frequency of (20MHz/div)
void pwmInit(int div) {
	TCCR1A=0x82;	//Fast pwm, top=ICR1
	TCCR1B=0x19;	//no prescaler
	OCR1A=div-1;
	ICR1=(div*2);
}

int adGetSample(void) {
	ADMUX=0x40; //sample AD0 with Vcc as ref
	ADCSRA=0xC7; //Sample at max division factor
	while(ADCSRA&0x40) ;
	return ADC;
}

int main(void) {
	int rets[MAXDIV];
	int div;
	DDRB=(1<<PWMOUT);
	stdoutInit(64); //19200 baud
	pwmInit(0);
	while(1) {
		//Do a frequency sweep.
		for (div=0; div<MAXDIV; div++) {
			pwmInit(div+1);
			//Wait for the RC-filter to settle
			_delay_ms(40);
			//Sample the Analog value
			rets[div]=adGetSample();
		}
		pwmInit(1);
		for (div=0; div<MAXDIV; div++) {
			printf("%i ", rets[div]);
		}
		printf("\n");
	}
}
