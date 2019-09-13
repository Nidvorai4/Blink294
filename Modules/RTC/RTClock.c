#include "./Modules/RTC/RTClock.h"

uint8_t aTxBuffer[19];






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



void RTClockWrite(uint8_t H, uint8_t M)
{
	aTxBuffer[0] = 0;
	HAL_Delay(500);
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)DS3231_ADDR_W, (uint8_t*)&aTxBuffer, 1, 1000);  //посыл адреса
	aTxBuffer[0] = 0;//адрес начального регистра (сдвиг от начального)
	aTxBuffer[1] = RTC_ConvertFromDecBin(10); // секунды
	aTxBuffer[2] = RTC_ConvertFromDecBin(20); // минуты
	aTxBuffer[3] = RTC_ConvertFromDecBin(H); // часы 7бит всегда 0, 6=0 для 24 часов, 5 -	20 часов, 4 - 10 часов, 3-0 = единицы
//	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
//	{
//	}
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)DS3231_ADDR_W, (uint8_t*) &aTxBuffer, 4, (uint32_t)1000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)
		{
			OtladkaPrint(1, "Error in sending time process");	
		}
	}
	
}


void RTClockRead()
{
	aTxBuffer[0] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*)&aTxBuffer, 1, 1000); //посыл адреса
	
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*) &aTxBuffer, 19, (uint32_t)1000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) == HAL_I2C_ERROR_AF)
		{
			OtladkaPrint(1, "Error in recieving time process");	
		}
	}
	//RTClockTemperatura 
	
	OtladkaPrint(1, "Recieved time: %i:%i:%i", RTC_ConvertFromBinDec(aTxBuffer[2]),RTC_ConvertFromBinDec(aTxBuffer[1]), RTC_ConvertFromBinDec(aTxBuffer[0]));
	RTClockTemperatura = ((aTxBuffer[17] >> 7) * -1 + (aTxBuffer[17] & 0x7F)) * 100 + (aTxBuffer[18] >> 6) * 25;
	OtladkaPrint(1, "Recieved chto-to: %i", RTClockTemperatura);


	
	
//	while (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)DS3231_ADDR_R, (uint8_t*) &aTxBuffer, 7, (uint32_t)1000) != HAL_OK)
//	{
//		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//		{
//			OtladkaPrint("Error of recieve");	
//		}
//	}
	
	
}