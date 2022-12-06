#pragma once

#include <winsock2.h>

class Client
{
public:
	enum
	{
		MAX_RECV_BUFFER = (256 * 1024),
		MAX_PACKET_BUFFER = (512 * 1024),
	};

	enum State
	{
		WAIT,
		ACCEPTED,
		DISCONNECTED,
	};

public:
	static Client* Create();
	static void Destroy(Client* client);
	static void Destroy();
	void InitializePacket();
	void ReleasePacket();

public:
	void SetTPIO(TP_IO* pTPIO) { m_pTPIO = pTPIO; }
	TP_IO* GetTPIO() { return m_pTPIO; }

	void SetState(State state) { m_State = state; }
	State GetState() { return m_State; }

	SOCKET GetSocket() { return m_Socket; }
	BYTE* GetRecvBuff() { return m_recvBuffer; }

	bool SetPacket(int nSize);
	void PacketMove(int nSize);
	void InitPacket();

	BYTE* GetPacket() { return m_pPacket; }
	int GetPacketSize() { return m_nPacketPoint; }

private:
	Client(void);
	~Client(void);
	Client& operator=(Client& rhs);
	Client(const Client& rhs);

private:
	TP_IO* m_pTPIO;
	State m_State;
	SOCKET m_Socket;
	BYTE m_recvBuffer[MAX_RECV_BUFFER];
	BYTE* m_pPacket;
	int m_nPacketPoint;
	DWORD m_dwLastRecv;
};
