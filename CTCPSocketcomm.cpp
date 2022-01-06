#include "pch.h"
#include "CTCPSocketcomm.h"

CTCPSocketcomm::CTCPSocketcomm()
{

}

CTCPSocketcomm::~CTCPSocketcomm()
{

}

bool CTCPSocketcomm::createTCPServerSocket(UINT uiSocketPort, CWnd* pwnd)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	m_pConnectWnd = pwnd;
	// socket()
	m_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Socket address initialization
	ZeroMemory(&m_ServerAddress, sizeof(m_ServerAddress));
	m_ServerAddress.sin_family = AF_INET;
	m_ServerAddress.sin_port = htons(uiSocketPort);
	m_ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);// //INADDR_BROADCAST
	int retval = bind(m_Sock, (SOCKADDR*)&m_ServerAddress, sizeof(m_ServerAddress));
	listen(m_Sock, SOMAXCONN);
	int addrlen = sizeof(m_ClientAddress);
	hClient = accept(m_Sock, (SOCKADDR*)&m_ClientAddress, &addrlen);

	return TRUE;
}

bool CTCPSocketcomm::createTCPClientSocket(UINT uiSocketPort)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// socket()
	m_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Socket address initialization
	ZeroMemory(&m_ServerAddress, sizeof(m_ServerAddress));
	m_ServerAddress.sin_family = AF_INET;
	m_ServerAddress.sin_port = htons(uiSocketPort);
	m_ServerAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
	int retVal = connect(m_Sock, (SOCKADDR*)&m_ServerAddress, sizeof(m_ServerAddress));
	if (retVal == 0)
		TRACE(_T("Connected"));

	return TRUE;
}

void CTCPSocketcomm::sendStr(CString str)
{
	CT2CA convertedString(str);
	strcpy(m_szBuffer, std::string(convertedString).c_str());

	// Sending data

	int m_nReturnVal = send(m_Sock, m_szBuffer, strlen(m_szBuffer), 0);
	if (m_nReturnVal > 0)
		TRACE(_T("Sent"));
}

void CTCPSocketcomm::sendPacket(PacketD packet)
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
	m_nReturnVal = send(m_Sock, pArr, sizeof(packet), 0);

	delete[] pArr;
}

void CTCPSocketcomm::receiveStr()
{
	CString* pString = new CString();
	while (m_bIsSocketRunning)
	{
		// Receive data******************

		m_nReturnVal = recv(hClient, m_szBuffer, BUFSIZE, 0);
		if (m_nReturnVal > 0)
		{
			m_szBuffer[m_nReturnVal] = '\0';
			*pString = m_szBuffer;

			m_pConnectWnd->SendMessage(WM_MY_MESSAGE, (WPARAM)pString, 0);
		}
	}
	delete pString;
}

void CTCPSocketcomm::receivePacket()
{
	PacketD* pPacketD = new PacketD();
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
			pPacketD->data = data;
			pPacketD->length = length;
			pPacketD->packetNo = packetNo;

			// 
			//memcpy(pPacket, &m_szBuffer, sizeof(*pPacket));

			m_pConnectWnd->SendMessage(WM_MY_MESSAGE, (WPARAM)pPacketD, 0);
		}

	}
	delete pPacketD;
}

int CTCPSocketcomm::_AsyncTask(LPVOID pNetworkDialog)
{
	CTCPSocketcomm* comm = reinterpret_cast<CTCPSocketcomm*>(pNetworkDialog);
	comm->receiveStr();
	return 0;
}

int CTCPSocketcomm::_AsyncTaskAccept(LPVOID pNetworkDialog)
{
	CTCPSocketcomm* comm = reinterpret_cast<CTCPSocketcomm*>(pNetworkDialog);
	comm->receiveStr();
	return 0;
}

void  CTCPSocketcomm::beginReceiving()
{
	m_bIsSocketRunning = true;
	m_hAsyncTask = std::async(std::launch::async, _AsyncTask, this);
}

void CTCPSocketcomm::beginAccepting()
{
	m_hAsyncTaskAccept = std::async(std::launch::async, _AsyncTaskAccept, this);
}