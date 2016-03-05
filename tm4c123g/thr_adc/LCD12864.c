#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"


#include "LCD12864.H"


//发送一个字节数据
void LCD12864SendByte(uint8_t ByteData)
{
   uint8_t i;
   for(i=0;i<8;i++)
   {
     GPIOPinWrite(LCD12864_BASE, LCD12864_SCLK, LCD12864_SCLK);
     if((ByteData&0x80))
        {
			GPIOPinWrite(LCD12864_BASE, LCD12864_SID, LCD12864_SID);
		}
     else
        {
			GPIOPinWrite(LCD12864_BASE, LCD12864_SID, ~LCD12864_SID);
		}
     ByteData <<= 1;
	 //输出脉冲
     GPIOPinWrite(LCD12864_BASE, LCD12864_SCLK, ~LCD12864_SCLK);
     SysCtlDelay(200);
     GPIOPinWrite(LCD12864_BASE, LCD12864_SCLK,  LCD12864_SCLK);
   }   
}

//写指令
void LCD12864SendCmd(uint8_t Cmd)
{
     GPIOPinWrite(LCD12864_BASE , LCD12864_CS, 0);
     SysCtlDelay(50);
     GPIOPinWrite(LCD12864_BASE , LCD12864_CS, 1);
     LCD12864SendByte(0xf8); 
     LCD12864SendByte(0xf0&Cmd);   
     LCD12864SendByte(0xf0&(Cmd<<4)); 
     GPIOPinWrite(LCD12864_BASE , LCD12864_CS, 0);
}

//写数据
void LCD12864SendData(uint8_t Data)
{
     GPIOPinWrite(LCD12864_BASE , LCD12864_CS, 1);
     LCD12864SendByte(0xfa);          
     LCD12864SendByte(0xf0&Data);     
     LCD12864SendByte(0xf0&(Data<<4));  
     GPIOPinWrite(LCD12864_BASE , LCD12864_CS, 0);
}

//初始化
void LCD12864Init(void)     
{
    SysCtlPeripheralEnable(LCD12864_PERIPH);
    GPIOPinTypeGPIOOutput(LCD12864_BASE, LCD12864_CS|LCD12864_SID|LCD12864_SCLK);

    LCD12864SendCmd(0x30);
    LCD12864SendCmd(0x0C);;
    LCD12864SendCmd(0x01);
	LCD12864SendCmd(0x06);
    SysCtlDelay(200);
}


//写ASCII码	(第x排1-4，第y列1-8,7+'0'就显示7）
void LCD12864WRAsc(uint32_t  x,uint32_t  y,uint8_t Num)
{	 const uint8_t PosTable[4] = {0x80,0x90,0x88,0x98};
	if(x>=1&&x<=4)
	{
		x = PosTable[x-1];
	}
	else 
	{
		x = 0x80;
	}
	LCD12864SendCmd(x+y-1);
	LCD12864SendData(Num);
}

//写汉字(第x排1-4，第y列1-8）
void LCD12864WRStr(uint32_t x,uint32_t y,uint8_t *pStr)
{
	const uint8_t PosTable[4] = {0x80,0x90,0x88,0x98};
	if(x>=1&&x<=4)
	{
		x = PosTable[x-1];
	}
	else 
	{
		x = 0x80;
	}
	LCD12864SendCmd(x+y-1);
    while(*pStr != '\0')
    {
        LCD12864SendData(*(pStr++));
    }
}

void LCD12864Cls(void)
{
	LCD12864SendCmd(0x01);
	LCD12864SendCmd(0x06);
	SysCtlDelay(50);
}

//End of 12864.c
