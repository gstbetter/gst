//#include"SI4432_TX.h"
#include "exti.h"
#include "USART1.h"
#include "delay.h"
extern void SetRX_Mode(void);
extern unsigned char RF4432_RxPacket(unsigned char *dataBuffer);

#define RxBuf_Len 5     //定义RF4432数据包长度

extern unsigned char RxBuf[RxBuf_Len];

void EXTIX_Init(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);	
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority=0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource0);
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
		}

void EXTI0_1_IRQHandler(void)
{
	    EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE4上的中断标志位 	
	

}
