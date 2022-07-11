/*
 * i2c1_drv.h
 * @brief i2c driver for i2c bus 2, provides abstraction for read and write operations
 */

#ifndef ENVCORE_I2C1_DRV_H_
#define ENVCORE_I2C1_DRV_H_

//I2C timeout for tx and rx
#define I2C_TIMEOUT   10




HAL_StatusTypeDef i2c1_trx(uint8_t addr, uint8_t *txbuf, uint8_t txlength, uint8_t *rxbuf, uint8_t rxlength);
void i2c1_scan(void);


#endif /* ENVCORE_I2C1_DRV_H_ */
