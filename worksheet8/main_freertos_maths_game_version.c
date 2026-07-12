/* Worksheet 8 - Credit Exercise
 * FreeRTOS maths game task with clock/countdown task
 */

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

#define mainCOM_PORT_BAUD_RATE 9600

#define MATH_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define CLOCK_TASK_PRIORITY  (tskIDLE_PRIORITY + 1)

volatile int game_time_left = 30;
volatile int game_running = 0;

void COMPortInit(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

    /* USART2 TX: PD5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* USART2 RX: PD6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = mainCOM_PORT_BAUD_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}

void outbyte(int c)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART2, (uint16_t)c);
}

int input_available(void)
{
    return USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET;
}

int inbyte(void)
{
    return (int)(USART_ReceiveData(USART2) & 0xff);
}

void send_string(const char *s)
{
    taskENTER_CRITICAL();

    while (*s != '\0')
    {
        outbyte(*s);
        s++;
    }

    taskEXIT_CRITICAL();
}

void send_int(int value)
{
    char buffer[12];
    int i = 0;
    int j;
    int negative = 0;

    if (value == 0)
    {
        outbyte('0');
        return;
    }

    if (value < 0)
    {
        negative = 1;
        value = -value;
    }

    while (value > 0)
    {
        buffer[i] = '0' + (value % 10);
        value = value / 10;
        i++;
    }

    if (negative)
    {
        outbyte('-');
    }

    for (j = i - 1; j >= 0; j--)
    {
        outbyte(buffer[j]);
    }
}

void send_int_safe(int value)
{
    taskENTER_CRITICAL();
    send_int(value);
    taskEXIT_CRITICAL();
}

int read_number_with_timeout(int *result)
{
    int value = 0;
    int got_digit = 0;
    int c;
    TickType_t start_time;

    start_time = xTaskGetTickCount();

    while ((xTaskGetTickCount() - start_time) < (game_time_left * 1000 / portTICK_PERIOD_MS))
    {
        if (game_time_left <= 0)
        {
            return 0;
        }

        if (input_available())
        {
            c = inbyte();

            if (c >= '0' && c <= '9')
            {
                got_digit = 1;
                value = (value * 10) + (c - '0');
                outbyte(c);
            }
            else if (c == '\r' || c == '\n')
            {
                send_string("\r\n");

                if (got_digit)
                {
                    *result = value;
                    return 1;
                }
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    return 0;
}

void vClockTask(void *pvParameters)
{
    (void) pvParameters;

    for (;;)
    {
        if (game_running && game_time_left > 0)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            game_time_left--;

            send_string("\r\n[Time left: ");
            send_int_safe(game_time_left);
            send_string(" seconds]\r\n");
        }
        else
        {
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}

void vMathTask(void *pvParameters)
{
    int question;
    int a;
    int b;
    int correct;
    int answer;
    int score = 0;

    (void) pvParameters;

    send_string("\r\nWorksheet 8 FreeRTOS Maths Game\r\n");
    send_string("You have 30 seconds to answer 5 questions.\r\n");
    send_string("Enter numbers only and press Enter.\r\n\r\n");

    game_time_left = 30;
    game_running = 1;

    for (question = 1; question <= 5; question++)
    {
        if (game_time_left <= 0)
        {
            break;
        }

        a = (question * 3) + 2;
        b = (question * 2) + 1;
        correct = a + b;

        send_string("Question ");
        send_int_safe(question);
        send_string(": ");
        send_int_safe(a);
        send_string(" + ");
        send_int_safe(b);
        send_string(" = ");

        if (read_number_with_timeout(&answer))
        {
            if (answer == correct)
            {
                send_string("Correct!\r\n\r\n");
                score++;
            }
            else
            {
                send_string("Wrong. Correct answer was ");
                send_int_safe(correct);
                send_string("\r\n\r\n");
            }
        }
        else
        {
            send_string("\r\nTime expired.\r\n");
            break;
        }
    }

    game_running = 0;

    send_string("\r\nGame finished.\r\n");
    send_string("Final score: ");
    send_int_safe(score);
    send_string(" out of 5\r\n");

    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
    COMPortInit();

    xTaskCreate(vMathTask, "MathGame", 160, NULL, MATH_TASK_PRIORITY, NULL);
    xTaskCreate(vClockTask, "Clock", 160, NULL, CLOCK_TASK_PRIORITY, NULL);

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
