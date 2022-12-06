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

public:
	void ProcessCommonRequest(USHORT protocol, int nValue);
	void ProcessRequestLogin(WCHAR* szID, WCHAR* szPW);

	void ProcessRequestAddNewSystem(CString sSystemName, CString sLocation, CString sEtc);
	void ProcessRequestModSystem(int nSeq, CString sSystemName, CString sLocation, CString sEtc);
	void ProcessRequestDelSystem(int nSeq);

	void ProcessRequestAddManager(int nWorksiteSeq, int nUserLimit, CString sID, CString sPW, CString sEtc);
	void ProcessRequestModManager(int nSeq, int nUserLimit, CString szID, CString szPW, CString szEtc);
	void ProcessRequestDelManager(int nSeq);

	void ProcessRequestAddUser(int nManagerSeq, CString sName, CString sMobile, CString sEtc);
	void ProcessRequestModUser(int nSeq, CString sName, CString sMobile, CString sEtc);
	void ProcessRequestDelUser(int nSeq);

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

