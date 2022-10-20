/*
 * monitoring.c
 * @brief Monitoring thread, monitors bmp280 temperature and pressure
 */
#include "app_threadx.h"
#include <stdbool.h>


#ifndef ENVCORE_MONITORING_MONITORING_H_
#define ENVCORE_MONITORING_MONITORING_H_

//Stack size
#define    MT_TH_STACK_SIZE   1024


//Default cycle limit/trigger value. Sets interval for reporting (sending) data. RTOS will produce 100 timer ticks / second (1000ms).
//Therefore 1000ms/100 = 10ms delay per tick. This means that 50 ticks will equals 500ms delay
#define    MT_REPORT_INTERVAL     50


//Stack array
extern uint8_t    mt_thread_stack[MT_TH_STACK_SIZE];

//Thread pointer
extern TX_THREAD  mt_thread_ptr;






/**
 * @brief Monitoring thread settings struct
 */
typedef struct {

	bool has_active;
	bool active;

	bool has_interval_tick;
	int32_t interval_tick;

	bool has_active_temp;
	bool active_temp;

	bool has_active_pres;
	bool active_pres;

}MonSettings;







void mt_thread(ULONG initial_input);
void mt_debug_read(void);
void mt_settings(MonSettings *s);
bool mt_get_status(void);


#endif /* ENVCORE_MONITORING_MONITORING_H_ */
