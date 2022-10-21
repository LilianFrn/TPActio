#include "shell.h"

extern UART_HandleTypeDef huart2;
extern uint32_t uartRxReceived;

extern uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

extern int cmd_ready;

uint8_t prompt[]="user@Nucleo-STM32G431>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G431 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t cmdNotFound[]="Command not found\r\n";
uint8_t *com[4]={"help", "pinout", "start", "stop"};
uint8_t *pins[10]={"PA2 : USART2_TX", "PA3 : USART2_RX",
					"PA5 : GREEN_LED", "PA13 : T_SWDIO",
					"PA14 : T_SWCLK", "PC13 : BLUE_BUTTON",
					"PC14 : RCC_OSC32_IN", "PC15 : RCC_OSC32_OUT",
					"PF0 : RCC_OSC_IN", "PF1 : RCC_OSC_OUT"};

static char	 		cmdBuffer[CMD_BUFFER_SIZE];
static int			size;
static int 			idx_cmd;
static char* 		argv[MAX_ARGS];
static int		 	argc = 0;
static char*		token;

void shell_start(void) {
	memset(argv,NULL,MAX_ARGS*sizeof(char*));
	memset(cmdBuffer,NULL,CMD_BUFFER_SIZE*sizeof(char));
	memset(uartRxBuffer,NULL,UART_RX_BUFFER_SIZE*sizeof(char));
	memset(uartTxBuffer,NULL,UART_TX_BUFFER_SIZE*sizeof(char));

	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
	HAL_Delay(10);
	HAL_UART_Transmit(&huart2, started, sizeof(started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
}

void shell_process(void) {
	switch(uartRxBuffer[0]){
		// Nouvelle ligne, instruction à traiter
		case ASCII_CR:
		  HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
		  cmdBuffer[idx_cmd] = '\0';
		  argc = 0;
		  token = strtok(cmdBuffer, " ");
		  while(token!=NULL){
			  argv[argc++] = token;
			  token = strtok(NULL, " ");
		  }

		  idx_cmd = 0;
		  cmd_ready = 1;
		  break;
		// Suppression du dernier caractère
		case ASCII_DEL:
		  cmdBuffer[idx_cmd--] = '\0';
		  HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
		  break;
		// Nouveau caractère
		default:
		  cmdBuffer[idx_cmd++] = uartRxBuffer[0];
		  HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		uartRxReceived = 0;
}

void shell_execute(void) {
	if(strcmp(argv[0],com[0])==0)
	{
	  sprintf(uartTxBuffer,"Commands :\r\n%s\r\n%s\r\n%s\r\n%s\r\n", com[0], com[1], com[2], com[3]);
	  HAL_UART_Transmit(&huart2, uartTxBuffer, UART_TX_BUFFER_SIZE, HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],com[1])==0)
	{
	  for (int i = 0; i < 10; i++) {
		  size = sprintf(uartTxBuffer,"%s\r\n", pins[i]);
		  HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	  }
	}
	else if(strcmp(argv[0],com[2])==0)
	{
	  size = sprintf(uartTxBuffer,"Motor ON\r\n");
	  HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],com[3])==0)
	{
	  size = sprintf(uartTxBuffer,"Motor OFF\r\n");
	  HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	}
	else{
	  HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
	cmd_ready = 0;
}













