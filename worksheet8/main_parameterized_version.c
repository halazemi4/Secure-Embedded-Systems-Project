/* Worksheet 8 - Credit Exercise
 * One parameterized FreeRTOS LED task function
 */

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <partest.h>
#include <task.h>

#define mainGREEN_LED_PRIORITY   (tskIDLE_PRIORITY + 1)
#define mainYELLOW_LED_PRIORITY  (tskIDLE_PRIORITY + 1)

typedef struct
{
    unsigned int led_number;
    unsigned int delay_ms;
} LEDTaskParameters;

LEDTaskParameters green_led_parameters = {0, 1000};
LEDTaskParameters yellow_led_parameters = {1, 500};

void vLEDTask(void *pvParameters)
{
    LEDTaskParameters *params;

    params = (LEDTaskParameters *) pvParameters;

    for (;;)
    {
        vParTestToggleLED(params->led_number);
        vTaskDelay(params->delay_ms / portTICK_PERIOD_MS);
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

    xTaskCreate(
        vLEDTask,
        "GreenLED",
        128,
        &green_led_parameters,
        mainGREEN_LED_PRIORITY,
        NULL
    );

    xTaskCreate(
        vLEDTask,
        "YellowLED",
        128,
        &yellow_led_parameters,
        mainYELLOW_LED_PRIORITY,
        NULL
    );

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
