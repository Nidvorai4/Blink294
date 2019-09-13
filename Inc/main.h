/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h> // для функции вывода в консоль
#include "stdio.h" // для функции вывода в консоль
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
	//void OtladkaPrint(char* Tekst, uint32_t Chislo, uint8_t T1Ch2TandCh3, uint8_t NewLine);
//void OtladkaPrint(const char* args, ...);
void OtladkaPrint(uint8_t NextLine, const char* args, ...);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HUED_Pin GPIO_PIN_13
#define HUED_GPIO_Port GPIOC
#define GradusnikPin_Pin GPIO_PIN_2
#define GradusnikPin_GPIO_Port GPIOA
#define SD_SPI1_OnOff_Pin_Pin GPIO_PIN_4
#define SD_SPI1_OnOff_Pin_GPIO_Port GPIOA
#define SD_SPI1_SCK_Pin GPIO_PIN_5
#define SD_SPI1_SCK_GPIO_Port GPIOA
#define SD_SPI1_Miso_Pin GPIO_PIN_6
#define SD_SPI1_Miso_GPIO_Port GPIOA
#define SD_SPI1_Mosi_Pin GPIO_PIN_7
#define SD_SPI1_Mosi_GPIO_Port GPIOA
#define EncoderCLKint_Pin GPIO_PIN_0
#define EncoderCLKint_GPIO_Port GPIOB
#define EncoderCLKint_EXTI_IRQn EXTI0_IRQn
#define EncoderDT_Pin GPIO_PIN_1
#define EncoderDT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define VyvodInfoFromKepka 1 // раскоментировать, если в консоли нужна инфа по внутрянке работы с СД
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
