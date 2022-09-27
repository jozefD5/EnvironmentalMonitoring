/*
 * serialcom.c
 * @brief Serial communication interface
 */
#include <stdbool.h>
#include "main.h"
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

//Receive flag
static bool rx_ir = false;




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


	//Init
	serial_init();

	tx_thread_sleep(50);
	int strLength = strlen(SM_ACTIVATE);

	serial_print("\nCommand: ");
	serial_print(SM_ACTIVATE);

	uint8_t  strTest[10];
	sprintf((char *)strTest, "C: %d", strLength);
	serial_print((char *)strTest);



	while(1){

		//serial_print("TH: S\n\r");



		tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);


		if(rx_ir){
			serial_print("\n\r");
		    serial_print( ((char*)(rx_buf)));
			rx_ir = false;
		}




		if(rx_complete == true){

			//Compare buffer string to available commands
			if( strcmp( ((char *)(rx_buf)), SM_ACTIVATE) == 0 ){
				HAL_GPIO_TogglePin(hb_led_GPIO_Port, hb_led_Pin);
			}


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

	while(  HAL_UART_GetState(UART_DRV) != HAL_UART_STATE_READY   ){}
	HAL_UART_Transmit(UART_DRV, (uint8_t*)str, strlen(str), 50);

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

		rx_ir = true;
		rx_count++;
	}







/*
	if(USART1->ISR & USART_ISR_IDLE){

		HAL_GPIO_TogglePin(hb_led_GPIO_Port, hb_led_Pin);
		//rx_buf[rx_count] = USART1->RDR & 0xFF;

		__HAL_UART_CLEAR_IDLEFLAG(UART_DRV);
		//serial_print("rx \n\r");
	}
*/

	tx_mutex_put(&mutex_ptr);
}














