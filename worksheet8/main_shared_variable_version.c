/* Worksheet 8 - FreeRTOS button tasks with shared variables */

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <partest.h>
#include <task.h>

#define mainGREEN_LED_TOGGLE   (tskIDLE_PRIORITY + 1)
#define mainYELLOW_LED_TOGGLE  (tskIDLE_PRIORITY + 1)
#define mainBUTTON_TASK        (tskIDLE_PRIORITY + 1)

volatile int green_led_enabled = 1;
volatile int yellow_led_enabled = 1;

void configure_buttons(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOA and GPIOC clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    /* WKUP button on PA0 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* TAMPER button on PC13 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void vGreenToggleTask(void *pvParameters)
{
    for (;;)
    {
        if (green_led_enabled)
        {
            vParTestToggleLED(0);
        }
        else
        {
            vParTestSetLED(0, pdFALSE);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vYellowToggleTask(void *pvParameters)
{
    for (;;)
    {
        if (yellow_led_enabled)
        {
            vParTestToggleLED(1);
        }
        else
        {
            vParTestSetLED(1, pdFALSE);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void vWKUPButtonTask(void *pvParameters)
{
    int previous_state = 0;
    int current_state;

    for (;;)
    {
        current_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

        /*
         * WKUP is active high.
         * Toggle green LED enable only once per press.
         */
        if (current_state == 1 && previous_state == 0)
        {
            green_led_enabled = 1 - green_led_enabled;
        }

        previous_state = current_state;

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void vTAMPERButtonTask(void *pvParameters)
{
    int previous_state = 1;
    int current_state;

    for (;;)
    {
        current_state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

        /*
         * TAMPER is commonly active low.
         * Toggle yellow LED enable only once per press.
         */
        if (current_state == 0 && previous_state == 1)
        {
            yellow_led_enabled = 1 - yellow_led_enabled;
        }

        previous_state = current_state;

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pxTask;
    (void) pcTaskName;

    for (;;)
    {
    }
}

int main(void)
{
    vParTestInitialise();
    configure_buttons();

    xTaskCreate(vGreenToggleTask, "GreenLed", 128, NULL, mainGREEN_LED_TOGGLE, NULL);
    xTaskCreate(vYellowToggleTask, "YellowLed", 128, NULL, mainYELLOW_LED_TOGGLE, NULL);
    xTaskCreate(vWKUPButtonTask, "WKUPButton", 128, NULL, mainBUTTON_TASK, NULL);
    xTaskCreate(vTAMPERButtonTask, "TAMPERButton", 128, NULL, mainBUTTON_TASK, NULL);

    vTaskStartScheduler();

    for (;;)
    {
    }

    return 0;
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}

#endif
