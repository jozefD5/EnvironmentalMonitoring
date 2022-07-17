/*
 * i2c1_drv.c
 * @brief i2c driver for i2c bus 2, provides abstraction for read and write operations
 */
#include "main.h"
#include "i2c1_drv.h"
#include "../serialcom/serialcom.h"


static char uart_buf[25];

/**
 * @brief   I2C-1 transmit and receive function, blocking function
 * @param   addr       address of target i2c device, might need to be left shifted 1-bit
 * @param   *txbuf     buffer that holds transmit data
 * @param   txlength   number of bytes to be transmitted
 * @param   *rxbuf     retrieve buffer, to hold retrieve data
 * @param   rxlength   number of bytes to be retrieve, set to zero for read only!
 * @retval  hal i2c state
 */
HAL_StatusTypeDef i2c1_trx(uint8_t addr,
							  uint8_t *txbuf,
							  uint8_t txlength,
							  uint8_t *rxbuf,
							  uint8_t rxlength){


	HAL_StatusTypeDef ref = HAL_OK;



	//Transmit only
	if(rxlength < 1){

		ref = HAL_I2C_Master_Transmit(&hi2c1, addr, txbuf, txlength, I2C_TIMEOUT);

		//sprintf(uart_buf, "ref: %d\n\r", ref);
		//serial_print(uart_buf);
		if(ref != HAL_OK) {return ref;  }


	}else{
	//Transmit and receive

		ref = HAL_I2C_Master_Transmit(&hi2c1, addr, txbuf, txlength, I2C_TIMEOUT);

		//sprintf(uart_buf, "ref: %d\n\r", ref);
		//serial_print(uart_buf);
		if(ref != HAL_OK) {return ref;  }

		ref = HAL_I2C_Master_Receive(&hi2c1, addr, rxbuf, rxlength, I2C_TIMEOUT);
		//sprintf(uart_buf, "ref: %d\n\r", ref);
		//serial_print(uart_buf);

	}


	return ref;
}




/**
 * @brief I2C_1 buss scanner
 */
void i2c1_scan(void){


	HAL_StatusTypeDef ref = HAL_OK;
	uint8_t addr = 0;
	uint8_t txbuf = 1;
	uint8_t rxbuf = 0;


	for(uint8_t i = 0; i < 255; i++){

		//addr = (i >> 1);
		addr = i;

		ref = i2c1_trx(addr, &txbuf, 1, &rxbuf, 0);

		if(ref == HAL_OK){
			sprintf(uart_buf, "addr: %d \t\t OK\n\r", addr );
		}else{
			sprintf(uart_buf, "addr: %d\n\r", addr);
		}

		serial_print(uart_buf);

	}
}


