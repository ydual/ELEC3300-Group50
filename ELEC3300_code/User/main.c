/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   摄像头火眼ov7725测试例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./ov7725/bsp_ov7725.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./led/bsp_led.h"   
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h"  
#include "./systick/bsp_SysTick.h"
#include "./lcd/bsp_lcd.h"
#include "./bmp/bsp_bmp.h"
#include "./57StepMotor/bsp_TB6600_TIM.h" 
#include "./SG90/bsp_GeneralTIM.h"
#include "./Group50/Group50.h"
#include "..\Mass_Storage\src\hw_config.h" 
#include "..\STM32_USB-FS-Device_Driver\inc\usb_lib.h"
#include "./hc05/bsp_hc05.h"
#include "./usart/bsp_usart_blt.h"

#include "..\Mass_Storage\src\usb_pwr.h"
#include "ff.h"

FATFS fs;										
FIL fnew;													
FRESULT res_sd;                
UINT fnum;
char ReadBuffer[10]={0};     
BYTE WriteBuffer[] =  "Hello World!\r\n";  
strType_XPT2046_Coordinate coordinate;
char output[10];
char speed[10];

extern __IO uint16_t prescaler[4];

extern uint8_t Ov7725_vsync;

extern OV7725_MODE_PARAM cam_mode;

FATFS fs;			
FRESULT res_sd;

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */

int main(void) 	
{
	
	float frame_count = 0;


	LCD_Init();
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_GramScan ( 6 );
	
	//USART_Config();
	
	LED_GPIO_Config();
	Key_GPIO_Config();
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	
	
	
	TB6600_TIMx_PWM_Init();
	GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
	GPIO_ResetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);

	
	ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
	GENERAL_TIMx_PWM_Init();
	res_sd = f_mount(&fs,"0:",1);
	
	while(1)
	{
		ILI9341_DrawMainMenu();
		XPT2046_TouchEvenHandler(&coordinate);
		
		if ((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 50 && coordinate.y<90)){
			Camera_Mode();
		}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 120 && coordinate.y < 160)){
			Control_Mode();
		}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 190 && coordinate.y < 230)){
			USB_Mode();
		}else if((coordinate.x > 50 && coordinate.x <170)&&(coordinate.y > 260 && coordinate.y < 300)){
			SD_Card_Mode();
		}
	}
}




/*********************************************END OF FILE**********************/

