#pragma once
#include <WinSock2.h>
#include <string>
#include <future>

#pragma comment(lib, "ws2_32.lib")
#define BUFSIZE 65527 //512
#define SERVER_IP "192.168.0.34"
#pragma warning(disable: 4996)

typedef struct PacketD
{
	PacketD() { ZeroMemory(this, sizeof(*this)); }
	double data;
	int packetNo;
	int length;
}tagPacketD, * ptagPacketD;

class CTCPSocketcomm
{
public:
	CTCPSocketcomm();
	~CTCPSocketcomm();

	UINT m_uiSocketPort;
	void _CloseSocket() noexcept {
		m_bIsSocketRunning = false;
		closesocket(m_Sock);
		WSACleanup();
	}
	void sendStr(CString str);
	void sendPacket(PacketD packet);
	void receiveStr();
	void receivePacket();
	void beginReceiving();
	void beginAccepting();
	bool createTCPServerSocket(UINT uiSocketPort, CWnd* pwnd);
	bool createTCPClientSocket(UINT uiSocketPort);

protected:
	static int _AsyncTask(LPVOID pNetworkDialog);
	static int _AsyncTaskAccept(LPVOID pNetworkDialog);
	SOCKET m_Sock;
	SOCKET hClient;
	SOCKADDR_IN m_ServerAddress;
	SOCKADDR_IN m_ClientAddress;

private:
	std::future<int> m_hAsyncTask;
	std::future<int> m_hAsyncTaskAccept;
	CWnd* m_pConnectWnd = nullptr;
	int m_nReturnVal;
	char m_szBuffer[BUFSIZE + 1];
	bool m_bIsSocketRunning = false;
};

