/*
 * bmp280.c
 * @brief bmp280 driver for communication and configuration via i2c bus
 */
#include "main.h"
#include "bmp280.h"
#include "../i2cdriver/i2c1_drv.h"
#include "../serialcom/serialcom.h"

//i2c buffers
static uint8_t  txbuf[3];
static uint8_t  rxbuf[6];


//uart debug string buffer
static char uart_buf[25];








//Write single byte
static HAL_StatusTypeDef bmp_write_byte(BMP_Module *bmpP, uint8_t reg, uint8_t val){
	txbuf[0] = reg;
	txbuf[1] = val;
	return i2c1_trx(bmpP->i2c_addr, txbuf, 2, rxbuf, 0);
}



//Read single byte
static uint8_t bmp_read_byte(BMP_Module *bmpP, uint8_t reg){
	txbuf[0] = reg;

	if(i2c1_trx(bmpP->i2c_addr, txbuf, 1, rxbuf, 1) == HAL_OK){
		return rxbuf[0];
	}else {
		return 0;
	}
}



//Read register and output unsigned 16bit value
static uint16_t bmp_read_u16(BMP_Module *bmpP, uint8_t reg){

	uint16_t val = 0;
	txbuf[0] = reg;

	if( i2c1_trx(bmpP->i2c_addr, txbuf, 1, rxbuf, 2) != HAL_OK ){ return 0;  }
	val = (rxbuf[1] << 8)  | rxbuf[0];

	return val;
}



//Read register and output signed 16bit value
static int16_t bmp_read_i16(BMP_Module *bmpP, uint8_t reg){

	int16_t val = 0;
	txbuf[0] = reg;

	if( i2c1_trx(bmpP->i2c_addr, txbuf, 1, rxbuf, 2) != HAL_OK ){ return 0;  }
	val = ( ((int16_t)rxbuf[1]) << 8)  |  ((int16_t)rxbuf[0]);

	return val;
}





//Read calibration data
static HAL_StatusTypeDef bmp_read_calibration(BMP_Module *bmpP){

	HAL_StatusTypeDef ref[12];

	//Read unsigned values
	ref[0] = bmpP->dig_T1  =  bmp_read_u16(bmpP, 0x88);
	ref[1] = bmpP->dig_P1  =  bmp_read_u16(bmpP, 0x8E);

	//Read signed values
	ref[2] = bmpP->dig_T2  =  bmp_read_i16(bmpP, 0x8A);
	ref[3] = bmpP->dig_T3  =  bmp_read_i16(bmpP, 0x8C);

	ref[4] = bmpP->dig_P2  =  bmp_read_i16(bmpP, 0x90);
	ref[5] = bmpP->dig_P3  =  bmp_read_i16(bmpP, 0x92);
	ref[6] = bmpP->dig_P4  =  bmp_read_i16(bmpP, 0x94);
	ref[7] = bmpP->dig_P5  =  bmp_read_i16(bmpP, 0x96);
	ref[8] = bmpP->dig_P6  =  bmp_read_i16(bmpP, 0x98);
	ref[9] = bmpP->dig_P7  =  bmp_read_i16(bmpP, 0x9A);
	ref[10] = bmpP->dig_P8  =  bmp_read_i16(bmpP, 0x9C);
	ref[11] = bmpP->dig_P9  =  bmp_read_i16(bmpP, 0x9E);

	//Check for any failed data transmission
	for(uint32_t i = 0; i<12; i++) {
		if (ref[i] != HAL_OK){ return ref[i];  }
	}

	return HAL_OK;
}



//Print calibration data parameters
static void bmp_print_calibration_data(BMP_Module *bmpP){


	sprintf(uart_buf, "T1: %d\n\r", bmpP->dig_T1);
	serial_print(uart_buf);
	sprintf(uart_buf, "T2: %d\n\r", bmpP->dig_T2);
	serial_print(uart_buf);
	sprintf(uart_buf, "T3: %d\n\r", bmpP->dig_T3);
	serial_print(uart_buf);



	sprintf(uart_buf, "P1: %d\n\r", bmpP->dig_P1);
	serial_print(uart_buf);

	sprintf(uart_buf, "P2: %d\n\r", bmpP->dig_P2);
	serial_print(uart_buf);

	sprintf(uart_buf, "P3: %d\n\r", bmpP->dig_P3);
	serial_print(uart_buf);

	sprintf(uart_buf, "P4: %d\n\r", bmpP->dig_P4);
	serial_print(uart_buf);

	sprintf(uart_buf, "P5: %d\n\r", bmpP->dig_P5);
	serial_print(uart_buf);

	sprintf(uart_buf, "P6: %d\n\r", bmpP->dig_P6);
	serial_print(uart_buf);

	sprintf(uart_buf, "P7: %d\n\r", bmpP->dig_P7);
	serial_print(uart_buf);

	sprintf(uart_buf, "P8: %d\n\r", bmpP->dig_P8);
	serial_print(uart_buf);

	sprintf(uart_buf, "P9: %d\n\r", bmpP->dig_P9);
	serial_print(uart_buf);
}



//Calibrate config register
static HAL_StatusTypeDef bmp_configure(BMP_Module *bmpP){

	HAL_StatusTypeDef ref = HAL_OK;
	uint8_t config_val = 0;
	uint8_t ctrl_val = 0;



	//Apply configuration
	config_val  |=  (bmpP->params.standby_time << 5);
	config_val  |=  (bmpP->params.filter << 2);

	ref = bmp_write_byte(bmpP, BMP_REG_CONFIG, config_val);

	if(ref != HAL_OK){return ref; }


	//Set acquasition option
	ctrl_val  |=  (bmpP->params.osp_temperature << 5);
	ctrl_val  |=  (bmpP->params.osp_pressure << 2);
	ctrl_val  |=  (bmpP->params.mode);

	ref = bmp_write_byte(bmpP, BMP_REG_CTRL, ctrl_val);

	if(ref != HAL_OK){return ref; }

	return ref;
}





/**
 * @brief Set default parameters for bmp280 parameter struct
 * @param[in]   bmpP  pointer to bmp280 structure
 */
void bmp_init_default_conf(BMP_Module *bmpP){

	bmpP->params.mode = BMP_MODE_NORMAL;
	bmpP->params.filter = BMP_FILTER_OFF;
	bmpP->params.osp_pressure = BMP_STANDARD;
	bmpP->params.osp_temperature = BMP_STANDARD;
	bmpP->params.standby_time = BMP_STANDBY_05;
}




/**
 * @brief       Initiate BMP280
 * @param[in]   bmpP  pointer to bmp280 structure
 */
HAL_StatusTypeDef bmp_init(BMP_Module *bmpP, uint16_t addr){

	uint8_t id = 0;

	//Assign i2c address
	bmpP->i2c_addr = addr;


	//Reset
	if (bmp_write_byte(bmpP, BMP_REG_RESET, BMP_RESET_VAL) == HAL_OK){
		sprintf(uart_buf, "Reset: OK\n\r");
	}else{
		sprintf(uart_buf, "Reset: Failed\n\r");
	}
	serial_print(uart_buf);


	//Read id
	id = bmp_read_byte(bmpP, BMP_REG_ID);

	sprintf(uart_buf, "ID: %x\n\r", id);
	serial_print(uart_buf);


	//Read calibration data
	if(bmp_read_calibration(bmpP) == HAL_OK){
		serial_print("Read cal: OK\n\r");
	}else{
		serial_print("Read cal: OK\n\r");
	}

	//Print calibration data
	bmp_print_calibration_data(bmpP);

	//Configure
	bmp_configure(bmpP);
	bmp_configure(bmpP);

	return HAL_OK;
}



/**
 * @brief          Read temperature in deg C and barometric pressure. Readings are
 * 				   stored in bmp280 struct
 * @param[in]      bmpP       pointer to bmp280 structure
 */
HAL_StatusTypeDef bmp_read_temp_and_press(BMP_Module *bmpP){

	uint32_t  adc_p = 0;
	int64_t   pressure = 0;

	uint32_t  adc_t = 0;
	int32_t   var1 = 0;
	int32_t   var2 = 0;
	int32_t   t_fine = 0;
	float     temperature = 0.0f;



	//Request data
	txbuf[0] = BMP_REG_READ_STR;
	i2c1_trx(bmpP->i2c_addr, txbuf, 1, rxbuf, 6);

	//Calculate adc values
	adc_p  =  ((uint32_t)rxbuf[0]<<12)  |  ((uint32_t)rxbuf[1]<<4)  |  ((uint32_t)rxbuf[2] >> 4);
	adc_t  =  ((uint32_t)rxbuf[3]<<12)  |  ((uint32_t)rxbuf[4]<<4)  |  ((uint32_t)rxbuf[5] >> 4);


	//Calculate temperature
	var1 = ((((adc_t >> 3) - ((int32_t)bmpP->dig_T1 << 1))) * ((int32_t)bmpP->dig_T2)) >> 11;
	var2 = (((((adc_t >> 4) - ((int32_t)bmpP->dig_T1)) * ((adc_t >> 4) - ((int32_t)bmpP->dig_T1))) >> 12) * ((int32_t)bmpP->dig_T3)) >> 14;

	t_fine = var1 + var2;

	temperature = (t_fine * 5 + 128) >> 8;
	bmpP->temp = temperature/100;


	//Calculate pressure
	var1  = (int64_t) t_fine - 128000;
	var2  = var1 * var1 * (int64_t)bmpP->dig_P6;
	var2  = var2 + ((var1 * (int64_t)bmpP->dig_P5) << 17);
	var2  = var2 + (((int64_t)bmpP->dig_P4) << 35);

	var1  = ((var1 * var1 * (int64_t)bmpP->dig_P3) >> 8)  + ((var1 * (int64_t)bmpP->dig_P2) << 12);
	var1 = (((int64_t) 1 << 47) + var1) * ((int64_t)bmpP->dig_P1) >> 33;

	//don't divide by zero
	if(var1 == 0){ bmpP->pres = 0;   }

	pressure = 1048576 - adc_p;
	pressure = (((pressure << 31) - var2) * 3125) / var1;

	var1 = ((int64_t) bmpP->dig_P9 * (pressure >> 13) * (pressure >> 13)) >> 25;
	var2 = ((int64_t) bmpP->dig_P8 * pressure) >> 19;

	bmpP->pres  = (float)((pressure + var1 + var2) >> 8) + ((int64_t) bmpP->dig_P7 << 4);

	return HAL_OK;
}














