#include "adc1.h"

//__IO u16 ADC1_ConvertedValue
__IO u16 ADC1_ConvertedValue[ADC_TONGDAO_NUM];

//配置ADC1相关的GPIO以及打开时钟，选择通道需要更改此处
static void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStr;

    //打开DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //打开GPIO时钟，根据需要改此处的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO,ENABLE);

    //PA0/1/2作为模拟通道输入引脚，若要选择通道改此处的GPIO引脚
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//配置为模拟输入引脚
    GPIO_Init(GPIOA,&GPIO_InitStr);

    //PB0/1作为模拟通道输入引脚，若要选择通道改此处的GPIO引脚
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//配置为模拟输入引脚
    GPIO_Init(GPIOB,&GPIO_InitStr);

    //PC0/1/2/3/4/5作为模拟通道输入引脚，若要选择通道改此处的GPIO引脚
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//配置为模拟输入引脚
    GPIO_Init(GPIOC,&GPIO_InitStr);

}
//配置ADC1,选择通道需要更改此处
static void ADC1_Config(void)
{
    ADC_InitTypeDef ADC1_InitStr;

    ADC_DeInit(ADC1);//将adc的寄存器全设为缺省值

    ADC1_InitStr.ADC_Mode = ADC_Mode_Independent;//adc1和adc2工作在独立模式
    ADC1_InitStr.ADC_ScanConvMode = ENABLE;//使能扫描模式(扫描用于多通道)
    ADC1_InitStr.ADC_ContinuousConvMode = DISABLE;//连续转换
    ADC1_InitStr.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//关闭外部触发
    ADC1_InitStr.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
    ADC1_InitStr.ADC_NbrOfChannel = ADC_TONGDAO_NUM; //所需转换的通道数

    ADC_Init(ADC1, &ADC1_InitStr);

    /*设置指定ADC的规则组通道，设置它们的转化顺序和采样时间*/
    /*ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 6,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 7,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 8,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 9,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 10,ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 11,ADC_SampleTime_239Cycles5);

    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数)
    ADC_DMACmd(ADC1, ENABLE);

    //使能adc1
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);          //复位指定的ADC1的校准寄存器

    while(ADC_GetResetCalibrationStatus(ADC1));//待复位完成

     ADC_StartCalibration(ADC1);//让ADC自动校正
     while(ADC_GetCalibrationStatus(ADC1));//待校正完成

}

/*配置dma*/
static void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStr;

    DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值

    DMA_InitStr.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA外设ADC基地址
    DMA_InitStr.DMA_MemoryBaseAddr = (u32)&ADC1_ConvertedValue;//DMA内存地址
    DMA_InitStr.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStr.DMA_BufferSize = ADC_TONGDAO_NUM;//设置缓存区大小
    DMA_InitStr.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStr.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStr.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度为16位
    DMA_InitStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //内存数据宽度为16位
    DMA_InitStr.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式
    DMA_InitStr.DMA_Priority = DMA_Priority_High; //DMA通道 1拥有高优先级
    DMA_InitStr.DMA_M2M = DMA_M2M_Disable;  //DMA通道1没有设置为内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStr);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
    DMA_Cmd(DMA1_Channel1,ENABLE);

}

void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Config();
    DMA_Config();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //让ADC开始工作
}

/*获取ADC的值，将二进制换算为十进制*/
float GetValue(u16 advalue)
{
    float value;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    value = (float)advalue/ 4096 * 3.3;
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    return value;
}
