
#include "./57StepMotor/bsp_TB6600_TIM.h"

__IO uint32_t pulse_count_1=0;
__IO uint32_t pulse_count_2=0;


static void TB6600_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  TB6600_TIM_GPIO_APBxClock_FUN(TB6600_TIM_GPIO_CLK, ENABLE);
  
  RCC_APB2PeriphClockCmd(TB6600_DIR1_GPIO_CLK|TB6600_ENA1_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(TB6600_DIR2_GPIO_CLK|TB6600_ENA2_GPIO_CLK,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = TB6600_TIM_CH1_PIN|TB6600_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TB6600_TIM_CH1_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =TB6600_DIR1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(TB6600_DIR1_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
  
  GPIO_InitStructure.GPIO_Pin =TB6600_DIR2_PIN;
  GPIO_Init(TB6600_DIR2_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
  
  
  GPIO_InitStructure.GPIO_Pin =TB6600_ENA1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(TB6600_ENA1_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
  
  GPIO_InitStructure.GPIO_Pin =TB6600_ENA2_PIN;
  GPIO_Init(TB6600_ENA2_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
  
}



static void TB6600_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
  
  NVIC_InitTypeDef NVIC_InitStructure;
  

	TB6600_TIM_APBxClock_FUN (TB6600_TIM_CLK, ENABLE);
  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 

  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	

  NVIC_Init(&NVIC_InitStructure); 
  

  TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 

  TIM_TimeBaseStructure.TIM_Prescaler =4-1;

  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TB6600_TIMx, &TIM_TimeBaseStructure);


  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;

  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  

  TIM_OCInitStructure.TIM_Pulse = prescaler[0];

  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC3Init(TB6600_TIMx, &TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = prescaler[1];
  TIM_OC2Init(TB6600_TIMx, &TIM_OCInitStructure);
  
//  TIM_OCInitStructure.TIM_Pulse = prescaler[2];
//  TIM_OC3Init(TB6600_TIMx, &TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = prescaler[3];
  TIM_OC4Init(TB6600_TIMx, &TIM_OCInitStructure);
  

  TIM_OC1PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_OC2PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_OC3PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_OC4PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC1,ENABLE);
  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC2,ENABLE);
  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC3,ENABLE);
  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC4,ENABLE);

  TIM_Cmd(TB6600_TIMx, ENABLE);
  

  TIM_CtrlPWMOutputs(TB6600_TIMx, ENABLE);
}


void TB6600_TIMx_PWM_Init(void)
{
	TB6600_GPIO_Config();
	TB6600_TIMx_Configuration();	
}


void TIM1_CC_IRQHandler(void)
{
  uint16_t capture;
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC1)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC1);
    capture=TIM_GetCapture1(TB6600_TIMx);
    TIM_SetCompare1(TB6600_TIMx,capture+prescaler[0]);
  }
  
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC2)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC2);
    capture=TIM_GetCapture2(TB6600_TIMx);
    TIM_SetCompare2(TB6600_TIMx,capture+prescaler[1]);
		pulse_count_2++;
  }
  
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC3)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC3);
    capture=TIM_GetCapture3(TB6600_TIMx);
    TIM_SetCompare3(TB6600_TIMx,capture+prescaler[0]);
		pulse_count_1++;
  }
  
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC4)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC4);
    capture=TIM_GetCapture4(TB6600_TIMx);
    TIM_SetCompare4(TB6600_TIMx,capture+prescaler[3]);
  }
}

void Delayus_2(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}

void TB6600_step_y(int step){
	if(step<0){
		GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
		GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		Delayus_2(-step*Step_time);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA2_PIN);

	}else if (step>0){
		GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
		GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		Delayus_2(step*Step_time);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA2_PIN);
	}
}

void TB6600_step_x(int step){
	if(step<0){
		GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_SetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
		GPIO_SetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		Delayus_2(-step*Step_time);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA2_PIN);

	}else if (step>0){
		GPIO_SetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_DIR1_PORT,TB6600_DIR1_PIN);
		GPIO_SetBits(TB6600_ENA2_PORT,TB6600_ENA2_PIN);
		GPIO_ResetBits(TB6600_DIR2_PORT,TB6600_DIR2_PIN);
		Delayus_2(step*Step_time);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA1_PIN);
		GPIO_ResetBits(TB6600_ENA1_PORT,TB6600_ENA2_PIN);
	}
}

/******************* (C) COPYRIGHT 2015-2020 ????????? *****END OF FILE****/
