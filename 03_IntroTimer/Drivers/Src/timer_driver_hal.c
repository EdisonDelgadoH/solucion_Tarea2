/*
 * timer_driver_hal.c
 *
 *  Created on: 8/10/2023
 *      Author: edison
 */

#include "stm32f4xx.h"
#include "stm32_assert.h"
#include "timer_driver_hal.h"

/* variable que guarda la referencia del periférico que se está utilizando*/
TIM_TypeDef *ptrTimerUsed;

/*Headers de funcions privadas*/

static void timer_enable_clock_peripheral (Timer_Handler_t *pTimerHandler);
static void timer_set_prescaler (Timer_Handler_t *pTimerHandler);
static void timer_set_period (Timer_Handler_t *pTimerHandler);
static void timer_set_mode (Timer_Handler_t *pTimerHandler);
static void timer_config_interrupt (Timer_Handler_t *pTimerHandler);

/*Función en la que cargamos la configuración del Timer
 * se debe comenzar por activar señal de reloj
 * tener cuidado con las interrupciones
 */

void timer_Config(Timer_Handler_t *pTimerHandler){
	//Se guarda referencia al periférico que se estpa utilizando
	ptrTimerUsed =pTimerHandler -> pTIMx;
	/* desactivar interripciones globales, para luego activar señal de reloj, configurar prescaler, etc
	 */
	_disable_irq();

	timer_enable_clock_peripheral(pTimerHandler);
	timer_set_mode(pTimerHandler);
	timer_set_period(pTimerHandler);
	timer_config_interrupt(pTimerHandler);

	/*Activar de nuevo interrupciones globales*/
	_enable_irq();

	/*El timer inicia apagado*/
	timer_SetState(pTimerHandler, TIMER_OFF);
}

void timer_enable_clock_peripheral(Timer_Handler_t *pTimerHandler){
	//Verificamos que es un timer permitido
	assert_param(IS_TIM_INSTANCE(pTimerHandler -> pTIMx));

	if (pTimerHandler ->pTIMx == TIM2){
		RCC ->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}

	else if (pTimerHandler ->pTIMx == TIM3){
		RCC ->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else{
		_NOP();
	}
	//Hacerlo para los otros timer

}

/** El prescaler configura la velocidad a la aque se incrementa  el registro CNT del timer
	 */

void timer_set_prescaler(Timer_Handler_t *pTimerHandler){
	//verificamos validez del valor
	assert_param(IS_TIMER_PRESC(pTimerHandler->TIMx_Config.TIMx_Prescaler));

	//Configurar valor del prescaler.

	pTimerHandler->pTIMx->PSC = pTimerHandler->TIMx_Config.TIMx_Prescaler -1;
}

/* Función para configurar el límite hasta donde cuenta el Timer para generaer un evento de
 * Update cuando se cuenta de forma ascendente, o configura el valor desde donde se comienza a contar, cuando funciona de forma descendente
 *
 */

void timer_set_period(Timer_Handler_t *pTimerHandler){
	//verificamos validez del valor
	assert_param(IS_TIMER_PERIOD(pTimerHandler->TIMx_Config.TIMx_Period));

	//Acá falta algo

	//Configurar valor del pAUTORELOAD
	pTimerHandler->pTIMx->ARR = pTimerHandler->TIMx_Config.TIMx_Period -1;
}

//Upcounter y downcounter
void timer_set_mode(Timer_Handler_t *pTimerHandler){

	//verificamos validez del valor
	assert_param(IS_TIMER_MODE(pTimerHandler->TIMx_Config.TIMx_mode));

	//verificamos el modo a configurar
	if(pTimerHandler->TIMx_Config.TIMx_mode == TIMER_UP_COUNTER){
		//modo counter DIR 0
		pTimerHandler->pTIMx->CR1 &= ~TIM_CR1_DIR;
	}
	else{
		//modo counter DIR 1
		pTimerHandler->pTIMx->CR1 |= TIM_CR1_DIR;
	}

	//Configurar valor del pAUTORELOAD
	pTimerHandler->pTIMx->ARR = pTimerHandler->TIMx_Config.TIMx_mode -1;
}

//*Interrupción*//
void timer_config_interrupt(Timer_Handler_t *pTimerHandler){
	//verificamos validez del valor
	assert_param(IS_TIMER_INTERRUPT(pTimerHandler->TIMx_Config.TIMx_InterruptEnable));

	if(pTimerHandler->pTIMx_Config.TIMx_InterruptEnable == TIMER_INT_ENABLE){
		/*ACTIVAMOS INTERRUPCION*/
		pTimerHandler->pTIMx->DIER |= TIM_DIER_UIE;

		//Activamos el canal para que lea la interrupcion
		if(pTimerHandler->pTIMx == TIM2){
				NVIC_EnableIRQ(TIM2_IRQn);
			 }
		else if(pTimerHandler->pTIMx == TIM3){
		NVIC_EnableIRQ(TIM3_IRQn);
	    }
		else{
		_NOP();
	    }
	  }
    	else{
    		/*desactivamos la interrupción debida al Timerx utilizado*/
    		pTimerHandler->pTIMx->DIER &= ~TIM_DIER_UIE;
    		/*Desactivamos el canal del sistema NVIC para que lea la interrupción*/

	       if(pTimerHandler->pTIMx==TIM2){
	        	NVIC_DisableIRQ(TIM2_IRQn);
	        }
	        else if(pTimerHandler->pTIMx == TIM3){
	        	NVIC_DisableIRQ(TIM3_IRQn);

	        }else{
	    		_NOP();
	    	    }
    	}
}

void timer_SetState(Timer_Handler_t *pTimerHandler, uint8_t newState){
	//verificamos validez del valor
		assert_param(IS_TIMER_STATE(newState));

		//Reiniciar registro del counter
		pTimerHandler->pTIMx->CNT = 0;

		if(newState == TIMER_ON){
			/*Comienza a contar el Timer*/
			pTimerHandler->pTIMx->CR1 |= TIM_CR1_CEN;
		}else{
			/*Desactivamos timer*/
			pTimerHandler->pTIMx->CR1 &= ~TIM_CR1_CE;
		}
}

/*SOBREESCRIBIR*/
__attribute__((weak)) void Timer2_Callback(void){
	_NOP( );
}


/*Función a la que apunta el sistema en un vector de interrupciones
 * Se debe utilizar usando exactamente el mismo nombre definido en el vector de interrupciones
 * al hacerlo correctamente, el sistema apunta a esta función
 */
void TIM2_IRQHandler(void){
	/*Limpiamos la bandera*/
	TIM2->SR &= ~TIM_SR_UIF;

	/*Llamamos la función que hará algo con la interrupción
	 *
	 */
	Timer2_Callback();
}









