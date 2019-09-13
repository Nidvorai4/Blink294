#pragma once
//--------------------------------------------------
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
//--------------------------------------------------
//#define SKIP_ROM 0
//#define NO_SKIP_ROM 1
//#define TEMPERATURA_ERROR 9999
//--------------------------------------------------
#define RESOLUTION_9BIT 0x1F
#define RESOLUTION_10BIT 0x3F
#define RESOLUTION_11BIT 0x5F
#define RESOLUTION_12BIT 0x7F
//--------------------------------------------------
void GraduskikPin_init(void);
uint8_t ds18b20_init();
int16_t Temperatura;
void GradusnikRead(void);


