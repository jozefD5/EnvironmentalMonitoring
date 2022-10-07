/*
 * monitoring.c
 * Author:
 * @brief Monitoring thread, monitors bmp280 temperature and pressure
 */
#include "main.h"
#include <stdbool.h>
#include "app_threadx.h"
#include "monitoring.h"

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

//Cycle count trigger value
static uint32_t report_ticks;

//Measured temperature
static float temperature_c = 0.0f;

//Measured pressure
static float pressure_hpa = 0.0f;

//Activate/deactivate monitoring
static bool env_active;





//Output most recent data
static void mt_output_data(void){

	sprintf(uart_buf, "%s: %0.2f\n\r",SC_TX_TEMP_DATA, temperature_c);
	serial_print(uart_buf);

	sprintf(uart_buf, "%s: %0.2f\n\r",SC_TX_PRES_DATA, pressure_hpa);
	serial_print(uart_buf);
}


//Reset all internal data to power-up/reset state
static void data_reset(void){
		cycle_count = 0;
		temperature_c = 0.0f;
		pressure_hpa = 0.0f;
}


//Read and average recorded data
static void record_data(void){

	//Read sensor data
	bmp_read_temp_and_press(&bmp_c1);

	//Add new data
	temperature_c += bmp_c1.temp;;
	pressure_hpa += bmp_c1.pres;;
}


//Report recorded data
static void report_data(void){

	//Calculate average values
	temperature_c /= (float)cycle_count;
	pressure_hpa /= (float)cycle_count;

	//Send data
	mt_output_data();
	data_reset();
}




//Init function, configures bmp280 sensor and required parameter
static void mt_init(void){

	//Create mutex
	tx_mutex_create(&mutex_ptr, "mt_mutex", TX_NO_INHERIT );

	//deactivate monitoring
	env_active = false;

	//Set default report interval
	report_ticks = MT_REPORT_INTERVAL;

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
		if(env_active){

			cycle_count++;

			record_data();

			if(cycle_count >= report_ticks){
				report_data();
			}

			HAL_GPIO_TogglePin(hb_led_GPIO_Port, hb_led_Pin);
		}
		tx_mutex_put(&mutex_ptr);
		tx_thread_sleep(1);
	}
}







//Internal settings control
static void mt_settings_i(bool acc){
	env_active = acc;
}



/**
 * Apply new settings to monitoring thread
 */
void mt_settings(bool acc){
	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		mt_settings_i(acc);
	tx_mutex_put(&mutex_ptr);
}





/**
 *
 */
bool mt_get_status(void){

	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		bool status = env_active;
	tx_mutex_put(&mutex_ptr);

	return status;
}



















