#pragma once
#include <WinSock2.h>
#include <string>
#include <future>

#pragma comment(lib, "ws2_32.lib")
#define BUFSIZE 512
#pragma warning(disable: 4996)

typedef struct Packet
{
	Packet() { ZeroMemory(this, sizeof(*this)); }
	double data;
	int packetNo;
	int length;
};

class SocketComm
{
};

