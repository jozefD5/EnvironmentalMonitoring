/*
 * bmp280.h
 * @brief bmp280 driver for communication and configuration via i2c bus
 */
#ifndef _BMP280_HANDLER
#define _BMP280_HANDLER_

//Device parameters
#define BMP_I2C_ADDR        (0x76 << 1)
#define BMP_ID              0x58



//BMP registers
#define  BMP_REG_ID           0xD0
#define  BMP_REG_RESET        0xE0
#define  BMP_REG_CONFIG       0xF5
#define  BMP_REG_CTRL         0xF4
#define  BMP_REG_READ_STR     0xF7


//BMP reset value/command
#define  BMP_RESET_VAL       0xB6





/**
 * @brief Operation mode
 */
typedef enum {
    BMP_MODE_SLEEP = 0,      // No measurements
    BMP_MODE_FORCED = 1,     // Measurement is set by user.
    BMP_MODE_NORMAL = 3      // Continues measurement
} BMP_Mode;



/**
 * @brief IIR filter selection
 */
typedef enum {
    BMP_FILTER_OFF = 0,
    BMP_FILTER_2 = 1,
    BMP_FILTER_4 = 2,
    BMP_FILTER_8 = 3,
    BMP_FILTER_16 = 4
} BMP_Filter;



/**
 * @brief Pressure oversampling
 */
typedef enum {
    BMP_SKIPPED = 0,          // no measurement
    BMP_ULTRA_LOW_POWER = 1,  //  x1
    BMP_LOW_POWER = 2,        //  x2
    BMP_STANDARD = 3,         //  x4
    BMP_HIGH_RES = 4,         //  x8
    BMP_ULTRA_HIGH_RES = 5    //  x16
} BMP_Oversampling;



/**
 * @brief Stand by time between measurements in normal mode
 */
typedef enum {
    BMP_STANDBY_05 = 0,      // 0.5ms
    BMP_STANDBY_62 = 1,      // 62.5ms
    BMP_STANDBY_125 = 2,     // 125ms
    BMP_STANDBY_250 = 3,     // 250ms
    BMP_STANDBY_500 = 4,     // 500ms
    BMP_STANDBY_1000 = 5,    // 1s
    BMP_STANDBY_2000 = 6,    // 2s
    BMP_STANDBY_4000 = 7,    // 4s
} BMP_StandbyTime;





/**
 * @brief Sensor settings
 */
typedef struct {
    BMP_Mode          mode;
    BMP_Filter        filter;
    BMP_Oversampling  osp_pressure;
    BMP_Oversampling  osp_temperature;
    BMP_StandbyTime   standby_time;
} bmp_params_t;






/**
 * @brief Sensor struct, holds all required parameters, settings
 *        temperature in degree C  and  barometric pressure in hPa
 */
typedef struct {

	uint16_t i2c_addr;
	bmp_params_t params;

    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    float    temp;
    float    pres;


} BMP_Module;










void bmp_init_default_conf(BMP_Module *bmpP);
HAL_StatusTypeDef bmp_init(BMP_Module *bmpP, uint16_t addr);
HAL_StatusTypeDef bmp_read_temp_and_press(BMP_Module *bmpP);



#endif
















