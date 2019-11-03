#include "epd_drive.h"

void DEV_SPI_WriteByte(uint8_t value)
{
	HAL_SPI_Transmit(&hspi1, &value, 1, 0xff);
}
