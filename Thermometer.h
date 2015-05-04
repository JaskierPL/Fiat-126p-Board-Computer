#ifndef THERMOMETER_H
#define THERMOMETER_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define THERM_DATA_SET      PORTB |=  (1<<PB0);
#define THERM_DATA_CLR      PORTB &= ~(1<<PB0);
#define THERM_DATA_SET_OUT  DDRB  |=  (1<<PB0);
#define THERM_DATA_SET_IN   DDRB  &= ~(1<<PB0);

//Zmienne
char therm_presence;
char therm_string_buffer[8];
float therm_temp_f;
char measurement_in_process;

//Reset
unsigned char therm_reset_pulse(void);

//Wysy³anie i odbieranie pojedyñczych bitów
void therm_send_bit(char c_bit);
unsigned char therm_read_bit(void);

//Wysy³anie i odbieranie bajtów danych
void therm_send_byte(char c_byte);
unsigned char therm_read_byte(void);

//Mierzenie temperatury
char therm_measure_begin(void);
char therm_measure_fetch_results(void);

#endif