#ifndef TCP_SERVICE
#define TCP_SERVICE

bool AcceptTcpSocket(int serv_sock, int epfd);

bool CloseTcpSocket(int clnt_sock, int epfd);

#endif