/*
 * serialcom.h
 * @brief Serial communication interface
 */


#ifndef ENVCORE_SERIALCOM_SERIALCOM_H_
#define ENVCORE_SERIALCOM_SERIALCOM_H_




void seria_init(UART_HandleTypeDef *u);
void serial_print(char *str);

#endif /* ENVCORE_SERIALCOM_SERIALCOM_H_ */
