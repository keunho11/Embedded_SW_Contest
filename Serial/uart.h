#ifndef _UART_H
#define _UART_H

#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>


// Define Constants
#define     NSERIAL_CHAR   256
#define     VMINX          1
#define     BAUDRATE       B9600
#define		OPSZ		   4

/* data */
int fd;
unsigned char serial_message[NSERIAL_CHAR];

/*function*/
void openUart();
void sendUart(unsigned char Motor ,unsigned int Angle, unsigned char Dir);
void readUart();
void closeUart();

#endif
