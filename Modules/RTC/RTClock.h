#pragma once

#include <stdint.h>
#include "i2c.h"
#include "main.h"

#define DS3231_ADDR_R	0xD1
#define DS3231_ADDR_W	0xD0
#define DS3231_ADDR		0x68

uint16_t RTClockTemperatura;


void RTClockRead();
void RTClockWrite(uint8_t H, uint8_t M);
