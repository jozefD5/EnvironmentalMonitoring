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
#include "../EnvCore/serialcom/serialcom.h"
#include "../EnvCore/monitoring/monitoring.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static uint32_t ret_val;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

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




  //Monitoring thread
  ret_val = tx_thread_create(
		  &mt_thread_ptr,            //pointer to thread structure
		  "mt_thread",               //thread name
		  mt_thread,                 //main thread function
		  0x1234,                    //thread input value, not used
		  mt_thread_stack,           //stack array
		  MT_TH_STACK_SIZE,          //stack array size
		  15,15,                     //priority, preemption threshold,
		  1,                         //thread slice count, active for 1=systick
		  TX_AUTO_START);            //start thread as ready


  //Serial communication thread
    ret_val = tx_thread_create(
  		  &sc_thread_ptr,            //pointer to thread structure
  		  "sc_thread",               //thread name
  		  sc_thread,                 //main thread function
  		  0x1234,                    //thread input value, not used
  		  sc_thread_stack,           //stack array
  		  SC_TH_STACK_SIZE,          //stack array size
  		  15,15,                     //priority, preemption threshold,
  		  1,                         //thread slice count, active for 1=systick
  		  TX_AUTO_START);            //start thread as ready





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


/* USER CODE END 1 */
