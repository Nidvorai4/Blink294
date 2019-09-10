#include "./Modules/SD/SD.h"
//-----------------------------------------------
sd_info_ptr sdinfo;
char str1[60] = { 0 };
//-----------------------------------------------
//--------------------------------------------------
// Definitions for MMC/SDC command
#define CMD0 (0x40+0) // GO_IDLE_STATE
#define CMD1 (0x40+1) // SEND_OP_COND (MMC)
#define ACMD41 (0xC0+41) // SEND_OP_COND (SDC)
#define CMD8 (0x40+8) // SEND_IF_COND
#define CMD9 (0x40+9) // SEND_CSD
#define CMD16 (0x40+16) // SET_BLOCKLEN
#define CMD17 (0x40+17) // READ_SINGLE_BLOCK
#define CMD24 (0x40+24) // WRITE_BLOCK
#define CMD55 (0x40+55) // APP_CMD
#define CMD58 (0x40+58) // READ_OCR
//--------------------------------------------------
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


//-----------------------------------------------

static uint8_t SD_cmd(uint8_t cmd, uint32_t arg)

{
	uint8_t n, res;

	// ACMD<n> is the command sequense of CMD55-CMD<n>
	if(cmd & 0x80)
	{
		cmd &= 0x7F;
		res = SD_cmd(CMD55, 0);
		if (res > 1) return res;
	}
	

	// Select the card
	SS_SD_DESELECT();
	SPI_ReceiveByte();
	SS_SD_SELECT();
	SPI_ReceiveByte();


	// Send a command packet
	SPI_SendByte(cmd);   // Start + Command index
	SPI_SendByte((uint8_t)(arg >> 24));   // Argument[31..24]
	SPI_SendByte((uint8_t)(arg >> 16));   // Argument[23..16]
	SPI_SendByte((uint8_t)(arg >> 8));   // Argument[15..8]
	SPI_SendByte((uint8_t)arg);   // Argument[7..0]	

	
	n = 0x01;   // Dummy CRC + Stop
	if(cmd == CMD0) {n = 0x95; } // Valid CRC for CMD0(0)
	if(cmd == CMD8) {n = 0x87; } // Valid CRC for CMD8(0x1AA)
	SPI_SendByte(n);	
	

	// Receive a command response
	n = 10;  // Wait for a valid response in timeout of 10 attempts
	do {
		res = SPI_ReceiveByte();
	} while ((res & 0x80) && --n) ;
	return res;
}

//-----------------------------------------------

/*
static uint8_t SD_cmd_old(uint8_t cmd, uint32_t arg)
{
	uint8_t n, res,RR;
    //Продолжим нашу функцию передачи команды, вторым аргументом которой являются наши аргументы, 
    //объединённые в одну 32-битную величину, и напишем код, который будет определять, какого типа у нас команда. 
    //Команду ACMD отличает от CMD включенный седьмой бит 
	// ACMD<n> is the command sequense of CMD55-CMD<n>

	if (cmd & 0x80)
	{
	  cmd &= 0x7F;
	  res = SD_cmd(CMD55, 0);
	  if (res > 1) return res;
	}

	//Далее производим выбор карты вот таким вот интересным образом

	// Select the card
	SS_SD_DESELECT();
	SPI_ReceiveByte();
	SS_SD_SELECT();
	SPI_ReceiveByte();
	//Сначала мы поднимаем ножку CS, затем прогоняем по шине байт 0xFF, затем опускаем ножку и прогоняем ещё один такой же байт.
	//Далее уже передаём непосредственно команду
	// Send a command packet
	SPI_SendByte(cmd); // Start + Command index
	SPI_SendByte((uint8_t)(arg >> 24)); // Argument[31..24]
	SPI_SendByte((uint8_t)(arg >> 16)); // Argument[23..16]
	SPI_SendByte((uint8_t)(arg >> 8)); // Argument[15..8]
	SPI_SendByte((uint8_t)arg); // Argument[7..0]

	//То есть здесь мы последовательно передаём в шину аргументы команды, сначала передав её индекс.
	//Далее занесём контрольную сумму вместе со стоповым битом в переменную в зависимости от команды. 
	//То есть для этих двух команд сумма уже просчитана, для других нулевая, понятное дело со стоповым битом (0x01)
	n = 0x01; // Dummy CRC + Stop
	if (cmd == CMD0) {n = 0x95;} // Valid CRC for CMD0(0)
	if (cmd == CMD8) {n = 0x87;} // Valid CRC for CMD8(0x1AA)
	SPI_SendByte(n);
	//Пока нас интересует только первый тип. Если будут нужны другие типы, то первый байт нам все равно ждать, а с остальными разберёмся в коде после вызова команды
	//Добавим условный цикл в нашу функцию
	n = 100; // Wait for a valid response in timeout of 10 attempts
	do {
       	res = SPI_ReceiveByte();
		if (res != 0xFF && res != 0)
		{
			n = n + 1;
			n = n - 1;
		}
		//if (res == 0)res = 0xFF;
		RR = res & 0x80;
	} while ((res & 0x80) && --n);//То есть здесь мы ждём отклика от карты.
	return res;
}*/
//-----------------------------------------------
//Теперь наша задача — считать какие-то данные с карты. 
//Пока попробуем считать блок, так как читать файлы без умения считать элементарный блок мы попросту не сможем.
//Добавим для этого функцию в файле sd.c чуть выше функции sd_ini, во входных параметрах которой будет указатель на буфер, 
//в который будут считываться данные блока и адрес блока
uint8_t SD_Read_Block(uint8_t *buff, uint32_t lba)
{
	uint8_t result;
	uint16_t cnt;
	//Для того, чтобы считать один блок, существует команда CMD17, аргументом которой будет 32-битный адрес блока. 
	//Причём если карта повышенной вместимости (SDHC или SDXC), то адрес блока считается именно в блоках по 512 или более байт в зависимости от величины блока.
	//В других случаях адресом является номер байта от начала адресного пространства памяти карты. Адреса считаются с 0.
	result = SD_cmd(CMD17, lba);  //CMD17 даташит стр 50 и 96
	if(result != 0x00) return 5;  //Выйти, если результат не 0x00
	SPI_Release();
	cnt = 0;
	do {		 //Ждем начала блока
		result = SPI_ReceiveByte();
		cnt++;
	} while ((result != 0xFE)&&(cnt < 0xFFFF));
	if (cnt >= 0xFFFF) return 5;
	for (cnt = 0; cnt < 512; cnt++) 
		buff[cnt] = SPI_ReceiveByte();  //получаем байты блока из шины в буфер
	SPI_Release();  //Пропускаем контрольную сумму
	SPI_Release();
	return 0;
}
//-----------------------------------------------

uint8_t SD_Write_Block(uint8_t *buff, uint32_t lba)
{
	uint8_t result;
	uint16_t cnt;
	result = SD_cmd(CMD24, lba); //CMD24 даташит стр 51 и 97-98
	if(result != 0x00) return 6;  //Выйти, если результат не 0x00
	//SPI_Release();
	SPI_SendByte(0xFE);  //Начало буфера
	for(cnt = 0 ; cnt < 512 ; cnt++)
		SPI_SendByte(buff[cnt]);  //Данные
	SPI_Release();  //Пропустим котрольную сумму
	SPI_Release();
	result = SPI_ReceiveByte();
	if ((result & 0x05) != 0x05) return 6; //Выйти, если результат не 0x05 (Даташит Data Response Token: ...0101 = Data Accepted
	cnt = 0;
	do {		 //Ждем окончания состояния BUSY
	  result = SPI_ReceiveByte();
		cnt++;
	} while ((result != 0xFF)&&(cnt < 0xFFFF));
	if (cnt >= 0xFFFF) return 6;
	return 0;
}
//-----------------------------------------------










//-----------------------------------------------

uint8_t sd_ini(void)

{
	uint8_t i, cmd;
	uint8_t ocr[4];
	int16_t tmr;
	uint32_t temp;
	LD_OFF;
	sdinfo.type = 0;
	//Сохраним значение делителя шины SPI, установим пока другой делитель и заново инициализируем шину, 
	//чтобы уменьшить скорость, так как слишком быструю сразу карта может не понять
	
	//temp = hspi1.Init.BaudRatePrescaler;
	//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //156.25 kbbs
	//HAL_SPI_Init(&hspi1);
	//Затем передадим несколько импульсов на ножку синхронизации SPI. По даташиту нужно не менее 74, мы передадим на всякий случай 80.
	//Это можно сделать обычным прогоном байта по шине SPI. Причём есть ещё требование — ножка выбора должна быть поднята

	temp = hspi1.Init.BaudRatePrescaler;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;  //156.25 kbbs
	HAL_SPI_Init(&hspi1);	
	
	SS_SD_DESELECT();
	for (i = 0; i < 10; i++) //80 импульсов (не менее 74) Даташит стр 91
	  SPI_Release();

	hspi1.Init.BaudRatePrescaler = temp;
	HAL_SPI_Init(&hspi1);
	SS_SD_SELECT();
	//Таймаут включения мы добавили, 74 цикла мы уже проделали, теперь надо передать команду CMD0, 
	//которая делает программную перезагрузку, вводя карту в режим IDLE, в котором мы можем считать определённые параметры, 
	//прежде чем решить, как вести себя дальше с картой. В качестве аргументов данная команда требует все нули

	if(SD_cmd(CMD0, 0) == 1) // Enter Idle state
		{
			//отправим команду CMD8 в нашу карту с целью узнать версию нашей карты. 
			//Команду мы вызываем в теле положительного возврата из функции вызова команды CMD0. 
			//В качестве аргументов выступает регистр OCR, данные в который мы и заносим. 
			//Мы заносим 0,0,1,0xAA, такие требования именно для версии 2 карты SD.
			//Перед вызовом команды прогоним байт через шину
			//Следует отметить что команда CMD8 возвращает уже результат формата R7
			SPI_Release();
			if (SD_cmd(CMD8, 0x1AA) == 1) // SDv2
				{
					//Как мы видим, возвращённый результат состоит из 5 байтов, 
					//самый старший из них (он же тот, который прийдёт первым) является результатом формата R1. 
					//Мы его получим сразу в возврате из функции. А остальные 4 байта мы получим отдельно.
					for(i = 0 ; i < 4 ; i++) ocr[i] = SPI_ReceiveByte();
					//sprintf(str1, "OCR: 0x%02X 0x%02X 0x%02X 0x%02Xrn", ocr[0], ocr[1], ocr[2], ocr[3]);
					//HAL_UART_Transmit(&huart1, (uint8_t*)str1, strlen(str1), 0x1000);
					// Get trailing return value of R7 resp
					if(ocr[2] == 0x01 && ocr[3] == 0xAA) // The card can work at vdd range of 2.7-3.6V
					{
						temp = 1;
						for (tmr = 12000; tmr && temp; tmr--)// Wait for leaving idle state (ACMD41 with HCS bit) HostCapacitySupport
						{
							temp = SD_cmd(ACMD41, 1UL << 30);
							//temp = SD_cmd(ACMD41, 0);
							i = 4;
						}
						
						//Затем мы вызываем команду CMD58 для чтения регистра OCR, в котором мы узнаём состояние бита CCS, 
						//и если он установлен, это позволяет нам считать, что карта у нас типа SDHC, то есть повышенной вместимости, 
						//а если состояние бита ноль, то SDSC (стандартной вместимости). Входные аргументы — нули
						//LD_ON;
						if(tmr && SD_cmd(CMD58, 0) == 0)  // Check CCS bit in the OCR
						{ 
							for (i = 0; i < 4; i++) ocr[i] = SPI_ReceiveByte();
							//sprintf(str1,"OCR: 0x%02X 0x%02X 0x%02X 0x%02Xrn",ocr[0],ocr[1],ocr[2],ocr[3]);
							//HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
							sdinfo.type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; // SDv2 (HC or SC)
							LD_ON;
						}
					}
				}
			else //SDv1 or MMCv3
				{
					if (SD_cmd(ACMD41, 0) <= 1)
					{
						sdinfo.type = CT_SD1; cmd = ACMD41;  // SDv1
					}
					else
					{
						sdinfo.type = CT_MMC; cmd = CMD1;  // MMCv3
					}
					for (tmr = 25000; tmr && SD_cmd(cmd, 0); tmr--) ; // Wait for leaving idle state
					if(!tmr || SD_cmd(CMD16, 512) != 0) // Set R/W block length to 512
					sdinfo.type = 0;
				}
		}
	else
		{
			return 1;
		}	
//	sprintf(str1, "Type SD: 0x%02Xrn", sdinfo.type);
//	HAL_UART_Transmit(&huart1, (uint8_t*)str1, strlen(str1), 0x1000);
	return 0;
}

//-----------------------------------------------


































/*

uint8_t sd_ini_old(void)
{
	HAL_Delay(20);
	uint8_t i, cmd;
	uint8_t ocr[4]; // для куска ответа в формате R7
	int16_t tmr;
	uint32_t temp;
	LD_OFF;
	sdinfo.type = 0;
	//Сохраним значение делителя шины SPI, установим пока другой делитель и заново инициализируем шину, 
	//чтобы уменьшить скорость, так как слишком быструю сразу карта может не понять
	
		//temp = hspi1.Init.BaudRatePrescaler;
		//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //156.25 kbbs
		//HAL_SPI_Init(&hspi1);
	//Затем передадим несколько импульсов на ножку синхронизации SPI. По даташиту нужно не менее 74, мы передадим на всякий случай 80.
	//Это можно сделать обычным прогоном байта по шине SPI. Причём есть ещё требование — ножка выбора должна быть поднята
	SS_SD_DESELECT();
	for(i=0;i<10;i++) //80 импульсов (не менее 74) Даташит стр 91
	  SPI_Release();

		//hspi1.Init.BaudRatePrescaler = temp;
		//HAL_SPI_Init(&hspi1);

	SS_SD_SELECT();
	
	//Таймаут включения мы добавили, 74 цикла мы уже проделали, теперь надо передать команду CMD0, 
	//которая делает программную перезагрузку, вводя карту в режим IDLE, в котором мы можем считать определённые параметры, 
	//прежде чем решить, как вести себя дальше с картой. В качестве аргументов данная команда требует все нули
	 
	if(SD_cmd(CMD0, 0) == 1) // Enter Idle state
	{
		//отправим команду CMD8 в нашу карту с целью узнать версию нашей карты. 
		//Команду мы вызываем в теле положительного возврата из функции вызова команды CMD0. 
		//В качестве аргументов выступает регистр OCR, данные в который мы и заносим. 
		//Мы заносим 0,0,1,0xAA, такие требования именно для версии 2 карты SD.
		//Перед вызовом команды прогоним байт через шину
		//Следует отметить что команда CMD8 возвращает уже результат формата R7
		SPI_Release();
		if (SD_cmd(CMD8, 0x1AA) == 1) // SDv2
			{
				//Как мы видим, возвращённый результат состоит из 5 байтов, 
				//самый старший из них (он же тот, который прийдёт первым) является результатом формата R1. 
				//Мы его получим сразу в возврате из функции. А остальные 4 байта мы получим отдельно.
				for(i = 0 ; i < 4 ; i++) ocr[i] = SPI_ReceiveByte();
				// Get trailing return value of R7 resp
				if(ocr[2] == 0x01 && ocr[3] == 0xAA) // The card can work at vdd range of 2.7-3.6V
				{
					for (tmr = 12000; tmr && SD_cmd(ACMD41, 1UL << 30); tmr--) ; // Wait for leaving idle state (ACMD41 with HCS bit)
					//Затем мы вызываем команду CMD58 для чтения регистра OCR, в котором мы узнаём состояние бита CCS, 
					//и если он установлен, это позволяет нам считать, что карта у нас типа SDHC, то есть повышенной вместимости, 
					//а если состояние бита ноль, то SDSC (стандартной вместимости). Входные аргументы — нули
					LD_ON;
					if(tmr && SD_cmd(CMD58, 0) == 0) {
						 // Check CCS bit in the OCR
						for(i = 0 ; i < 4 ; i++) ocr[i] = SPI_ReceiveByte();
						sdinfo.type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;   // SDv2 (HC or SC)
						
					}
				}
				else //SDv1 or MMCv3
				{
					if (SD_cmd(ACMD41, 0) <= 1)
					{
						sdinfo.type = CT_SD1; cmd = ACMD41;   // SDv1
					}
					else
					{
						sdinfo.type = CT_MMC; cmd = CMD1;   // MMCv3
					}
					for (tmr = 25000; tmr && SD_cmd(cmd, 0); tmr--) ; // Wait for leaving idle state
					if(!tmr || SD_cmd(CMD16, 512) != 0) // Set R/W block length to 512
					sdinfo.type = 0;
				}
			}
	}
	else
	{
		return 1;
	}
	//SPI_SendByte(0x35); 	SPI_SendByte(0x53); // это были тестовые тычки для просмотра на анализаторе
	
	return 0;
}

//-----------------------------------------------
*/