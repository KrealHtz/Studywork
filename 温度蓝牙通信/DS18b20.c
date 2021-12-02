#include "DS18b20.h"

unsigned int tvalue;//温度值
unsigned char tflag;//温度正负标志
//延时函数
void delay(unsigned int i)
{
 while(i--);
}
//初始化DS18B20
void Init_DS18B20(void)
{
	unsigned char x=0;
	DQ = 1;    //让DQ置1
	delay(8);  
	DQ = 0;    //DQ拉低
	delay(80); //延时480-960us
	DQ = 1;    //释放总线
	delay(14);
	x=DQ;      
	delay(20);
}

unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)		//循环八次得出数据
	{
		DQ = 0; 		//发送启动信号
		dat>>=1;
		DQ = 1; 	//释放总线
		if(DQ)		//判断是否高电平
		dat|=0x80;	//若是dat最高位置1，不是则置0
		delay(4);
	}
	return(dat);
}

void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)	//循环八次得出数据
 {
  DQ = 0;		 //DQ先置低电平
  DQ = dat&0x01;  //取最低位
  delay(5);
  DQ = 1;	
  dat>>=1;		//由低位向高位发送数据
 }
}

int ReadTemperature(void)
{
	unsigned char a=0;
	unsigned char b=0;

	Init_DS18B20();		//启动DS18B20
	WriteOneChar(0xCC); 	//跳过读序列号的操作
	WriteOneChar(0x44); 	//启动温度转化
	Init_DS18B20();				//启动DS18B20
	WriteOneChar(0xCC);		//跳过读序列号的操作
	WriteOneChar(0xBE);		//读取温度寄存器等（共可读9个寄存器）前两个是温度
	a=ReadOneChar();			//读取温度低位
	b=ReadOneChar();			//读取温度高位
	tvalue = b;					//处理数据
	tvalue <<= 8;				//高位左移八位
	tvalue = tvalue|a;
	if(tvalue<0x0fff)		//小于0x0fff为正数
		tflag=0;
	else
  {
		tvalue=~tvalue+1;		//取反加一
		tflag=1;						//负数
  }
	tvalue = tvalue*(0.625);//温度值扩大10倍，精确到1位小数
	return(tvalue);
}