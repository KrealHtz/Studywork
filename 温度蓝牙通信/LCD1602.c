#include <REGX52.H>

//引脚配置：
sbit LCD_RS=P2^6;			//数据/指令选择，1为数据，0为指令
sbit LCD_RW=P2^5;			//读写选择段，1为读，0为写
sbit LCD_EN=P2^7;			//使能，1为数据有效，下降沿执行命令
#define LCD_DataPort P0

//延时函数
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 539;
	do
	{
		while (--j);
	} while (--i);
}

//LCD写命令
void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;		//选择命令
	LCD_RW=0;			
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

//LCD写数据
void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;		//选择数据
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;			
	LCD_Delay();
}

// LCD1602设置光标位置

void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1)); //设置光标位置，最高位为1时指令为设置地址
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));//第二行地址是从0x40开始的
	}
}

//LCD1602初始化函数
 
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0c);//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);//光标复位，清屏
}

//在LCD1602指定位置上显示一个字符
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

//在LCD1602指定位置开始显示所给字符串

void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

//  返回值=X的Y次方
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

//  在LCD1602指定位置开始显示所给数字
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}



