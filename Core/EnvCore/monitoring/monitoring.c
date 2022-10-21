/*
 * monitoring.c
 * Author:
 * @brief Monitoring thread, monitors bmp280 temperature and pressure
 */
#include "main.h"
#include <stdbool.h>
#include "app_threadx.h"
#include "monitoring.h"
#include "../serialcom/serialcommands.h"
#include "../serialcom/serialcom.h"
#include "../i2cdriver/i2c1_drv.h"
#include "../bmp280/bmp280.h"

//Thread parameters
uint8_t    mt_thread_stack[MT_TH_STACK_SIZE];
TX_THREAD  mt_thread_ptr;

//Mutex
static TX_MUTEX mutex_ptr;


//Sensor bmp280 channel 1
static BMP_Module bmp_c1;

//UART tx buff
static char uart_buf[25];

//Reading cycle counter
static uint32_t cycle_count;

//Measured temperature
static float temperature_c = 0.0f;

//Measured pressure
static float pressure_hpa = 0.0f;

//Monitoring settings
static MonSettings monitorringSettings;








//Reset settings
static void settings_reset(void){
	monitorringSettings.has_active = false;
	monitorringSettings.active = false;

	monitorringSettings.has_interval_tick = false;
	monitorringSettings.interval_tick = MT_REPORT_INTERVAL_NORMAL;

	monitorringSettings.has_active_temp = false;
	monitorringSettings.active_temp = false;

	monitorringSettings.has_active_pres = false;
	monitorringSettings.active_pres = false;
}


//Reset all internal data to power-up/reset state
static void data_reset(void){
		cycle_count = 0;
		temperature_c = 0.0f;
		pressure_hpa = 0.0f;
}


//Read and average recorded data
static void record_data(void){
	bmp_read_temp_and_press(&bmp_c1);
	temperature_c += bmp_c1.temp;;
	pressure_hpa += bmp_c1.pres;;
}


//Report recorded data
static void report_data(void){

	//Calculate average values
	temperature_c /= (float)cycle_count;
	pressure_hpa /= (float)cycle_count;

	//Only output required data
	if(monitorringSettings.active_temp){
		sprintf(uart_buf, "%s:%0.2f\n\r",ENV_SC_TEMP_DATA, temperature_c);
		serial_print(uart_buf);
	}
	if(monitorringSettings.active_pres){
		sprintf(uart_buf, "%s:%0.2f\n\r",ENV_SC_PRES_DATA, pressure_hpa);
		serial_print(uart_buf);
	}
}





//Init function, configures bmp280 sensor and required parameter
static void mt_init(void){

	//Create mutex
	tx_mutex_create(&mutex_ptr, "mt_mutex", TX_NO_INHERIT );

	//Reset settings
	settings_reset();

	//Reset data
	data_reset();

	//Setup bmp280 sensor
	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		bmp_init_default_conf(&bmp_c1);
		bmp_init(&bmp_c1, BMP_I2C_ADDR);
	tx_mutex_put(&mutex_ptr);
}






/**
 * @brief Main thread function
 */
void mt_thread(ULONG initial_input){

	(void)initial_input;

	//Init
	mt_init();
	tx_thread_sleep(20);



	while(1){
		tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);

		//Only run when monitoring is activated
		if(monitorringSettings.active){

			cycle_count++;
			record_data();

			if(cycle_count >= monitorringSettings.interval_tick){
				report_data();
				data_reset();
				HAL_GPIO_TogglePin(hb_led_GPIO_Port, hb_led_Pin);
			}
		}

		tx_mutex_put(&mutex_ptr);
		tx_thread_sleep(1);
	}
}







//Internal settings control
static void mt_settings_i(MonSettings *s){

	//activate/deactivate monitoring
	if(s->has_active){
		monitorringSettings.active = s->active;

	}
	//set interval in ticks
	if(s->has_interval_tick){
		monitorringSettings.interval_tick = s->interval_tick;

	}
	//activate/deactivate temperature monitoring
	if(s->has_active_temp){
		monitorringSettings.active_temp = s->active_temp;

	}
	//activate/deactivate pressure monitoring
	if(s->has_active_pres){
		monitorringSettings.active_pres = s->active_pres;

	}
}



/**
 * @brief Apply new settings to monitoring thread
 */
void mt_settings(MonSettings *s){
	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		mt_settings_i(s);
	tx_mutex_put(&mutex_ptr);
}



/**
 *@ brief Output current monitoring status
 */
bool mt_get_status(void){
	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		bool status = monitorringSettings.active;
	tx_mutex_put(&mutex_ptr);
	return status;
}



















