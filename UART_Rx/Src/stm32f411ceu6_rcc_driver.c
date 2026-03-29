/*
 * stm32f411ceu6_rcc_driver.c
 *
 *  Created on: Jun 8, 2025
 *      Author: bhoomika
 */

#include "stm32f411ceu6.h"
#include "stm32f411ceu6_rcc_driver.h"


uint16_t AHB_PreScalar[8]  = {2, 4, 8, 16, 64, 128, 256, 512};
uint8_t APB1_PreScalar[4] = {2, 4, 8, 16};
uint8_t APB2_PreScalar[4] = {2, 4, 8, 16};

uint32_t RCC_GetPLLOutputClock(void){
	return 0;
}

//For APB1 bus
uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, sysclk;
	uint8_t clksrc, temp, ahbps, apb1ps;
	clksrc = ( (RCC->CFGR >> 2) & 0x3 );
	if(clksrc == 0) sysclk = 16000000;  //HSE
	else if(clksrc == 1) sysclk = 8000000;  //HSI
	else if(clksrc == 2) sysclk = RCC_GetPLLOutputClock();  //PLL

	temp = ( (RCC->CFGR >> 4) & 0xf );
	if(temp < 8) ahbps = 1;
	else{
		ahbps = AHB_PreScalar[temp - 8];
	}
	temp = ( (RCC->CFGR >> 10) & 0x7 );
	if(temp < 4) apb1ps = 1;
	else{
		apb1ps = APB1_PreScalar[temp - 4];
	}

	pclk1 = (sysclk / ahbps) / apb1ps;
	return pclk1;
}

//For APB2 bus
uint32_t RCC_GetPCLK2Value(void){
	uint32_t pclk2, sysclk;
	uint8_t clksrc, temp, ahbps, apb2ps;
	clksrc = ( (RCC->CFGR >> 2) & 0x3 );
	if(clksrc == 0) sysclk = 16000000;  //HSE
	else if(clksrc == 1) sysclk = 8000000;  //HSI
	else if(clksrc == 2) sysclk = RCC_GetPLLOutputClock();  //PLL

	temp = ( (RCC->CFGR >> 4) & 0xf );
	if(temp < 8) ahbps = 1;
	else{
		ahbps = AHB_PreScalar[temp - 8];
	}
	temp = ( (RCC->CFGR >> 10) & 0x7 );
	if(temp < 4) apb2ps = 1;
	else{
		apb2ps = APB1_PreScalar[temp - 4];
	}

	pclk2 = (sysclk / ahbps) / apb2ps;
	return pclk2;
}
