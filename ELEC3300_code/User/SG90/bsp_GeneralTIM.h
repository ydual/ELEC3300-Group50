#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/********************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2 & TIM3 & TIM4 & TIM5************/
#define GENERAL_TIMx                        TIM4
#define GENERAL_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK                     RCC_APB1Periph_TIM4
#define GENERAL_TIM_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define GENERAL_TIM_GPIO_CLK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO )

#define GENERAL_TIM_CH1_PORT                GPIOB        
#define GENERAL_TIM_CH1_PIN                 GPIO_Pin_6



/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void GENERAL_TIMx_PWM_Init(void);

#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
