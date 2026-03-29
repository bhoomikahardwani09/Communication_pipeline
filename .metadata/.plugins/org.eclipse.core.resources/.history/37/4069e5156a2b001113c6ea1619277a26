/*
 * stm32f411ceu6_usart_driver.c
 *
 *  Created on: Jun 8, 2025
 *      Author: bhoomika
 */

#include "stm32f411ceu6.h"
#include "stm32f411ceu6_usart_driver.h"

//enabling the peripheral clock
void USART_PeriphCLKCtrl(USART_RegDef_t *pUSARTx, uint8_t ENorDI){
	if(ENorDI == ENABLE){
			if(pUSARTx == USART1)          USART1_PCLK_EN();
			else if(pUSARTx == USART2)     USART2_PCLK_EN();
			else if(pUSARTx == USART6)     USART6_PCLK_EN();
		}
		else{
			if(pUSARTx == USART1)          USART1_PCLK_DI();
	        else if(pUSARTx == USART2)     USART2_PCLK_DI();
	        else if(pUSARTx == USART6)     USART6_PCLK_DI();
		}

}

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate){

	uint32_t pclkx, usartdiv, M_part, F_part, tempreg = 0;

	if(pUSARTx == USART1 || pUSARTx == USART6){
		//on APB2 bus
		pclkx = RCC_GetPCLK2Value();
	}
	else{
		//on APB1 bus
		pclkx = RCC_GetPCLK1Value();
	}

	//check for the OVER8 configuration bit
	if( pUSARTx->USART_CR1 & (1 << USART_CR1_OVER8) ){
		//oversampling by 8
		usartdiv = ((25 * pclkx) / (2 * BaudRate));
	}
	else{
		//oversampling by 16
		usartdiv = ((25 * pclkx) / (4 * BaudRate));
	}

	//Mantissa part calculation
	M_part = usartdiv/100;
	tempreg |= M_part << 4;

	//Fraction part calculation
	F_part = ( usartdiv - (M_part * 100) );

	//calculate the final fractional
	if(pUSARTx->USART_CR1 & (1 << USART_CR1_OVER8)){
		//oversampling by 8
		F_part = (((F_part * 8) + 50 ) / 100 ) & ((uint8_t)0x07);
	}
	else{
		//oversampling by 16
		F_part = (((F_part * 16) + 50 ) / 100 ) & ((uint8_t)0x0F);
	}

	tempreg |= F_part;

	pUSARTx->USART_BRR = tempreg;


}
//Init & DeInit
void USART_Init(USART_Handle_t *pUSARTHandle){
	uint8_t tempreg = 0;

	//Peripheral CLK Enable
	USART_PeriphCLKCtrl(pUSARTHandle->pUSARTx, ENABLE);

	//Enable the USART_Mode Configurations
	if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_RX){
		tempreg |= (1 << USART_CR1_RE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TX){
		tempreg |= (1 << USART_CR1_TE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX){
		tempreg |= (1 << USART_CR1_RE) | (1 << USART_CR1_TE);
	}

	//Enable the wordlength
	tempreg |= pUSARTHandle->USART_Config.WordLength << USART_CR1_M;

	//configuration of parity control bit fields
	if( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN){
		//enable the parity control
		tempreg |= (1 << USART_CR1_PCE);
		//once we enable the parity control, even parity will be selected
	}
	else if( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD){
		//enable the parity control
		tempreg |= (1 << USART_CR1_PCE);
		tempreg |= (1 << USART_CR1_PS);
	}

	pUSARTHandle->pUSARTx->USART_CR1 = tempreg;

	tempreg = 0;

	//congiguring no. of stopbits
	tempreg |= pUSARTHandle->USART_Config.USART_Stopbits << USART_CR2_STOP;

	pUSARTHandle->pUSARTx->USART_CR2 = tempreg;

	tempreg = 0;

	//configuring the Hardware flow control
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
		tempreg |= (1 << USART_CR3_CTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
		tempreg |= (1 << USART_CR3_RTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
		tempreg |= (1 << USART_CR3_CTSE) | (1 << USART_CR3_RTSE);
	}

	pUSARTHandle->pUSARTx->USART_CR3 = tempreg;

	//configuration of baudrate register
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baudrate);
}

void USART_DeInit(USART_RegDef_t *pUSARTx){
	if(pUSARTx == USART1)      USART1_RST();
	else if(pUSARTx == USART2) USART2_RST();
	else if(pUSARTx == USART6) USART6_RST();

}

//Data send and receive
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len){

	uint16_t *pdata;
	for(uint32_t i = 0; i < len; i++){

		//wait untill TXE flag is set in SR
		while(! USART_GetFlagStatus( pUSARTHandle->pUSARTx, (1 << USART_SR_TXE) ) );

		//check the wordlength
		if(pUSARTHandle->USART_Config.WordLength == USART_WORDLEN_9BITS)
		{

		// load the DR with 2 bytes, while masking the other 7 bits
			pdata = (uint16_t*)pTxBuffer;
			pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				pTxBuffer += 2;
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else{

			//This is 8 bits data transfer
			//the 9th bit will be replaced by the parity bit by the hardware
			pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++;

		}
	}

    //	wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,(1 << USART_SR_TC)) );

}


void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len){

	for(uint32_t i = 0; i < len; i++){

		//wait untill RXNE flag is set in SR
		while(! USART_GetFlagStatus( pUSARTHandle->pUSARTx, (1 << USART_SR_RXNE) ) );

		//check the wordlength
		if(pUSARTHandle->USART_Config.WordLength == USART_WORDLEN_9BITS){
			//(bit data frame

			//checking the parity control bit
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI)
			{
				//no parity is used , so all 9bits will be user data
				//so read only the first 9bits and mask other

				*((uint16_t*) pRxBuffer) = ( pUSARTHandle->pUSARTx->USART_DR & (uint16_t)0x01FF );
				pRxBuffer += 2;
			}
			else
			{
				//parity bit is used so 8bits will be the user data
				*pRxBuffer = ( pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF );
				pRxBuffer++;
			}

		}
		else{
			//8bit data frame
			//checking the parity control bit
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI){
				// no parity is used , so all 8 bit data will be user data
				//read it from DR
				*pRxBuffer = ( pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF );

			}
			else{
				//parity is used , so 7bits will be user data
				*pRxBuffer = ( pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0x7F );

			}
			pRxBuffer++;

		}
	}
}


uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t len){

	uint8_t txstate = pUSARTHandle->TxBusyState;
	if(txstate != USART_BUSY_IN_TX){

		pUSARTHandle->Txlen = len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//enable interrupt for TxE
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TXEIE);
		//enable interrupt for TC
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TCIE);

	}
	return txstate;
}


uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len){

	uint8_t rxstate = pUSARTHandle->RxBusyState;
	if(rxstate != USART_BUSY_IN_RX){

		pUSARTHandle->Rxlen = len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		//enable interrupt for TxE
		pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_RXNEIE);

	}
	return rxstate;
}

//IRQ Configuration & ISR Handling
void USART_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI){
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

void USART_IRQPriority(uint8_t IRQNumber, uint8_t IRQPriority){
	//1.IPR register
	uint8_t IPRx = IRQNumber / 4;
	uint8_t IPRx_section = (IRQNumber % 4);
	uint8_t shift_parameter = (IPRx_section * 8) + ( 8 - NO_OF_PR_BITS_IMPLEMENTED);
	*( NVIC_PR_BASEADDR + (IPRx * 4) ) |= (IRQPriority << shift_parameter);
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI){
	if(ENorDI == ENABLE) pUSARTx->USART_CR1 |= (1 << USART_CR1_UE);
	else pUSARTx->USART_CR1 &= ~(1 << USART_CR1_UE);

}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t flag){
	if(pUSARTx->USART_SR & flag) return FLAG_SET;
	return FLAG_RESET;
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

	uint32_t temp1, temp2, temp3;

	//////////////Check for TC flag///////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_TC);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_TCIE); //transmission complete interrupt enable

	if(temp1 && temp2)
	{
		//TC interrupt//

		//close transmission & call application_callback if Tx len = 0
		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{

			//check the TXlen
			if(! pUSARTHandle->Txlen)
			{
				//clear TC and TCIE flags
				pUSARTHandle->pUSARTx->USART_SR &= ~(1 << USART_SR_TC);
				pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TCIE);

				//Reset the application state
				pUSARTHandle->TxBusyState == USART_READY;
				pUSARTHandle->pTxBuffer = NULL;
				pUSARTHandle->Txlen = 0;
				USART_ApplicationEventCallback(pUSARTHandle, USART_EV_TX_CMPLT);
			}

		}

	}

	///////////Check for TXE flag//////////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_TXE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_TXEIE);
	uint16_t *pdata;

	if(temp1 && temp2)
	{
		//TXE interrupt//

		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{

			//send data until len = 0
			if(pUSARTHandle->Txlen > 0)
			{

				//check for word length
				if(pUSARTHandle->USART_Config.WordLength == USART_WORDLEN_9BITS)
				{
					//9 bit data//
					//load DR with 2bytes data and mask the other 7 bits
					pdata = (uint16_t*)pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

					//check for parity control
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI)
					{
						//no parity used
						pUSARTHandle->pTxBuffer += 2;

					}
					else
					{
						//parity is used
						//9bit is replaced by the parity bit
						pUSARTHandle->pTxBuffer ++;
					}
					pUSARTHandle->Txlen --;

				}
				else
				{
					//8 bit data//
					//load DR with 1byte data
					pUSARTHandle->pUSARTx->USART_DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
					pUSARTHandle->pTxBuffer ++;
					pUSARTHandle->Txlen --;
				}


			}
			if(! pUSARTHandle->Txlen)
			{
				//disable the TXEIE bit
				pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TXEIE);

			}
		}

	}

	////////////////check for RXNE flag///////////////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_RXNEIE);

	if(temp1 && temp2)
	{
		//RXNE Interrupt//
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
		{
			if(pUSARTHandle->Rxlen > 0)
			{
				//check for word length
				if(pUSARTHandle->USART_Config.WordLength == USART_WORDLEN_9BITS)
				{
					//we'll receive 9bits of data

					//check for parity control
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI)
					{
						//no parity is used
						//read the 1st 9 bits and mask the other
						*((uint16_t*)pUSARTHandle->pRxBuffer) = ((pUSARTHandle->pUSARTx->USART_DR ) & (uint16_t)0x01FF);
						pUSARTHandle->pRxBuffer += 2;
						pUSARTHandle->Rxlen--;

					}
					else
					{
						//parity is used, 8bits will be received
						*(pUSARTHandle->pRxBuffer) = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);
						pUSARTHandle->pRxBuffer += 2;
						pUSARTHandle->Rxlen--;
					}

				}
				else
				{
					//we'll receive 8bits of data

					//check for parity
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DI)
					{
						//no parity is used
						*(pUSARTHandle->pRxBuffer) = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);

					}
					else{
						//parity is used
						//1st 7bits will be read and other will be masked
						*(pUSARTHandle->pRxBuffer) = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0x7F);

					}
					pUSARTHandle->pRxBuffer ++;
					pUSARTHandle->Rxlen--;

				}

			}
			if(! pUSARTHandle->Rxlen)
			{
				//disable the RXNEIE interrupt
				pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_RXNEIE);
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle, USART_EV_RX_CMPLT);
			}

		}

	}

	//////////check for CTS flag//////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_CTS);
	temp2 = pUSARTHandle->pUSARTx->USART_CR3 & (1 << USART_CR3_CTSE);
	temp3 = pUSARTHandle->pUSARTx->USART_CR3 & (1 << USART_CR3_CTSIE);

	if(temp1 && temp2 && temp3)
	{
		//clear the CTS flag in SR
		pUSARTHandle->pUSARTx->USART_SR &= ~(1 << USART_SR_CTS);
		//call application_callback
		USART_ApplicationEventCallback(pUSARTHandle, USART_EV_CTS);

	}

	////////check for IDLE detection flag/////////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_IDLE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_IDLEIE);

	if(temp1 && temp2)
	{
		//clear the IDLE flag
		//read the SR and then DR
		uint8_t dummy;
		dummy = pUSARTHandle->pUSARTx->USART_SR;
		dummy = pUSARTHandle->pUSARTx->USART_DR;
		(void)dummy;
		USART_ApplicationEventCallback(pUSARTHandle, USART_EV_IDLE);

	}

	////////check for Overrun detection flag//////////////
	temp1 = pUSARTHandle->pUSARTx->USART_SR & (1 << USART_SR_ORE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_RXNEIE);
	if(temp1 && temp2)
	{
		USART_ApplicationEventCallback(pUSARTHandle, USART_EV_ORE);

	}

	////////check for error flag///////
	//Error Interrupt Enable Bit is required to enable interrupt generation in case of a framing
	//error, overrun error or noise flag , in case of Multi Buffer Communication

	temp1 = pUSARTHandle->pUSARTx->USART_CR3 & (1 << USART_CR3_EIE);

	if(temp1)
	{
		temp2 = pUSARTHandle->pUSARTx->USART_SR;

		if(temp2 & (1 << USART_SR_FE)){
			USART_ApplicationEventCallback(pUSARTHandle, USART_ER_FE);

		}
		if(temp2 & (1 << USART_SR_NF)){
			USART_ApplicationEventCallback(pUSARTHandle, USART_ER_NF);
		}
		if(temp2 & (1 << USART_SR_ORE)){
			USART_ApplicationEventCallback(pUSARTHandle, USART_ER_ORE);
		}

	}





}
