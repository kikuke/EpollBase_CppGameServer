#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <iostream>
#include <sys/epoll.h>
#include<termios.h>

#include "PacketDefine.h"
#include "spepoll.h"
#include "PacketGenerator.h"

using namespace std;

TCPTestPacketHeader* EchoHeaderFactory();
MessageEchoData* SelfEchoDataFactory();
void SendEchoPacket(int sock, char* buf, TCPTestPacketHeader* header, MessageEchoData* data, unsigned char end);
ssize_t PrintRecv(int sock, const void* buf, size_t sz);
void EpollClientsThread(const int maxEpollClients);
void InputThread(int sock, int room_id);
void RequestCreateClientsId(void* buf, GameRoomCreateData* data, int* clntSocks);
void RequestCreateGameRoom(int authSock, void* buf, GameRoomCreateData* createData);
void PrintObjInfo(Object_Info* info);
void EchoPacketTest();
void GameRoomTest();
int getch();

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

void PrintObjInfo(Object_Info* info)
{
	printf("Object Update - ID: %d, StartTime: %ld, State: %d\n\
    Pos: (%f, %f, %f), Force: (%f, %f, %f)\n",\
    info->id, info->st_time.tv_sec, info->state,\
    info->pos.x, info->pos.y, info->pos.z, info->force.x, info->force.y, info->force.z);
}

int getch()
{
    int ch;
	struct termios buf, save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);

    return ch;
}

void InputThread(int sock, int room_id)
{
	unsigned char buf[BUF_SIZE];
	size_t packetLen;
	size_t sendLen;
	char keyInput;
	size_t data_sz = sizeof(PlayerUpdateObjectData);
	TCPTestPacketHeader send_header = {TCP_PACKET_START_CODE, (unsigned int)(data_sz + sizeof(unsigned char)),
        PLAYER, PLAYER_UPDATE_OBJECT, 0x000, 0x000};
	PlayerUpdateObjectData inputData;
	inputData.room_id = room_id;
	inputData.obj_data.info.force = {0};

	puts("Start Input Thread");
	
	while(keyInput = getch())
	{
		if(!((keyInput == 'w') || (keyInput == 'a') || (keyInput == 's') || (keyInput == 'd')))
			continue;

		//Todo: 가속도, 시작시간까지 붙여줘야 서버에서 이동처리를 해준다. 여기에서 시작시간과 가속도를 붙여주진 않아서 포지션 이동만 될것.
		switch (keyInput)
		{
		case 'w':
			inputData.obj_data.info.pos = {0, 10, 0};
			break;
		case 'a':
			inputData.obj_data.info.pos = {-10, 0, 0};
			break;
		case 's':
			inputData.obj_data.info.pos = {0, -10, 0};
			break;
		case 'd':
			inputData.obj_data.info.pos = {10, 0, 0};
			break;
		
		default:
			break;
		}

		packetLen = MakePacket(buf, &send_header, &inputData, data_sz, TCP_PACKET_END_CODE);
		sendLen = write(sock, buf, packetLen);
		printf("Send Data %ld\n", sendLen);
	}
}

void GameRoomTest()
{
	unsigned char buf[BUF_SIZE];

	GameRoomCreateData createRoomData;
	Object_Rule game_rule;
	int* clnt_id;
	int* clntSocks;
	float speed;
	int roomid;

	size_t recv_len;

	TCPTestPacketHeader header;
	GameRoomUpdateObjectData updateData;

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

	createRoomData.rule = game_rule;

	clntSocks = new int[createRoomData.clnt_num];
	//Comment: 게임서버로 게임 룸 생성 요청은 모든 클라이언트가 서버로부터 id등록 OK응답을 받고 난 후 진행.
	//Comment: 이를 위해서 인증 서버에서는 클라이언트들에게 id들을 보내고, 클라이언트들로부터 id등록 확인을 모두 받은 후 등록된 id명단들을 다시 게임서버로 전송함.
	RequestCreateClientsId(buf, &createRoomData, clntSocks);

	//Comment: 임시로 아무소켓이나 인증서버로 설정함.
	RequestCreateGameRoom(clntSocks[0], buf, &createRoomData);

	//Comment: 임시 처리. 최초로 받는 update에서 roomid를 받기 위해
	recv_len = read(clntSocks[0], (unsigned char*)&header, sizeof(header));
	recv_len = read(clntSocks[0], (unsigned char*)&updateData, sizeof(unsigned int));
	updateData.objs_data = new OBJECT_DATA[updateData.update_obj_num];
	recv_len = read(clntSocks[0], updateData.objs_data, sizeof(OBJECT_DATA)*updateData.update_obj_num);
			
	printf("\nupdate obj Num: %d\n", updateData.update_obj_num);

	for(int i=0; i<updateData.update_obj_num; i++){
		PrintObjInfo(&(updateData.objs_data[i].info));
	}
	//Comment: 임시처리 끝

	roomid = updateData.room_id;
	thread inputThread(InputThread, clntSocks[0], roomid);

	//Comment: TCP 특성상 이렇게 받으면 안되고 링버퍼로 해야 하나, 테스트 용도로 간단하게.
	while(true)
	{
		puts("Try Parse");
		try{
			recv_len = read(clntSocks[0], (unsigned char*)&header, sizeof(header));
			recv_len = read(clntSocks[0], (unsigned char*)&updateData, sizeof(unsigned int));
			updateData.objs_data = new OBJECT_DATA[updateData.update_obj_num];
			recv_len = read(clntSocks[0], updateData.objs_data, sizeof(OBJECT_DATA)*updateData.update_obj_num);
			
			if(recv_len <= 0)
				continue;

			printf("\nupdate obj Num: %d\n", updateData.update_obj_num);

			for(int i=0; i<updateData.update_obj_num; i++){
				PrintObjInfo(&(updateData.objs_data[i].info));
			}
		} catch (exception e){
			puts("broken packet");
		}
	}
}

void RequestCreateGameRoom(int authSock, void* buf, GameRoomCreateData* createData)
{
	//Comment: 가변형이라 이렇게 세팅해야함.
	size_t data_sz = 8 + sizeof(Object_Rule) + (sizeof(int) * createData->clnt_num);
	size_t clnt_sz = (sizeof(int) * createData->clnt_num);
	unsigned char endCode = TCP_PACKET_END_CODE;
	TCPTestPacketHeader send_header = {TCP_PACKET_START_CODE, (unsigned int)(data_sz + sizeof(unsigned char)),
        GAMEROOM, GAMEROOM_CREATE, 0x000, 0x000};
	size_t packetLen = 0;

	//Comment: 포인터 형때문에 이렇게 처리
	memcpy((unsigned char*)buf + packetLen, &send_header, sizeof(send_header));
	packetLen += sizeof(send_header);
	memcpy((unsigned char*)buf + packetLen, createData, data_sz - clnt_sz);
	packetLen += data_sz - clnt_sz;
	memcpy((unsigned char*)buf + packetLen, createData->clnt_id, clnt_sz);
	packetLen += clnt_sz;
	memcpy((unsigned char*)buf + packetLen, &endCode, sizeof(endCode));
	packetLen += sizeof(endCode);

	write(authSock, buf, packetLen);
}

void RequestCreateClientsId(void* buf, GameRoomCreateData* data, int* clntSocks)
{
	const TCPTestPacketHeader send_header = {TCP_PACKET_START_CODE, sizeof(PlayerSetIdRequestData) + sizeof(unsigned char),
        PLAYER, PLAYER_SET_ID_REQUEST, 0x000, 0x000};
	PlayerSetIdRequestData requestData;
	TCPTestPacketHeader recv_header;

	size_t packetLen;

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