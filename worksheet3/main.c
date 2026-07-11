#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

GPIO_InitTypeDef GPIO_InitStructure;

#define DEBOUNCE_LIMIT 5000

typedef enum
{
    BUTTON_RELEASED,
    BUTTON_MAYBE_PRESSED,
    BUTTON_PRESSED,
    BUTTON_MAYBE_RELEASED
} ButtonState;

static void delay(volatile uint32_t count)
{
    while (count--) {
    }
}

int main(void)
{
    ButtonState state = BUTTON_RELEASED;
    uint32_t confidence = 0;
    uint8_t rawButton = 0;
    uint8_t showGreen = 1;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    /* Green PC6 and Yellow PC7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* WKUP button PA0 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);

    while (1)
    {
        rawButton = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

        switch (state)
        {
            case BUTTON_RELEASED:
                if (rawButton == 1) {
                    confidence = 0;
                    state = BUTTON_MAYBE_PRESSED;
                }
                break;

            case BUTTON_MAYBE_PRESSED:
                if (rawButton == 1) {
                    confidence++;
                    if (confidence > DEBOUNCE_LIMIT) {
                        showGreen = 1 - showGreen;

                        GPIO_WriteBit(GPIOC, GPIO_Pin_6, showGreen ? Bit_SET : Bit_RESET);
                        GPIO_WriteBit(GPIOC, GPIO_Pin_7, showGreen ? Bit_RESET : Bit_SET);

                        state = BUTTON_PRESSED;
                    }
                } else {
                    state = BUTTON_RELEASED;
                }
                break;

            case BUTTON_PRESSED:
                if (rawButton == 0) {
                    confidence = 0;
                    state = BUTTON_MAYBE_RELEASED;
                }
                break;

            case BUTTON_MAYBE_RELEASED:
                if (rawButton == 0) {
                    confidence++;
                    if (confidence > DEBOUNCE_LIMIT) {
                        state = BUTTON_RELEASED;
                    }
                } else {
                    state = BUTTON_PRESSED;
                }
                break;
        }

        delay(100);
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while (1) {
    }
}
#endif
