#ifndef  __LCD12864_H__
#define  __LCD12864_H__


//宏定义
//12864控制管脚
//使用串行模式15脚PSB接地(L:串行数据模式 H:并行数据模式)
//17脚RST接高(L:复位)

#define  LCD12864_PERIPH		SYSCTL_PERIPH_GPIOD
#define  LCD12864_BASE       GPIO_PORTD_BASE
//4脚RS(CS)		数据/命令选择端(片选信号输入)
#define  LCD12864_CS			GPIO_PIN_0
//5脚R/W(STD)		读/写控制信号(串行数据输入)
#define  LCD12864_SID		GPIO_PIN_1
//6脚E(SCLK)		使能信号(串行移位脉冲输入)
#define  LCD12864_SCLK		GPIO_PIN_2


//清屏
	extern void LCD12864Cls(void);
//发送一个字节数据
	extern void LCD12864SendByte(uint8_t ByteData);

//写指令
	extern void LCD12864SendCmd(uint8_t Cmd);

//写数据
	extern void LCD12864SendData(uint8_t Data);

//初始化
	extern void LCD12864Init(void);


//写ASCII码
	extern void LCD12864WRAsc(uint32_t  x,uint32_t  y,uint8_t Num);

//写汉字
	extern void LCD12864WRStr(uint32_t x,uint32_t y,uint8_t *pStr);



#endif  

//End of 12864.h
