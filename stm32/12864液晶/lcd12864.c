#include "stm32f10x_gpio.h"
#include "lcd12864.h"
#include "delay.h"
#include "STDIO.H "


extern void LCD_POS(unsigned char x, unsigned char y)
{
	unsigned char pos;
	switch(x)
	{
		case 0: x = 0x80; break;
		case 1: x = 0x90; break;
		case 2: x = 0x88; break;
		case 3: x = 0x98; break;
		default:x = 0x80; break;
	}
	y = y > 7 ? 0 : y;
	pos = x + y;
	Write_Com(pos);
	delay1ms(5);
}

static void LCD_CLC(void)
{
	Write_Com(0x01);
    delay1ms(100);
}

/**
 * 12864的初始化
 **/
extern void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //定义初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LCD12864_GPIO, ENABLE); //使能控制口

	GPIO_InitStructure.GPIO_Pin = LCD12864_CS_PIN | LCD12864_SID_PIN |
								  LCD12864_SCLK_PIN | LCD12864_RST_PIN |
								  LCD12864_CH_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD12864_GPIO, &GPIO_InitStructure);

	GPIO_ResetBits(LCD12864_GPIO, LCD12864_CH_PIN); //选择LCD为串口通信
	delay1us(2);
	LCD_init();
}

extern void LCD_Display(char *s)
{
	while(*s > 0)
	{
		Write_Data(*s);
		s++;
		delay1ms(5);
	}
}

static void LCD_init(void)
{
	GPIO_ResetBits(LCD12864_GPIO, LCD12864_RST_PIN);
	delay1ms(10);
	GPIO_SetBits(LCD12864_GPIO, LCD12864_RST_PIN);
	delay200ms();
	Write_Com(0x30);
	delay1ms(5);
	Write_Com(0x0C);
	delay1ms(5);
}

static void Write_Data(unsigned char disdata)
{
	GPIO_SetBits(LCD12864_GPIO, LCD12864_CS_PIN);
	Send_Byte(0xfa);
	Send_Byte(disdata & 0xf0);
	Send_Byte((disdata << 4) & 0xf0);
	delay1us(2);
}

static void Write_Com(unsigned char cmdcode)
{
	GPIO_SetBits(LCD12864_GPIO, LCD12864_CS_PIN);
	Send_Byte(0xf8);
	Send_Byte(cmdcode & 0xf0);
	Send_Byte((cmdcode << 4) & 0xf0);
	delay1us(2);
}

static void Send_Byte(unsigned char data)
{
	unsigned int i;
	for(i=0; i<8; i++)
	{
		if((data << i) & 0x80)
		{
			GPIO_SetBits(LCD12864_GPIO, LCD12864_SID_PIN);
		}
		else
		{
			GPIO_ResetBits(LCD12864_GPIO, LCD12864_SID_PIN);
		}
		GPIO_ResetBits(LCD12864_GPIO, LCD12864_SCLK_PIN);
		GPIO_SetBits(LCD12864_GPIO, LCD12864_SCLK_PIN);
	}
}



/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -string 转换后的字符串
 *         
 * 返回  ：字符串 string
 * 
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/*
 *File : ftoa.c
 */

/*Float ===> String*/

static int ftoa(char *str, float num, int n)        //n是转换的精度，即是字符串'.'后有几位小数
{
    int     sumI;
    float   sumF;
    int     sign = 0;
    int     temp;
    int     count = 0;

    char *p;
    char *pp;

    if(str == NULL) 
	return -1;
    p = str;

    /*Is less than 0*/
    if(num < 0)
    {
        sign = 1;
        num = 0 - num;
    }

    sumI = (int)num;    //sumI is the part of int
    sumF = num - sumI;  //sumF is the part of float

    /*Int ===> String*/
    do
    {
        temp = sumI % 10;
        *(str++) = temp + '0';
    }while((sumI = sumI /10) != 0);

    if(sign == 1)
    {
        *(str++) = '-';
    }

    pp = str;
    
    pp--;
    while(p < pp)
    {
        *p = *p + *pp;
        *pp = *p - *pp;
        *p = *p -*pp;
        p++;
        pp--;
    }

    *(str++) = '.';     //point

    /*Float ===> String*/
    do
    {
        temp = (int)(sumF*10);
        *(str++) = temp + '0';

        if((++count) == n)
            break;
    
        sumF = sumF*10 - temp;

    }while(!(sumF > -0.000001 && sumF < 0.000001));

    *str = '\0';

    return 0;

}



extern void LCD_Num(int value)
{
	char buffer[80];
	itoa(value,buffer,10);
	LCD_Display(buffer);
}


extern void LCD_FNum(float fvalue)
{
	char buffer[80];
	ftoa(buffer,fvalue,2);
	LCD_Display(buffer);
}

