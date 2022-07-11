/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "../EnvCore/i2cdriver/i2c1_drv.h"
#include "../EnvCore/bmp280/bmp280.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define THREAD_STACK_SIZE 1024

static uint8_t    env_thread_stack[THREAD_STACK_SIZE];
static TX_THREAD  env_thread_ptr;

static uint32_t ret_val;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */


VOID env_thread_entry(ULONG initial_input);

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */

  //Initiate thread
  ret_val = tx_thread_create(&env_thread_ptr, "env_thread",
		  env_thread_entry, 0x1234,
		  env_thread_stack, THREAD_STACK_SIZE,
		  15,15,1,
		  TX_AUTO_START);



  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

VOID env_thread_entry(ULONG initial_input)
{
	char   uart_buf[30];
	float  temperature = 0.0f;
	float  pressure = 0.0f;

	serial_print("Env thread: running\n\r");




	//bmp280 channel 1
	BMP_Module bmp_c1;



	bmp_init_default_conf(&bmp_c1);
	bmp_init(&bmp_c1, BMP_I2C_ADDR);



	bmp_read_temp_and_press(&bmp_c1, &temperature, &pressure);
	sprintf(uart_buf,"temp: %0.2f \t pres: %0.2f\n\r", temperature, pressure);
	serial_print(uart_buf);


	while(1){

		bmp_read_temp_and_press(&bmp_c1, &temperature, &pressure);
		sprintf(uart_buf,"temp: %0.2f \t pres: %0.2f\n\r", temperature, pressure);
		serial_print(uart_buf);
		tx_thread_sleep(100);

	}

}









/* USER CODE END 1 */
