#include <REGX52.H>
#include "DS18b20.h"
#include "LCD1602.h"
 


char RECEIVED_CMD;
unsigned char flag = 0 ;      	// 数据接收的标志位
extern unsigned int tvalue;			//温度值
extern unsigned char tflag;			//温度正负标志
unsigned char disdata[7]; 			// 温度数据，使用8字节数组来存储
unsigned char chg[3];
unsigned int wrong = 30;


void UART_Init(); // 初始化串口
void UART_SendData(char dat); // 串口发送数据
void UART_SendStr(char* str); // 串口发送字符串
void ds1820disp(); 	// 温度显示
void change();			//转化阈值为字符数组


void main() 
{
	unsigned int temperature , old ; // 保存温度数值
	int A=5000;
	
	UART_Init();  // 串口初始化
	LCD_Init();  // 显示屏 初始化
	LCD_ShowString(1,1,"Temperature");
	P1_4 = 0;
	temperature = ReadTemperature(); 
	old = temperature ; 
	ds1820disp(); // 转换温度
	LCD_ShowString(2,1,disdata);
	LCD_ShowNum(2,13,wrong,2);
	
	while(1)
	{
		temperature=ReadTemperature();  // 读取一次新的温度
		LCD_ShowNum(2,13,wrong,2);
		LCD_ShowChar(2,16,'C');

    if (temperature != old )	  
	  {	 
			old = temperature;
			ds1820disp(); // 转化温度
			LCD_ShowString(2,1,disdata); // 显示温度
	  }
		
		if(temperature > wrong * 10)
		{
				P1_4 = !P1_4; //发出报警声
				while(A--);
				A=5000;
				LCD_ShowChar(1,16,'W');
		}
		else
		{
			LCD_ShowChar(1,16,'N');
			P1_4 = 0;
		}
				
		
		if(flag) // 接收数据完毕一次，就会进入中断一次
		{
			flag = 0 ; // 将标志位还原，使得串口又可以重新接收数据
					
			if(RECEIVED_CMD == '0')
			{
				UART_SendStr(disdata);//向串口发送数据，发送的是当前温度
			}
			else if(RECEIVED_CMD == '3')
			{
				change();
				UART_SendStr(chg);		//发送阈值
			}
			else if(RECEIVED_CMD == '1')
			{
				wrong ++ ;//阈值加
			}
			else if(RECEIVED_CMD  == '2')
			{
				wrong -- ;//阈值减
			}			
			RECEIVED_CMD =' ';
		}
	}
}

//串口初始化
void UART_Init()
{
	SCON=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
	//打开两个外部中断
	IT0 = 1;
	IT1 = 1;
	EX0 = 1;
	EX1 = 1;
	
}


void UART_Isr() interrupt 4 using 1
{
	// 串口接收中断处理
	if(RI) 
	{
		RI = 0 ;                              // 清除中断标志位
		RECEIVED_CMD = SBUF ; // 保存串口接收的数据
		flag = 1 ;           // 接收结束，到循环中处理接收的数据
		}

	// 串口发送中断处理
	if(TI)
	{
		TI = 0 ;  // 清发送中断标志位
	}
		
}

//开关K1
void int0() interrupt 0
{
	wrong ++;
}
//开关K2
void int1() interrupt 2
{
	wrong --;
}

//通过串口发送一位数据
void UART_SendData(char dat)
{
	ES = 0 ;      // 串口工作的时候禁止中断
	SBUF = dat ;  // 待发送的数据放到SBUF中
	while(!TI) ;  // 等待发送完毕
	TI = 0 ;      // 清TI中断
	ES = 1 ;      // 打开中断
}

//发送字符串
void UART_SendStr(char *str)
{
		do
		{
			UART_SendData(*str);
		}while(*str ++  != '\0' ); // 一直到字符串结束
}

//温度转化函数，将测得的温度值转化为字符数组存放
void ds1820disp()
{ 	
	  unsigned char flagdat;
	
		if(tflag==0)
		  flagdat=0x2b;//正温度显示符号
		else
		  flagdat=0x2d;//负温度显示负号:-
		
		disdata[1]=tvalue/1000+0x30;//百位数
		disdata[2]=tvalue%1000/100+0x30;//十位数
		disdata[3]=tvalue%100/10+0x30;//个位数
		disdata[4]= 0x2E ;//小数点
		disdata[5]=tvalue%10/1+0x30;//小数位

		if(disdata[1]==0x30) // 如果百位为0
		{
			disdata[0]= 0x20; // 第一位不显示
			disdata[1]= flagdat; // 百位显示符号
			if(disdata[2]==0x30) //如果百位为0，十位为0
			{
				disdata[1]=0x20; // 百位不显示符号
				disdata[2]=flagdat; // 10位显示符号
			}
		}
}


//转换阈值
void change()
{
	chg[0] = wrong / 10 + 0x30;
	chg[1] = wrong % 10 + 0x30;
}