#include "main.h"

/* Macros utiles au Shell*/
#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9

#define COM_NB	6
#define PIN_NB	17

// LF = line feed, saut de ligne
#define ASCII_LF 0x0A
// CR = carriage return, retour chariot
#define ASCII_CR 0x0D
// DEL = delete
#define ASCII_DEL 0x7F

/* Vitesse max gérée par le PWM */
#define VITESSE_MAX 1024

/* Prototypes */
void shell_start(void);
void shell_process(void);
void shell_execute(void);
