/*
 * stm32f411ceu6_gpio_driver.c
 *
 *  Created on: Dec 23, 2024
 *      Author: bhoomika
 */

#include "stm32f411ceu6_gpio_driver.h"

//Function   : To enable/disable the peripheral clock for given GPIO Port
//parameters : address of port, EN/DI macros
//return     : none
void GPIO_PeriphCLKCtrl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI){
	if(ENorDI == ENABLE){
		if(pGPIOx == GPIOA)        GPIOA_PCLK_EN();
		else if(pGPIOx == GPIOB)   GPIOB_PCLK_EN();
		else if(pGPIOx == GPIOC)   GPIOC_PCLK_EN();
		else if(pGPIOx == GPIOD)   GPIOD_PCLK_EN();
		else if(pGPIOx == GPIOE)   GPIOE_PCLK_EN();
		else if(pGPIOx == GPIOH)   GPIOH_PCLK_EN();
	}
	else{
		if(pGPIOx == GPIOA)        GPIOA_PCLK_DI();
        else if(pGPIOx == GPIOB)   GPIOB_PCLK_DI();
        else if(pGPIOx == GPIOC)   GPIOC_PCLK_DI();
        else if(pGPIOx == GPIOD)   GPIOD_PCLK_DI();
        else if(pGPIOx == GPIOE)   GPIOE_PCLK_DI();
	    else if(pGPIOx == GPIOH)   GPIOH_PCLK_DI();
	}

}
//

//Function: To initialize the GPIO port
//Parameters:
//Return:
void GPIO_Init(GPIOx_HANDLE_t *pGPIOHandle){
	uint32_t temp = 0;
	//enabling the peripheral clock
	GPIO_PeriphCLKCtrl(pGPIOHandle->pGPIOx, ENABLE);
	//configuring the mode of GPIO Pins
	if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle -> pGPIOx -> MODER &= (3 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);   //clearing
		pGPIOHandle -> pGPIOx -> MODER |= temp;

	}
	else{
		//for interrupt mode
		if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//1. configuring the interrupt falling trigger selection register
			EXTI -> FTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
			EXTI -> RTSR &= ~(1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);  // clearing that corresponding pin RTSR
		}
		else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//1. configuring the interrupt rising trigger selection register
			EXTI -> RTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
			EXTI -> FTSR &= ~(1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);  // clearing that corresponding pin FTSR

		}
		else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//1. configuring the interrupt falling and rising trigger selection register
			EXTI -> FTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
			EXTI -> RTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
		}
		//2.Configure the GPIO port selection from SYSCFG_EXTICR
		uint8_t tempv1 = pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t tempv2 = 4 * (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber % 4);
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle -> pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG -> EXTICR[tempv1] = portcode << tempv2;

		//3.Enable the EXTI interrupt delivery
		EXTI -> IMR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	}
	temp = 0;

	//configuring the speed of GPIO Pins
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> OSPEEDR &= (3 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);   //clearing
	pGPIOHandle -> pGPIOx -> OSPEEDR |= temp;
	temp = 0;

	//configuring the pull up/down settings of GPIO Pins
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> PUPDR &= (3 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);   //clearing
	pGPIOHandle -> pGPIOx -> PUPDR |= temp;
	temp = 0;

	//configuring the output type settings of GPIO Pins
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> OTYPER &= (3 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);   //clearing
	pGPIOHandle -> pGPIOx -> OTYPER |= temp;
	temp = 0;

	//configuring the alternate functionality mode
	if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		uint32_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = 4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8);
		pGPIOHandle -> pGPIOx -> AFR[temp1] |=  (0xF << (temp2) );  //clearing
		pGPIOHandle -> pGPIOx -> AFR[temp1] |=  (pGPIOHandle -> GPIO_PinConfig.GPIO_PinAltFunMode << (temp2) );

	}

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA)        GPIOA_REG_RESET();
	else if(pGPIOx == GPIOB)   GPIOB_REG_RESET();
	else if(pGPIOx == GPIOC)   GPIOC_REG_RESET();
	else if(pGPIOx == GPIOD)   GPIOD_REG_RESET();
	else if(pGPIOx == GPIOE)   GPIOE_REG_RESET();
	else if(pGPIOx == GPIOH)   GPIOH_REG_RESET();

}

uint8_t GPIO_ReadfromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t data;
	data = (uint8_t)((pGPIOx -> IDR >> PinNumber) & 0x00000001);
	return data;
}

uint16_t GPIO_ReadfromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t data;
	data = (uint16_t)(pGPIOx -> IDR);
	return data;
}

void GPIO_WritetoOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint16_t data){
	if(data == 1){
		pGPIOx -> ODR |= (1 >> PinNumber);
	}else{
		pGPIOx -> ODR &= ~(1 >> PinNumber);
	}
}

void GPIO_WritetoOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t data){
	pGPIOx -> ODR = data;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx -> ODR ^= (1 << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI){
	if(ENorDI == ENABLE){
		if(IRQNumber <= 31){
			//program ISER0
			*NVIC_ISER0 |= ( 1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber <= 63){
			//program ISER1
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber > 63 && IRQNumber < 96){
			//ISER2
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
		}
	}
	else{
		if(IRQNumber <= 31){
			//program ISER0
			*NVIC_ICER0 |= ( 1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber <= 63){
			//program ISER1
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber > 63 && IRQNumber < 96){
			//ISER2
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
		}
	}
}

void GPIO_IRQPriority(uint8_t IRQNumber, uint8_t IRQPriority){
	//1.IPR register
	uint8_t IPRx = IRQNumber / 4;
	uint8_t IPRx_section = (IRQNumber % 4);
	uint8_t shift_parameter = (IPRx_section * 8) + ( 8 - NO_OF_PR_BITS_IMPLEMENTED);
	*( NVIC_PR_BASEADDR + (IPRx * 4) ) |= (IRQPriority << shift_parameter);

}
void GPIO_IRQHandling(uint8_t PinNumber){
	//INTERRUPT FOR A PARTICULAR PIN NUMBER WILL BE MANAGED
	//clear the EXTI PR(pending register) register corresponding to the pin number
	if(EXTI -> PR & ( 1 << PinNumber)){
		EXTI -> PR |= ( 1 << PinNumber);
	}

}





































