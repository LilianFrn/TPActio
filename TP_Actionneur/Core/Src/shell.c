/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           shell.c
  * @brief          Shell program body
  * @author			Solène Altaber, Lilian Fournier, Corentin Fraysse
  ****************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ****************************************************************************
  */
/* USER CODE END Header */
#include "string.h"
#include <math.h>
#include "shell.h"

/* Variables externes */
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc2;
extern uint32_t uartRxReceived;

extern uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

extern int cmd_ready;

/* Variables pouvant être utilisées par le main */
int vitesse = 0;
uint32_t courant = 0;
char vitesse_buffer[4];

/* constantes de texte */
uint8_t prompt[]="user@Nucleo-STM32G474>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G474RET |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t cmdNotFound[]="Command not found\r\n";
uint8_t *com[COM_NB]={"help", "pinout", "start", "stop", "speed", "current"};
uint8_t *pins[PIN_NB]={"PA0 : ADC2_IN1", "PA2 : USART2_TX", "PA3 : USART2_RX",
					"PA5 : GREEN_LED", "PA8 : TIM1_CH1",
					"PA9 : TIM1_CH2", "PA11 : TIM1_CH1N",
					"PA12 : TIM1_CH2N", "PA13 : T_SWDIO",
					"PA14 : T_SWCLK", "PC1 : ADC2_IN7",
					"PC3 : ISO_RESET", "PC13 : BLUE_BUTTON",
					"PC14 : RCC_OSC32_IN", "PC15 : RCC_OSC32_OUT",
					"PF0 : RCC_OSC_IN", "PF1 : RCC_OSC_OUT"};
uint8_t vitesseMaxTx[]="Vitesse trop importante";

/* variables du shell partagées par les fonctions */
static char	 		cmdBuffer[CMD_BUFFER_SIZE];
static int			size;
static int 			idx_cmd;
static char* 		argv[MAX_ARGS];
static int		 	argc = 0;
static char*		token;

/**
 * @fn void shell_start(void)
 * @brief Set la mémoire des variables utilisées par le Shell,
 * print le mssage de bienvenue et lance la première interruption UART.
 *
 * @param none
 * @return nothing
 */
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

/**
 * @fn void shell_process(void)
 * @brief Traite le caractère reçu à chaque entrée,
 * met le flag pour lancer la commande à 1 si le CR est détecté
 *
 * @param none
 * @return nothing
 */
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


/**
 * @fn void shell_execute(void)
 * @brief Fonction permettant de reconnaître une commande et l'éxécuter\n
 * 		-help : Liste les fonctions disponibles\n
 * 		-pinout : Donne le pinout de la carte\n
 * 		-start : Génère des PWM avec rapport cyclique a 50%\n
 * 		-speed : Argument : nombre compris entre 0 et 1023 (512 etant le rapport cyclique de 50%)\n
 * 		-stop : Arrête les PWM\n
 * 		-current : Récupère le courant pour l'asservissement (non testée)
 *
 *
 * \param none
 * \return nothing
 */
void shell_execute(void) {
	/* HELP command */
	if(strcmp(argv[0],com[0])==0)
	{
	  sprintf(uartTxBuffer,"Commands :\r\n%s\r\n%s\r\n%s\r\n%s\r\n", com[0], com[1], com[2], com[3]);
	  HAL_UART_Transmit(&huart2, uartTxBuffer, UART_TX_BUFFER_SIZE, HAL_MAX_DELAY);
	}
	/* PINOUT command */
	else if(strcmp(argv[0],com[1])==0)
	{
	  for (int i = 0; i < 10; i++) {
		  size = sprintf(uartTxBuffer,"%s\r\n", pins[i]);
		  HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	  }
	}
	/* START command */
	else if(strcmp(argv[0],com[2])==0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		TIM1->CCR1 = 512;
		TIM1->CCR2 = 512;
		PWN_start();
		size = sprintf(uartTxBuffer,"PWN ON\r\n");
		HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	}
	/* STOP command */
	else if(strcmp(argv[0],com[3]) == 0)
	{
		PWN_stop();
		size = sprintf(uartTxBuffer,"PWN OFF\r\n");
		HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	}
	/* SPEED command */
	else if(strcmp(argv[0],com[4]) == 0){
		vitesse = atoi(argv[1]);
		printf("%d\r\n", vitesse);

		while (TIM1->CCR1 < vitesse){
			TIM1->CCR1++;
			TIM1->CCR2--;
			printf("%d %d\r\n", TIM1->CCR1, TIM1->CCR2);
			HAL_Delay(50);
		}
		while (TIM1->CCR1 > vitesse){
			TIM1->CCR1--;
			TIM1->CCR2++;
			printf("%d %d\r\n", TIM1->CCR1, TIM1->CCR2);
			HAL_Delay(50);
		}
	}
	/* CUREENT command */
	else if(strcmp(argv[0],com[5])==0)
	{
		courant = HAL_ADC_GetValue(&hadc2);
		size = sprintf(uartTxBuffer,"%d\r\n", courant);
		HAL_UART_Transmit(&huart2, uartTxBuffer, size, HAL_MAX_DELAY);
	}
	/* Unknown */
	else{
	  HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
	cmd_ready = 0;
}














