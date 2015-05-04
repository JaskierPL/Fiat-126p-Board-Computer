#ifndef RTC_H
#define RTC_H

#define F_CPU 16000000UL // run CPU at 16 MHz
// I2C routines
#define F_SCL 100000L // I2C clock speed 100 KHz
#define READ 1
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // don't return ACK to slave
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register#define I2C_Stop() TWCR = TW_STOP // inline macro for stop condition

// DS1307 RTC ROUTINES
#define DS1307 0xD0 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

void I2C_Init (void);
byte I2C_Detect (byte addr);
byte I2C_FindDevice (byte start);
void I2C_Start (byte slaveAddr);
byte I2C_Write (byte data);
byte I2C_ReadACK ();
byte I2C_ReadNACK ();
void I2C_WriteByte (byte busAddr, byte data);
void I2C_WriteRegister (byte busAddr, byte deviceRegister, byte data);
byte I2C_ReadRegister (byte busAddr, byte deviceRegister);

void DS1307_GetTimeDate(byte *hours, byte *minutes, byte *seconds, byte *days, byte *months, byte *years);
void DS1307_SetTimeDate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

uint8_t DS1307_Dec2Bcd(uint8_t val);
uint8_t DS1307_Bcd2Dec(uint8_t val);

uint16_t DS1307_Date2Days(uint8_t y, uint8_t m, uint8_t d);
uint8_t DS1307_GetDayOfWeek(uint8_t y, uint8_t m, uint8_t d);

#endif