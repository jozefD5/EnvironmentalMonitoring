/*
 * serialcom.h
 * @brief Serial communication interface
 */
#include "app_threadx.h"



#ifndef ENVCORE_SERIALCOM_SERIALCOM_H_
#define ENVCORE_SERIALCOM_SERIALCOM_H_




//Stack size
#define    SC_TH_STACK_SIZE   1024

//Stack array
extern uint8_t sc_thread_stack[SC_TH_STACK_SIZE];

//Thread pointer
extern TX_THREAD sc_thread_ptr;





void serial_init(UART_HandleTypeDef *uart);
void sc_thread(ULONG initial_input);
void serial_print(char *str);

#endif /* ENVCORE_SERIALCOM_SERIALCOM_H_ */
