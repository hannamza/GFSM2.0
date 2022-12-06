#pragma once

#include <winsock2.h>
#include <string>

class Client
{
private:
	enum
	{
		MAX_RECV_BUFFER = (512 * 1024),
		MAX_SEND_BUFFER = (512 * 1024),
	};

public:
	enum State
	{
		WAIT,
		CREATED,
		CONNECTED,
		CLOSED,
	};

private:
	static void CALLBACK IoCompletionCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PVOID Overlapped, ULONG IoResult, ULONG_PTR NumberOfBytesTransferred, PTP_IO Io);

public:
	Client();
	~Client();

	bool Create(short port);
	void Destroy();

	bool PostConnect(const char* ip, short port);
	void PostReceive();
	void PostSend(const char* buffer, unsigned int size);

	bool Shutdown();

	void OnConnect();
	void OnRecv(DWORD dwNumberOfBytesTransfered);
	void OnSend(DWORD dwNumberOfBytesTransfered);
	void OnClose();

	State GetState() { return m_State; }
	SOCKET GetSocket() { return m_Socket; }

public: // process
	void ProcessResponseAdminLogin(BYTE* pData, BYTE* pPacket);		// ����: ������ �α���
	void ProcessResponseAddNewSystem(BYTE* pData, BYTE* pPacket);	// ����(BYTE* pData, BYTE* pPacket); �ǹ�, ���� �� �ý��� �߰�
	void ProcessResponseModSystem(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); �ý��� ����
	void ProcessResponseDelSystem(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); �ý��� ����
	void ProcessResponseGetSystemList(BYTE* pData, BYTE* pPacket);	// ����(BYTE* pData, BYTE* pPacket); �ý��� ��� ��û
	void ProcessResponseGetManagerList(BYTE* pData, BYTE* pPacket);	// ����(BYTE* pData, BYTE* pPacket); ������ ��� ��û
	void ProcessResponseAddManager(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); ������ �߰�
	void ProcessResponseModManager(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); ������ ����
	void ProcessResponseDelManager(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); ������ ����
	void ProcessResponseGetUserList(BYTE* pData, BYTE* pPacket);	// ����(BYTE* pData, BYTE* pPacket); ����� ��� ��û
	void ProcessResponseAddUser(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); ����� �߰�
	void ProcessResponseModUser(BYTE* pData, BYTE* pPacket);		// ����(BYTE* pData, BYTE* pPacket); ����� ����
	void ProcessResponseDelUser(BYTE* pData, BYTE* pPacket);		// ����: ����� ����

protected:
	void WorkerProcessRecvPacket(BYTE* pPacket, int nSize);

private:
	Client(const Client& rhs);
	Client& operator=(const Client& rhs);

private:
	TP_IO* m_pTPIO;

	State m_State;
	SOCKET m_Socket;
	BYTE m_recvBuffer[MAX_RECV_BUFFER];
	BYTE m_sendBuffer[MAX_SEND_BUFFER];
	struct addrinfo* m_infoList;
};
