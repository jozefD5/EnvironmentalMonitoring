/*
 * serialcom.c
 * @brief Serial communication interface
 */
#include <stdbool.h>
#include "main.h"
#include "app_threadx.h"
#include "serialcom.h"
#include "../monitoring/monitoring.h"


//Thread parameters
uint8_t sc_thread_stack[SC_TH_STACK_SIZE];
TX_THREAD sc_thread_ptr;

//Local  UART handler pointer
UART_HandleTypeDef   *uart_drv;



//TODO, add serial read via DMA




/**
 * @brief     Initialise serial handler for serial communication
 * @param[in] u   pointer to USART driver
 */
void serial_init(UART_HandleTypeDef *uart){
	//uart_dma_drv =udma;
	uart_drv = uart;
}





/**
 * @brief Main thread function
 */
void sc_thread(ULONG initial_input){

	(void)initial_input;


	while(1){

		//mt_debug_read();

		tx_thread_sleep(50);
	}

}






/**
 * @brief Serial print via UART1, print char string
 * @param[in] *str  pointer to char array
 */
void serial_print(char *str)
{

	while(  HAL_UART_GetState(uart_drv) != HAL_UART_STATE_READY   ){}

	while( HAL_UART_Transmit_DMA(uart_drv, (uint8_t*)str, strlen(str))  != HAL_OK);

	uart_drv->gState = HAL_UART_STATE_READY;

}






