#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <misc.h>

#define mainCOM_PORT_BAUD_RATE 9600
#define BUFFER_SIZE 20

volatile char rx_buffer[BUFFER_SIZE];
volatile int rx_head = 0;
volatile int rx_tail = 0;
volatile int rx_count = 0;

volatile char tx_buffer[BUFFER_SIZE];
volatile int tx_head = 0;
volatile int tx_tail = 0;
volatile int tx_count = 0;

volatile int rx_overflow = 0;

static void lock_interrupts(void)
{
    __asm volatile ("cpsid i");
}

static void unlock_interrupts(void)
{
    __asm volatile ("cpsie i");
}

void COMPortInit(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

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

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_EnableIRQ(USART2_IRQn);

    USART_Cmd(USART2, ENABLE);
}

int outbyte(int c)
{
    int stored = 0;

    while (!stored)
    {
        lock_interrupts();

        if (tx_count < BUFFER_SIZE)
        {
            tx_buffer[tx_head] = (char)c;
            tx_head = (tx_head + 1) % BUFFER_SIZE;
            tx_count++;

            USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

            stored = 1;
        }

        unlock_interrupts();
    }

    return c;
}

int inbyte(void)
{
    int c = -1;

    lock_interrupts();

    if (rx_count > 0)
    {
        c = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) % BUFFER_SIZE;
        rx_count--;
    }

    unlock_interrupts();

    return c;
}

void send_string(const char *s)
{
    while (*s != '\0')
    {
        outbyte(*s);
        s++;
    }
}

void USART2_IRQHandler(void)
{
    int data;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        data = (int)(USART_ReceiveData(USART2) & 0xff);

        if (rx_count < BUFFER_SIZE)
        {
            rx_buffer[rx_head] = (char)data;
            rx_head = (rx_head + 1) % BUFFER_SIZE;
            rx_count++;
        }
        else
        {
            rx_overflow = 1;
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        if (tx_count > 0)
        {
            USART_SendData(USART2, (uint16_t)tx_buffer[tx_tail]);
            tx_tail = (tx_tail + 1) % BUFFER_SIZE;
            tx_count--;
        }
        else
        {
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }
}

int main(void)
{
    int c;
    volatile int delay;

    COMPortInit();

    send_string("\r\nWorksheet 7 Full Interrupt IO\r\n");
    send_string("Type characters. Press Enter to start a new line.\r\n");
    send_string("Paste a long sentence quickly to test RX buffer full.\r\n\r\n");

    while (1)
    {
        c = inbyte();

        if (c != -1)
        {
            if (c == '\r' || c == '\n')
            {
                send_string("\r\n[Enter received]\r\n");
            }
            else
            {
                outbyte(c);
            }

            for (delay = 0; delay < 50000; delay++)
            {
            }
        }

        if (rx_overflow)
        {
            rx_overflow = 0;
            send_string("\r\nRX buffer full\r\n");
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
