#pragma once

#include <stdint.h>
#include "i2c.h"
#include "main.h"

#define DS3231_ADDR_R	0xD1
#define DS3231_ADDR_W	0xD0
#define DS3231_ADDR		0x68


void RTClockInit();
void RTClockRead();