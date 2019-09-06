#ifndef SD_H_

#define SD_H_

//--------------------------------------------------

#include "stm32f1xx_hal.h"

#include <string.h>

#include <stdlib.h>

#include <stdint.h>

#include "main.h"


//--------------------------------------------------



#define SS_SD_SELECT() HAL_GPIO_WritePin(SD_SPI1_OnOff_Pin_GPIO_Port,SD_SPI1_OnOff_Pin_Pin, GPIO_PIN_RESET)

#define SS_SD_DESELECT() HAL_GPIO_WritePin(SD_SPI1_OnOff_Pin_GPIO_Port,SD_SPI1_OnOff_Pin_Pin, GPIO_PIN_SET)

#define LD_ON HAL_GPIO_WritePin(HUED_GPIO_Port,  HUED_Pin, GPIO_PIN_RESET); //green

#define LD_OFF HAL_GPIO_WritePin(HUED_GPIO_Port,  HUED_Pin, GPIO_PIN_SET); //green
void SPI_Release(void);

//--------------------------------------------------
/* Card type flags (CardType) */
#define CT_MMC 0x01 /* MMC ver 3 */
#define CT_SD1 0x02 /* SD ver 1 */
#define CT_SD2 0x04 /* SD ver 2 */
#define CT_SDC (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK 0x08 /* Block addressing */
//--------------------------------------------------
typedef struct sd_info {
	volatile uint8_t type; //��� �����
} sd_info_ptr;
//--------------------------------------------------
//--------------------------------------------------

#endif /* SD_H_ */