#include <REG52.H>
#include "SoundPlay.h"
#include "LCD1602.h"
#include "song.h"
int i = 1, times = 1; next = 2;  previous = 3;
char moth = 'O';


void Interrupt_Init()
{
	//外部中断0初始化
	IT0 = 1; //脉冲出触发
	EA = 1;
	EX0 = 1;
	
	//外部中断1初始化
	IT1 = 1; //脉冲出触发
	EA = 1;
	EX1 = 1;
	
	//设置优先级
	IP=0X05;
}

void Delay1ms(unsigned int count)
{
	unsigned int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<120;j++);
}


void main()
{
	InitialSound();
//	LCD_Init();
	Interrupt_Init();
	while(1)
	{	
//		LCD_ShowString(1,6,"NOW:");
//		LCD_ShowNum(1,10,i % 3 + 1,1);
//		LCD_ShowChar(1,16,moth);
//		LCD_ShowString(2,3,"N:");
//		LCD_ShowNum(2,5,next,1);
//		LCD_ShowString(2,8,"P:");
//		LCD_ShowNum(2,10,previous,1);
//		LCD_ShowString(2,14,"T:");
//		LCD_ShowNum(2,16,times,1);
		
//音乐播放 默认单曲循环	
		change=0;
		if(i % 3 == 0)
		{Interrupt_Init();
			Play(Music_Girl,0,3,360);
		}
		else if(i % 3 == 1)
		{Interrupt_Init();
			Play(Music_Same,0,3,360);
		}
		else if(i % 3 == 2)
		{Interrupt_Init();
			Play(Music_Two,0,3,360);
		}
	}
	
}
/****************************************
             中断处理部分
****************************************/
void nst() interrupt 0
{
	ET0 = !ET0;
	Delay1ms(10);
	BeepIO = 0;
	
//	LCD_ShowNum(1,10,i % 3 + 1,1);
}


void INTI1() interrupt 2
{
	i ++;
	change=1;
}

