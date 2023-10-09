/*
 * stm32f4xx_hal.h
 *
 *  Created on: 11/09/2023
 *      Author: edison
 */

#ifndef _STM32F4XX_HAL_H_
#define _STM32F4XX_HAL_H_

#include <stdint.h>
#include <stddef.h>

#define HSI_CLOCK_SPEED     8000000  //Value for the main clock sIgnal (HSI -> High Speed Internal)
#define HSE_CLOCK_SPEED     16000000  //Value for the main clock signal (HSE -> High Speed External)

#define NOP()    asm("NOP")
#define _weak    _attribute_((weak))


#define FLASH_BASE_ADDR       0x08000000U   //Memoria del programa 512kb
#define SRAM_BASE_ADDR        0x20000000U //Memoria RAM, 128KB

/*Nota: existen registros específicos del cortex M4 en la región 0xE0000000u
 * Los controles de las interrupciones se encuentran allí
 *
 * Nota: Posteriormente agregamos la direccion de la memoria base para cada uno de los periféricos que posee el micro
 * Revisar datasheet figura 14
 */

#define APB1_BASE_ADDR       0x40000000U
#define APB2_BASE_ADDR       0x40010000U
#define AHB1_BASE_ADDR       0x40020000U
#define AHB2_BASE_ADDR       0x50000000U

/* Ahora, debemos hacer lo mismo con cada una de las posiciones de memoria  de
 * los periféricos descritos en la tabla 1 del manual de referencia.
 * Los segmentos en los que está dividida la tabla  corresponden a APB1, APB2, AHB1, AHB2
 *
 */

/*Posiciones de memoria para AHB2 */
#define USB_OTG_FS_BASE_ADDR    (AHB2_BASE_ADDR + 0x000U)


/*Posiciones de memoria para periféricos del AHB1 */
#define DMA2_BASE_ADDR          (AHB1_BASE_ADDR + 0x6400U)
#define DMA1_BASE_ADDR          (AHB1_BASE_ADDR + 0x6000U)
#define FIR_BASE_ADDR           (AHB1_BASE_ADDR + 0x3C00U) //FLASH INTERFACE REGISTER
#define RCC_BASE_ADDR           (AHB1_BASE_ADDR + 0x3800U)
#define CRC_BASE_ADDR           (AHB1_BASE_ADDR + 0x3000U)
#define GPIOH_BASE_ADDR         (AHB1_BASE_ADDR + 0x1C00U)
#define GPIOE_BASE_ADDR         (AHB1_BASE_ADDR + 0x1000U)
#define GPIOD_BASE_ADDR         (AHB1_BASE_ADDR + 0x0C00U)
#define GPIOC_BASE_ADDR         (AHB1_BASE_ADDR + 0x0800U)
#define GPIOB_BASE_ADDR         (AHB1_BASE_ADDR + 0x0400U)
#define GPIOA_BASE_ADDR         (AHB1_BASE_ADDR + 0x0000U)

/* Macros genericos
 *
 */
#define DISABLE                 (0)
#define ENABLE                  (1)
#define SET                     ENABLE
#define CLEAR                   DISABLE
#define RESET                   DISABLE
//#define GPIO_PIN_SET            SET
//#define GPIO_PIN_RESET          RESET
#define FLAG_SET                SET
#define FLAG_RESET              RESET
#define I2C_WRITE               (0)
#define I2C_READ                (1)

typedef struct
{
	volatile uint32_t CR;            //Clock control register        DDR_OFFSET: 0X00
	volatile uint32_t PLLCFGR;        //PLL CONFIGURATIO register     DDR_OFFSET: 0X04
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t reserved0;
	volatile uint32_t reserved1;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t reserved2;
	volatile uint32_t reserved3;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t reserved4;
	volatile uint32_t reserved5;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t reserved6;
	volatile uint32_t reserved7;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t reserved8;
	volatile uint32_t reserved9;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t reserved10;
	volatile uint32_t reserved11;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t reserved12;
	volatile uint32_t reserved13;
	volatile uint32_t SSGCR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t reserved14;
	volatile uint32_t DCKCFGR;
}RCC_RegDef_t;

/*Se hace merge, donde se ubica la estructura RCC_RegDef_t  a apuntar a la
 * posición de memoria correspondiente, de forma que cada variable dentro de
 * las estructura coincide con cada uno de los
 * SFR en la memoria del micro controlador
 *
 *
 * Poner aquí la descripción de cada uno de los registros que componen al periférico RCC */


//#define RCC ((RCC_RegDef_t *) RCC_BASE_ADDR)

/*6.3.9 RCC_AHB1ENR*/
#define RCC_AHB1ENR_GPIOAEN     0
#define RCC_AHB1ENR_GPIOBEN     1
#define RCC_AHB1ENR_GPIOCEN     2
#define RCC_AHB1ENR_GPIODEN     3
#define RCC_AHB1ENR_GPIOEEN     4
#define RCC_AHB1ENR_GPIOHEN     7
#define RCC_AHB1ENR_CRCEN       12
#define RCC_AHB1ENR_DMA1EN      21
#define RCC_AHB1ENR_DMA2EN      22

/*Creamos un objeto de estructura definida y hacemos que quede exactamente
 * sobre la posición de memeoria que se encuentra en registro
 * debido a que son varios  GPIOx ES necesario  hacer la definición para cada uno.
 * Tener cuidado que cada elemento coincida con su respectiva dirección base
 *
 */

#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE_ADDR)
#define GPIOB      ((GPIO_TypeDef *) GPIOB_BASE_ADDR)
#define GPIOC      ((GPIO_TypeDef *) GPIOC_BASE_ADDR)
#define GPIOD      ((GPIO_TypeDef *) GPIOD_BASE_ADDR)
#define GPIOE      ((GPIO_TypeDef *) GPIOE_BASE_ADDR)
#define GPIOH      ((GPIO_TypeDef *) GPIOH_BASE_ADDR)



//FOR TESTING ASSERT PARAMETERS
#define IS_GPIO_ALL_INSTANCE(GPIOx)((GPIOx == GPIOA) || \
                                    (GPIOx == GPIOB) || \
	                               	(GPIOx == GPIOC) || \
									(GPIOx == GPIOD) || \
									(GPIOx == GPIOE) || \
									(GPIOx == GPIOH))




/* vALORES ESTANDAR PARA LAS CONFIGURACIONES*/
/*8.4.1 GPIOx MODER (dos bit por cada pin) */

//#define GPIO_MODE_IN          0
//#define GPIO_MODE_OUT         1
//#define GPIO_MODE_ALTFN       2
//#define GPIO_MODE_ANALOG      3

/*8.4.1 GPIOx OTYPER (UN bit por cada pin) */
//#define GPIO_OTYPE_PUSHPULL          0
//#define GPIO_OTYPE_OPENDRAIN         1

/*8.4.1 GPIOx OSPEEDR (dos bit por cada pin) */
//#define GPIO_OSPEED_LOW       0
//#define GPIO_OSPEED_MEDIU     1
//#define GPIO_OSPEED_FAST      2
//#define GPIO_OSPEED_HIGH      3

/*8.4.1 GPIOx PUPDR (dos bit por cada pin) */
//#define GPIO_PUPDR_NOTHING       0
//#define GPIO_PUPDR_PULLUP        1
//#define GPIO_PUPDR_PULLDOWN      2
//#define GPIO_PUPDR_RESERVED      3


/* Definición de nombres de los pines */
//#define PIN_0     0
//#define PIN_1     1
//#define PIN_2     2
//#define PIN_3     3
//#define PIN_4     4
//#define PIN_5     5
//#define PIN_6     6
//#define PIN_7     7
//#define PIN_8     8
//#define PIN_9     9
//#define PIN_10    10
//#define PIN_11    11
//#define PIN_12    12
//#define PIN_13    13
//#define PIN_14    14
//#define PIN_15    15

/*Definición de las funciones alternativas

 */

//#define AF0     0b0000
//#define AF1     0b0001
//#define AF2     0b0010
//#define AF3     0b0011
//#define AF4     0b1000
//#define AF5     0b0101
//#define AF6     0b0110
//#define AF7     0b0111
//#define AF8     0b1000
//#define AF9     0b1001
//#define AF10    0b1010
//#define AF11    0b1011
//#define AF12    0b1100
//#define AF13    0b1101
//#define AF14    0b1110
//#define AF15    0b1111

#endif
