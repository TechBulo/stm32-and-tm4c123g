#include <key.h>

extern void KEY_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStr;
    int i;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_KEY, ENABLE);
    GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStr.GPIO_Pin = GPIO_Key_0|GPIO_Key_1|GPIO_Key_2|GPIO_Key_3;
    GPIO_InitStr.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIO_KEY, &GPIO_InitStr);

    GPIO_InitStr.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStr.GPIO_Pin = GPIO_Key_4|GPIO_Key_5|GPIO_Key_6|GPIO_Key_7;
    
    GPIO_Init(GPIO_KEY, &GPIO_InitStr);

    for(i = 0; i < 4; i++)
       GPIO_SetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);
   
}


extern char ENTER_Key(void)
{
    int i ,j;
    for(i = 0; i<4; i++)
    {
        GPIO_ResetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);
        for(j = 0; j<4; j++)
        {
            if(!GPIO_ReadInputDataBit(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin))
            {
                delay10ms();
                 if(!GPIO_ReadInputDataBit(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin))
                {
                    while(!GPIO_ReadInputDataBit(Key_Int[j].GPIO_X, Key_Int[j].GPIO_Pin))
                        ;
                    return Key[i][j];
                 }
           }
         }
         GPIO_SetBits(Key_Out[i].GPIO_X, Key_Out[i].GPIO_Pin);

    }
  return 0;
}


