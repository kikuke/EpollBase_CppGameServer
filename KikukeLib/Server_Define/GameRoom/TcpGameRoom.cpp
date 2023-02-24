
#include "TcpGameRoom.h"

TcpGameRoom::TcpGameRoom()
{
    gen = new std::mt19937(rand_dv());
    rand = new std::uniform_int_distribution<int>(0, 99);
}

TcpGameRoom::~TcpGameRoom()
{
    delete gen;
    delete rand;
}