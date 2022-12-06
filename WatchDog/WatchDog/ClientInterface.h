#pragma once

#include "TSingleton.h"
#include "ClientMan.h"
#include <thread>

using namespace std;

class CClientInterface : public TSingleton<CClientInterface>
{
public:
	CClientInterface();
	~CClientInterface();

public:
	bool TryConnection(const char* ip, u_short port);
	bool TryShutdown();
	bool Send(const char* msg);
	bool Send(BYTE* buffer, int nSize);
	void Destroy();

protected:
	bool IsConnected();

private:
	char m_ip[32];
	u_short m_port;
	bool m_bConnecting;
	DWORD m_dwLastConnected;
	DWORD m_dwLastTime;
	CRITICAL_SECTION mConnectionCheck;
};

