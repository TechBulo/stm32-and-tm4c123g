#ifndef  __LCD12864_H__
#define  __LCD12864_H__


//�궨��
//12864���ƹܽ�
//ʹ�ô���ģʽ15��PSB�ӵ�(L:��������ģʽ H:��������ģʽ)
//17��RST�Ӹ�(L:��λ)

#define  LCD12864_PERIPH		SYSCTL_PERIPH_GPIOD
#define  LCD12864_BASE       GPIO_PORTD_BASE
//4��RS(CS)		����/����ѡ���(Ƭѡ�ź�����)
#define  LCD12864_CS			GPIO_PIN_0
//5��R/W(STD)		��/д�����ź�(������������)
#define  LCD12864_SID		GPIO_PIN_1
//6��E(SCLK)		ʹ���ź�(������λ��������)
#define  LCD12864_SCLK		GPIO_PIN_2


//����
	extern void LCD12864Cls(void);
//����һ���ֽ�����
	extern void LCD12864SendByte(uint8_t ByteData);

//дָ��
	extern void LCD12864SendCmd(uint8_t Cmd);

//д����
	extern void LCD12864SendData(uint8_t Data);

//��ʼ��
	extern void LCD12864Init(void);


//дASCII��
	extern void LCD12864WRAsc(uint32_t  x,uint32_t  y,uint8_t Num);

//д����
	extern void LCD12864WRStr(uint32_t x,uint32_t y,uint8_t *pStr);



#endif  

//End of 12864.h
