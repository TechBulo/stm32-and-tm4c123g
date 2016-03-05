#include "18b20.h"

void DS18B20_RCC(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_18B20|RCC_APB2Periph_AFIO, ENABLE);//打开时钟
}
/************************************18B20GPIO设置********************************
* 函  数：static void GPIO_18B20_Confing(u8 way)
* 功  能：DS18B20引脚方向配置
* 参  数：uint8_t way(DATAIN , DATAOUT)
* 返回值：无
****************************************************************************/
static void GPIO_18B20_Confing(u8 way)
{
    GPIO_InitTypeDef GPIO_InitStr;

    

    GPIO_InitStr.GPIO_Pin = GPIO_18B20_Pin;
    GPIO_InitStr.GPIO_Speed = GPIO_Speed_50MHz;
    if(way == DATA_IN)
    {
        GPIO_InitStr.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIO_18B20, &GPIO_InitStr);
    }
    else
    {
        GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIO_18B20, &GPIO_InitStr);
    }
}

/************************************18B20初始化********************************
* 函  数：extern ErrorStatus Init_18B20(void)
* 功  能：DS18B20初始化
* 参  数：无
* 返回值：ErrorStatus判断是否初始化成功success 成功 error失败
****************************************************************************/
static ErrorStatus Init_18B20(void)
{
    ErrorStatus Flag = ERROR;

    GPIO_18B20_Confing(DATA_OUT);//配置为输出
    
    DS18B20_Pin_H;
    delay1us(10);
    DS18B20_Pin_L;
    delay1us(600);//至少480us的拉低
    DS18B20_Pin_H;//拉高以便接收所返回的低电平
    delay1us(100);//15--65us后回复60--240us的低电平

    GPIO_18B20_Confing(DATA_IN);
    if(GPIO_ReadInputDataBit(GPIO_18B20, GPIO_18B20_Pin))
    {
        //握手失败
        Flag = ERROR;
        GPIO_18B20_Confing(DATA_OUT);
    }
    else
    {
        Flag = SUCCESS;
        GPIO_18B20_Confing(DATA_OUT);
        DS18B20_Pin_H;
    }

    delay1us(400);
    return Flag;

}

/******************************接收一个字节********************************
* 函  数：u8 Read_Byte_18B20(void);
* 功  能：从DS18B20接收一个字节
* 参  数：无
* 返回值：DS18B20返回的一个字节
* 更  新：无
* 备  注：无
****************************************************************************/
static u8 Read_Byte_18B20(void)
{
        u8 data = 0 , i = 0;
        GPIO_18B20_Confing(DATA_OUT);//引脚配置成输出
        for(i=0; i<8; i++)
        {
                GPIO_18B20_Confing(DATA_OUT);
                DS18B20_Pin_H;
                delay1us(5);//延时7US
                DS18B20_Pin_L;
                delay1us(5);//拉低7US表示要读数据
                DS18B20_Pin_H;
                delay1us(5);//延时7US
                GPIO_18B20_Confing(DATA_IN);//引脚配置成输入
                data >>= 1;
                if(GPIO_ReadInputDataBit(GPIO_18B20, GPIO_18B20_Pin))
                {
                    data |= 0x80;
                }
                
                GPIO_18B20_Confing(DATA_OUT);//释放总线
                DS18B20_Pin_H;
                delay1us(60);//延时60US保证整个位的读取时间60-120uS
        }
        return (data);
       
}

/******************************发送一个字节********************************
* 函  数：void Write_Byte_18B20(u8 data);
* 功  能：从DS18B20发送一个字节
* 参  数：给DS18B20发送的字节
* 返回值：无
* 更  新：无
* 备  注：无
****************************************************************************/
static void Write_Byte_18B20(u8 data)
{
    u8 i;

    for(i = 0;i < 8;i++)
    {
        GPIO_18B20_Confing(DATA_OUT);
        DS18B20_Pin_L;
        delay1us(15);
        if( data & 0x01)
        {
          DS18B20_Pin_H;
          delay1us(45);
        }
        else
        {
            DS18B20_Pin_L;
            delay1us(45);
        }
        data>>=1;
        
        DS18B20_Pin_H;
        delay1us(40);
    }
}

/********************************跳过rom指令*****************************
* 函  数：void Skip_ID(void);
* 功  能：发送跳过读取产品ID号命令
* 参  数：无
* 返回值：无
* 更  新：无
* 备  注：无
****************************************************************************/
static void Skip_ID(void)
{
        Write_Byte_18B20(0xcc);
}

/******************************发送温度转换指令***********************************
* 函  数：void Change(void);
* 功  能：向DS18B20发送温度转换命令
* 参  数：无
* 返回值：无
* 更  新：无
* 备  注：无
****************************************************************************/
static void Change(void)
{
        Write_Byte_18B20(0x44);
}

/******************************发送读ram命令*********************************
* 函  数：void Read_Ram(void);
* 功  能：发送读ScratchPad命令
* 参  数：无
* 返回值：无
* 更  新：无
* 备  注：无
****************************************************************************/
static void Read_Ram(void)
{
        Write_Byte_18B20(0xbe);
}

/****************************读温度转换结果*********************************
* 函  数：U16 Tmp_Get(void);
* 功  能：从DS18B20的RAM读取温度转换结果
* 参  数：无
* 返回值：返回转换值
* 更  新：无
* 备  注：无
****************************************************************************/
static u16 Tmp_Get(void)
{
        u8 datalow = 0x00;
        u16 data = 0x0000;

        datalow = Read_Byte_18B20();//低8位
        data = Read_Byte_18B20();//高八位
        data = (data<<8) | datalow;

        return(data);
}

/******************************完成一次转换********************************
* 函  数：u16 One_Change(void);
* 功  能：控制DS18B20完成一次温度转换
* 参  数：无
* 返回值：无
* 更  新：无
* 备  注：无
若要读出当前的温度数据我们需要执行两次工作周期，
第一个周期为复位、跳过ROM指令（0xCC）、
执行温度转换存储器操作指令（0x44）、
等待500uS温度转换时间。
紧接着执行第二个周期为复位、
跳过ROM指令（0xCC）、执行读RAM的存储器操作指令（0xBE）、
读数据（最多为9个字节，中途可停止，只读简单温度值则读前2个字节即可）。
****************************************************************************/
extern u16 One_Change(void)
{
    ErrorStatus i;
    do
    {
        i = Init_18B20();
    }while(!i);

    Skip_ID();//跳过ROM指令
    Change();//执行温度转换存储器操作指令
    delay1ms(10);

    do
    {
        i = Init_18B20();
    }while(!i);
    Skip_ID();//跳过ROM指令
    Read_Ram();
    return (Tmp_Get());
}
