#include "pwm.h"

static void TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
/*    GPIO_Pin_8----PWMV       0~24%对应0-Imax
			GPIO_Pin_9----PWMI       0~65%对应0-Vomax
			GPIO_Pin_10----SYNC-FB 	 输出同步脉冲133.34KHz
			GPIO_Pin_11----SYNC-AC   输出同步脉冲114.944KHz       */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);
}

static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_Pulse = 500;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 300;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 200;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 600;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM1, ENABLE); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
}


void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}
