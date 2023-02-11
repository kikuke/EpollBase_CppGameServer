# Cpp Game Server 개발 현황

## KikukeLib
### Server_Define
 - PacketDefine: 패킷 헤더, 패킷 데이터, 패킷 관련 매크로 정의.
 - socketinfo: 서버에서 관리하는 소켓 기준 클라이언트 정보.
 - impl/: TcpMessagePacket과 같은 서버에서 패킷 핸들링에 이용되는 PacketHandler의 구현체나 기타 서버 필요 클래스들의 구현체들.
 
### lsys
 - C로 작성된 리눅스 기반 기본 함수들.
   
### Service
 - PacketHandler: TcpPacketHandler와 같이 패킷을 처리하는 핸들러가 사용하는 핸들 객체를 구현하는데 사용.
 - PacketIO: 패킷 수신 관련 함수들.
 - SocketManager: 소켓fd값을 이용해 소켓 기준 클라이언트 정보를 불러오는데 사용.
 - TcpPacketHandler: TCP로 들어온 패킷들을 멤버변수가 가리키는 PacketHandler 객체들을 이용해 처리함.
 - TcpService: 클라이언트들의 연결/해제에 따른 클라이언트 관련 데이터 추가/삭제.

### struct
 - 서버 구현에 필요한 자료구조들.
 
### utility
 - Logger: 로그를 출력하고, 출력된 로그를 각 객체별로 초기화시 지정한 경로에 분산해 저장함.
 
## Server
 - Server_Define에 정의된 패킷에 따라 처리하는 Edgetrigger Epoll 기반 서버

## Client
 - Server_Define에 따라 패킷 통신하는 클라이언트
