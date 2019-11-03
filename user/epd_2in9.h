#ifndef __EPD_2IN9_H
#define __EPD_2IN9_H

#include "epd_drive.h"

#define EPD_WIDTH 128
#define EPD_HEIGHT 296

#define EPD_FULL 1
#define EPD_PART 0

void EPD_Init(uint8_t Mode);
void EPD_Clear(uint8_t data);
void EPD_Display(uint8_t* Image);
void EPD_Sleep(void);

#endif
