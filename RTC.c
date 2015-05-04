#include "RTC.h"

#define I2C_Stop() TWCR = TW_STOP 

const uint8_t DS1307_DaysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

void I2C_Init()
// at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming prescalar of 0.
// so for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72.
{
	TWSR = 0; // set prescalar to zero
	TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
}

byte I2C_Detect(byte addr)
// look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START; // send start condition
	while (!TW_READY); // wait
	TWDR = addr; // load device's bus address
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}

byte I2C_FindDevice(byte start)
// returns with address of first device found; 0=not found
{
	for (byte addr=start;addr<0xFF;addr++) // search all 256 addresses
	{
		if (I2C_Detect(addr)) // I2C detected?
		return addr; // leave as soon as one is found
	}
	return 0; // none detected, so return 0.
}

void I2C_Start (byte slaveAddr)
{
	I2C_Detect(slaveAddr);
}

byte I2C_Write (byte data) // sends a data byte to slave
{
	TWDR = data; // load data to be sent
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS!=0x28);
}

byte I2C_ReadACK () // reads a data byte from slave
{
	TWCR = TW_ACK; // ack = will read more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}

byte I2C_ReadNACK () // reads a data byte from slave
{
	TWCR = TW_NACK; // nack = not reading more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}

void I2C_WriteByte(byte busAddr, byte data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(data); // then send the data byte
	I2C_Stop();
}

void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data){
	I2C_Start(busAddr); // send bus address
	I2C_Write(deviceRegister); // first byte = device register address
	I2C_Write(data); // second byte = data for device register
	I2C_Stop();
}

byte I2C_ReadRegister(byte busAddr, byte deviceRegister)
{
	byte data = 0;
	I2C_Start(busAddr); // send device address
	I2C_Write(deviceRegister); // set register pointer
	I2C_Start(busAddr+READ); // restart as a read operation
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}

void DS1307_GetTimeDate(byte *hours, byte *minutes, byte *seconds, byte *days, byte *months, byte *years)
// returns hours, minutes, and seconds in BCD format
{
	*hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
	*minutes = DS1307_Bcd2Dec(I2C_ReadRegister(DS1307,MINUTES_REGISTER));
	*seconds = DS1307_Bcd2Dec(I2C_ReadRegister(DS1307,SECONDS_REGISTER));
	
	if (*hours & 0x40) // 12hr mode:
		*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else 
		*hours &= 0x3F; // 24hr mode: use bottom 6 bits
	
	*hours = DS1307_Bcd2Dec(*hours);
	
	*days = DS1307_Bcd2Dec(I2C_ReadRegister(DS1307,DAYS_REGISTER));
	*months = DS1307_Bcd2Dec(I2C_ReadRegister(DS1307,MONTHS_REGISTER));
	*years = DS1307_Bcd2Dec(I2C_ReadRegister(DS1307,YEARS_REGISTER));
}

void DS1307_SetTimeDate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	I2C_WriteRegister(DS1307,YEARS_REGISTER, DS1307_Dec2Bcd(year));
	I2C_WriteRegister(DS1307,MONTHS_REGISTER, DS1307_Dec2Bcd(month));
	I2C_WriteRegister(DS1307,DAYS_REGISTER, DS1307_Dec2Bcd(day));
	I2C_WriteRegister(DS1307,HOURS_REGISTER, DS1307_Dec2Bcd(hour)); // add 0x40 for PM
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, DS1307_Dec2Bcd(minute));
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, DS1307_Dec2Bcd(second));
}

uint8_t DS1307_Dec2Bcd(uint8_t val) {
	return val + 6 * (val / 10);
}

uint8_t DS1307_Bcd2Dec(uint8_t val) {
	return val - 6 * (val >> 4);
}

uint16_t DS1307_Date2Days(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
	days += pgm_read_byte(DS1307_DaysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
	++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}

uint8_t DS1307_GetDayOfWeek(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t day = DS1307_Date2Days(y, m, d);
	return (day + 6) % 7;
}