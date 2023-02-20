#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <iostream>
#include <sys/epoll.h>

#include "PacketDefine.h"
#include "spepoll.h"

using namespace std;

constexpr int BUF_SIZE = 2048;

const char* SERV_ADDR = "127.0.0.1";
constexpr int SERV_PORT = 1234;

TCPTestPacketHeader* EchoHeaderFactory();
MessageEchoData* SelfEchoDataFactory();
void SendEchoPacket(int sock, char* buf, TCPTestPacketHeader* header, MessageEchoData* data, unsigned char end);
ssize_t PrintRecv(int sock, const void* buf, size_t sz);
void EpollClientsThread(const int maxEpollClients);

int mode;

int main(void)
{
	int maxClientsThread;
	int maxEpollClients;
	thread** clientsThreads;

	cout << "Input Max Clients Thread Size: ";
	cin >> maxClientsThread;
	clientsThreads = new thread*[maxClientsThread];
	cout << "Input Max Epoll Clients Size: ";
	cin >> maxEpollClients;
	cout << "\nFull Message: 1\nReceive Count: 2\nSelect Mode: ";
	cin >> mode;

	for(int i=0; i<maxClientsThread; i++){
		clientsThreads[i] = new thread(EpollClientsThread, maxEpollClients);
	}

	for(int i=0; i<maxClientsThread; i++){
		clientsThreads[i]->join();
	}

	for(int i=0; i<maxClientsThread; i++){
		delete clientsThreads[i];
	}

	return 0;
}

void EpollClientsThread(const int maxEpollClients)
{
	int clntSocks[maxEpollClients];
	int clntRecvCnt = 0;
	int epfd, event_cnt, recv_sz;
	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE];

	struct epoll_event* ep_events;

	TCPTestPacketHeader* header = EchoHeaderFactory();
	MessageEchoData* msgData = SelfEchoDataFactory();

	cout << "Start Epoll Clients" << endl;

	epfd = InitEpoll(&ep_events, maxEpollClients);
	if(epfd == -1){
		perror("Init Epoll Error: ");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(SERV_ADDR);
	serv_addr.sin_port=htons(SERV_PORT);

	//connecting
	for(int i=0; i<maxEpollClients; i++){
		clntSocks[i] = socket(PF_INET, SOCK_STREAM, 0);
		if(clntSocks[i] == -1){
			perror("Socket Error: ");
			exit(1);
		}
		if(connect(clntSocks[i], (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
			perror("Connecting Error: ");
			exit(1);
		}

		AddETClntSock(epfd, clntSocks[i]);

		SendEchoPacket(clntSocks[i], buf, header, msgData, TCP_PACKET_END_CODE);
	}

	while(true)
	{
		event_cnt = epoll_wait(epfd, ep_events, maxEpollClients, -1);
		if(event_cnt == -1){
			perror("EpollWait Error: ");
			exit(1);
		}

		for(int i=0; i<event_cnt; i++){
			recv_sz = ReadET(ep_events[i].data.fd, buf, BUF_SIZE, PrintRecv);
			SendEchoPacket(ep_events[i].data.fd, buf, header, msgData, TCP_PACKET_END_CODE);
			if(mode == 1){
				printf("Receive Packet - Socket: %d	Size: %d\n", ep_events[i].data.fd, recv_sz);
			}
			else if(mode == 2){
				if(ep_events[i].data.fd == 50){
					clntRecvCnt++;
					printf("Socket Receive Count: %d\n", clntRecvCnt);
				}
			}
		}
	}

	cout << "End Epoll Clients" << endl;

	delete header;
	delete msgData;
	return;
}

ssize_t PrintRecv(int sock, const void* buf, size_t sz)
{
	return sz;
}

void SendEchoPacket(int sock, char* buf, TCPTestPacketHeader* header, MessageEchoData* data, unsigned char end)
{
	memcpy(buf, header, sizeof(TCPTestPacketHeader));
	memcpy(buf + sizeof(TCPTestPacketHeader), data, sizeof(MessageEchoData));
	memcpy(buf + sizeof(TCPTestPacketHeader) + sizeof(MessageEchoData), &end, sizeof(unsigned char));
	write(sock, buf, sizeof(TCPTestPacketHeader) + sizeof(MessageEchoData) + sizeof(unsigned char));
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
	strcpy((char*)(data->msg), "TestMessage");
    return data;
}