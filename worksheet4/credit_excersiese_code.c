#include "com_port.h"

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

int __io_putchar(int c)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART2, (uint16_t)c);
    return c;
}

void send_string(const char *s)
{
    while (*s)
    {
        __io_putchar(*s++);
    }
}

void report_usart_error(uint16_t status)
{
    if (status & USART_FLAG_ORE)
    {
        send_string("\r\n[ERROR: Overrun error - ORE]\r\n> ");
    }

    if (status & USART_FLAG_NE)
    {
        send_string("\r\n[ERROR: Noise error - NE]\r\n> ");
    }

    if (status & USART_FLAG_FE)
    {
        send_string("\r\n[ERROR: Framing error - FE]\r\n> ");
    }

    if (status & USART_FLAG_PE)
    {
        send_string("\r\n[ERROR: Parity error - PE]\r\n> ");
    }
}

int __io_getchar(void)
{
    uint16_t status;
    uint16_t data;

    while (1)
    {
        status = USART2->SR;

        if (status & (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
        {
            /*
             * On STM32 USART, error flags are cleared by reading SR first,
             * then reading DR.
             */
            data = USART2->DR;
            report_usart_error(status);
            (void)data;
            continue;
        }

        if (status & USART_FLAG_RXNE)
        {
            data = USART_ReceiveData(USART2);
            return (int)(data & 0xFF);
        }
    }
}

void COMPortInit(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

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

int main(void)
{
    int c;

    COMPortInit();

    send_string("\r\nWorksheet 4 USART Echo Test\r\n");
    send_string("Type characters and they will be echoed back.\r\n");
    send_string("Receive error handling enabled: ORE, NE, FE, PE.\r\n");
    send_string("> ");

    while (1)
    {
        c = __io_getchar();

        __io_putchar(c);

        if (c == '\r')
        {
            send_string("\n> ");
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
