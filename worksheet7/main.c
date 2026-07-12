#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

GPIO_InitTypeDef GPIO_InitStructure;

void configure_pwm_timer(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    /* Enable TIM3, GPIOC, and AFIO clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    /*
     * Configure PC6 and PC7 as alternate function push-pull.
     * PC6 = TIM3_CH1 green LED
     * PC7 = TIM3_CH2 yellow LED
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Full remap TIM3 channels to GPIOC pins */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    /*
     * PWM frequency setup.
     * Timer counts from 0 to 999.
     * Duty cycle value can be 0 to 999.
     */
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 240 - 1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    /*
     * Configure channel 1 for green LED.
     */
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /*
     * Configure channel 2 for yellow LED.
     */
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_Pulse = 999;
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* Start TIM3 */
    TIM_Cmd(TIM3, ENABLE);
}

int main(void)
{
    int i;
    int direction = 1;
    int brightness = 0;

    configure_pwm_timer();

    while (1)
    {
        if (brightness >= 999)
        {
            direction = -1;
        }

        if (brightness <= 0)
        {
            direction = 1;
        }

        brightness += direction;

        /*
         * Green LED gets brighter while yellow gets dimmer.
         * Then the direction reverses.
         */
        TIM3->CCR1 = brightness;        /* PC6 green LED */
        TIM3->CCR2 = 999 - brightness;  /* PC7 yellow LED */

        for (i = 0; i < 0x4000; i++)
        {
        }
    }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}

#endif
