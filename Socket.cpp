#include "pch.h"
#include "Socket.h"

CSocket::CSocket()
{

}

CSocket::~CSocket()
{

}

bool CSocket::createUDPServerSocket(UINT uiSocketPort, CWnd* pWndPtr)
{
	m_pConnectWnd = pWndPtr;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// socket()
	m_Sock = socket(AF_INET, SOCK_DGRAM, 0);
	// Socket address initialization
	ZeroMemory(&m_ServerAddress, sizeof(m_ServerAddress));
	m_ServerAddress.sin_family = AF_INET;
	m_ServerAddress.sin_port = htons(uiSocketPort);
	m_ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);// //INADDR_BROADCAST
	int retval = bind(m_Sock, (SOCKADDR*)&m_ServerAddress, sizeof(m_ServerAddress));

	return TRUE;
}

bool CSocket::createUDPClientSocket(UINT uiSocketPort)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// socket()
	m_Sock = socket(AF_INET, SOCK_DGRAM, 0);
	// Socket address initialization
	ZeroMemory(&m_ServerAddress, sizeof(m_ServerAddress));
	m_ServerAddress.sin_family = AF_INET;
	m_ServerAddress.sin_port = htons(uiSocketPort);
	//m_ServerAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);// //
	m_ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	return TRUE;
}

void CSocket::send(CString str)
{
	CT2CA convertedString(str);
	strcpy(m_szBuffer, std::string(convertedString).c_str());

	// Sending data
	m_nReturnVal = sendto(m_Sock, m_szBuffer, strlen(m_szBuffer), 0,
		(SOCKADDR*)&m_ServerAddress, sizeof(m_ServerAddress));
}

void CSocket::sendPacket(Packet packet)
{
	char* pArr = new char[sizeof(packet)];

	double data = packet.data;
	int length = packet.length;
	int packetNo = packet.packetNo;

	memcpy(pArr, &data, 8);
	memcpy(pArr + 8, &length, 4);
	memcpy(pArr + 12, &packetNo, 4);

	//
	//memcpy(pArr, &packet, sizeof(packet));

	// Sending data
	m_nReturnVal = sendto(m_Sock, pArr, sizeof(packet), 0,
		(SOCKADDR*)&m_ServerAddress, sizeof(m_ServerAddress));

	delete[] pArr;
}

void CSocket::receive()
{
	CString* pString = new CString();
	while (m_bIsSocketRunning)
	{
		// Receive data******************
		int addrlen = sizeof(m_ClientAddress);
		m_nReturnVal = recvfrom(m_Sock, m_szBuffer, BUFSIZE, 0, (SOCKADDR*)&m_ClientAddress, &addrlen);
		m_szBuffer[m_nReturnVal] = '\0';
		*pString = m_szBuffer;

		m_pConnectWnd->SendMessage(WM_MY_MESSAGE, (WPARAM)pString, 0);
	}
	delete pString;
}

void CSocket::receivePacket()
{
	Packet* pPacket = new Packet();
	while (m_bIsSocketRunning)
	{
		// Receive data******************
		int addrlen = sizeof(m_ClientAddress);
		m_nReturnVal = recvfrom(m_Sock, m_szBuffer, BUFSIZE, 0, (SOCKADDR*)&m_ClientAddress, &addrlen);
		if (m_nReturnVal > 0)
		{
			m_szBuffer[m_nReturnVal] = '\0';

			double data;
			int length;
			int packetNo;
			memcpy(&data, m_szBuffer, 8);
			memcpy(&length, m_szBuffer + 8, 4);
			memcpy(&packetNo, m_szBuffer + 12, 4);
			pPacket->data = data;
			pPacket->length = length;
			pPacket->packetNo = packetNo;

			// 
			//memcpy(pPacket, &m_szBuffer, sizeof(*pPacket));

			m_pConnectWnd->SendMessage(WM_MY_MESSAGE, (WPARAM)pPacket, 0);
		}
		
	}
	delete pPacket;
}

int CSocket::_AsyncTask(LPVOID pNetworkDialog)
{
	CSocket* comm = reinterpret_cast<CSocket*>(pNetworkDialog);
	comm->receive();
	return 0;
}

void  CSocket::beginReceiving()
{
	m_bIsSocketRunning = true;
	m_hAsyncTask = std::async(std::launch::async, _AsyncTask, this);
}