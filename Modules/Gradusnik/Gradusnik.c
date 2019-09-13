#include "./Modules/Gradusnik/Gradusnik.h"


//--------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
	micros *= (SystemCoreClock / 1000000) / 10;
	//Делитель 9 был выбран эксперементальным путём с помощью логического анализатора.
	/* Wait till done */
	while (micros--) ;
}
//--------------------------------------------------

uint8_t ds18b20_ReadBit(void)
{
	uint8_t bit = 0;
	GPIOA->ODR &= ~GPIO_ODR_ODR2; //низкий уровень
	DelayMicro(2);
	GPIOA->ODR |= GPIO_ODR_ODR2; //высокий уровень
	DelayMicro(13);
	bit = (GPIOA->IDR & GPIO_IDR_IDR2 ? 1 : 0); //проверяем уровень
	DelayMicro(45);
	return bit;	
}

//-----------------------------------------------

uint8_t ds18b20_ReadByte(void)
{
	uint8_t data = 0;
	for (uint8_t i = 0; i <= 7; i++)
		data += ds18b20_ReadBit() << i;
	return data;
}

//  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  --- --

void ds18b20_WriteBit(uint8_t bit)
{
	GPIOA->ODR &= ~GPIO_ODR_ODR2;
	DelayMicro(bit ? 3 : 65);
	GPIOA->ODR |= GPIO_ODR_ODR2;
	DelayMicro(bit ? 65 : 3);
}
//-----------------------------------------------

void ds18b20_WriteByte(uint8_t dt)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		ds18b20_WriteBit(dt >> i & 1);
		//Delay Protection
		DelayMicro(5);
	}
}
//-----------------------------------------------

void GraduskikPin_init(void)
{
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);   
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL |= GPIO_CRL_CNF2_0;
	GPIOA->CRL &= ~GPIO_CRL_CNF2_1;
}

//--------------------------------------------------
//--------------------------------------------------

uint8_t ds18b20_Reset(void)
{
	uint16_t status;
	GPIOA->ODR &= ~GPIO_ODR_ODR2; //низкий уровень
	DelayMicro(485); //задержка как минимум на 480 микросекунд 485
	GPIOA->ODR |= GPIO_ODR_ODR2; //высокий уровень
	DelayMicro(65); //задержка как минимум на 60 микросекунд
	status = GPIOA->IDR & GPIO_IDR_IDR2; //проверяем уровень
	DelayMicro(500); //задержка как минимум на 480 микросекунд    500
	//(на всякий случай подождём побольше, так как могут быть неточности в задержке)
	return (status ? 1 : 0); //вернём результат
}

//----------------------------------------------------------
uint8_t ds18b20_init()
{
	if (ds18b20_Reset()) return 1;
		ds18b20_WriteByte(0xCC);//SKIP ROM
		ds18b20_WriteByte(0x4E);//WRITE SCRATCHPAD
		ds18b20_WriteByte(0x64);//TH REGISTER 100 градусов
		ds18b20_WriteByte(0x9E);//TL REGISTER - 30 градусов
		ds18b20_WriteByte(RESOLUTION_12BIT);//Resolution 12 bit

	return 0;
}
//----------------------------------------------------------

//void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum)
//{
//	ds18b20_Reset();
//	if (mode == SKIP_ROM)
//	{
//		//SKIP ROM
//		ds18b20_WriteByte(0xCC);
//	}
//	//CONVERT T
//	ds18b20_WriteByte(0x44);
//}
//----------------------------------------------------------
void GradusnikRead()
{
	Temperatura = -100;
	uint8_t Data[8];
	uint8_t i;
	
	ds18b20_Reset();
    ds18b20_WriteByte(0xCC);//SKIP ROM
	ds18b20_WriteByte(0x44);//CONVERT T
	HAL_Delay(800);

	ds18b20_Reset();
	ds18b20_WriteByte(0xCC);//SKIP ROM
	ds18b20_WriteByte(0xBE);	//READ SCRATCHPAD
	for (i = 0; i < 8; i++)
	{
		Data[i] = ds18b20_ReadByte();
	}	

	//((uint16_t)dt[1] << 8) | dt[0]
	Temperatura = 100*((Data[1] & 0x7) << 4 | Data[0]>>4);
	Temperatura += 100*(Data[0] & 0xF)/16;
	//SSSS S654 3210 1234
	//Temperatura = ((dt & 0x07FF) >> 4);    //отборосим знаковые и дробные биты
	//Прибавим дробную часть
	//t += (float)(dt & 0x000F) / 16.0f;
	
	
}

//----------------------------------------------------------




//void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum)
//{
//	uint8_t i;
//	ds18b20_Reset();
//	if (mode == SKIP_ROM)
//	{
//		//SKIP ROM
//		ds18b20_WriteByte(0xCC);
//	}
//	//READ SCRATCHPAD
//	ds18b20_WriteByte(0xBE);
//	for (i = 0; i < 8; i++)
//	{
//		Data[i] = ds18b20_ReadByte();
//	}
//}

//----------------------------------------------------------

//uint8_t ds18b20_GetSign(uint16_t dt)
//{
//	//Проверим 11-й бит
//	if(dt&(1 << 11)) return 1;
//	else return 0;
//}

//----------------------------------------------------------

//float ds18b20_Convert(uint16_t dt)
//{
//	float t;
//	t = (float)((dt & 0x07FF) >> 4);  //отборосим знаковые и дробные биты
//	//Прибавим дробную часть
//	t += (float)(dt & 0x000F) / 16.0f;
//	return t;
//}
//----------------------------------------------------------