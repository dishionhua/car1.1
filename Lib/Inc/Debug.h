#ifndef IMU_H
#define IMU_H
#include "main.h"

extern uint8_t sofa_data[50];
extern uint8_t camera_data[50];
extern int stop_flag;
uint8_t  parse_command(const char *cmd);
void UART_Printf(UART_HandleTypeDef *huart,const char *format, ...);
float parse_float_from_uart(const uint8_t* data, uint16_t length);
#endif
