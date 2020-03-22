//#pragma once
#ifndef DEBUGGINGRTOS_H
#define DEBUGGINGRTOS_H

		//#include <_stdint.h>
		#include "FreeRTOS.h"
		#include "task.h"
		#include "string.h"

		/*
		* Macros used by vListTask to indicate which state a task is in.
		*/
		#define tskRUNNING_CHAR		( 'X' )
		#define tskBLOCKED_CHAR		( 'B' )
		#define tskREADY_CHAR		( 'R' )
		#define tskDELETED_CHAR		( 'D' )
		#define tskSUSPENDED_CHAR	( 'S' )
	
		#define TaskKolvoWithIDLE 4 // нужно сюда вписать количество тасков + 1(айдл) для корректного выделения памяти под структуру по полкам
		#define TaskListLeng 300
		char forVtaskList[TaskListLeng];

		uint16_t Ostatok;
		//uint16_t Mal = 0;
		struct Tasko
		{
			char Name[16];
			char Status; 
			int8_t Prioritet;
			uint16_t Stack;
			uint8_t Nomer; 	
		} ;

		struct Tasko Taskos[TaskKolvoWithIDLE];



		void TaskiPoPolkam(void);

# endif