#include "USART1.h"
#include "bsp_adc.h"
uint8_t  STM32_LPC[10]={0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x16};
uint32_t data[8];
extern float ADC_data[6];
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}
#endif 




/* USART初始化 */
void USART1_Init(uint32_t baud)
{
		//	NVIC_InitTypeDef NVIC_InitStructure;
     GPIO_InitTypeDef GPIO_InitStruct;
     USART_InitTypeDef USART_InitStructure;
        
     RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
     RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
        
     GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_0);
     GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_0);
	
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;  
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
     GPIO_Init( GPIOB, &GPIO_InitStruct );

		 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
		 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
		 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		 GPIO_Init(GPIOB,&GPIO_InitStruct);
 
			RCC_USARTCLKConfig( RCC_USART1CLK_PCLK );
			USART_DeInit( USART1 );
			USART_InitStructure.USART_BaudRate = baud;              //???
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

			USART_Init(USART1, &USART_InitStructure);		
			USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //??????
			USART_Cmd(USART1, ENABLE);                             //??USART1
				
			USART_ClearFlag(USART1,USART_FLAG_TXE);
}


//单片机接收数据
//unsigned char Usart_receive()
//{
//	unsigned char temp;
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//485通信，引脚置高
//	
// 
//  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);//??????
//  temp = (unsigned char)(USART_ReceiveData(USART1));
//  return(temp);    

//}

	int i;

void USART1_IRQHandler(void)
{

	unsigned char ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
       data[i] = (USART_ReceiveData(USART1));
			 i++;
			 if(i>7) i=0;
	} 

}

//单片机发送数据
void usart_transmit(unsigned char data)
{
	//GPIO_SetBits(GPIOB, GPIO_Pin_8);		//485通信，引脚置低
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}
void STM32toLPC()
{
	
}



void usart_send_string(char *str)
{
	while(*str)
	{
		usart_transmit(*str);
		str++;
	}
}
