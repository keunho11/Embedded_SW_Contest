#include "uart.h"

const char *uart_target = "/dev/ttyACM0";

// SETUP SERIAL WORLD
void openUart()
{
	struct termios  oldtio, newtio;   // Create the structure

	fd = open(uart_target, O_RDWR | O_NOCTTY );
	if (fd < 0){
		printf("**Error - Unable to open UART**.\n");
	}

	tcgetattr(fd, &oldtio);	
	memset(&newtio,0,sizeof(newtio));	
	newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD; 	
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;

	newtio.c_cc[VMIN]  = 1;// Read at least 1 character
	newtio.c_cc[VTIME] > 0;// Wait indefinetly

	tcflush(fd, TCIFLUSH);
	tcflush(fd, TCIOFLUSH);
	int att = tcsetattr(fd, TCSANOW, &newtio);

	if (att != 0 ){
		printf("\nERROR in Setting port attributes");
	}
	else{
		printf("\nSERIAL Port Good to Go.\n");
	}
	usleep(500000);   // 0.5 sec delay
}

// TRANSMITTING BYTES
void sendUart(unsigned char Motor ,unsigned int Angle, unsigned char Dir)
{
	unsigned char tx_buffer[20];
	unsigned char *p_tx_buffer;
	int index = 0;
	int count = 0;
	tcflush(fd, TCOFLUSH);

	tx_buffer[0] = Motor;
	tx_buffer[1] = (unsigned char)Angle;
	tx_buffer[2] = Dir;
	tx_buffer[3] = '\n';

	if (fd != -1)
	{
		count = write(fd, tx_buffer, 4);		
		usleep(1000);   // .001 sec delay
		printf("Count = %d\n", count);
		if (count < 0)  
			printf("UART TX error\n");
	}
	//usleep(1000000);  // 1 sec delay
}

// RECEIVING BYTES - AND BUILD MESSAGE RECEIVED
void readUart()
{
	unsigned char rx_buffer[VMINX];
	bool          pickup = true;
	int           rx_length;
	int           nread = 0;

	tcflush(fd, TCIFLUSH);
	usleep(1000);   // .001 sec delay
	printf("Ready to receive message.\n");

	memset(serial_message, ' ', NSERIAL_CHAR);
	while (pickup && fd != -1)
	{
		nread++;
		rx_length = read(fd, (void*)rx_buffer, VMINX);  

		if (rx_length < 0){
			printf("UART RX error\n");
		}
		if (rx_length == 0){
			printf("no data\n");
		}
		if (rx_length >0)
		{
			if (nread<=NSERIAL_CHAR){
				serial_message[nread-1] = rx_buffer[0];
				printf("%x ",serial_message[nread-1]);
			}

			if (rx_buffer[0]=='\n')   pickup=false;
		}
	}
	printf("\nMessage Received");
}

void closeUart()
{
	//-------------------------------------------
	//  CLOSE THE SERIAL PORT
	//-------------------------------------------
	close(fd);
}
