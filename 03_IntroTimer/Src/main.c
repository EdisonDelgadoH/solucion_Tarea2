/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Edison Delgado
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32_assert.h"
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#define SET ENABLE
//Definimos un pin de prueba
GPIO_Handler_t userLed = {0}; //PinA5
Timer_Handler_t blinkTimer = {0};

int main(void)
{
	//UserLedType userLed;

	  /*configuramos el pin */
	   userLed.pGPIOx                        = GPIOA;
	   userLed.pinConfig.GPIO_PinNumber      = 5;
	   userLed.pinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	   userLed.pinConfig.GPIO_PinOutputType  = GPIO_OTYPE_PUSHPULL;
	   userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	   userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	  /*Cargamos la configuración en los registros que gobiernan al puerto*/

	   gpio_config(&userLed);
	   gpio_WritePin(&userLed, SET);

	   //Configuracion del timer(2)
	   blinkTimer.pTIMx                     = TIM2;
	   blinkTimer.TIMx_Config.TIMx_Prescaler = 16000; //Genera incrementos de 1 ms
	   blinkTimer.TIMx_Config.TIMx_Period = 250; //Periodo del preescaler
	   blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	   blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	   //Configuramos el timer
	   timer_Config(&blinkTimer);

	   //Encendemos el Timer
	   timer_SetState(&blinkTimer, TIMER_ON);

//
	while(1){

		}

	}
void Timer2_Callback(void){
gpioTogglePin(&userLed);
}


void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}
}
