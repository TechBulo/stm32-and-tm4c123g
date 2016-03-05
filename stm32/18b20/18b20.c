#include "18b20.h"

void DS18B20_RCC(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_18B20|RCC_APB2Periph_AFIO, ENABLE);//��ʱ��
}
/************************************18B20GPIO����********************************
* ��  ����static void GPIO_18B20_Confing(u8 way)
* ��  �ܣ�DS18B20���ŷ�������
* ��  ����uint8_t way(DATAIN , DATAOUT)
* ����ֵ����
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

/************************************18B20��ʼ��********************************
* ��  ����extern ErrorStatus Init_18B20(void)
* ��  �ܣ�DS18B20��ʼ��
* ��  ������
* ����ֵ��ErrorStatus�ж��Ƿ��ʼ���ɹ�success �ɹ� errorʧ��
****************************************************************************/
static ErrorStatus Init_18B20(void)
{
    ErrorStatus Flag = ERROR;

    GPIO_18B20_Confing(DATA_OUT);//����Ϊ���
    
    DS18B20_Pin_H;
    delay1us(10);
    DS18B20_Pin_L;
    delay1us(600);//����480us������
    DS18B20_Pin_H;//�����Ա���������صĵ͵�ƽ
    delay1us(100);//15--65us��ظ�60--240us�ĵ͵�ƽ

    GPIO_18B20_Confing(DATA_IN);
    if(GPIO_ReadInputDataBit(GPIO_18B20, GPIO_18B20_Pin))
    {
        //����ʧ��
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

/******************************����һ���ֽ�********************************
* ��  ����u8 Read_Byte_18B20(void);
* ��  �ܣ���DS18B20����һ���ֽ�
* ��  ������
* ����ֵ��DS18B20���ص�һ���ֽ�
* ��  �£���
* ��  ע����
****************************************************************************/
static u8 Read_Byte_18B20(void)
{
        u8 data = 0 , i = 0;
        GPIO_18B20_Confing(DATA_OUT);//�������ó����
        for(i=0; i<8; i++)
        {
                GPIO_18B20_Confing(DATA_OUT);
                DS18B20_Pin_H;
                delay1us(5);//��ʱ7US
                DS18B20_Pin_L;
                delay1us(5);//����7US��ʾҪ������
                DS18B20_Pin_H;
                delay1us(5);//��ʱ7US
                GPIO_18B20_Confing(DATA_IN);//�������ó�����
                data >>= 1;
                if(GPIO_ReadInputDataBit(GPIO_18B20, GPIO_18B20_Pin))
                {
                    data |= 0x80;
                }
                
                GPIO_18B20_Confing(DATA_OUT);//�ͷ�����
                DS18B20_Pin_H;
                delay1us(60);//��ʱ60US��֤����λ�Ķ�ȡʱ��60-120uS
        }
        return (data);
       
}

/******************************����һ���ֽ�********************************
* ��  ����void Write_Byte_18B20(u8 data);
* ��  �ܣ���DS18B20����һ���ֽ�
* ��  ������DS18B20���͵��ֽ�
* ����ֵ����
* ��  �£���
* ��  ע����
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

/********************************����romָ��*****************************
* ��  ����void Skip_ID(void);
* ��  �ܣ�����������ȡ��ƷID������
* ��  ������
* ����ֵ����
* ��  �£���
* ��  ע����
****************************************************************************/
static void Skip_ID(void)
{
        Write_Byte_18B20(0xcc);
}

/******************************�����¶�ת��ָ��***********************************
* ��  ����void Change(void);
* ��  �ܣ���DS18B20�����¶�ת������
* ��  ������
* ����ֵ����
* ��  �£���
* ��  ע����
****************************************************************************/
static void Change(void)
{
        Write_Byte_18B20(0x44);
}

/******************************���Ͷ�ram����*********************************
* ��  ����void Read_Ram(void);
* ��  �ܣ����Ͷ�ScratchPad����
* ��  ������
* ����ֵ����
* ��  �£���
* ��  ע����
****************************************************************************/
static void Read_Ram(void)
{
        Write_Byte_18B20(0xbe);
}

/****************************���¶�ת�����*********************************
* ��  ����U16 Tmp_Get(void);
* ��  �ܣ���DS18B20��RAM��ȡ�¶�ת�����
* ��  ������
* ����ֵ������ת��ֵ
* ��  �£���
* ��  ע����
****************************************************************************/
static u16 Tmp_Get(void)
{
        u8 datalow = 0x00;
        u16 data = 0x0000;

        datalow = Read_Byte_18B20();//��8λ
        data = Read_Byte_18B20();//�߰�λ
        data = (data<<8) | datalow;

        return(data);
}

/******************************���һ��ת��********************************
* ��  ����u16 One_Change(void);
* ��  �ܣ�����DS18B20���һ���¶�ת��
* ��  ������
* ����ֵ����
* ��  �£���
* ��  ע����
��Ҫ������ǰ���¶�����������Ҫִ�����ι������ڣ�
��һ������Ϊ��λ������ROMָ�0xCC����
ִ���¶�ת���洢������ָ�0x44����
�ȴ�500uS�¶�ת��ʱ�䡣
������ִ�еڶ�������Ϊ��λ��
����ROMָ�0xCC����ִ�ж�RAM�Ĵ洢������ָ�0xBE����
�����ݣ����Ϊ9���ֽڣ���;��ֹͣ��ֻ�����¶�ֵ���ǰ2���ֽڼ��ɣ���
****************************************************************************/
extern u16 One_Change(void)
{
    ErrorStatus i;
    do
    {
        i = Init_18B20();
    }while(!i);

    Skip_ID();//����ROMָ��
    Change();//ִ���¶�ת���洢������ָ��
    delay1ms(10);

    do
    {
        i = Init_18B20();
    }while(!i);
    Skip_ID();//����ROMָ��
    Read_Ram();
    return (Tmp_Get());
}
