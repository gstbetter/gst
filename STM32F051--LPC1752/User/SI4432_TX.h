/***********************************************************************************************************
�������ƣ�Si4432ģ��SPI����+��׼SPI�������Ѳ��ԣ�
��������ICC-AVR v6.31A 
����: 2011-10-21 17:49:02
Ŀ��оƬ: M16L
ʱ��: 8.0000Mhz
���ߣ�QQ:40560335���˳����Ǳ���ͨ������Demo�����޸Ķ����������������Ͷ��ɹ�����ת����ע����лл��~��
���ң�QQ:632598294��Si4432ģ�����17mm X 17mm��Ŀǰȫ�Ա���С��ģ�飬�Ƽ����ʹ�ð���������~
ע�⣺����ʱ��ԭ�򣬽�ͨ��ģ��SPI���ԣ�ֱ���޸�IO�ڼ���ʹ�ã����д���ӭָ������ϵ����һ��̽�֣�~


�������÷�Χ�� ����Si4432���ԣ�Si4431��Si4431PA������
RFģ��Ĳ�������Ϊ��FSKģʽ��433.92MHz�� 1.2KBPS�� +/-10PPM, Ƶƫ��30KHz�� ���ƴ���61.2KHz 
���䲻��ҪCLK��FiFo��FSKģʽ��ʹ��AFC��ʹ��CRC�� PH + FIFOģʽ
ÿ���ӷ���Ĺ̶���������Ϊ�� 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x6d,
ע��0x6d = (0x41 +0x42 +0x43 +0x44 +0x45 +0x46 +0x47 + 0x48 +0x49)


Si4432���Ŷ��壺
1   GND    ��
2   GPIO0  �ڲ��ѽ�ģ��ķ�����ƽ�
3   GPIO1  �ڲ��ѽ�ģ��Ľ��տ��ƽ�
4   GPIO2  ֱ����оƬ��GPIO2����
5   VCC    +3.3V
6   SDO    �����������
7   SDI    ������������
8   SCLK   ����ʱ������?9   nSEL   SPIƬѡ
10  nIRQ   �ж��������
11  SDN    �ر��������ţ��ڳ��ػ�ģʽ������ģʽSDN=0����SDN=1 ʱоƬ�������׹رղ��ҼĴ��������ݽ���ʧ��
12  GND    ��
13  ANT    ����
**********************************************************************************************************/
#define uchar unsigned char
#define uint  unsigned int
	
//#include "sys.h"
#include "stm32f0xx.h"

//PA0 PA1 PA8
//PB0-���ж� PB1 PB5


int k=0;

void si4432_IOInit(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO���ٶ�Ϊ50MHz
 GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA
 //GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO���ٶ�Ϊ50MHz
 GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_0;//PB0,PB1
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN ; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}



uint TX_Timeout=1;  //���ͳ�ʱ s
uchar TX_Reach=1;    //���Ͷ�ʱ s
uchar RSSI_Timeout=0;//RSSIˢ�³�ʱ s
uchar ComFlag=0;


#define   Hign_Si4432_SDN       (GPIO_SetBits(GPIOA,GPIO_Pin_0));
#define   Low_Si4432_SDN        (GPIO_ResetBits(GPIOA,GPIO_Pin_0));

#define   Hign_Si4432_nSEL      (GPIO_SetBits(GPIOA,GPIO_Pin_1));
#define   Low_Si4432_nSEL       (GPIO_ResetBits(GPIOA,GPIO_Pin_1));

#define   Hign_Si4432_SCLK      (GPIO_SetBits(GPIOA,GPIO_Pin_8));
#define   Low_Si4432_SCLK       (GPIO_ResetBits(GPIOA,GPIO_Pin_8));	

#define   Hign_Si4432_SDI       (GPIO_SetBits(GPIOB,GPIO_Pin_5));
#define   Low_Si4432_SDI        (GPIO_ResetBits(GPIOB,GPIO_Pin_5));

#define   Hign_D1_LED     (GPIO_SetBits(GPIOB,GPIO_Pin_6));
#define   Low_D1_LED      (GPIO_ResetBits(GPIOB,GPIO_Pin_6));

#define   Hign_D2_LED     (GPIO_SetBits(GPIOB,GPIO_Pin_7));
#define   Low_D2_LED      (GPIO_ResetBits(GPIOB,GPIO_Pin_7));

#define   Read_Si4432_SDO      (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))//��ȡ����3(WK_UP) )

#define   Read_Si4432_nIRQ     (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))

void SetRX_Mode(void);
#define  Max_P_Set_Pages 19
unsigned char P_set[Max_P_Set_Pages];

void  delay_us1(uint us)
{
	  us=us*100;
	  while(us--);
		}
/*******************************************
��������: Delayms
��    ��: ��ʱָ�����루8M����
��    ��: MS--��ʱ�ĺ�����
����ֵ  : ��
********************************************/
void Delayms(uint MS)		  
{
 uint i,j;
 for( i=0;i<MS;i++)
 for(j=0;j<1141;j++);	//1141����8MHz�����£�ͨ��������淴��ʵ��õ�����ֵ
}
#define   SPI_NOP     _NOP();//_NOP();_NOP();_NOP();_NOP();
//***************************************Si4432�Ĵ���ָ��*******************************************************
#define S_READ_REG        0x00  	//���Ĵ���ָ��
#define S_WRITE_REG       0x80 	//д�Ĵ���ָ��

#define  TX1_RX0	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x01) //����״̬�����߿��ض���
#define  TX0_RX1	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x02) //����״̬�����߿��ض���
#define  TX0_RX0	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x00) //�Ƿ��䣬����״̬�����߿��ض���

uchar TxBuf_Len=10;
#define RxBuf_Len 3     //����RF4432���ݰ����� ������ȴ�����������1��2��

uchar RxBuf[RxBuf_Len];
uchar TxBuf[30] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //ÿ�뷢��Ĺ̶����ݵĲ����źţ���10��������ǰ9�����ݵ�У��ͣ��ֱ�Ϊ65��66��67��68��69��70��71��72��73��109
uchar ItStatus;  //���ͽ����ж�
uchar RSSI;      //RSSI
	
/***********************************************************************************************************
������SPI_Init(void)
����: SPI���Žӿڼ�״̬��ʼ��
**********************************************************************************************************/
void SPI_Init1(void)
{
    Hign_Si4432_nSEL;
    delay_us1(2);
    Low_Si4432_SCLK;
    delay_us1(2);
}
/***********************************************************************************************************
������uchar SPI_Byte(uchar data)
���ܣ�Si4432��SPIдʱ��
˵��������һλ����1��Ϊд������0ʱΪ����
**********************************************************************************************************/
uchar SPI_Byte(uchar data)
{
/*-----------------------��׼Ӳ��SPI-------------------------------------------
  	SPDR = data;
	while(!(SPSR & (1<<SPIF) ))
	{
	};			// �ȴ�SPI�������
	return SPDR;
----------------------������ģ��SPIʱ��ʽ-------------------------------*///��ʹ�ñ�׼SPIʱ������һ��///ǰ���С�����Ƶ�������һ�е�/ǰ���OK
	uchar i,temp=0;
	Low_Si4432_SCLK;
       delay_us1(2);
   	for(i=0;i<8;i++)
   	{
	    if(data & 0x80)
	    {
		Hign_Si4432_SDI;
	    }
	    else
	    {
	 	 Low_Si4432_SDI; 
	    }	
           delay_us1(2);
		data <<= 1;
		temp<<=1;
		Hign_Si4432_SCLK ;
                delay_us1(2);
		if(Read_Si4432_SDO)temp++;
                delay_us1(2);
		Low_Si4432_SCLK;
               delay_us1(2);
   	}
    delay_us1(2);
    return(temp);
   
/*----------------------------------------------------------------------------*/
}
/***********************************************************************************************************
������uchar SPI_RW_Reg(uchar addr, uchar data)
���ܣ����ڶ�д���ݣ�addr��Ϊ�Ĵ�����ַ��data��Ϊ��������д������
**********************************************************************************************************/
uchar SPI_RW_Reg(uchar addr, uchar data)
{
	uchar status;
	
	Low_Si4432_nSEL;
      delay_us1(2);
	SPI_Byte(addr);
       delay_us1(2);
	status = SPI_Byte(data);
       delay_us1(2);
	Hign_Si4432_nSEL;
	delay_us1(2);
	return(status);
}
/***********************************************************************************************************
������SPI_Read_Buf(uchar addr, uchar *data, uchar mun)
����: ���ڶ����ݣ�addr��Ϊ�Ĵ�����ַ��data��Ϊ���������ݵ�ַ��number���������ݵĸ���
**********************************************************************************************************/
void SPI_Read_Buf(uchar addr, uchar *data, uchar number)
{
	//int status,number_ctr;
	int number_ctr;
	Low_Si4432_nSEL;
      delay_us1(2);
	SPI_Byte(addr);
       delay_us1(2);
	for(number_ctr=0;number_ctr<number;number_ctr++)
	data[number_ctr] = SPI_Byte(0);
       delay_us1(2);
	Hign_Si4432_nSEL;
       delay_us1(2);
}
/***********************************************************************************************************
������uchar SPI_Write_Buf(uchar addr, uchar *data, uchar number)
����: ����д���ݣ�Ϊ�Ĵ�����ַ��data��Ϊ��д�����ݵ�ַ��number��д�����ݵĸ���
**********************************************************************************************************/
void SPI_Write_Buf(uchar addr, uchar *data, uchar number)
{
	//uchar status,number_ctr;
	uchar number_ctr;
        
	Low_Si4432_nSEL;  
       delay_us1(2);
	SPI_Byte(addr);   
       delay_us1(2);
	for(number_ctr=0; number_ctr<number; number_ctr++)
	SPI_Byte(*data++);
        delay_us1(2);
	Hign_Si4432_nSEL;
        delay_us1(2);
}
uint const send_f[8]={0x1900,0x3200,0x4b00,0x6400,0x7d00,0x9600,0xaf00,0xc800};
/***********************************************************************************************************
������Si4432_init(void)
����: ���ڳ�ʼ��Si4432��SPI���ƽӿڼ��Ĵ���
**********************************************************************************************************/
void Si4432_init()
{
  SPI_Init1();
        
  Hign_Si4432_SDN;// 	SDN = 1;
 	Delayms(200);	// RF ģ���ϵ縴λ
 	Low_Si4432_SDN;//SDN = 0;
 	Delayms(200);	// ��ʱ200ms RF ģ����빤��״̬

	Delayms(500);
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //��RFģ���ж�	
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);
	
        SPI_RW_Reg(S_WRITE_REG + 0x05, 0x02);  // RFģ���յ��������ݺ󣬲����ж�
	SPI_RW_Reg(S_WRITE_REG + 0x06, 0x00);  //��ʹ��ͬ�������
        
	SPI_RW_Reg(S_WRITE_REG + 0x07, 0x01);  //���� Ready ģʽ
	
	SPI_RW_Reg(S_WRITE_REG + 0x09, 0x7f);  //���ص���= 12P
	SPI_RW_Reg(S_WRITE_REG + 0x0a, 0x05);  //�رյ�Ƶ���
	SPI_RW_Reg(S_WRITE_REG + 0x0b, 0xea);  //GPIO 0 ������ͨ�����
	SPI_RW_Reg(S_WRITE_REG + 0x0c, 0xea);  //GPIO 1 ������ͨ�����
	SPI_RW_Reg(S_WRITE_REG + 0x0d, 0xf4);  //GPIO 2 ����յ�������
	SPI_RW_Reg(S_WRITE_REG + 0x70, 0x2c);  
	SPI_RW_Reg(S_WRITE_REG + 0x1d, 0x40);  //ʹ�� afc
	
	// 1.2K bps setting
	SPI_RW_Reg(S_WRITE_REG + 0x1c, 0x16);  //����16��Nibble��Preamble
	SPI_RW_Reg(S_WRITE_REG + 0x20, 0x83);   
	SPI_RW_Reg(S_WRITE_REG + 0x21, 0xc0);  //
	SPI_RW_Reg(S_WRITE_REG + 0x22, 0x13);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x23, 0xa9);  //
	SPI_RW_Reg(S_WRITE_REG + 0x24, 0x00);  //
	SPI_RW_Reg(S_WRITE_REG + 0x25, 0x04);  //
	SPI_RW_Reg(S_WRITE_REG + 0x2a, 0x14);
	SPI_RW_Reg(S_WRITE_REG + 0x6e, 0x09);
	SPI_RW_Reg(S_WRITE_REG + 0x6f, 0xd5);
	//1.2K bps setting end		
	
	SPI_RW_Reg(S_WRITE_REG + 0x30, 0x8c);  //ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ��
	SPI_RW_Reg(S_WRITE_REG + 0x32, 0xff);  //byte0, 1,2,3 ��Ϊͷ��
	SPI_RW_Reg(S_WRITE_REG + 0x33, 0x42);  //byte 0,1,2,3 ��ͷ�룬ͬ����3,2 ��ͬ����
	SPI_RW_Reg(S_WRITE_REG + 0x34, 16);    //����16��Nibble��Preamble
	SPI_RW_Reg(S_WRITE_REG + 0x35, 0x20);  //��Ҫ���4��nibble��Preamble
	SPI_RW_Reg(S_WRITE_REG + 0x36, 0x2d);  //ͬ����Ϊ 0x2dd4
	SPI_RW_Reg(S_WRITE_REG + 0x37, 0xd4);
	SPI_RW_Reg(S_WRITE_REG + 0x38, 0x00);
	SPI_RW_Reg(S_WRITE_REG + 0x39, 0x00);
	SPI_RW_Reg(S_WRITE_REG + 0x3a, 's');   //�����ͷ��Ϊ�� ��swwx"
	SPI_RW_Reg(S_WRITE_REG + 0x3b, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x3c, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x3d, 'x');
	SPI_RW_Reg(S_WRITE_REG + 0x3e,  TxBuf_Len);   //�ܹ�����10���ֽڵ�����
	SPI_RW_Reg(S_WRITE_REG + 0x3f, 's');   //��ҪУ���ͷ��Ϊ����swwx"
	SPI_RW_Reg(S_WRITE_REG + 0x40, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x41, 'w'); 
	SPI_RW_Reg(S_WRITE_REG + 0x42, 'x');
	SPI_RW_Reg(S_WRITE_REG + 0x43, 0xff);  //ͷ��1,2,3,4 ������λ����ҪУ��
	SPI_RW_Reg(S_WRITE_REG + 0x44, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x45, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x46, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x6d, 0X00);  //���书������  0x00:+0dBM  0x01:+3dBM  0x02:+6dBM  0x03:+9dBM  0x04:+11dBM  0x05:+14dBM  0x06:+17dBM  0x07:20dBM
	
        SPI_RW_Reg(S_WRITE_REG + 0x79, 0x0);   //����Ҫ��Ƶ
	SPI_RW_Reg(S_WRITE_REG + 0x7a, 0x0);   //����Ҫ��Ƶ
	SPI_RW_Reg(S_WRITE_REG + 0x71, 0x22);  //���䲻��ҪCLK��FiFo��FSKģʽ
	SPI_RW_Reg(S_WRITE_REG + 0x72, 0x30);  //ƵƫΪ 30KHz
	SPI_RW_Reg(S_WRITE_REG + 0x73, 0x0);   //û��Ƶ��ƫ��
	SPI_RW_Reg(S_WRITE_REG + 0x74, 0x0);   //û��Ƶ��ƫ��
	SPI_RW_Reg(S_WRITE_REG + 0x75, 0x53);  //Ƶ������ 434
	SPI_RW_Reg(S_WRITE_REG + 0x76, 0x64);  //fc[15:8]
	SPI_RW_Reg(S_WRITE_REG + 0x77, 0x00);     //fc[7:0]

	TX0_RX0;	// ���߿��ز��ڷ��䣬����״̬
        //SetRX_Mode();
        //P1IE|=Si4432_nIRQ;
}
/***********************************************************************************************************
������uchar RF4432_RSSI(void)
����: ����Si4432��ѯ�ж��Ƿ�Ϊ ��⵽ͬ���� �ж�
**********************************************************************************************************/
uchar RF4432_RSSI(void)
{	
	if(!(Read_Si4432_nIRQ))
	{
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x04,0x00);  //���жϼĴ���
	 	if((ItStatus&0x80)==0x80)
		{
		    RSSI = SPI_RW_Reg(S_READ_REG + 0x26, 0x00);
		    return 1;
		}
		else
		{
		    return 0;
		}
	}
	else
	{
	return 0;
	}
}
/***********************************************************************************************************
������uchar RF4432_RxPacket(uchar *dataBuffer)
����: ����Si4432��ѯ�ж��Ƿ�Ϊ ���յ���Ч�� �ж�
**********************************************************************************************************/
uchar RF4432_RxPacket(uchar *dataBuffer)
{	 		//	  Hign_D1_LED;
	
	k=1;
	
	
	if(!(Read_Si4432_nIRQ))
	{       
              delay_us1(2);
			  Hign_D1_LED;
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x03,0x00);  //���жϼĴ���
		 	if((ItStatus&0x02)==0x02)
		{   	  
                   delay_us1(2);
		    SPI_Read_Buf(0x7f, dataBuffer, RxBuf_Len);
			    Hign_D2_LED;
                    return 1;
		}
		//else return 0;
	}
	else
	{
	return 0;
	}
return 0;
}
/***********************************************************************************************************
������void RF4432_TxPacket(uchar *Tx_Buf)
����: ����Si4432��ѯ�ж��Ƿ�Ϊ ���յ���Ч�� �ж�
**********************************************************************************************************/
void RF4432_TxPacket(uchar *dataBuffer)
{
	SPI_RW_Reg(S_WRITE_REG + 0x34, 40);   //����40��Nibble ��ǰ����
	SPI_RW_Reg(S_WRITE_REG + 0x3e, TxBuf_Len);  //�ܹ�����10���ֽڵ�����
	
	SPI_Write_Buf(S_WRITE_REG + 0x7f, dataBuffer, TxBuf_Len);   //��Ҫ���͵�����д��Ĵ���
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x04);	//�������ݷ�����󣬲����ж�
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //��RFģ���ж�	
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);
        
        SPI_RW_Reg(S_WRITE_REG + 0x07, 0x09); //���뷢��ģʽ
	TX_Timeout = 0;
	while(Read_Si4432_nIRQ)		//�ȴ��ж�
	{      
		Delayms(1);
		if(TX_Timeout>800)//8M ������ʵ��350(8���ֽ�)
		{
                    ComFlag=0;//����ʧ��
                    //TX_Timeout=0;
                    //delay_ms(20000);
                    //Si4432_init();
                    delay_us1(2);
                    break;		//��ǿ������
		}
                TX_Timeout++;
	}

	if(!(Read_Si4432_nIRQ))
	{
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x03,0x00);  //���жϼĴ���
		if((ItStatus&0x04)==0x04)
		{
                   ComFlag=1;//���ͳɹ�
                 delay_us1(2);
		}
	}
}
/***********************************************************************************************************
������SetRX_Mode(void)
����: ��������Si4432����ģʽ�õ��ļĴ���
**********************************************************************************************************/
void SetRX_Mode(void)
{	
	
	
        //SPI_RW_Reg(WRITE_REG + 0x07, 0x01);	//���� Ready ģʽ
	     //delay_ms(5);
	      TX0_RX1;		//�������߿���
        Delayms(5);
        
        SPI_RW_Reg(S_WRITE_REG + 0x4b, 8);  //�������ݰ�����
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)|0x02); //����FIFO��0
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)&0xFD);
	
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //������е��жϱ�־
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);	 //������е��жϱ�־
         
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x02);  //RFģ���յ��������ݺ󣬲����ж�
	SPI_RW_Reg(S_WRITE_REG + 0x07, 0x05);  //RF ģ��������ģʽ
         
}
/***********************************************************************************************************
������SetRX_Mode(void)
����: ��������Si4432����ģʽ�õ��ļĴ���
**********************************************************************************************************/
void SetTX_Mode(void)
{
	//SPI_RW_Reg(WRITE_REG + 0x07, 0x01);	//rfģ�����Readyģʽ
	//delay_ms(5);
	TX1_RX0;		//�������߿��صķ���
	//delay_ms(5);
	 Delayms(5);
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)|0x01); //����FIFO��0
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)&0xFE);
	
	//SPI_RW_Reg(READ_REG + 0x03, 0x00);  //������е��жϱ�־
        //SPI_RW_Reg(READ_REG + 0x04, 0x00);  //������е��жϱ�־
         
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x04);	//�������ݷ�����󣬲����ж�
	//SPI_RW_Reg(WRITE_REG + 0x07, 0x09); //���뷢��ģʽ
}
/*******************************************************************************
�����ж�
1.������:  Main_ID  Node_Total  Ref_Time  SumL
2.������:  Node_ID  "K"         Main_ID   SumL
3.�ڵ�����:Main_ID  Node_ID     AD_H      AD_L  SumL
*******************************************************************************/
/*******************************************************************************
�����ж�
1.������:  Main_ID  Node_Total  Ref_Time  SumL
2.������:  Node_ID  "K"         Main_ID   SumL
3.�ڵ�����:Main_ID  Node_ID     AD_H      AD_L  SumL
*******************************************************************************/
//void EXTI0_IRQHandler(void)
//{
// //���յ�����
// //���ж��Ƿ�Ϊ��������ID
//  RF4432_RxPacket(RxBuf); //��������     
//  SetRX_Mode(); 
//	EXTI_ClearITPendingBit(EXTI_Line0);  //���LINE4�ϵ��жϱ�־λ 	
//}
/***********************************************************************************************************
??:CLOSE_Mode(void)
??: ????Si4432??????????
**********************************************************************************************************/
void CLOSE_Mode(void)
{
 //TX0_RX0;
  Hign_Si4432_SDN;      
  Hign_Si4432_nSEL;     
  Hign_Si4432_SCLK;     
  Hign_Si4432_SDI;      
}
