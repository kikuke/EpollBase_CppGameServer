#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "PacketDefine.h"
	
#define BUF_SIZE 70
#define NAME_SIZE 20
	
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(const char * msg);
	
char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];
char tempName[NAME_SIZE-3];
	
int main(void)
{
    constexpr int SERV_PORT = 1234;

	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(SERV_PORT);
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");

    printf("Server Connected...\n");
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);  
	return 0;
}

TCPTestPacketHeader* EchoHeaderFactory()
{
    TCPTestPacketHeader* header = new TCPTestPacketHeader();
    
    header->startCode = TCP_PACKET_START_CODE;
    header->packetLen = sizeof(MessageEchoData) + sizeof(unsigned char);//바디 + 트레일러
    header->mainOp = MESSAGE;
    header->subOp = MESSAGE_ECHO;
    header->flag = 0x00;
    header->auth = 0x00;
    return header;
}

MessageEchoData* SelfEchoDataFactory()
{
    MessageEchoData* data = new MessageEchoData();

    data->target = MESSAGE_ECHO_DATA_TARGET_SELF;
    return data;
}
	
void * send_msg(void * arg)   // send thread main
{
    TCPTestPacketHeader* header = EchoHeaderFactory();
    MessageEchoData* msgData = SelfEchoDataFactory();

	int sock=*((int*)arg);
	char name_msgBuf[2048];
    char end[2] = {TCP_PACKET_END_CODE, '\0'};
	while(1) 
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) 
		{
			close(sock);
			exit(0);
		}

		sprintf((char *)msgData->msg,"%s %s", name, msg);
        memcpy(name_msgBuf, header, sizeof(TCPTestPacketHeader));
        memcpy(name_msgBuf + sizeof(TCPTestPacketHeader), msgData, sizeof(MessageEchoData));
        strcpy(name_msgBuf + sizeof(TCPTestPacketHeader) + sizeof(MessageEchoData), end);
		write(sock, name_msgBuf, sizeof(TCPTestPacketHeader) + sizeof(MessageEchoData) + sizeof(unsigned char));
	}
	return NULL;
}
	
void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
		if(str_len==-1) 
			return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg, stdout);
	}
	return NULL;
}
	
void error_handling(const char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
