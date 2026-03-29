/*
 * stm32f411ceu6_gpio_driver.h
 *
 *  Created on: Apr 1, 2025
 *      Author: bhoomika
 */

#ifndef INC_STM32F411CEU6_GPIO_DRIVER_H_
#define INC_STM32F411CEU6_GPIO_DRIVER_H_

#include "stm32f411ceu6.h"

//Configuration structure for GPIO pin
typedef struct{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_Pin_Configuration_t;

typedef struct{
	GPIO_RegDef_t *pGPIOx;    //it holds the address of the GPIO port to which the pin belongs
	GPIO_Pin_Configuration_t GPIO_PinConfig;

}GPIOx_HANDLE_t;
//GPIO Pin Number
#define GPIO_PIN0  0
#define GPIO_PIN1  1
#define GPIO_PIN2  2
#define GPIO_PIN3  3
#define GPIO_PIN4  4
#define GPIO_PIN5  5
#define GPIO_PIN6  6
#define GPIO_PIN7  7
#define GPIO_PIN8  8
#define GPIO_PIN9  9
#define GPIO_PIN10 10
#define GPIO_PIN11 11
#define GPIO_PIN12 12
#define GPIO_PIN13 13
#define GPIO_PIN14 14
#define GPIO_PIN15 15

//GPIO Pin Modes
#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3
#define GPIO_MODE_IT_FT   4       //GPIO interrupt rising edge trigger
#define GPIO_MODE_IT_RT   5       //GPIO interrupt falling edge trigger
#define GPIO_MODE_IT_RFT  6

//GPIO Output Types
#define GPIO_PUSH_PULL_Config   0
#define GPIO_OPEN_DRAIN_Config  1

//GPIO Output Speeds
#define GPIO_SPEED_LOW       0
#define GPIO_SPEED_MEDIUM    1
#define GPIO_SPEED_FAST      2
#define GPIO_SPEED_HIGH      3

//GPIO PULL UP  & PULL DOWN
#define GPIO_NO_PUPD  0
#define GPIO_PU       1
#define GPIO_PD       2
/////////////////////////////////APIs that this driver supports/////////////////////////

void GPIO_Init(GPIOx_HANDLE_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

void GPIO_PeriphCLKCtrl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI);

uint8_t GPIO_ReadfromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadfromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WritetoOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint16_t data);
void GPIO_WritetoOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t data);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI);
void GPIO_IRQPriority(uint8_t IRQNumber, uint8_t IRQPriority);
//void GPIO_IRQHandling(uint8_t PinNumber);   //INTERRUPT FOR A PARTICULAR PIN NUMBER WILL BE MANAGED


#endif /* INC_STM32F411CEU6_GPIO_DRIVER_H_ */
