#include "./Modules/SD/SD.h"
//-----------------------------------------------
extern SPI_HandleTypeDef hspi1;
sd_info_ptr sdinfo;
//-----------------------------------------------

static void Error(void)
{
	LD_ON;
}


//Ниже этой функции добавим функцию для записи и чтения шины SPI

//-----------------------------------------------

uint8_t SPIx_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) &Byte,(uint8_t*) &receivedbyte,1,0x1000)!=HAL_OK)
  {
    Error();
  }
  return receivedbyte;
}

//-----------------------------------------------
//Также добавим ниже этой функции три функции для SPI — чтение, запись и обычный прогон байта
//-----------------------------------------------
void SPI_SendByte(uint8_t bt)
{
  SPIx_WriteRead(bt);
}
//-----------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
  uint8_t bt = SPIx_WriteRead(0xFF);
  return bt;
}
//-----------------------------------------------
void SPI_Release(void)
{
  SPIx_WriteRead(0xFF);
}
//-----------------------------------------------
uint8_t sd_ini(void)
{
	uint8_t i;
	int16_t tmr;
	uint32_t temp;
	LD_OFF;
	sdinfo.type = 0;
	//Сохраним значение делителя шины SPI, установим пока другой делитель и заново инициализируем шину, 
	//чтобы уменьшить скорость, так как слишком быструю сразу карта может не понять
	sdinfo.type = 0;
	temp = hspi1.Init.BaudRatePrescaler;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //156.25 kbbs
	HAL_SPI_Init(&hspi1);
	//Затем передадим несколько импульсов на ножку синхронизации SPI. По даташиту нужно не менее 74, мы передадим на всякий случай 80.
	//Это можно сделать обычным прогоном байта по шине SPI. Причём есть ещё требование — ножка выбора должна быть поднята
	HAL_SPI_Init(&hspi1);
	SS_SD_DESELECT();
	for(i=0;i<10;i++) //80 импульсов (не менее 74) Даташит стр 91
	  SPI_Release();
	return 0;
}

//-----------------------------------------------