#include "key.h"

static void delay10ms()
{
	int i = 0;
	for(i = 20; i > 0; i--)
		SysCtlDelay(SysCtlClockGet() / 3000);
}

extern void KEY_GPIO_Config(void)
{
	int i;
	SysCtlPeripheralEnable(GPIO_KEY_CTL);

	GPIOPinTypeGPIOInput(GPIO_KEY, GPIO_Key_4 | GPIO_Key_5 | GPIO_Key_6 | GPIO_Key_7);
	GPIOPinTypeGPIOOutput(GPIO_KEY, GPIO_Key_0 | GPIO_Key_1);// | GPIO_Key_2 | GPIO_Key_3);
	for(i = 0; i < 2; i++)
	       GPIO_SetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);
}

extern char ENTER_Key(void)
{
    int i ,j;
    for(i = 0; i<2; i++)
    {
        GPIO_SetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);

        for(j = 0; j<4; j++)
        {
            if(GPIOPinRead(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin)  != 0x00 && GPIOPinRead(Key_Int[i].GPIO_X, Key_Out[i].GPIO_Pin)  != 0x00)
            {
                delay10ms();
                 if(GPIOPinRead(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin) != 0x00 && GPIOPinRead(Key_Int[i].GPIO_X, Key_Out[i].GPIO_Pin)  != 0x00)
                {
                    while(GPIOPinRead(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin) != 0x00 && GPIOPinRead(Key_Int[i].GPIO_X, Key_Out[i].GPIO_Pin)  != 0x00)
                        ;

                    return Key[i][j];
                 }
           }
         }
         GPIO_ResetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);

    }
  return 0;
}
