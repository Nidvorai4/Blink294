/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "DebuggingRTOS.h"
#include "WorkWithOLED.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId FR_DISPLAYtaskHandle;
osThreadId FR_Gradusnik_taHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void FR_DISPLAY(void const * argument);
void FR_Gradusnik(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of FR_DISPLAYtask */
  osThreadDef(FR_DISPLAYtask, FR_DISPLAY, osPriorityIdle, 0, 128);
  FR_DISPLAYtaskHandle = osThreadCreate(osThread(FR_DISPLAYtask), NULL);

  /* definition and creation of FR_Gradusnik_ta */
  osThreadDef(FR_Gradusnik_ta, FR_Gradusnik, osPriorityIdle, 0, 128);
  FR_Gradusnik_taHandle = osThreadCreate(osThread(FR_Gradusnik_ta), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  TaskiPoPolkam();
	  osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_FR_DISPLAY */
/**
* @brief Function implementing the FR_DISPLAYtask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FR_DISPLAY */
void FR_DISPLAY(void const * argument)
{
  /* USER CODE BEGIN FR_DISPLAY */
	LCD_Clear();
	/* Infinite loop */
  for(;;)
  {
	  
//	  LCD_PrintPik(0, 106, PikiBatteryKorp, LCDpt_NOIV);              //                          батарея
//	  for (int i = 1; i  <= 8; i++) {
//		  //LCD_Clear();
//		  
//		  
//		  LCD_PrintPik(0, 106 + i * 2, PikiBatteryDelenie, LCDpt_NOIV);
//		  osDelay(50);
//		  //LCD_PrintPik(0, 106 + i * 2, PikiBatteryDelenie, DEL);
//		  
//	  }
//	  osDelay(500);
//	  LCD_PrintPik(0, 106, PikiBatteryKorp, LCDpt_DEL);
	  LCD_PrintPik(0, 0, PikiBatteryKorp, LCDpt_NOINV);
	  //osDelay(50);
    
	  
	  RTClockRead();
	  
	  	  //LCD_Clear();

	  
	  //LCD_String("1234567890abcdefghjklmnoprstuvwxyz");
	  //LCD_GoTo(1, 1);
	  //LCD_String("12345678901234");
	  //LCD_GoTo(21, 4);
	  //LCD_String("X");
	  //LCD_Num_To_Str(Taskos[1].Stack,3);
    osDelay(500);
  }
  /* USER CODE END FR_DISPLAY */
}

/* USER CODE BEGIN Header_FR_Gradusnik */
/**
* @brief Function implementing the FR_Gradusnik_ta thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FR_Gradusnik */
void FR_Gradusnik(void const * argument)
{
  /* USER CODE BEGIN FR_Gradusnik */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END FR_Gradusnik */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
