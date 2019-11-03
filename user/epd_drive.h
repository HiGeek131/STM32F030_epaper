#ifndef __EPD_DRIVE_H
#define __EPD_DRIVE_H

#include "main.h"
#include "spi.h"
#include "gpio.h"

//定义硬件驱动引脚
#define EPD_RST_PIN		RES_GPIO_Port, RES_Pin
#define EPD_DC_PIN		DC_GPIO_Port, DC_Pin
#define EPD_CS_PIN		CS_GPIO_Port, CS_Pin
#define EPD_BUSY_PIN	BUSY_GPIO_Port, BUSY_Pin

//定义硬件驱动接口
#define DEV_GPIO_Write(_pin, _value)	HAL_GPIO_WritePin(_pin, _value == 0 ? GPIO_PIN_RESET:GPIO_PIN_SET)
#define DEV_GPIO_Read(_pin)						HAL_GPIO_ReadPin(_pin)

void DEV_SPI_WriteByte(uint8_t value);

#endif
