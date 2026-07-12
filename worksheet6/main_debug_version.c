/*
	Copyright 2012, Michael Scott

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*changed USART3 to USART2 for Rev B boards - craig 18/2/14 */
#include "com_port.h"

#include <stdio.h>
#include <stdlib.h>

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>


extern char _sidata;
extern char _sdata;
extern char _edata;
extern char _sbss;
extern char _ebss;
extern char _estack;

static inline unsigned int get_msp_main(void)
{
    unsigned int result;
    __asm volatile ("MRS %0, msp\n" : "=r" (result));
    return result;
}

int outbyte(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART2, (u16) c);

	return c;
}

int inbyte(void)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {
    }

    return (int)(USART_ReceiveData(USART2) & 0xFF);
}


void COMPortInit ( void ) {

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE ); 

	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );

	/* Remap USART, as USART2 uses alternate pins */
	GPIO_PinRemapConfig( GPIO_Remap_USART2,ENABLE );

	/* Configure USART2 Tx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   /*changed from 8 to 5 for USART craig 2*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART2 Rx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* changed from 9 to 6 for USART 2 craig */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART 8N1 */
	USART_InitStructure.USART_BaudRate = mainCOM_PORT_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init( USART2, &USART_InitStructure );

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );
}


void print_memory_debug(const char *stage)
{
    int local_variable;
    void *heap_before;
    void *heap_after;

    printf("\r\n--- Memory debug: %s ---\r\n", stage);

    printf("_sidata  Flash data source : 0x%08x\r\n", (unsigned int)&_sidata);
    printf("_sdata   Data start RAM    : 0x%08x\r\n", (unsigned int)&_sdata);
    printf("_edata   Data end RAM      : 0x%08x\r\n", (unsigned int)&_edata);
    printf("_sbss    BSS start RAM     : 0x%08x\r\n", (unsigned int)&_sbss);
    printf("_ebss    BSS end/heap start: 0x%08x\r\n", (unsigned int)&_ebss);
    printf("_estack  Initial stack top : 0x%08x\r\n", (unsigned int)&_estack);

    printf("Current MSP stack pointer  : 0x%08x\r\n", get_msp_main());
    printf("Local variable address     : 0x%08x\r\n", (unsigned int)&local_variable);

    heap_before = malloc(16);
    heap_after = malloc(16);

    printf("malloc 16 block 1          : 0x%08x\r\n", (unsigned int)heap_before);
    printf("malloc 16 block 2          : 0x%08x\r\n", (unsigned int)heap_after);
}



int main(void)
{
    COMPortInit();

    printf("\r\nWorksheet 6 Memory Debug Test\r\n");

    print_memory_debug("program start");

    printf("\r\nAllocating more heap memory...\r\n");

    print_memory_debug("after more malloc calls");

    printf("\r\nMemory debug complete.\r\n");

    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
