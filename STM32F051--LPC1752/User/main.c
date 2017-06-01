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
			usart_transmit(4); //测试用的			
	//	Si4432_init();		     //LED端口初始化
	//  SetRX_Mode();
}

void Ctl_GPIO()
{
	//PC13:PFC_CTRL		---输出：0开启 1关断
	//PB13:BRG_CTRL		---输出：0开启 1关断
	//PB2:JK_CTRL			---输出：0关断 1开启
	//PB11:PFC_State	---读入：0过压 1正常
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
		STM32_LPC[5] = (char)ADC_data[5]>>8;		//输入交流电压低字节
		STM32_LPC[6] = (char)ADC_data[5]&0xff;	//输入交流电压高字节
//		STM32_LPC[7] = (char)ADC_data[2]>>8;		//控制板温度
//		STM32_LPC[8] = (char)ADC_data[2]&0xff;	//主板温度
		
		STM32_LPC[9]  = (char)ADC_data[3]>>8;		//实际输出电流低字节
		STM32_LPC[10] = (char)ADC_data[3]&0xff;	//实际输出电流高字节
		STM32_LPC[11] = (char)ADC_data[2]>>8;		//实际输出电压低字节
		STM32_LPC[12] = (char)ADC_data[2]&0xff;	//实际输出电压高字节
		
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
