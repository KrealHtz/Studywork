#ifndef __DS18B20_H_
#define __DS18B20_H_

#include <REGX52.H>


sbit DQ = P1^3; // 定义温度接口

void Init_DS18B20(void);  // 初始化
unsigned char ReadOneChar(void);  // 读一位数据
void WriteOneChar(unsigned char dat); // 写一位数据
int ReadTemperature(void); // 读取温度

#endif 