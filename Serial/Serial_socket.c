#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define ARR_CNT 5

void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[Default]";
char msg[BUF_SIZE];

int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in serv_addr;
	pthread_t rcv_thread;
	void * thread_return;

	if(argc != 4) {
		printf("Usage : %s <IP> <port> <name>\n",argv[0]);
		exit(1);
	}   

	openUart();
	sprintf(name, "%s",argv[3]);

	sock = socket(PF_INET, SOCK_STREAM, 0); 
	if(sock == -1) 
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
		error_handling("connect() error");

	sprintf(msg,"[%s:PASSWD]",name);
	write(sock, msg, strlen(msg));
	
	pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);
	pthread_join(rcv_thread, &thread_return);

	close(sock);
	closeUart();

	return 0;
}

void* recv_msg(void* arg)
{
    int* sock = (int*)arg;
    int i = 0;
    char *pToken;
    char *pArray[ARR_CNT]={0};
	const char* ON = "[1]ON";
	const char* OFF = "[1]OFF";
	const char* ISP = "[2]";

    char name_msg[NAME_SIZE + BUF_SIZE +1];
	int str_len;

    char temp_msg[NAME_SIZE + BUF_SIZE +1];
	int temp_len;
	while(1) {
		memset(name_msg,0x0,sizeof(name_msg));
		str_len = read(*sock, name_msg, 9);
		printf("read length : %d\n", str_len);
		if(str_len <= 0)
		{
			*sock = -1;
			return NULL;
		}
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);

		if(!strncmp(name_msg,ON,5)){
			sendUart('0',0,'0');
		}
		else if(!strncmp(name_msg,OFF,6)){
			sendUart('1',1,'1');
		}
		
		else if(!strncmp(name_msg,ISP,3)){
			pToken = strtok(name_msg, "]");
			while(pToken != NULL) {
				pArray[i] = pToken;
				if(i++ >= ARR_CNT) break;
				pToken = strtok(NULL, "@");
			}

			if(*pArray[1] == 'U') {
				sendUart('B', 'i', '0');
				sendUart('C', 'j', '0');
			}
			else if(*pArray[1] == 'D') {
				sendUart('B', 'j', '0');
				sendUart('C', 'i', '0');
			}
			else{}

			if(*pArray[2] == 'L') {
				sendUart('D', 'i', '0');
				sendUart('E', '0', 'L');
			}
			else if(*pArray[2] == 'R') {
				sendUart('D', 'j', '0');
				sendUart('E', '0', 'R');
			}
			else{}

			if(*pArray[3] == 'F') {
				sendUart('A', 'i', '0');
				sendUart('B', 'i', '0');
				sendUart('B', 'i', '0');
				sendUart('C', 'j', '0');
			}
			else if(*pArray[3] == 'C') {
				sendUart('A', 'j', '0');
				sendUart('B', 'j', '0');
				sendUart('B', 'j', '0');
				sendUart('C', 'i', '0');
			}
			else{

			}
		}
		else{}
	}
}

void error_handling(char * msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
