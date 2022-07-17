/*
 * monitoring.h
 *
 *  Created on: Jul 17, 2022
 *      Author: jozef.duda
 */

#ifndef ENVCORE_MONITORING_MONITORING_H_
#define ENVCORE_MONITORING_MONITORING_H_

//Stack size
#define    MT_TH_STACK_SIZE   1024


//Stack array
extern uint8_t    mt_thread_stack[MT_TH_STACK_SIZE];

//Thread pointer
extern TX_THREAD  mt_thread_ptr;




void mt_thread(ULONG initial_input);
void mt_debug_read(void);


#endif /* ENVCORE_MONITORING_MONITORING_H_ */
