#include "./Modules/RTC/RTClock.h"

uint8_t aTxBuffer[8];






uint8_t RTC_ConvertFromBinDec(uint8_t c)
{
	uint8_t ch = ((c >> 4) * 10 + (0x0F&c));
	return ch;
}

uint8_t RTC_ConvertFromDecBin(uint8_t c)
{
	uint8_t ch = ((c / 10) << 4) | (c % 10);
	return ch;
}


void RTClockInit()
{
}

void RTClockRead()
{
	HAL_Delay(1000);
	aTxBuffer[0] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*)&aTxBuffer, 1, 1000); //посыл адреса
	
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*) &aTxBuffer, 7, (uint32_t)1000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)
		{
			OtladkaPrint(1, "Error in recieving process");	
		}
	}

	
	OtladkaPrint(1, "Recieved time: %i:%i:%i", RTC_ConvertFromBinDec(aTxBuffer[2]),RTC_ConvertFromBinDec(aTxBuffer[1]), RTC_ConvertFromBinDec(aTxBuffer[0]));
		
	
//	while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*) &aTxBuffer, 7, (uint32_t)1000) != HAL_OK)
//	{
//		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//		{
//			OtladkaPrint("Error of recieve");	
//		}
//	}
	
	
}