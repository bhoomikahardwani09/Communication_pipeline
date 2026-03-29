#ifndef INC_STM32F411CEU6_USART_DRIVER_H_
#define INC_STM32F411CEU6_USART_DRIVER_H_

#include "stm32f411ceu6.h"

typedef struct{
	uint8_t USART_Mode;
	uint32_t USART_Baudrate;
	uint8_t USART_Stopbits;
	uint8_t WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;

}USART_Config_t;

typedef struct{
	USART_RegDef_t *pUSARTx;
	USART_Config_t USART_Config;
	uint8_t *pTxBuffer; //stores the application Tx buffer address
	uint8_t *pRxBuffer; //stores the application Rx buffer address
	uint32_t Txlen;     //stores the Tx len
	uint32_t Rxlen;     //stores the Rx len
	uint8_t TxBusyState;  //stores the communication state
	uint8_t RxBusyState;

}USART_Handle_t;


//@USART_Mode
#define USART_MODE_TX     0
#define USART_MODE_RX     1
#define USART_MODE_TXRX   2

//USART_Baudrate
#define USART_STD_BAUD_1200			        1200
#define USART_STD_BAUD_2400			        2400
#define USART_STD_BAUD_9600				9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 				2000000
#define SUART_STD_BAUD_3M                               3000000

//@USART_ParityControl
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DI       0

//@USART_STOPbits
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

//@USART_Wordlength
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

//@USART_HWFlowControl
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

#define USART_BUSY_IN_TX  0
#define USART_BUSY_IN_RX  1
#define USART_READY       2

//USART application events MACROS//
#define USART_EV_TX_CMPLT  1
#define USART_EV_RX_CMPLT  2
#define USART_EV_CTS       3
#define USART_EV_IDLE      4
#define USART_EV_ORE       5
#define USART_ER_FE        6
#define USART_ER_NF        7
#define USART_ER_ORE       8

////////////////////////////////APIs supported by this driver//////////////////////////////
//enabling the peripheral clock
void USART_PeriphCLKCtrl(USART_RegDef_t *pUSARTx, uint8_t ENorDI);

//Init & DeInit
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

//Data send and receive
void USART_SendData(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);


//Other peripheral APIs
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t flag);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

//IRQ Configuration & ISR Handling
void USART_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI);
void USART_IRQPriority(uint8_t IRQNumber, uint8_t IRQPriority);

void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEvent);


















#endif /* INC_STM32F411CEU6_USART_DRIVER_H_ */
