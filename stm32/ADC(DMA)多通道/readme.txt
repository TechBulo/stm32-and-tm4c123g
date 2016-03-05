多通道ADC驱动使用注意事项：

1.IO口占用情况：接口定义：a0-a8可以更换(除a6外，依次与键盘的8个端口相连)；

2.main函数中需要添加的头文件：

#include "adc1.h"
在主函数中需加extern vu16 ADC1_ConvertedValue[ADC_TONGDAO_NUM];

3.调用该函数必须调用该程序为用户提供了返回值为 float GetValue(u16 advalue)函数，
 该函数可以返回AD转换后的实际电压值，在调用该库时必须调用ADC1_Init()进行ADC的配置。

4.以下是各个通道的接口，程序默认打开所有的，要用到那些通道自己要进行连接,在未用到的接口，最好自己在程序中将其删除，以免产生不必要的错误

/*GPIO管脚的配置
选用ADC的通道0    1     2   8    9   10    11   12    13    14  15，分别对应的管脚为
            PA0  PA1  PA2  PB0  PB1  PC0  PC1  PC2  PC3  PC4  PC5
串口使用USART1其中TX为PA9，RX为PA10 */