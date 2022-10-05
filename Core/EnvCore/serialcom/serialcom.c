/*
 * serialcom.c
 * @brief Serial communication interface
 */
#include "main.h"
#include <stdbool.h>
#include "app_threadx.h"
#include "serialcom.h"
#include "../EnvCore.h"
#include "../monitoring/monitoring.h"



//Thread parameters
uint8_t sc_thread_stack[SC_TH_STACK_SIZE];
TX_THREAD sc_thread_ptr;



//Mutex
static TX_MUTEX mutex_ptr;
static TX_MUTEX mutex_print_ptr;


//UART rx buffer
static uint8_t  rx_buf[RX_BUFFER_SIZE];

//UART rx counter
static uint8_t  rx_count;

//Receive complete flag
static bool rx_complete;




//Output monitoring thread active status
static void serialhandler_read_status(void){
	char str_buf[20];
	bool status = mt_get_status();

	sprintf(str_buf, "%s: %d\n\r", SC_TX_STATUS, status);
	serial_print(str_buf);
}








//Reset local variables to start-up values
static void reset_data_values(void){

	memset(rx_buf, 0, RX_BUFFER_SIZE);
	rx_count = 0;
	rx_complete = 0;
}


//Initiate serial thread
static void serial_init(void){

	tx_mutex_create(&mutex_ptr,"thread mutex",TX_INHERIT);
	tx_mutex_create(&mutex_print_ptr, "print mutex", TX_INHERIT);

	reset_data_values();
}





/**
 * @brief Main thread function
 */
void sc_thread(ULONG initial_input)
{
	(void)initial_input;

	//serial received command
	char command_str[RX_BUFFER_SIZE];


	//Init
	serial_init();
	tx_thread_sleep(10);

	serialhandler_read_status();



	while(1){

		tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);

			//Check of received is completed
			if(rx_complete == true){

				//assign received string to local variable in char format
				memcpy(command_str, ((char *)(rx_buf)), sizeof(rx_buf));


				//Compare buffer string to available commands
				if(strcmp(command_str, SC_RX_ACTIVATE) == 0){
					mt_settings(true);

				}else if(strcmp(command_str, SC_RX_DEACTIVATE) == 0){
					mt_settings(false);

				}else if(strcmp(command_str, SC_RX_STATUS) == 0){
					serialhandler_read_status();



				//default, do nothing
				}else{}




				//Reset
				reset_data_values();
				rx_complete = false;
			}

		tx_mutex_put(&mutex_ptr);
		tx_thread_sleep(1);
	}
}






/**
 * @brief Serial print via UART1, print char string
 * @param[in] *str  pointer to char array
 */
void serial_print(char *str)
{
	tx_mutex_get(&mutex_print_ptr, MT_MUTEX_WAIT);

	HAL_UART_Transmit(UART_DRV, (uint8_t*)str, strlen(str), 50);
	while(  HAL_UART_GetState(UART_DRV) != HAL_UART_STATE_READY   ){}

	tx_mutex_put(&mutex_print_ptr);
}



/**
 * @brief Serial rx interrupt handler for UART 1
 */
void serial_IRQHandler(void){

	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);

		//Only read if rx buffer is ready to read
		if(USART1->ISR & USART_ISR_RXNE){

			rx_buf[rx_count] = USART1->RDR & 0xFF;

			//check if buffer is full or escape character was received
			if( rx_count == RX_SIZE_LIMIT || ((char)rx_buf[rx_count]) == '\t'){
				rx_complete = true;
			}

			rx_count++;
		}
	tx_mutex_put(&mutex_ptr);
}














