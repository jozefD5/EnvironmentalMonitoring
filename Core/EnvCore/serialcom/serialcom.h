/*
 * serialcom.h
 * @brief Serial communication interface
 */
#include "app_threadx.h"



#ifndef ENVCORE_SERIALCOM_SERIALCOM_H_
#define ENVCORE_SERIALCOM_SERIALCOM_H_


//Stack size
#define    SC_TH_STACK_SIZE     1024


//UART buffer size
#define    RX_BUFFER_SIZE          15
#define    RX_SIZE_LIMIT           (RX_BUFFER_SIZE - 3)




//Stack array
extern uint8_t sc_thread_stack[SC_TH_STACK_SIZE];

//Thread pointer
extern TX_THREAD sc_thread_ptr;



//Serial commands, .Net System.IO.Ports  adds '\n' at the end of every message. System is using '\n\t' to
//indicate end of the message
#define    SM_ACTIVATE      "scactive\t\n"




void sc_thread(ULONG initial_input);
void serial_print(char *str);
void serial_IRQHandler(void);


#endif /* ENVCORE_SERIALCOM_SERIALCOM_H_ */
