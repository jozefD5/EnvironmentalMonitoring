/*
 * serialcom.c
 * @brief Serial communication interface
 */
#include "main.h"


//Local  UART handler pointer
static UART_HandleTypeDef *uart_drv;



/**
 * @brief     Initialise serial handler for serial communication
 * @param[in] u   pointer to USART driver
 */
void seria_init(UART_HandleTypeDef *u){
	uart_drv = u;
}



/**
 * @brief Serial print via UART1
 * @param[in] *str  pointer to char array
 */
void serial_print(char *str)
{
	HAL_UART_Transmit(uart_drv, (uint8_t *)str, strlen(str), 10);
	//HAL_UART_Transmit_DMA(uart_drv, (uint8_t*)str, strlen(str));
}


