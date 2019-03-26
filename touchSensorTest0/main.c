/*
 * touchSensorTest0.c
 *
 * Created: 26.03.2019 09:36:12
 * Author : Dominik
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

volatile uint16_t chargeCycleCount;

ISR(PCINT2_vect) {
	chargeCycleCount++;                                   /* zliczanie cykli */

	DDRD |= (1 << DDD7);                     /* tryb wyj�cia */
	_delay_us(1);                                    /* op�nienie �adowania */

	DDRD &= ~(1 << DDD7);                    /* tryb wej�cia */
	PCIFR |= (1 << PCIF2);            /* wyczy�� bit przerwania zmiany stanu */
}

int main(void)
{
    initUSART();
	printString("==[Sensor pojemno�ciowy]==\r\n");

	MCUCR |= _BV(PUD);
	PORTD |= _BV(PORTD7);
	PCICR |= _BV(PCIE2);
	PCMSK2 |= _BV(PORTD7);
    while (1) 
    {
		chargeCycleCount = 0;                            /* zerowanie licznika */
		DDRD |= (1 << DDD7);  /* zaczynamy z na�adowanym kond. */
		sei();                                 /* w��cz przerwania i zliczanie */
		_delay_ms(50);
		cli();                                                       /* gotowe */
		printWord(chargeCycleCount);                             /* do analizy */
		printString("\r\n");
    }
}

