/*
 * ds18b20_stm32.c
 *
 * Created on: 20-Nov-2020
 * Author: Nimal T
 *
 * Modified for general use by Controllerstech.com
 */

#include "ds18b20.h"

void DS18B20_Delay(TIM_HandleTypeDef *htim, uint16_t time)
{
    __HAL_TIM_SET_COUNTER(htim, 0);
    while ((__HAL_TIM_GET_COUNTER(htim)) < time);
}

void Set_Pin_Output(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // Must be Open-Drain
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

uint8_t DS18B20_Start(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin)
{
    uint8_t Response = 0;
    Set_Pin_Output(port, pin);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    DS18B20_Delay(htim, 480);

    Set_Pin_Input(port, pin);
    DS18B20_Delay(htim, 80);

    if (!(HAL_GPIO_ReadPin(port, pin))) Response = 1;
    else Response = 0;

    DS18B20_Delay(htim, 400);
    return Response;
}

void DS18B20_Write(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin, uint8_t data)
{
    Set_Pin_Output(port, pin);
    for (int i = 0; i < 8; i++)
    {
        if ((data & (1 << i)) != 0)
        {
            Set_Pin_Output(port, pin);
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
            DS18B20_Delay(htim, 1);
            Set_Pin_Input(port, pin);
            DS18B20_Delay(htim, 60);
        }
        else
        {
            Set_Pin_Output(port, pin);
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
            DS18B20_Delay(htim, 60);
            Set_Pin_Input(port, pin);
        }
    }
}

uint8_t DS18B20_Read(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin)
{
    uint8_t value = 0;
    Set_Pin_Input(port, pin);
    for (int i = 0; i < 8; i++)
    {
        Set_Pin_Output(port, pin);
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        DS18B20_Delay(htim, 2);
        Set_Pin_Input(port, pin);
        if (HAL_GPIO_ReadPin(port, pin))
        {
            value |= 1 << i;
        }
        DS18B20_Delay(htim, 60);
    }
    return value;
}

// Simplified functions for the main application
uint8_t DS18B20_StartConv(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin)
{
    if (!DS18B20_Start(htim, port, pin)) return 0; // No device found
    DS18B20_Write(htim, port, pin, 0xCC); // Skip ROM
    DS18B20_Write(htim, port, pin, 0x44); // Convert T
    return 1;
}

uint8_t DS18B20_ReadTemp(TIM_HandleTypeDef *htim, GPIO_TypeDef *port, uint16_t pin, float *temp)
{
    uint8_t temp_byte1, temp_byte2;
    uint16_t TEMP;

    if (!DS18B20_Start(htim, port, pin)) return 0;
    DS18B20_Write(htim, port, pin, 0xCC); // Skip ROM
    DS18B20_Write(htim, port, pin, 0xBE); // Read Scratchpad

    temp_byte1 = DS18B20_Read(htim, port, pin);
    temp_byte2 = DS18B20_Read(htim, port, pin);
    TEMP = (temp_byte2 << 8) | temp_byte1;
    *temp = (float)TEMP / 16.0;

    // Simple check for a common error value (-127 is often a read failure)
    if (*temp == -127.0 || *temp == 85.0) return 0;

    return 1;
}
