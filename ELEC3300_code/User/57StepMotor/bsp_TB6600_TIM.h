#ifndef __TB6600_TIM_H__
#define __TB6600_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define Step_time														100000

#define TB6600_TIMx                         TIM1
#define TB6600_TIM_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define TB6600_TIM_CLK                      RCC_APB2Periph_TIM1

#define TB6600_TIM_GPIO_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                 (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )

/* 第一路*/
#define TB6600_TIM_CH1_PORT                 GPIOA                 // 对应TB6600的PUL-，
#define TB6600_TIM_CH1_PIN                  GPIO_Pin_10            // 而PLU+直接接开发板的+5V(或者3.3V)

#define TB6600_DIR1_GPIO_CLK                RCC_APB2Periph_GPIOA  // 电机旋转方向控制
#define TB6600_DIR1_PORT                    GPIOA                 // 对应TB6600的DIR-，
#define TB6600_DIR1_PIN                     GPIO_Pin_5           // 而DIR+直接接开发板的+5V(或者3.3V)

#define TB6600_ENA1_GPIO_CLK                RCC_APB2Periph_GPIOA  // 电机使能控制，该引脚可不接悬空
#define TB6600_ENA1_PORT                    GPIOA                 // 对应TB6600的ENA-，
#define TB6600_ENA1_PIN                     GPIO_Pin_4           // 而ENA+直接接开发板的+5V(或者3.3V)

/* 第二路*/
#define TB6600_TIM_CH2_PORT                 GPIOA                 // 对应TB6600的PUL-，
#define TB6600_TIM_CH2_PIN                  GPIO_Pin_9           // 而PLU+直接接开发板的+5V(或者3.3V)

#define TB6600_DIR2_GPIO_CLK                RCC_APB2Periph_GPIOA  // 电机旋转方向控制
#define TB6600_DIR2_PORT                    GPIOA                 // 对应TB6600的DIR-，
#define TB6600_DIR2_PIN                     GPIO_Pin_6           // 而DIR+直接接开发板的+5V(或者3.3V)

#define TB6600_ENA2_GPIO_CLK                RCC_APB2Periph_GPIOA  // 电机使能控制，该引脚可不接悬空
#define TB6600_ENA2_PORT                    GPIOA                // 对应TB6600的ENA-，
#define TB6600_ENA2_PIN                     GPIO_Pin_7           // 而ENA+直接接开发板的+5V(或者3.3V)


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void TB6600_TIMx_PWM_Init(void);
void TB6600_step_x(int step);
void TB6600_step_y(int step);

#endif	/* __TB6600_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
