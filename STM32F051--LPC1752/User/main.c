#include "stm32f0xx.h"
#include "delay.h"
#include "USART1.h"
#include "bsp_adc.h"
#include "pwm.h"
#include "key.h"
extern float ADC_data[6];
extern char STM32_LPC[14];

void jieshou()
{
// USART_ReceiveData(USART1);
			usart_transmit(4); //�����õ�			
	//	Si4432_init();		     //LED�˿ڳ�ʼ��
	//  SetRX_Mode();
}

void Ctl_GPIO()
{
	//PC13:PFC_CTRL		---�����0���� 1�ض�
	//PB13:BRG_CTRL		---�����0���� 1�ض�
	//PB2:JK_CTRL			---�����0�ض� 1����
	//PB11:PFC_State	---���룺0��ѹ 1����
}
int main(void)
{
	
	//TIMx_PWM_Init();
//  USART1_Init(115200);
	Adc_Init();
	key();
  while (1)
	{		
		//usart_transmit(STM32_LPC[10]); 
		STM32_LPC[5] = (char)ADC_data[5]>>8;		//���뽻����ѹ���ֽ�
		STM32_LPC[6] = (char)ADC_data[5]&0xff;	//���뽻����ѹ���ֽ�
//		STM32_LPC[7] = (char)ADC_data[2]>>8;		//���ư��¶�
//		STM32_LPC[8] = (char)ADC_data[2]&0xff;	//�����¶�
		
		STM32_LPC[9]  = (char)ADC_data[3]>>8;		//ʵ������������ֽ�
		STM32_LPC[10] = (char)ADC_data[3]&0xff;	//ʵ������������ֽ�
		STM32_LPC[11] = (char)ADC_data[2]>>8;		//ʵ�������ѹ���ֽ�
		STM32_LPC[12] = (char)ADC_data[2]&0xff;	//ʵ�������ѹ���ֽ�
		
		usart_send_string(STM32_LPC);
		filter();
	}
 
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
