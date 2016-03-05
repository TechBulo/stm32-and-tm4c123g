#include "adc1.h"

//__IO u16 ADC1_ConvertedValue
__IO u16 ADC1_ConvertedValue[ADC_TONGDAO_NUM];

//����ADC1��ص�GPIO�Լ���ʱ�ӣ�ѡ��ͨ����Ҫ���Ĵ˴�
static void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStr;

    //��DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //��GPIOʱ�ӣ�������Ҫ�Ĵ˴���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO,ENABLE);

    //PA0/1/2��Ϊģ��ͨ���������ţ���Ҫѡ��ͨ���Ĵ˴���GPIO����
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//����Ϊģ����������
    GPIO_Init(GPIOA,&GPIO_InitStr);

    //PB0/1��Ϊģ��ͨ���������ţ���Ҫѡ��ͨ���Ĵ˴���GPIO����
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//����Ϊģ����������
    GPIO_Init(GPIOB,&GPIO_InitStr);

    //PC0/1/2/3/4/5��Ϊģ��ͨ���������ţ���Ҫѡ��ͨ���Ĵ˴���GPIO����
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_AIN;//����Ϊģ����������
    GPIO_Init(GPIOC,&GPIO_InitStr);

}
//����ADC1,ѡ��ͨ����Ҫ���Ĵ˴�
static void ADC1_Config(void)
{
    ADC_InitTypeDef ADC1_InitStr;

    ADC_DeInit(ADC1);//��adc�ļĴ���ȫ��Ϊȱʡֵ

    ADC1_InitStr.ADC_Mode = ADC_Mode_Independent;//adc1��adc2�����ڶ���ģʽ
    ADC1_InitStr.ADC_ScanConvMode = ENABLE;//ʹ��ɨ��ģʽ(ɨ�����ڶ�ͨ��)
    ADC1_InitStr.ADC_ContinuousConvMode = DISABLE;//����ת��
    ADC1_InitStr.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�ر��ⲿ����
    ADC1_InitStr.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
    ADC1_InitStr.ADC_NbrOfChannel = ADC_TONGDAO_NUM; //����ת����ͨ����

    ADC_Init(ADC1, &ADC1_InitStr);

    /*����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��*/
    /*ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����*/
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

    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ���)
    ADC_DMACmd(ADC1, ENABLE);

    //ʹ��adc1
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);          //��λָ����ADC1��У׼�Ĵ���

    while(ADC_GetResetCalibrationStatus(ADC1));//����λ���

     ADC_StartCalibration(ADC1);//��ADC�Զ�У��
     while(ADC_GetCalibrationStatus(ADC1));//��У�����

}

/*����dma*/
static void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStr;

    DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

    DMA_InitStr.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA����ADC����ַ
    DMA_InitStr.DMA_MemoryBaseAddr = (u32)&ADC1_ConvertedValue;//DMA�ڴ��ַ
    DMA_InitStr.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStr.DMA_BufferSize = ADC_TONGDAO_NUM;//���û�������С
    DMA_InitStr.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStr.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStr.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݿ��Ϊ16λ
    DMA_InitStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�ڴ����ݿ��Ϊ16λ
    DMA_InitStr.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
    DMA_InitStr.DMA_Priority = DMA_Priority_High; //DMAͨ�� 1ӵ�и����ȼ�
    DMA_InitStr.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��1û������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStr);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
    DMA_Cmd(DMA1_Channel1,ENABLE);

}

void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Config();
    DMA_Config();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //��ADC��ʼ����
}

/*��ȡADC��ֵ���������ƻ���Ϊʮ����*/
float GetValue(u16 advalue)
{
    float value;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    value = (float)advalue/ 4096 * 3.3;
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    return value;
}
