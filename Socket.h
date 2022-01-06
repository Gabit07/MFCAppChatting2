#pragma once
#include <WinSock2.h>
#include <string>
#include <future>

#pragma comment(lib, "ws2_32.lib")
#define BUFSIZE 65527 //512
#pragma warning(disable: 4996)

typedef struct Packet
{
	Packet() { ZeroMemory(this, sizeof(*this));	}
	double data;
	int packetNo;
	int length;
}tagPacket, * ptagPacket;

class CSocket
{
public:
	CSocket();
	~CSocket();

	UINT m_uiSocketPort;
	void _CloseSocket() noexcept {
		m_bIsSocketRunning = false;
		closesocket(m_Sock);
		WSACleanup();
	}	
	void send(CString str);
	void sendPacket(Packet packet);
	void receive();
	void receivePacket();
	void beginReceiving();
	bool createUDPServerSocket(UINT uiSocketPort, CWnd* pWndPtr);
	bool createUDPClientSocket(UINT uiSocketPort);	

protected:
	static int _AsyncTask(LPVOID pNetworkDialog);
	SOCKET m_Sock;
	SOCKADDR_IN m_ServerAddress;
	SOCKADDR_IN m_ClientAddress;

private:
	std::future<int> m_hAsyncTask;
	CWnd* m_pConnectWnd = nullptr;
	int m_nReturnVal;
	char m_szBuffer[BUFSIZE + 1];
	bool m_bIsSocketRunning = false;
};

