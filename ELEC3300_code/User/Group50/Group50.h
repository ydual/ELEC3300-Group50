#ifndef __GROUP50_H__
#define __GROUP50_H__

#include <stm32f10x.h>
#include "./ov7725/bsp_ov7725.h"
#include "./sccb/bsp_sccb.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./lcd/bsp_lcd.h"
#include "./led/bsp_led.h"
#include "./57StepMotor/bsp_TB6600_TIM.h" 
#include "./Key/bsp_key.h"
#include "./SG90/bsp_GeneralTIM.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./bmp/bsp_bmp.h"
#include "../FatFs/ff.h"
#include "..\Mass_Storage\src\hw_config.h" 
#include "..\STM32_USB-FS-Device_Driver\inc\usb_lib.h"
#include "..\Mass_Storage\src\usb_pwr.h"
#include "./hc05/bsp_hc05.h"
#include "./usart/bsp_usart_blt.h"
#include "./usart/bsp_usart.h"
#include <stdio.h>
#include <stdlib.h>

#define ONE_STEP 256
#define MIN(x,y) x>y ? y:x
#define MAX(x,y) x>y ? x:y

void Bluetooth_Control(void);
void Touch_Control(void);
void Camera_Mode(void);
void Control_Mode(void);
void SD_Card_Mode(void);
void USB_Mode(void);
void Execute_Command(int num);
void Execute_Command_Move_Together(int num);
void Motor_Move_1(int step);
void Motor_Move_2(int step);
void Motor_Move_X(int step);
void Motor_Move_Y(int step);
void Motor_Move_XY(int x, int y);
void Set_Speed_X_times_Y(float rate);
void Set_Speed_Y_times_X(float rate);
void ILI9341_Draw4Square(void);
void ILI9341_DrawMainMenu(void);


#endif	/* __GROUP50_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
