#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Debug

#Additional flags
PREPROCESSOR_MACROS := DEBUG=1 flash_layout STM32F103xB USE_FREERTOS
INCLUDE_DIRS := C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\CMSIS\Include C:/Users/Basay/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source/include C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Middlewares\Third_Party\FreeRTOS\Source\CMSIS_RTOS Inc C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\CMSIS\Device\ST\STM32F1xx\Include C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\STM32F1xx_HAL_Driver\Inc C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\STM32F1xx_HAL_Driver\Src C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Middlewares\Third_Party\FreeRTOS\Source\include C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Middlewares\Third_Party\FreeRTOS\Source\portable\GCC\ARM_CM3 C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\STM32F1xx_HAL_Driver\Inc\Legacy C:\Users\Basay\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Middlewares\Third_Party\FatFs\src
LIBRARY_DIRS := C:/Users/Basay/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.0/Middlewares/Third_Party/FreeRTOS/Source
LIBRARY_NAMES := 
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O0
CXXFLAGS := -ggdb -ffunction-sections -fno-exceptions -fno-rtti -O0
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections -Wl,-q
COMMONFLAGS := 
LINKER_SCRIPT := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
CP_NOT_AVAILABLE := 1

ADDITIONAL_MAKE_FILES := stm32.mak
GENERATE_BIN_FILE := 1
GENERATE_IHEX_FILE := 0
