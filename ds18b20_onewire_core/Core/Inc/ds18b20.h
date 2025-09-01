/*
 * ds18b20_stm32.h
 *
 * Created on: 20-Nov-2020
 * Author: Nimal T
 *
 * Modified for general use by Controllerstech.com
 */

#ifndef DS18B20_STM32_H_
#define DS18B20_STM32_H_

#include "stm32f1xx_hal.h"

// Define the timer you are using for the microsecond delay
// This must match the timer initialized in main.c
#define DS18B20_TIMER htim1

// Function prototypes
void DS18B20_Delay(TIM_HandleTypeDef *htim, uint16_t time);
uint8_t DS18B20_Start(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin);
void DS18B20_Write(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin, uint8_t data);
uint8_t DS18B20_Read(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin);

// Simplified functions for the main application
uint8_t DS18B20_StartConv(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin);
uint8_t DS18B20_ReadTemp(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin, float *temp);

#endif /* DS18B20_STM32_H_ */
