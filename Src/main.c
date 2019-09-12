/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include <iostream>
#include <string.h>
#include "WorkWithOLED.h"
#include "./Modules/SD/SD.h"
#include "./Modules/RTC/RTClock.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//для работы с СД ->
						volatile uint8_t sect[512]; // для чтения с СД
						extern char str1[60];
						uint32_t byteswritten, bytesread;
						uint8_t result;
						extern char USER_Path[4]; /* logical drive path */
						FATFS SDFatFs;
						FATFS *fs;
						FIL MyFile;
// <- для работы с СД
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void i2c_init(void); // ??????? ????????????? ???? TWI
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile _Bool  EncLastState, EncState, EncFlag;
volatile int EncCount = 0;
#define ENC_TYPE 2


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//int8_t EncCLK, EncDT;
	if (GPIO_Pin == EncoderCLKint_Pin)
	{
		
		EncState = HAL_GPIO_ReadPin(EncoderCLKint_GPIO_Port, EncoderCLKint_Pin);
		if (EncState != EncLastState)
		{
            #if (ENC_TYPE==1)
			EncFlag = !EncFlag;
		    if (EncFlag)
			    EncCount +=	(HAL_GPIO_ReadPin(EncoderDT_GPIO_Port, EncoderDT_Pin) != EncLastState) ? - 1 : 1;
            #else
			EncCount +=	(HAL_GPIO_ReadPin(EncoderDT_GPIO_Port, EncoderDT_Pin) != EncLastState) ? - 1 : 1;
            #endif 
			EncLastState = EncState;
		}
	} 
}




void OtladkaPrint(uint8_t NextLine,const char* args, ...)
{
	char StrBuff[0xFF] = { 0 };
	va_list ap;
	va_start(ap, args);
	vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);
	if (NextLine) sprintf(StrBuff, "%s\n\r",StrBuff);
	StrBuff[strlen(StrBuff)] = 0;
	HAL_UART_Transmit(&huart3, (uint8_t *)StrBuff, (uint16_t)strlen(StrBuff), 100);

}
/* string to_binary_string(unsigned int n)
{
	string result;
	do
	{
		result = ('0' + (n % 2)) + result;
		n = n / 2;
	} while (n > 0);
	return result;
}*/



FRESULT ReadLongFile(void)
{
	uint16_t i = 0, i1 = 0;
	uint32_t ind = 0;
	uint32_t f_size = MyFile.fsize;
	#ifdef VyvodInfoFromKepka
		sprintf(str1, "fsize: %lu\r\n", (unsigned long)f_size);
		HAL_UART_Transmit(&huart3, (uint8_t*)str1, strlen(str1), 0x1000);
	#endif
	
	ind = 0;
	do
	{
		if (f_size < 512)
		{
			i1 = f_size;
		}
		else
		{
			i1 = 512;
		}
		f_size -= i1;
		f_lseek(&MyFile, ind);
		f_read(&MyFile, sect, i1, (UINT *)&bytesread);
		for (i = 0; i < bytesread; i++)
		{
			HAL_UART_Transmit(&huart3, sect + i, 1, 0x1000);
		}
		ind += i1;
	}
 while (f_size > 0);
	HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", 2, 0x1000);
	return FR_OK;

}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
// для работы с СД ->
						FRESULT res;  //результат выполнения
						uint8_t wtext[] = "Hello from STM32!!!";
						FILINFO fileInfo;
						char *fn;
						DIR dir;
						DWORD fre_clust, fre_sect, tot_sect;
// <- для работы с СД 
	
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
//	HAL_Delay(100);
//	LCD_init();
//	LCD_Clear();
//	
//	
//	LCD_Num_To_Str(23456789, 1);
//	LCD_Clear();
//	LCD_Num_To_Str(23456789, 5);
//	LCD_Clear();
//	LCD_Num_To_Str(23456789, 10);
	
#if 0
	//LCD_String("");
	//LCD_String("уфхцчшщъыьэюя");
    LCD_GoTo(2, 2);
	//LCD_String("рст РСТ {|}~#  ");
	LCD_GoTo(2, 3);
	LCD_String("АБВГДЕЁЖЗИЙКЛМН");
	LCD_GoTo(20, 4);
	//LCD_String("ХУЙ");
	LCD_String("ОПРСТУФХЦЧШЩЪЫьЭЮЯ");
	LCD_GoTo(5, 5);
	LCD_String("абвгдеёжзиклмнопрст");
	LCD_GoTo(6, 6);
	LCD_String("уфхцчшщъыьэюя");
	
	
#endif // 0
	 

/*#define	RIGHT_HORIZ_SCROLL     0x26
#define    LEFT_HORIZ_SCROLL      0x27
#define    VERT_AND_RIGHT_HORIZ_SCROLL  0x29
#define    VERT_AND_LEFT_HORIZ_SCROLL  0x2A
#define    DEACTIVATE_SCROLL      0x2E
#define    ACTIVATE_SCROLL        0x2F
#define    SET_START_LINE         0x40
#define    SET_CONTRAST           0x81
#define    CHARGE_PUMP            0x8D
#define    SEG_REMAP              0xA0
#define    SET_VERT_SCROLL_AREA   0xA3
	
#define SET_HWSCROLL_OFF    0x2E
#define SET_HWSCROLL_ON     0x2F
#define SET_HWSCROLL_RIGHT  0x26
#define SET_HWSCROLL_LEFT   0x27
#define SET_HWSCROLL_VR     0x29
#define SET_HWSCROLL_VL     0x2A

	
	//sendCommand(0xAE); //выкл диспл
	
	HAL_Delay(1000);
	
	
	
	
	
	sendCommand(SET_HWSCROLL_OFF);
	//LCD_String("ХУЙ");
	sendCommand(SET_HWSCROLL_VR);
	//HAL_Delay(10);
	sendCommand(0x00); //A
	//HAL_Delay(10);
	sendCommand(0x00); //B
	//HAL_Delay(10);
	sendCommand(0x00);  //C
	//HAL_Delay(10);
	sendCommand(0x0F); //D
	//HAL_Delay(10);
	sendCommand(0x00); //E
	//sendCommand(0x00); //F
	//HAL_Delay(10);
	//sendCommand(0xA3);
	//sendCommand(0x00);
	//sendCommand(0x64);
	
	
	sendCommand(SET_HWSCROLL_VR);
	sendCommand(SET_HWSCROLL_ON);
	
	
	//sendCommand(0xAF); // вкл диспл
	
	
/*	sendCommand(0x2E);
	sendCommand(0x26);
	sendCommand(0x00);
	sendCommand(0x05);
	sendCommand(0x01);
	sendCommand(0x07);
	sendCommand(0x2F);
	*/
	//	sendCommand(0x2E);
	//	sendCommand(0x29);
	//	sendCommand(0x29);
	//	sendCommand(0x29);
	//	sendCommand(0x2F);
	
	//uint8_t CheckVolt()
	//{
	//	pvdo
	//}

	//HAL_UART_Transmit(&huart3, (uint8_t*)("################"), 16, 0x1000);
	OtladkaPrint(1,"########################\r\n");
	
	
//disk_initialize(SDFatFs.drv);

//	//read dir
//	if(f_mount(&SDFatFs, (TCHAR const*)USER_Path, 0) != FR_OK)
//	{
//		Error_Handler();
//	}
//	else
//	{
//		fileInfo.lfname = (char*)sect;
//		fileInfo.lfsize = sizeof(sect);
//		result = f_opendir(&dir, "/");
//		if (result == FR_OK)
//		{
//			f_closedir(&dir);
//		}
//	}	
													/* Move to end of the file to append data */
													//res = f_lseek(fp, f_size(fp));
	
													//Размонтируем:
													//
													//if(f_mount(NULL,USER_Path,0)==FR_OK)  FATFS_UnLinkDriver(USER_Path);  
	
	
	
	
//	//write
//
//	if(f_mount(&SDFatFs, (TCHAR const*)USERPath, 0) != FR_OK)
//	{
//		Error_Handler();
//	}
//	else
//	{
//		if (f_open(&MyFile, "myHUI.txt", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK)
//		{
//			Error_Handler();
//		}
//		else
//		{
//			res = f_write(&MyFile, wtext, sizeof(wtext), (void*)&byteswritten);
//			res = f_lseek(&MyFile, f_size(&MyFile));
//			res = f_write(&MyFile, "HUiCSHE\n\r", 9, (void*)&byteswritten);
//			if ((byteswritten == 0) || (res != FR_OK))
//			{
//				Error_Handler();
//			}
//			f_close(&MyFile);
//		}
//	}
//	
//	
//	//read
//
//if(f_mount(&SDFatFs, (TCHAR const*)USERPath, 0) != FR_OK)
//	{
//		Error_Handler();
//	}
//	else
//	{
//		if (f_open(&MyFile, "myhui.txt", FA_READ) != FR_OK)
//		{
//			Error_Handler();
//		}
//		else
//		{
//			ReadLongFile();
//			f_close(&MyFile);
//		}
//	}
//
//	
//FATFS_UnLinkDriver(USERPath);	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  RTClockRead();
	  //RTClockInit();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //LCD_GoTo(52, 7);
	 // LCD_String("#");
	
	  
	  //LCD_GoTo(52, 7);
	  //LCD_Num_To_Str(gg++,5);
	  //LCD_Char2(++gg);
	  //LCD_String(  "  ");
	  
	  //LCD_Num_To_Str(EncCount ,3);
	  //if(gg > 169)gg = 1;
//	  LD_OFF;
//	  HAL_Delay(500);
	  //HAL_GPIO_TogglePin(HUED_GPIO_Port, HUED_Pin); 
	  
	  // unsigned int rrr = GPIOC->ODR;
	  // HexToChar(rrr);
	   //LCD_String("");
	  //LCD_String("1");
	  //LCD_String("2");
	  //LCD_String("34");
	  // LCD_PrintReg(GPIOC->ODR, "ODR");
//	  PWR->CR |= PWR_CR_PVDE | PWR_CR_PLS_2V2;
//	  //LCD_PrintReg(PWR->CR, "PWR->CR");
//	  LCD_PrintReg(PWR->CSR, "PWR->CSR");
//	 // int OFR=
//	  int OF = PWR->CSR & 1 << 2;
//	  LCD_Num_To_Str(OF, 1);
//	  
//	  
// 	  HAL_Delay(500);
//	  
//	  
//	  
//	  LCD_GoTo(3, 8);
//	  LCD_Num_To_Str( EncCount++,3);
//	  
//	  //odr
//	  
//	  if(0 && sd_ini() == 0)
//	  {
//		  
//		  char buffer1[512] = "Go fuck yourself fucking stupid duck. Fuck you Go fuck yourself fucking stupid duck. Fuck youGo fuck yourself fucking stupid duck. Fuck youGo fuck yourself fucking stupid duck. Fuck you";
//		  SD_Write_Block((uint8_t*)buffer1, 0x0400);
//		  SD_Read_Block(sect, 0x0400);    //Считаем блок из буфера
//		  //SD_Read_Block(sect, 0x6);
//		  if(sect[5]!=0)
//		   LCD_GoTo(3, 8);
//	  }
	  
	  

	  
	  
	  //HAL_UART_Transmit(&huart3, (uint8_t*)"USER_initializern\r\n", 19, 0x1000);
		  
	  //PWR_CSR_PVDO;
	  
	  
	  //printf("HUI");
	  //sendCommand(SET_HWSCROLL_LEFT);
	  //sendCommand(SET_HWSCROLL_ON);
	 // sendCommand(0xAF);  // вкл диспл
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	//HAL_UART_Transmit(&huart3, (uint8_t*)("\n\r...........Error"), 18, 0x1000);
	OtladkaPrint("Error.......",0);
	OtladkaPrint(result,1);
	
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
