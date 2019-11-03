#include "epd_2in9.h"

const uint8_t EPD_lut_full_update[30] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t EPD_lut_partial_update[30] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void EPD_Reset()
{
	DEV_GPIO_Write(EPD_RST_PIN, 1);
	HAL_Delay(200);
	DEV_GPIO_Write(EPD_RST_PIN, 0);
	HAL_Delay(200);
	DEV_GPIO_Write(EPD_RST_PIN, 1);
	HAL_Delay(200);
}

void EPD_SendCom(uint8_t Reg)
{
	DEV_GPIO_Write(EPD_DC_PIN, 0);
	DEV_GPIO_Write(EPD_CS_PIN, 0);
	DEV_SPI_WriteByte(Reg);
	DEV_GPIO_Write(EPD_CS_PIN, 1);
}

void EPD_SendData(uint8_t Data)
{
	DEV_GPIO_Write(EPD_DC_PIN, 1);
	DEV_GPIO_Write(EPD_CS_PIN, 0);
	DEV_SPI_WriteByte(Data);
	DEV_GPIO_Write(EPD_CS_PIN, 1);
}

void EPD_SendDatas(uint8_t *Data, uint16_t len)
{
	DEV_GPIO_Write(EPD_DC_PIN, 1);
	DEV_GPIO_Write(EPD_CS_PIN, 0);
	HAL_SPI_Transmit(&hspi1, Data, len, 0xff);
	DEV_GPIO_Write(EPD_CS_PIN, 1);
}

void EPD_WhitBusy()
{
	while(DEV_GPIO_Read(EPD_BUSY_PIN) == GPIO_PIN_SET);
}

void EPD_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
	EPD_SendCom(0x44);
	EPD_SendData(Xstart >> 3);
	EPD_SendData(Xend >> 3);
	
	EPD_SendCom(0x45);
	EPD_SendData(Ystart);
	EPD_SendData(Ystart >> 8);
	EPD_SendData(Yend);
	EPD_SendData(Yend >> 8);
}

void EPD_SetCursor(uint16_t Xstart, uint16_t Ystart)
{
	EPD_SendCom(0x4e);
	EPD_SendData(Xstart >> 3);
	EPD_SendCom(0x4f);
	EPD_SendData(Ystart);
	EPD_SendData(Ystart >> 8);
}

void EPD_DisON()
{
	EPD_SendCom(0x22);
	EPD_SendData(0xc4);
	EPD_SendCom(0x20);
	EPD_SendCom(0xff);
	EPD_WhitBusy();
}

void EPD_Init(uint8_t Mode)
{
	EPD_Reset();
	
	EPD_SendCom(0x01);
	EPD_SendData((uint8_t)(EPD_HEIGHT - 1));
	EPD_SendData((EPD_HEIGHT - 1) >> 8);
	EPD_SendData(0x00);
	
	EPD_SendCom(0x0c);
	EPD_SendData(0xd7);
	EPD_SendData(0xd6);
	EPD_SendData(0x9d);
	
	EPD_SendCom(0x2c);
	EPD_SendData(0xa8);
	
	EPD_SendCom(0x3a);
	EPD_SendData(0x1a);
	
	EPD_SendCom(0x3b);
	EPD_SendData(0x08);
	
	EPD_SendCom(0x3c);
	EPD_SendData(0x03);
	EPD_SendCom(0x11);
	EPD_SendData(0x03);
	
	EPD_SendCom(0x32);
	if(Mode == EPD_FULL)
		EPD_SendDatas((uint8_t*)EPD_lut_full_update, 30);
	else if(Mode == EPD_PART)
		EPD_SendDatas((uint8_t*)EPD_lut_partial_update, 30);
	else
		while(1);//error
}

void EPD_Clear(uint8_t data)
{
	uint8_t databuf[16];
	for(int i = 0; i < 16; i++)
		databuf[i] = data;
	
	EPD_SetWindows(0, 0, EPD_WIDTH, EPD_HEIGHT);
	for(uint16_t j = 0; j < EPD_HEIGHT; j++)
	{
		EPD_SetCursor(0, j);
		EPD_SendCom(0x24);
		EPD_SendDatas((uint8_t*)databuf, 16);
	}
	EPD_DisON();
}

void EPD_Display(uint8_t* Image)
{
	EPD_SetWindows(0, 0, EPD_WIDTH, EPD_HEIGHT);
	for(uint16_t j = 0; j < EPD_HEIGHT; j++)
	{
		EPD_SetCursor(0, j);
		EPD_SendCom(0x24);
		EPD_SendDatas((uint8_t*)Image + j * 16, 16);
	}
	EPD_DisON();
}

void EPD_Sleep()
{
	EPD_SendCom(0x10);
	EPD_SendData(0x01);
}
