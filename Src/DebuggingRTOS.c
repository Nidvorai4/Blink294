#include "DebuggingRTOS.h"

void TaskiPoPolkam(void)
{
#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

	Ostatok = xPortGetFreeHeapSize();
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	char cStatus;

	/*
	 * PLEASE NOTE:
	 *
	 * This function is provided for convenience only, and is used by many
	 * of the demo applications.  Do not consider it to be part of the
	 * scheduler.
	 *
	 * vTaskList() calls uxTaskGetSystemState(), then formats part of the
	 * uxTaskGetSystemState() output into a human readable table that
	 * displays task names, states and stack usage.
	 *
	 * vTaskList() has a dependency on the sprintf() C library function that
	 * might bloat the code size, use a lot of stack, and provide different
	 * results on different platforms.  An alternative, tiny, third party,
	 * and limited functionality implementation of sprintf() is provided in
	 * many of the FreeRTOS/Demo sub-directories in a file called
	 * printf-stdarg.c (note printf-stdarg.c does not provide a full
	 * snprintf() implementation!).
	 *
	 * It is recommended that production systems call uxTaskGetSystemState()
	 * directly to get access to raw stats data, rather than indirectly
	 * through a call to vTaskList().
	 */


	/* Make sure the write buffer does not contain a string. */
	//*pcWriteBuffer = 0x00;

	/* Take a snapshot of the number of tasks in case it changes while this
	function is executing. */
	uxArraySize = uxTaskGetNumberOfTasks();

	/* Allocate an array index for each task.  NOTE!  if
	configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
	equate to NULL. */
	pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

	if (pxTaskStatusArray != NULL)
	{
		/* Generate the (binary) data. */
		uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

		/* Create a human readable table from the binary data. */
		for (x = 0; x < uxArraySize; x++)
		{
			switch (pxTaskStatusArray[x].eCurrentState)
			{
			case eRunning:		cStatus = tskRUNNING_CHAR;
				break;

			case eReady:		cStatus = tskREADY_CHAR;
				break;

			case eBlocked:		cStatus = tskBLOCKED_CHAR;
				break;

			case eSuspended:	cStatus = tskSUSPENDED_CHAR;
				break;

			case eDeleted:		cStatus = tskDELETED_CHAR;
				break;

			default:			/* Should not get here, but it is included
								to prevent static checking errors. */
				cStatus = 0x00;
				break;
			}

			/* Write the task name to the string, padding with spaces so it
			can be printed in tabular form more easily. */
			//pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray[x].pcTaskName);
			//const char *Name = pxTaskStatusArray[x].pcTaskName;
					
			if(uxArraySize == TaskKolvoWithIDLE) {
				strcpy(Taskos[x].Name, pxTaskStatusArray[x].pcTaskName);
				Taskos[x].Status = cStatus;
				Taskos[x].Nomer = pxTaskStatusArray[x].xTaskNumber;
				Taskos[x].Prioritet = pxTaskStatusArray[x].uxCurrentPriority;
				Taskos[x].Stack = pxTaskStatusArray[x].usStackHighWaterMark;     //sprintf(,"" // Taskos[0].Name = "NESOVPADAETKOLVO";
			}
			else {
				strcpy(Taskos[0].Name, "NESOVPADAETKOLVO");
				Taskos[x].Status = 69;
				Taskos[x].Nomer = 69;
				Taskos[x].Prioritet = 69;
				Taskos[x].Stack = 69;
			}
			

			/* Write the rest of the string. */
			//sprintf(pcWriteBuffer, "\t%c\t%u\t%u\t%u\r\n", cStatus, (unsigned int) pxTaskStatusArray[x].uxCurrentPriority, (unsigned int) pxTaskStatusArray[x].usStackHighWaterMark, (unsigned int) pxTaskStatusArray[x].xTaskNumber);
			//pcWriteBuffer += strlen(pcWriteBuffer);
			
		}

		/* Free the array again.  NOTE!  If configSUPPORT_DYNAMIC_ALLOCATION
		is 0 then vPortFree() will be #defined to nothing. */
		vPortFree(pxTaskStatusArray);
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
	

#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */

		
}
