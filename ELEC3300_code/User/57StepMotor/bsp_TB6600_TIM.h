#ifndef __TB6600_TIM_H__
#define __TB6600_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define Step_time														100000

#define TB6600_TIMx                         TIM1
#define TB6600_TIM_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define TB6600_TIM_CLK                      RCC_APB2Periph_TIM1

#define TB6600_TIM_GPIO_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                 (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )

/* ��һ·*/
#define TB6600_TIM_CH1_PORT                 GPIOA                 // ��ӦTB6600��PUL-��
#define TB6600_TIM_CH1_PIN                  GPIO_Pin_10            // ��PLU+ֱ�ӽӿ������+5V(����3.3V)

#define TB6600_DIR1_GPIO_CLK                RCC_APB2Periph_GPIOA  // �����ת�������
#define TB6600_DIR1_PORT                    GPIOA                 // ��ӦTB6600��DIR-��
#define TB6600_DIR1_PIN                     GPIO_Pin_5           // ��DIR+ֱ�ӽӿ������+5V(����3.3V)

#define TB6600_ENA1_GPIO_CLK                RCC_APB2Periph_GPIOA  // ���ʹ�ܿ��ƣ������ſɲ�������
#define TB6600_ENA1_PORT                    GPIOA                 // ��ӦTB6600��ENA-��
#define TB6600_ENA1_PIN                     GPIO_Pin_4           // ��ENA+ֱ�ӽӿ������+5V(����3.3V)

/* �ڶ�·*/
#define TB6600_TIM_CH2_PORT                 GPIOA                 // ��ӦTB6600��PUL-��
#define TB6600_TIM_CH2_PIN                  GPIO_Pin_9           // ��PLU+ֱ�ӽӿ������+5V(����3.3V)

#define TB6600_DIR2_GPIO_CLK                RCC_APB2Periph_GPIOA  // �����ת�������
#define TB6600_DIR2_PORT                    GPIOA                 // ��ӦTB6600��DIR-��
#define TB6600_DIR2_PIN                     GPIO_Pin_6           // ��DIR+ֱ�ӽӿ������+5V(����3.3V)

#define TB6600_ENA2_GPIO_CLK                RCC_APB2Periph_GPIOA  // ���ʹ�ܿ��ƣ������ſɲ�������
#define TB6600_ENA2_PORT                    GPIOA                // ��ӦTB6600��ENA-��
#define TB6600_ENA2_PIN                     GPIO_Pin_7           // ��ENA+ֱ�ӽӿ������+5V(����3.3V)


/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void TB6600_TIMx_PWM_Init(void);
void TB6600_step_x(int step);
void TB6600_step_y(int step);

#endif	/* __TB6600_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
