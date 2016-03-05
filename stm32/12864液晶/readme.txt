LCD12864驱动使用注意事项：

1.IO口占用情况：PD0：RS   	D1：R/W  	D2：EN		D3：Reset		PD4：Vout；

2.VSS接地，VDD接5V电压；	背光灯：A接正，K接负，5V电压；

3.main函数中需要添加的头文件：

#include “delay.h”
#include "lcd12864.h"

4.main函数中添加Display函数，修改可直接显示；
或者调用			LCD_CLC();	         	//清屏
  				LCD_POS(0, 0);			//显示的坐标
  				LCD_Display("电子设计大赛");	//显示数据。
				LCD_Num(int value) ;		//显示整形变量。
  				LCD_FNum(float fvalue);		//显示浮点数。
5、注意事项
1)、在使用该库中的函数前必须现在主函数中调用void LCD_GPIO_Config(void);//初始化配置
以上函数用户在调用时必须先调用LCD_CLC();	//清屏
然后调用LCD_POS(0, 0);			//显示的坐标（汉字最大值为3,7英文的话7,15）
最后调用LCD_Display("电子设计大赛");	//显示字符（中文或英文）。
或者调用LCD_Num(int value) ;		//显示整形变量。
或者调用LCD_FNum(float fvalue);		//显示浮点数。