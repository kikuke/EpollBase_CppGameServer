# Cpp Game Server 개발 현황

## spsocket.h - 미완
 - 소켓 설정 간편화 (추가중)
 
## spepoll.h - 미완
 - epoll 설정 간편화 (추가중)
 - nonblocking EdgeTrigger epoll 모듈화
   - ReadET의 etReadFunc로 while문 안에서 read() 처리마다 동작하는 메서드를 설정 가능
   
## RingBuffer.h
 - 버퍼 공간이 부족하면 자동으로 늘려주는 RingBuffer 구현
