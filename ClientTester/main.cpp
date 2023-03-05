#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <iostream>
#include <sys/epoll.h>

#include "PacketDefine.h"
#include "spepoll.h"
#include "PacketGenerator.h"

using namespace std;

TCPTestPacketHeader* EchoHeaderFactory();
MessageEchoData* SelfEchoDataFactory();
void SendEchoPacket(int sock, char* buf, TCPTestPacketHeader* header, MessageEchoData* data, unsigned char end);
ssize_t PrintRecv(int sock, const void* buf, size_t sz);
void EpollClientsThread(const int maxEpollClients);
void RequestCreateClientsId(void* buf, GameRoomCreateData* data, int* clntSocks);
void EchoPacketTest();
void GameRoomTest();

struct sockaddr_in serv_addr;

constexpr int BUF_SIZE = 2048;

const char* SERV_ADDR = "127.0.0.1";
constexpr int SERV_PORT = 1234;

int mode=0;

int main(void)
{
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(SERV_ADDR);
	serv_addr.sin_port=htons(SERV_PORT);

	while(!mode)
	{
		cout << "Select Mode" << endl;
		cout << "1: EchoTest" << endl;
		cout << "2: GameRoomTest" << endl;
		cout << "Select Mode: ";

		cin >> mode;

		switch (mode)
		{
		case 1:
			EchoPacketTest();
			break;

		case 2:
			GameRoomTest();
			break;
		
		default:
			mode = 0;
			break;
		}
	}

	return 0;
}

void GameRoomTest()
{
	unsigned char buf[BUF_SIZE];

	GameRoomCreateData createRoomData;
	Object_Rule game_rule;
	int* clnt_id;
	int* clntSocks;

	cout << "Input Game rule" << endl;
	cout << "Object Speed: ";
	cin >> game_rule.speed;
	cout << "Input Max Number of Npc: ";
	cin >> createRoomData.npc_num;
	cout << "Input Max Number of Clients: ";
	cin >> createRoomData.clnt_num;
	clnt_id = new int[createRoomData.clnt_num];
	for(int i=0; i<createRoomData.clnt_num; i++){
		cout << "Input " << i << " Client ID(Integer): ";
		cin >> clnt_id[i];
	}
	createRoomData.clnt_id = clnt_id;

	//Comment: 게임서버로 게임 룸 생성 요청은 모든 클라이언트가 서버로부터 id등록 OK응답을 받고 난 후 진행.
	RequestCreateClientsId(buf, &createRoomData, clntSocks);

	//Todo: Send Create Room Packet
}

void RequestCreateClientsId(void* buf, GameRoomCreateData* data, int* clntSocks)
{
	const TCPTestPacketHeader send_header = {TCP_PACKET_START_CODE, sizeof(PlayerSetIdRequestData) + sizeof(unsigned char),
        PLAYER, PLAYER_SET_ID_REQUEST, 0x000, 0x000};
	PlayerSetIdRequestData requestData;
	TCPTestPacketHeader recv_header;

	size_t packetLen;

	clntSocks = new int[data->clnt_num];

	for(int i=0; i<data->clnt_num; i++){
		clntSocks[i] = socket(PF_INET, SOCK_STREAM, 0);
		if(clntSocks[i] == -1){
			perror("Socket Error: ");
			exit(1);
		}
		if(connect(clntSocks[i], (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
			perror("Connecting Error: ");
			exit(1);
		}

		requestData.id = data->clnt_id[i];
		packetLen = MakePacket(buf, &send_header, &requestData, sizeof(requestData), TCP_PACKET_END_CODE);

		write(clntSocks[i], buf, packetLen);

		read(clntSocks[i], buf, BUF_SIZE);
		memcpy(&recv_header, buf, sizeof(recv_header));
		if(!(recv_header.subOp == PLAYER_SET_ID_OK)){
			puts("Create ID Recv Error!");
			exit(1);
		}
		printf("%d Client Create ID Success!\n", data->clnt_id[i]);
	}
}

void EchoPacketTest()
{
	int maxClientsThread;
	int maxEpollClients;
	thread** clientsThreads;

	cout << "Input Max Clients Thread Size: ";
	cin >> maxClientsThread;
	clientsThreads = new thread*[maxClientsThread];
	cout << "Input Max Epoll Clients Size: ";
	cin >> maxEpollClients;

	for(int i=0; i<maxClientsThread; i++){
		clientsThreads[i] = new thread(EpollClientsThread, maxEpollClients);
	}

	for(int i=0; i<maxClientsThread; i++){
		clientsThreads[i]->join();
	}

	for(int i=0; i<maxClientsThread; i++){
		delete clientsThreads[i];
	}
}

void EpollClientsThread(const int maxEpollClients)
{
	int clntSocks[maxEpollClients];
	int clntRecvCnt = 0;
	int epfd, event_cnt, recv_sz;
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
			printf("Receive Packet - Socket: %d	Size: %d\n", ep_events[i].data.fd, recv_sz);
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