/*
 * gpio_driver_hal.c
 *
 *  Created on: 11/09/2023
 *      Author: Edison Delgado
 */
#include <stm32f4xx.h>
#include "gpio_driver_hal.h"
#include <stm32_assert.h>

/*headers for private functions */
static void gpio_enable_clock_peripheral (GPIO_Handler_t *pGPIOHandler);
static void gpio_config_mode (GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler);

/**
*para cualquier periférico hay varios pasos que se deben seguir en un orden estricto
*para poder que el sistema permita configurar el periférico X.
*Lo primero es activar la señal del reloj principal hacia ese elemento específico
*(Relacionado con el periférico RCC). Esto se conoce como
*activar el periférico o activar la señal del reloj del periférico)
*/
void gpio_config (GPIO_Handler_t *pGPIOHandler){
/* Verificamos que el pin señeccionado sea el correcto. */
assert_param(IS_GPIO_PIN(pGPIOHandler -> pinConfig.GPIO_PinNumber));

// 1. activar el periférico.
gpio_enable_clock_peripheral(pGPIOHandler);

//Posteriormente, comemzamos a configurar.

//2) configuración del registro GPIOx_MODER
gpio_config_mode(pGPIOHandler);

//3) configuración del registro GPIOx_otyper
gpio_config_output_type(pGPIOHandler);

//4) configuración de la velocidad
gpio_config_output_speed(pGPIOHandler);

//5) configurar si se desea pull-up o pull-down o flotante
gpio_config_pullup_pulldown(pGPIOHandler);

//6) configuración de las funciones alternativas
gpio_config_alternate_function(pGPIOHandler);

}//Fin del gpio config

/*Enable Clock signal for specific GPIOx port
**/
void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler){
  //verificamos que el puerto con figurado sí es permitido
  assert_param(IS_GPIO_ALL_INSTANCE(pGPIOHandler ->pGPIOx));

   //verificamos  para GPIOA
   if(pGPIOHandler ->pGPIOx == GPIOA){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOA
      RCC ->  AHB1ENR |= (1<< RCC_AHB1ENR_GPIOAEN);
   }
     //verificamos  para GPIOB
   else if(pGPIOHandler ->pGPIOx == GPIOB){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOB
      RCC -> AHB1ENR |= (1<< RCC_AHB1ENR_GPIOBEN);
   }

   //verificamos  para GPIOC
   else if(pGPIOHandler ->pGPIOx == GPIOC){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOC
      RCC ->  AHB1ENR |= (1<< RCC_AHB1ENR_GPIOCEN);
   }
   //verificamos  para GPIOD
   else if(pGPIOHandler ->pGPIOx == GPIOD){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOD
      RCC ->  AHB1ENR |= (1<< RCC_AHB1ENR_GPIODEN);
   }

    //verificamos  para GPIOE
   else if(pGPIOHandler ->pGPIOx == GPIOE){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOE
      RCC ->  AHB1ENR |= (1<< RCC_AHB1ENR_GPIOEEN);
   }

   //verificamos  para GPIOH
   else if(pGPIOHandler ->pGPIOx == GPIOH){
      //Escribimos 1 (set) en la posisción correspondiente al GPIOH
      RCC ->  AHB1ENR |= (1<< RCC_AHB1ENR_GPIOHEN);
   }
}

//Función de config mode
void gpio_config_mode(GPIO_Handler_t *pGPIOHandler){

   uint32_t auxConfig = 0;

    /* verificamos que el modo que se ha seleccionado sí es permitido*/
   assert_param(IS_GPIO_MODE(pGPIOHandler -> pinConfig.GPIO_PinMode));

   //Acá estamos leyendo la config, moviendo "pinnumber" veces hacia la izquierda ese valor (Shift lft)
   //Y todo eso lo cargamos en la variable auxConfig
   auxConfig = (pGPIOHandler ->pinConfig.GPIO_PinMode <<2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

   //Antes de cargar el nuevo valor, limpiamos los bits específico de ese registro (escribir 0b00)
   // Para lo cual aplicamos una mascara y una operación BitWise AND
   pGPIOHandler ->pGPIOx->MODER &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

   //cargamos a auxconfig en el registro MODER
   pGPIOHandler->pGPIOx->MODER |= auxConfig;

}
void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler){
    uint32_t auxConfig = 0;

    /* verificamos que el tipo de salida seleccionada se pueda utilizar */
    assert_param(IS_GPIO_OUTPUT_TYPE(pGPIOHandler->pinPinConfig.GPIO_PinOutputType));

    /*De nuevo movemos el valor pinnumber veces*/
    auxConfig = (pGPIOHandler->pinConfig.GPIO_PinOutputType << pGPIOHandler ->pinConfig.GPIO_PinNumber);

    //Limpiamos antes de cargar
    pGPIOHandler ->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->pinConfig.GPIO_PinNumber);

    //cargamos a auxconfig en el registro ADECUADO
    pGPIOHandler->pGPIOx->OTYPER |= auxConfig;
}

void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler){
    uint32_t auxConfig = 0;
    assert_param(IS_GPIO_OSPEED(pGPIOHandler->pinPinConfig.GPIO_PinOutputSpeed));
    auxConfig = (pGPIOHandler->pinConfig.GPIO_PinOutputSpeed <<2* pGPIOHandler->pinConfig.GPIO_PinNumber);
    pGPIOHandler->pGPIOx -> OSPEEDR &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

   //cargamos a auxconfig en el registro ADECUADO
    pGPIOHandler->pGPIOx->OSPEEDR |= auxConfig;
}

void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler){
   uint32_t auxConfig = 0;
   //Verificamos que la configuración cargada para las resistencias es correcta
   assert_param(IS_GPIO_PUPDR(pGPIOHandler->pinPinConfig.GPIO_PinPuPdControl));
   auxConfig = (pGPIOHandler->pinConfig.GPIO_PinPuPdControl <<2* pGPIOHandler ->pinConfig.GPIO_PinNumber);
   pGPIOHandler ->pGPIOx->PUPDR &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);
   //cargamos a auxconfig en el registro ADECUADO
    pGPIOHandler->pGPIOx->PUPDR |= auxConfig;
}


void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler){
   uint32_t auxPosition = 0;
   if(pGPIOHandler->pinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

   //SELECCIONAMOS PRIMERO SI SE DEBE UTILIZAR EL REGISTRO ALTRL O ALTRH

      if(pGPIOHandler->pinConfig.GPIO_PinNumber < 8){
         //Estamos en el registro AFRL, que controla los piner del PIN 0 AL 7
         auxPosition = 4 * (pGPIOHandler->pinConfig.GPIO_PinNumber);

         //limpiamos
         pGPIOHandler ->pGPIOx->AFR[0] &= ~(0b1111 << auxPosition);

         //escribimos
         pGPIOHandler->pGPIOx->AFR[0] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);
      }
      else {
        //Estamos en el registro AFRL, que controla los piner del PIN 8 AL 15
        auxPosition = 4 * (pGPIOHandler->pinConfig.GPIO_PinNumber -8);

       //limpiamos
         pGPIOHandler ->pGPIOx->AFR[1] &= ~(0b1111 << auxPosition);

       //escribimos
         pGPIOHandler->pGPIOx->AFR[1] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);
      }
}
}

void gpio_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){

    //verificamos si la accion que deseamos realizar es permitida
    assert_param(IS_GPIO_PIN_ACTION(newState));

    //limpiamos la posición deseada
    //pPinHandler->pGPIOx->ODR &= ~(SET << pGPIOHandler->pinConfig.GPIO_PinNumber);
    if(newState == SET){
       //trabajando con la parte baja del registro
        pPinHandler->pGPIOx->BSRR |= (SET << pPinHandler->pinConfig.GPIO_PinNumber);
       }
    else{
       //trabajando con la parte alta del registro
       pPinHandler->pGPIOx->BSRR |= (SET<< (pPinHandler->pinConfig.GPIO_PinNumber + 16));

        }

 }

/**
*Función para leer el estado de un pin específico.
*/

int gpio_ReadPin(GPIO_Handler_t *pPinHandler){
//Variable auxiliar
   uint32_t pinValue = 0;
//cargamos el valor del registro IDR, desplazado a derecha tantas veces como ubicación del pin específico
   pinValue = (pPinHandler->pGPIOx->IDR >> pPinHandler->pinConfig.GPIO_PinNumber);

   /* No se tenía  sintaxis de función
    * Se tenía declarada dicha función como Void, cuando debe retornar un valor
    * Hace una segunda asignación (Innecesaria) en pinValue
    * El shift se estaba generando hacia la izquierda y debe hacerse a la derecha
    * dado que lo que estaba haciendo era desplazar el bit de la posición 0 a la que nos interesa y con esto
    * dándonos un valor erróneo
    */
   return pinValue;
 }

void gpio_TooglePin(GPIO_Handler_t *pPinHandler){
	uint32_t auxState = pPinHandler->pGPIOx->ODR & (1<< pPinHandler->pinConfig.GPIO_PinNumber);
	pPinHandler -> pGPIOx-> ODR ^=(1 << pPinHandler->pinConfig.GPIO_PinNumber);
	auxState = pPinHandler->pGPIOx->ODR & (1<< pPinHandler->pinConfig.GPIO_PinNumber);
	return auxState;
}
