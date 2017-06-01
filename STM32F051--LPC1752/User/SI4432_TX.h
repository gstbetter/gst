/***********************************************************************************************************
驱动名称：Si4432模拟SPI驱动+标准SPI驱动（已测试）
编译器：ICC-AVR v6.31A 
日期: 2011-10-21 17:49:02
目标芯片: M16L
时钟: 8.0000Mhz
作者：QQ:40560335（此程序是本人通过卖家Demo程序修改而来，请尊重作者劳动成果，若转载请注明，谢谢！~）
卖家：QQ:632598294，Si4432模块仅仅17mm X 17mm，目前全淘宝最小的模块，推荐大家使用啊，哈哈！~
注意：由于时间原因，仅通过模拟SPI测试，直接修改IO口即可使用，若有错误欢迎指正或联系作者一起探讨！~


程序适用范围： 仅用Si4432测试，Si4431及Si4431PA不明。
RF模块的参数设置为：FSK模式，433.92MHz， 1.2KBPS， +/-10PPM, 频偏：30KHz， 调制带宽：61.2KHz 
发射不需要CLK，FiFo，FSK模式，使能AFC，使能CRC， PH + FIFO模式
每秒钟发射的固定测试数据为： 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x6d,
注：0x6d = (0x41 +0x42 +0x43 +0x44 +0x45 +0x46 +0x47 + 0x48 +0x49)


Si4432引脚定义：
1   GND    地
2   GPIO0  内部已接模块的发射控制脚
3   GPIO1  内部已接模块的接收控制脚
4   GPIO2  直接连芯片的GPIO2引脚
5   VCC    +3.3V
6   SDO    串行数据输出
7   SDI    串行数据输入
8   SCLK   串行时钟输入?9   nSEL   SPI片选
10  nIRQ   中断输出引脚
11  SDN    关闭输入引脚，在除关机模式的所有模式SDN=0。当SDN=1 时芯片将被彻底关闭并且寄存器的内容将丢失。
12  GND    地
13  ANT    天线
**********************************************************************************************************/
#define uchar unsigned char
#define uint  unsigned int
	
//#include "sys.h"
#include "stm32f0xx.h"

//PA0 PA1 PA8
//PB0-做中断 PB1 PB5


int k=0;

void si4432_IOInit(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO口速度为50MHz
 GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
 //GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO口速度为50MHz
	GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;		 //IO口速度为50MHz
 GPIO_InitStructure.GPIO_OType=	GPIO_OType_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_0;//PB0,PB1
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN ; //设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}



uint TX_Timeout=1;  //发送超时 s
uchar TX_Reach=1;    //发送定时 s
uchar RSSI_Timeout=0;//RSSI刷新超时 s
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

#define   Read_Si4432_SDO      (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))//读取按键3(WK_UP) )

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
函数名称: Delayms
功    能: 延时指定毫秒（8M晶振）
参    数: MS--延时的毫秒数
返回值  : 无
********************************************/
void Delayms(uint MS)		  
{
 uint i,j;
 for( i=0;i<MS;i++)
 for(j=0;j<1141;j++);	//1141是在8MHz晶振下，通过软件仿真反复实验得到的数值
}
#define   SPI_NOP     _NOP();//_NOP();_NOP();_NOP();_NOP();
//***************************************Si4432寄存器指令*******************************************************
#define S_READ_REG        0x00  	//读寄存器指令
#define S_WRITE_REG       0x80 	//写寄存器指令

#define  TX1_RX0	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x01) //发射状态的天线开关定义
#define  TX0_RX1	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x02) //接收状态的天线开关定义
#define  TX0_RX0	SPI_RW_Reg(S_WRITE_REG + 0x0e, 0x00) //非发射，接收状态的天线开关定义

uchar TxBuf_Len=10;
#define RxBuf_Len 3     //定义RF4432数据包长度 这个长度待定，可设置1或2的

uchar RxBuf[RxBuf_Len];
uchar TxBuf[30] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //每秒发射的固定内容的测试信号，第10个数据是前9个数据的校验和，分别为65，66，67，68，69，70，71，72，73，109
uchar ItStatus;  //发送接收中断
uchar RSSI;      //RSSI
	
/***********************************************************************************************************
函数：SPI_Init(void)
功能: SPI引脚接口及状态初始化
**********************************************************************************************************/
void SPI_Init1(void)
{
    Hign_Si4432_nSEL;
    delay_us1(2);
    Low_Si4432_SCLK;
    delay_us1(2);
}
/***********************************************************************************************************
函数：uchar SPI_Byte(uchar data)
功能：Si4432的SPI写时序
说明：当第一位等于1是为写，等于0时为读。
**********************************************************************************************************/
uchar SPI_Byte(uchar data)
{
/*-----------------------标准硬件SPI-------------------------------------------
  	SPDR = data;
	while(!(SPSR & (1<<SPIF) ))
	{
	};			// 等待SPI发送完毕
	return SPDR;
----------------------以下是模拟SPI时序方式-------------------------------*///←使用标准SPI时，把这一行///前面的小星星移到上面这一行的/前面就OK
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
函数：uchar SPI_RW_Reg(uchar addr, uchar data)
功能：用于读写数据，addr：为寄存器地址，data：为待读出或写入数据
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
函数：SPI_Read_Buf(uchar addr, uchar *data, uchar mun)
功能: 用于读数据，addr：为寄存器地址，data：为待读出数据地址，number：读出数据的个数
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
函数：uchar SPI_Write_Buf(uchar addr, uchar *data, uchar number)
功能: 用于写数据：为寄存器地址，data：为待写入数据地址，number：写入数据的个数
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
函数：Si4432_init(void)
功能: 用于初始化Si4432的SPI控制接口及寄存器
**********************************************************************************************************/
void Si4432_init()
{
  SPI_Init1();
        
  Hign_Si4432_SDN;// 	SDN = 1;
 	Delayms(200);	// RF 模块上电复位
 	Low_Si4432_SDN;//SDN = 0;
 	Delayms(200);	// 延时200ms RF 模块进入工作状态

	Delayms(500);
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //清RF模块中断	
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);
	
        SPI_RW_Reg(S_WRITE_REG + 0x05, 0x02);  // RF模块收到整包数据后，产生中断
	SPI_RW_Reg(S_WRITE_REG + 0x06, 0x00);  //不使能同步字侦测
        
	SPI_RW_Reg(S_WRITE_REG + 0x07, 0x01);  //进入 Ready 模式
	
	SPI_RW_Reg(S_WRITE_REG + 0x09, 0x7f);  //负载电容= 12P
	SPI_RW_Reg(S_WRITE_REG + 0x0a, 0x05);  //关闭低频输出
	SPI_RW_Reg(S_WRITE_REG + 0x0b, 0xea);  //GPIO 0 当做普通输出口
	SPI_RW_Reg(S_WRITE_REG + 0x0c, 0xea);  //GPIO 1 当做普通输出口
	SPI_RW_Reg(S_WRITE_REG + 0x0d, 0xf4);  //GPIO 2 输出收到的数据
	SPI_RW_Reg(S_WRITE_REG + 0x70, 0x2c);  
	SPI_RW_Reg(S_WRITE_REG + 0x1d, 0x40);  //使能 afc
	
	// 1.2K bps setting
	SPI_RW_Reg(S_WRITE_REG + 0x1c, 0x16);  //发射16个Nibble的Preamble
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
	
	SPI_RW_Reg(S_WRITE_REG + 0x30, 0x8c);  //使能PH+ FIFO模式，高位在前面，使能CRC校验
	SPI_RW_Reg(S_WRITE_REG + 0x32, 0xff);  //byte0, 1,2,3 作为头码
	SPI_RW_Reg(S_WRITE_REG + 0x33, 0x42);  //byte 0,1,2,3 是头码，同步字3,2 是同步字
	SPI_RW_Reg(S_WRITE_REG + 0x34, 16);    //发射16个Nibble的Preamble
	SPI_RW_Reg(S_WRITE_REG + 0x35, 0x20);  //需要检测4个nibble的Preamble
	SPI_RW_Reg(S_WRITE_REG + 0x36, 0x2d);  //同步字为 0x2dd4
	SPI_RW_Reg(S_WRITE_REG + 0x37, 0xd4);
	SPI_RW_Reg(S_WRITE_REG + 0x38, 0x00);
	SPI_RW_Reg(S_WRITE_REG + 0x39, 0x00);
	SPI_RW_Reg(S_WRITE_REG + 0x3a, 's');   //发射的头码为： “swwx"
	SPI_RW_Reg(S_WRITE_REG + 0x3b, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x3c, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x3d, 'x');
	SPI_RW_Reg(S_WRITE_REG + 0x3e,  TxBuf_Len);   //总共发射10个字节的数据
	SPI_RW_Reg(S_WRITE_REG + 0x3f, 's');   //需要校验的头码为：”swwx"
	SPI_RW_Reg(S_WRITE_REG + 0x40, 'w');
	SPI_RW_Reg(S_WRITE_REG + 0x41, 'w'); 
	SPI_RW_Reg(S_WRITE_REG + 0x42, 'x');
	SPI_RW_Reg(S_WRITE_REG + 0x43, 0xff);  //头码1,2,3,4 的所有位都需要校验
	SPI_RW_Reg(S_WRITE_REG + 0x44, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x45, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x46, 0xff);  // 
	SPI_RW_Reg(S_WRITE_REG + 0x6d, 0X00);  //发射功率设置  0x00:+0dBM  0x01:+3dBM  0x02:+6dBM  0x03:+9dBM  0x04:+11dBM  0x05:+14dBM  0x06:+17dBM  0x07:20dBM
	
        SPI_RW_Reg(S_WRITE_REG + 0x79, 0x0);   //不需要跳频
	SPI_RW_Reg(S_WRITE_REG + 0x7a, 0x0);   //不需要跳频
	SPI_RW_Reg(S_WRITE_REG + 0x71, 0x22);  //发射不需要CLK，FiFo，FSK模式
	SPI_RW_Reg(S_WRITE_REG + 0x72, 0x30);  //频偏为 30KHz
	SPI_RW_Reg(S_WRITE_REG + 0x73, 0x0);   //没有频率偏差
	SPI_RW_Reg(S_WRITE_REG + 0x74, 0x0);   //没有频率偏差
	SPI_RW_Reg(S_WRITE_REG + 0x75, 0x53);  //频率设置 434
	SPI_RW_Reg(S_WRITE_REG + 0x76, 0x64);  //fc[15:8]
	SPI_RW_Reg(S_WRITE_REG + 0x77, 0x00);     //fc[7:0]

	TX0_RX0;	// 天线开关不在发射，接收状态
        //SetRX_Mode();
        //P1IE|=Si4432_nIRQ;
}
/***********************************************************************************************************
函数：uchar RF4432_RSSI(void)
功能: 用于Si4432查询中断是否为 侦测到同步字 中断
**********************************************************************************************************/
uchar RF4432_RSSI(void)
{	
	if(!(Read_Si4432_nIRQ))
	{
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x04,0x00);  //读中断寄存器
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
函数：uchar RF4432_RxPacket(uchar *dataBuffer)
功能: 用于Si4432查询中断是否为 接收到有效包 中断
**********************************************************************************************************/
uchar RF4432_RxPacket(uchar *dataBuffer)
{	 		//	  Hign_D1_LED;
	
	k=1;
	
	
	if(!(Read_Si4432_nIRQ))
	{       
              delay_us1(2);
			  Hign_D1_LED;
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x03,0x00);  //读中断寄存器
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
函数：void RF4432_TxPacket(uchar *Tx_Buf)
功能: 用于Si4432查询中断是否为 接收到有效包 中断
**********************************************************************************************************/
void RF4432_TxPacket(uchar *dataBuffer)
{
	SPI_RW_Reg(S_WRITE_REG + 0x34, 40);   //发射40个Nibble 的前导码
	SPI_RW_Reg(S_WRITE_REG + 0x3e, TxBuf_Len);  //总共发射10个字节的数据
	
	SPI_Write_Buf(S_WRITE_REG + 0x7f, dataBuffer, TxBuf_Len);   //将要发送的数据写入寄存器
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x04);	//整包数据发射完后，产生中断
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //清RF模块中断	
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);
        
        SPI_RW_Reg(S_WRITE_REG + 0x07, 0x09); //进入发射模式
	TX_Timeout = 0;
	while(Read_Si4432_nIRQ)		//等待中断
	{      
		Delayms(1);
		if(TX_Timeout>800)//8M 晶振下实测350(8个字节)
		{
                    ComFlag=0;//发送失败
                    //TX_Timeout=0;
                    //delay_ms(20000);
                    //Si4432_init();
                    delay_us1(2);
                    break;		//则强制跳出
		}
                TX_Timeout++;
	}

	if(!(Read_Si4432_nIRQ))
	{
		ItStatus = SPI_RW_Reg(S_READ_REG + 0x03,0x00);  //读中断寄存器
		if((ItStatus&0x04)==0x04)
		{
                   ComFlag=1;//发送成功
                 delay_us1(2);
		}
	}
}
/***********************************************************************************************************
函数：SetRX_Mode(void)
功能: 用于配置Si4432接收模式用到的寄存器
**********************************************************************************************************/
void SetRX_Mode(void)
{	
	
	
        //SPI_RW_Reg(WRITE_REG + 0x07, 0x01);	//进入 Ready 模式
	     //delay_ms(5);
	      TX0_RX1;		//设置天线开关
        Delayms(5);
        
        SPI_RW_Reg(S_WRITE_REG + 0x4b, 8);  //接收数据包长度
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)|0x02); //接收FIFO清0
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)&0xFD);
	
	SPI_RW_Reg(S_READ_REG + 0x03, 0x00);	 //清掉现有的中断标志
	SPI_RW_Reg(S_READ_REG + 0x04, 0x00);	 //清掉现有的中断标志
         
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x02);  //RF模块收到整包数据后，产生中断
	SPI_RW_Reg(S_WRITE_REG + 0x07, 0x05);  //RF 模块进入接收模式
         
}
/***********************************************************************************************************
函数：SetRX_Mode(void)
功能: 用于配置Si4432发送模式用到的寄存器
**********************************************************************************************************/
void SetTX_Mode(void)
{
	//SPI_RW_Reg(WRITE_REG + 0x07, 0x01);	//rf模块进入Ready模式
	//delay_ms(5);
	TX1_RX0;		//设置天线开关的方向
	//delay_ms(5);
	 Delayms(5);
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)|0x01); //接收FIFO清0
	SPI_RW_Reg(S_WRITE_REG + 0x08, SPI_RW_Reg(S_READ_REG + 0x08, 0x00)&0xFE);
	
	//SPI_RW_Reg(READ_REG + 0x03, 0x00);  //清掉现有的中断标志
        //SPI_RW_Reg(READ_REG + 0x04, 0x00);  //清掉现有的中断标志
         
	SPI_RW_Reg(S_WRITE_REG + 0x05, 0x04);	//整包数据发射完后，产生中断
	//SPI_RW_Reg(WRITE_REG + 0x07, 0x09); //进入发射模式
}
/*******************************************************************************
接收中断
1.心跳包:  Main_ID  Node_Total  Ref_Time  SumL
2.心跳包:  Node_ID  "K"         Main_ID   SumL
3.节点数据:Main_ID  Node_ID     AD_H      AD_L  SumL
*******************************************************************************/
/*******************************************************************************
接收中断
1.心跳包:  Main_ID  Node_Total  Ref_Time  SumL
2.心跳包:  Node_ID  "K"         Main_ID   SumL
3.节点数据:Main_ID  Node_ID     AD_H      AD_L  SumL
*******************************************************************************/
//void EXTI0_IRQHandler(void)
//{
// //接收到数据
// //先判断是否为心跳主机ID
//  RF4432_RxPacket(RxBuf); //接收数据     
//  SetRX_Mode(); 
//	EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE4上的中断标志位 	
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
