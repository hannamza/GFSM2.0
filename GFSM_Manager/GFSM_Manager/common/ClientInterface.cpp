#include "stdafx.h"
#include "ClientInterface.h"

CClientInterface::CClientInterface()
{
	m_bConnecting = false;
	m_dwLastTime = 0;
	m_dwLastConnected = 0;
	InitializeCriticalSection(&mConnectionCheck);

	CEncDec::New();
}

CClientInterface::~CClientInterface()
{
	CEncDec::Delete();
}

void CClientInterface::Destroy()
{
	DeleteCriticalSection(&mConnectionCheck);
}

bool CClientInterface::TryConnection(const char* ip, u_short port)
{
	EnterCriticalSection(&mConnectionCheck);

	strcpy_s(m_ip, 32, ip);
	m_port = port;

	if (!m_bConnecting)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::Instance()->AddClients(1);
		ClientMan::Instance()->ConnectClients(ip, port);
	}

	LeaveCriticalSection(&mConnectionCheck);
	return true;
}

bool CClientInterface::TryShutdown()
{
	EnterCriticalSection(&mConnectionCheck);

	if (m_bConnecting)
	{
		m_bConnecting = false;

		ClientMan::Instance()->RemoveClients();
		ClientMan::Delete();
		Network::Deinitialize();

		m_dwLastConnected = GetTickCount();
	}
	LeaveCriticalSection(&mConnectionCheck);

	return true;
}

bool CClientInterface::IsConnected()
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::Instance()->GetClientState() == Client::CONNECTED)
	{
		LeaveCriticalSection(&mConnectionCheck);
		return true;
	}
	LeaveCriticalSection(&mConnectionCheck);
	return false;
}

bool CClientInterface::Send(const char* msg)
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::Instance()->GetNumClients() > 0)
	{
		ClientMan::Instance()->Send(msg);
	}
	else if(GetTickCount() - m_dwLastConnected >= 3000)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::Instance()->AddClients(1);
		ClientMan::Instance()->ConnectClients(m_ip, m_port);

		LeaveCriticalSection(&mConnectionCheck);
		return false;
	}

	LeaveCriticalSection(&mConnectionCheck);

	return true;
}

bool CClientInterface::Send(BYTE* buffer, int nSize)
{
	EnterCriticalSection(&mConnectionCheck);

	if (ClientMan::Instance()->GetNumClients() > 0)
	{
		ClientMan::Instance()->Send(buffer, nSize);
	}
	else if (GetTickCount() - m_dwLastConnected >= 3000)
	{
		m_bConnecting = true;

		if (Network::Initialize() == false)
		{
			LeaveCriticalSection(&mConnectionCheck);
			m_bConnecting = false;
			return false;
		}
		ClientMan::New();

		ClientMan::Instance()->AddClients(1);
		ClientMan::Instance()->ConnectClients(m_ip, m_port);

		LeaveCriticalSection(&mConnectionCheck);
		return false;
	}

	LeaveCriticalSection(&mConnectionCheck);
	return true;
}

// 시스템, 매니저, 사용자 목록 요청
void CClientInterface::ProcessCommonRequest(USHORT protocol, int nValue)
{
	ProtocolRequest req;
	req.protocol = protocol;
	req.nValue = nValue;

	Log::Trace("ProcessCommonRequest: protocol: %d / value: %d", protocol, nValue);

	Send((BYTE*)&req, sizeof(ProtocolRequest));
}

void CClientInterface::ProcessRequestLogin(WCHAR* szID, WCHAR* szPW)
{
	ProtocolRequestLogin login;
	login.protocol = ProtocolHeader::RequestAdminLogin;
	strcpy_s(login.szID, CCommonFunc::WcharToUtf8(szID));
	strcpy_s(login.szPW, CCommonFunc::WcharToUtf8(szPW));

	Log::Trace("로그인 요청(%s)", login.szID);

	CEncDec::Instance()->PacketEncode((BYTE*)&login, sizeof(ProtocolHeader), sizeof(ProtocolRequestLogin) - sizeof(ProtocolHeader));
	Send((BYTE*)&login, sizeof(ProtocolRequestLogin));
}

void CClientInterface::ProcessRequestAddNewSystem(CString sSystemName, CString sLocation, CString sEtc)
{
	ProtocolRequestAddNewSystem system;
	strcpy_s(system.szSystemName, CCommonFunc::WcharToUtf8(sSystemName.GetBuffer(0)));
	strcpy_s(system.szLocation, CCommonFunc::WcharToUtf8(sLocation.GetBuffer(0)));
	strcpy_s(system.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("시스템 추가 요청(%s, %s, %s)", system.szSystemName, system.szLocation, system.szEtc);

	Send((BYTE*)&system, sizeof(ProtocolRequestAddNewSystem));
}

void CClientInterface::ProcessRequestModSystem(int nSeq, CString sSystemName, CString sLocation, CString sEtc)
{
	ProtocolRequestModSystem system;
	system.nSequenceNo = nSeq;
	strcpy_s(system.szSystemName, CCommonFunc::WcharToUtf8(sSystemName.GetBuffer(0)));
	strcpy_s(system.szLocation, CCommonFunc::WcharToUtf8(sLocation.GetBuffer(0)));
	strcpy_s(system.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("시스템 수정 요청(%d, %s, %s, %s)", system.nSequenceNo, system.szSystemName, system.szLocation, system.szEtc);

	Send((BYTE*)&system, sizeof(ProtocolRequestModSystem));
}

void CClientInterface::ProcessRequestDelSystem(int nSeq)
{
	ProtocolRequestDelSystem system;
	system.nSequenceNo = nSeq;

	Log::Trace("시스템 삭제 요청(%d)", system.nSequenceNo);

	Send((BYTE*)&system, sizeof(ProtocolRequestDelSystem));
}

void CClientInterface::ProcessRequestAddManager(int nWorksiteSeq, int nUserLimit, CString sID, CString sPW, CString sEtc)
{
	ProtocolRequestAddManager manager;
	manager.nWorksiteSeq = nWorksiteSeq;
	manager.nUserLimit = nUserLimit;
	strcpy_s(manager.szID, CCommonFunc::WcharToUtf8(sID.GetBuffer(0)));
	strcpy_s(manager.szPW, CCommonFunc::WcharToUtf8(sPW.GetBuffer(0)));
	strcpy_s(manager.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("매니저 추가 요청(%d, %d, %s, %s)", manager.nWorksiteSeq, manager.nUserLimit, manager.szID, manager.szEtc);

	Send((BYTE*)&manager, sizeof(ProtocolRequestAddManager));
}

void CClientInterface::ProcessRequestModManager(int nSeq, int nUserLimit, CString sID, CString sPW, CString sEtc)
{
	ProtocolRequestModManager manager;
	manager.nSeq = nSeq;
	manager.nUserLimit = nUserLimit;
	strcpy_s(manager.szID, CCommonFunc::WcharToUtf8(sID.GetBuffer(0)));
	strcpy_s(manager.szPW, CCommonFunc::WcharToUtf8(sPW.GetBuffer(0)));
	strcpy_s(manager.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("매니저 수정 요청(%d, %d, %s, %s)", manager.nSeq, manager.nUserLimit, manager.szID, manager.szEtc);

	Send((BYTE*)&manager, sizeof(ProtocolRequestModManager));
}
void CClientInterface::ProcessRequestDelManager(int nSeq)
{
	ProtocolRequestDelManager manager;
	manager.nSeq = nSeq;

	Log::Trace("매니저 삭제 요청(%d)", manager.nSeq);

	Send((BYTE*)&manager, sizeof(ProtocolRequestDelManager));
}

void CClientInterface::ProcessRequestAddUser(int nManagerSeq, CString sName, CString sMobile, CString sEtc)
{
	ProtocolRequestAddUser user;

	user.nManagerSeq = nManagerSeq;
	strcpy_s(user.szName, CCommonFunc::WcharToUtf8(sName.GetBuffer(0)));
	strcpy_s(user.szMobile, CCommonFunc::WcharToUtf8(sMobile.GetBuffer(0)));
	strcpy_s(user.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("사용자 추가 요청(%d, %s, %s, %s)", user.nManagerSeq, user.szName, user.szMobile, user.szEtc);

	Send((BYTE*)&user, sizeof(ProtocolRequestAddUser));
}

void CClientInterface::ProcessRequestModUser(int nSeq, CString sName, CString sMobile, CString sEtc)
{
	ProtocolRequestModUser user;

	user.nSeq = nSeq;
	strcpy_s(user.szName, CCommonFunc::WcharToUtf8(sName.GetBuffer(0)));
	strcpy_s(user.szMobile, CCommonFunc::WcharToUtf8(sMobile.GetBuffer(0)));
	strcpy_s(user.szEtc, CCommonFunc::WcharToUtf8(sEtc.GetBuffer(0)));

	Log::Trace("사용자 수정 요청(%d, %s, %s, %s)", user.nSeq, user.szName, user.szMobile, user.szEtc);

	Send((BYTE*)&user, sizeof(ProtocolRequestModUser));
}

void CClientInterface::ProcessRequestDelUser(int nSeq)
{
	ProtocolRequestDelUser user;

	user.nSeq = nSeq;

	Log::Trace("사용자 삭제 요청(%d)", user.nSeq);

	Send((BYTE*)&user, sizeof(ProtocolRequestDelUser));
}
