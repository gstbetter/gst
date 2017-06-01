#include "bsp_adc.h"
//ntc ??????? 5k 3470k
float const Ntc5K_Tab[]=
{
/*??  ??  ????K*/	
/*1	*/-40	,	117.1029 ,
/*2	*/-39	,	109.6323 ,
/*3	*/-38	,	102.8754 ,
/*4	*/-37	,	96.7307  ,
/*5	*/-36	,	91.1137  ,
/*6	*/-35	,	85.9544  ,
/*7	*/-34	,	81.1938  ,
/*8	*/-33	,	76.7827  ,
/*9	*/-32	,	72.6797  ,
/*10*/-31	,	68.8497  ,
/*11*/-30	,	65.2632  ,
/*12*/-29	,	62.3303  ,
/*13*/-28	,	59.3971  ,
/*14*/-27	,	56.4972  ,
/*15*/-26	,	53.6576  ,
/*16*/-25	,	50.8994  ,
/*17*/-24	,	48.2384  ,
/*18*/-23	,	45.6858  ,
/*19*/-22	,	43.2491  ,
/*20*/-21	,	40.9326  ,
/*21*/-20	,	38.738   ,
/*22*/-19	,	36.7683  ,
/*23*/-18	,	34.8943  ,
/*24*/-17	,	33.1114  ,
/*25*/-16	,	31.4152  ,
/*26*/-15	,	29.8021  ,
/*27*/-14	,	28.2684  ,
/*28*/-13	,	26.8109  ,
/*29*/-12	,	25.4264  ,
/*30*/-11	,	24.1119  ,
/*31*/-10	,	22.8647  ,
/*32*/-9	,	21.7765  ,
/*33*/-8	,	20.7575  ,
/*34*/-7	,	19.8028  ,
/*35*/-6	,	18.9075  ,
/*36*/-5	,	18.0672  ,
/*37*/-4	,	17.2777  ,
/*38*/-3	,16.5349   ,
/*39*/-2	,	15.8351  ,
/*40*/-1	,	15.1748  ,
/*41*/0	,	14.5509 ,
/*42*/1	,13.8937  ,
/*43*/2	,	13.27   ,
/*44*/3	,12.6779  ,
/*45*/4	,12.1157  ,
/*46*/5	,	11.5818 ,
/*47*/6	,	11.0747 ,
/*48*/7	,	10.5929 ,
/*49*/8	,	10.1351 ,
/*50*/9	,	9.6999  ,
/*51*/10	,9.2861 ,
/*52*/11	,8.8926  ,
/*53*/12	,8.5183  ,
/*54*/13	,8.1621  ,
/*55*/14	,7.8231  ,
/*56*/15	,7.5004  ,
/*57*/16	,7.1931  ,
/*58*/17	,6.9003  ,
/*59*/18	,6.6214  ,
/*60*/19	,6.3556  ,
/*61*/20	,	6.1022 ,
/*62*/21	,	5.8606 ,
/*63*/22	,	5.6301 ,
/*64*/23	,	5.4102 ,
/*65*/24	,	5.2004 ,
/*66*/25	,	5      ,
/*67*/26	,	4.8087 ,
/*68*/27	,	4.6259 ,
/*69*/28	,	4.4513 ,
/*70*/29	,	4.2844 ,
/*71*/30	,	4.1249 ,
/*72*/31	,	3.9723 ,
/*73*/32	,	3.8264 ,
/*74*/33	,	3.6868 ,
/*75*/34	,	3.5531 ,
/*76*/35	,	3.4252 ,
/*77*/36	,	3.3027 ,
/*78*/37	,	3.1853 ,
/*79*/38	,	3.0728 ,
/*80*/39	,	2.9651 ,
/*81*/40	,	2.8618 ,
/*82*/41	,	2.7627 ,
/*83*/42	,2.6676  ,
/*84*/43	,	2.5764 ,
/*85*/44	,	2.4889 ,
/*86*/45	,	2.4049 ,
/*87*/46	,	2.3243 ,
/*88*/47	,2.2468  ,
/*89*/48	,	2.1723 ,
/*90*/49	,	2.1008 ,
/*91*/50	,	2.032  ,
/*92*/51	,	1.9659 ,
/*93*/52	,	1.9024 ,
/*94*/53	,	1.8412 ,
/*95*/54	,	1.7824 ,
/*96*/55	,	1.7257 ,
/*97*/56	,	1.6712 ,
/*98*/57	,	1.6187 ,
/*99*/58	,	1.5681 ,
/*100*/59	,	1.5194 ,
/*101*/60	,	1.4725 ,
/*102*/61	,	1.4273 ,
/*103*/62	,	1.3836 ,
/*104*/63	,	1.3416 ,
/*105*/64	,	1.301  ,
/*106*/65	,	1.2619 ,
/*107*/66	,	1.2242 ,
/*108*/67	,	1.1877 ,
/*109*/68	,	1.1526 ,
/*110*/69	,	1.1186 ,
/*111*/70	,	1.0858 ,
/*112*/71	,	1.0541 ,
/*113*/72	,	1.0235 ,
/*114*/73	,	0.994  ,
/*115*/74	,	0.9654 ,
/*116*/75	,	0.9378 ,
/*117*/76	,	0.9111 ,
/*118*/77	,	0.8852 ,
/*119*/78	,	0.8602 ,
/*120*/79	,	0.8361 ,
/*121*/80	,	0.8127 ,
/*122*/81	,	0.79   ,
/*123*/82	,	0.7681 ,
/*124*/83	,	0.7469 ,
/*125*/84	,	0.7264 ,
/*126*/85	,0.7065  ,
/*127*/86	,	0.6872 ,
/*128*/87	,	0.6685 ,
/*129*/88	,	0.6504 ,
/*130*/89	,	0.6329 ,
/*131*/90	,	0.6159 ,
/*132*/91	,	0.5994 ,
/*133*/92	,0.5834  ,
/*134*/93	,	0.5679 ,
/*135*/94	,	0.5529 ,
/*136*/95	,	0.5383 ,
/*137*/96	,0.5242  ,
/*138*/97	,	0.5105 ,
/*139*/98	,	0.4971 ,
/*140*/99	,	0.4842 ,
/*141*/100	,	0.4717 ,
/*142*/101	,	0.4591 ,
/*143*/102	,	0.4469 ,
/*144*/103	,	0.4351 ,
/*145*/104	,0.4235  ,
/*146*/105	,	0.4122 ,
/*147*/106	,	0.4013 ,
/*148*/107	,	0.3906 ,
/*149*/108	,0.3802  ,
/*150*/109	,	0.3701 ,
/*151*/110	,	0.3603 ,
/*152*/111	,	0.3507 ,
/*153*/112	, 0.3414 ,
/*154*/113	,	0.3323 ,
/*155*/114	,	0.3235 ,
/*156*/115	,	0.3149 ,
/*157*/116	,	0.3066 ,
/*158*/117	, 0.2985 ,
/*159*/118	,	0.2906 ,
/*160*/119	,	0.283  ,
/*161*/120	,	0.2756 ,
/*162*/121	,0.2684  ,
/*163*/122	,	0.2613 ,
/*164*/123	,	0.2545 ,
/*165*/124	,	0.2479 ,
/*166*/125	,	0.2415 ,
/*167*/126	,	0.2353 ,
/*168*/127	,	0.2293 ,
/*169*/128	,.2235   ,
/*170*/129	,	0.2178 ,
/*171*/130	,	0.2123 ,
/*172*/131	,	0.207  ,
/*173*/132	,	0.2019 ,
/*174*/133	,	0.1969 ,
/*175*/134	,	0.192  ,
/*176*/135	,	0.1874 ,
/*177*/136	,	0.1829 ,
/*178*/137	,	0.1785 ,
/*179*/138	,	0.1743 ,
/*180*/139	,	0.1702 ,
/*181*/140	,0.1663  ,
/*182*/141	,0.1625  ,
/*183*/142	,0.1588  ,
/*184*/143	,0.1553  ,
/*185*/144	,0.1518  ,
/*186*/145	,0.1486  ,
/*187*/146	,0.1454  ,
/*188*/147	,0.1423  ,
/*189*/148	,0.1394  ,
/*190*/149	,0.1366  ,
/*191*/150	,0.13384 ,
};
unsigned int Ntc5K_Tab_Max=191;
/*********************************************************************************************************
* Function name:       Ntc5K_To_R
* Descriptions:        ?????????,??-50 ?? ??-50~150 ,   ?????-40? ???? -50? 
* input parameters:    1/4096
* output parameters:    ????
* Returned value:       ?
*********************************************************************************************************/
unsigned char  Ntc5K_To_R(unsigned int DIGET)
{int i=0;
	float Ntc5K=0;
	Ntc5K=(4096*5000/DIGET-5000)/1000;
 for(i=1;i<Ntc5K_Tab_Max*2;i+=2)
      {
        if(Ntc5K_Tab[i]<=Ntc5K)
        {
         return ((i-1)/2+10);//??-50
         // break;
        }
      }
}
//__IO uint16_t ADC_ConvertedValue[macNOFCHANEL]={0,0,0,0};
__IO uint16_t ADC_ConvertedValue[6]={0,0,0,0,0,0};
float new_data[6];
unsigned char ADC_data[6]={0,0,0,0,0,0};

/*
	ADC_data[0]:TcbAdc控制板温度检测  --热敏电阻
	ADC_data[1]:TmbAdc主板温度检测		--热敏电阻
	ADC_data[2]:VoutAdc输出电压检测		--0-2.25V对应0-400V
	ADC_data[3]:IoutAdc输出电流检测		--0-2.25V对应0-8A
	ADC_data[4]:VbatcbAdc电池电压检测 --0-2.25V对应0-400V
	ADC_data[5]:VinAdc交流电压检测	--0-2.25V对应0-264VAC
*/


/**
  * @brief  ADC 初始化
  * @param  无
  * @retval 无
  */
void Adc_Init()
{
	ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  DMA_InitTypeDef   DMA_InitStructure;
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* GPIOC Periph clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_AHBPeriph_GPIOA,ENABLE);
  
   /* ADC1 Periph clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* Configure ADC Channel11 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)((uint32_t) 0x40012440);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC1, ENABLE);  
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 

  /* Convert the ADC1 Channel 1 with 55.5 Cycles as sampling time */ 
	 ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_55_5Cycles);   
	 ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles);   
	 ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_55_5Cycles);   
   ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_55_5Cycles);   
 	 ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_55_5Cycles);   
   ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_55_5Cycles);   
 
  ADC_GetCalibrationFactor(ADC1);
  ADC_Cmd(ADC1, ENABLE);     
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  ADC_StartOfConversion(ADC1);

}
void filter()
		{
			uint8_t i=0;
			for(i=0;i<6;i++)
			{
				new_data[i] = ((float)ADC_ConvertedValue[i]/4095)*3300 ;
				}
		   	ADC_data[0]=new_data[0];//	ADC_data[0]:TcbAdc控制板温度检测  --热敏电阻
				ADC_data[1]=new_data[5];//	ADC_data[1]:TmbAdc主板温度检测		--热敏电阻
				ADC_data[2]=(new_data[4]/1000)*177;//ADC_data[2]:VoutAdc输出电压检测		--0-2.25V对应0-400V    k=177
		
//				ADC_data[2]>>8;
//				ADC_data[2]&0xff;
				
				ADC_data[3]=(new_data[3]/1000)*3.5;//	ADC_data[3]:IoutAdc输出电流检测		--0-2.25V对应0-8A			 k=3.5
				ADC_data[4]=(new_data[2]/1000)*177;//	ADC_data[4]:VbatcbAdc电池电压检测 --0-2.25V对应0-400V		 k=177
				ADC_data[5]=(new_data[1]/1000)*117;//	ADC_data[5]:VinAdc交流电压检测  	--0-2.25V对应0-264VAC	 k=117
		} 	

		
		