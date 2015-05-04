#ifndef ENCODER_H
#define ENCODER_H

#include <avr/io.h>

//Przycisk impulsatora
unsigned char encoder_btn_down;
unsigned char encoder_btn_pressed;

//Pokrêt³o impulsatora
unsigned char encoder_channel_A;
unsigned char encoder_channel_B;
unsigned char encoder_channel_A_prev;
signed char encoder_rot;

//Inicjalizacja enkodera
void encoder_init(void);

//Obs³uga
unsigned char is_encoder_btn_down(void);
unsigned char is_encoder_btn_pressed(void);
signed char get_encoder_rot(void);
void encoder_update(void);

#endif