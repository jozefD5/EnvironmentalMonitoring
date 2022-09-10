/*
 * monitoring.c
 * @brief Monitoring thread, monitors bmp280 temperature and pressure
 */
#include "main.h"
#include "app_threadx.h"
#include "monitoring.h"

#include "../serialcom/serialcom.h"
#include "../i2cdriver/i2c1_drv.h"
#include "../bmp280/bmp280.h"

//Thread parameters
uint8_t    mt_thread_stack[MT_TH_STACK_SIZE];
TX_THREAD  mt_thread_ptr;

//Mutex
TX_MUTEX mutex_ptr;




//Sensor bmp280 channel 1
static BMP_Module bmp_c1;

//Measured temperature
static float temperature_c = 0.0f;

//Measured pressure
static float pressure_hpa = 0.0f;


//Init function, configures bmp280 sensor and required parameter
static void mt_init(void){

	//Create mutex
	tx_mutex_create(&mutex_ptr, "mt_mutex", TX_NO_INHERIT );

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




	while(1){

		//Read temperature and pressure
		tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);

			bmp_read_temp_and_press(&bmp_c1);

			temperature_c = bmp_c1.temp;
			pressure_hpa  = bmp_c1.pres;

		tx_mutex_put(&mutex_ptr);
		tx_thread_sleep(10);
	}
}







/**
 * Read temperature
 */
void mt_debug_read(void){

	char uart_buf[40];

	tx_mutex_get(&mutex_ptr, MT_MUTEX_WAIT);
		sprintf(uart_buf,"Temp:%0.2f\tPres:%0.2f \n\r ", temperature_c, pressure_hpa);
		serial_print(uart_buf);
	tx_mutex_put(&mutex_ptr);
}









