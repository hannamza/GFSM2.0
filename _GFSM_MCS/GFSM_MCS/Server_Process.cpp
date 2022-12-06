#include "stdafx.h"
#include "Server.h"
#include "Client.h"
#include "Packet.h"
#include "IOEvent.h"

#include "Log.h"
#include "Network.h"
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CALLBACK Server::WorkerProcessRecvPacket(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context)
{
	Packet* packet = static_cast<Packet*>(Context);
	assert(packet);

	BYTE* buffer = packet->GetData(), *pData = NULL;

	USHORT command, size;
	memcpy_s(&command, sizeof(USHORT), buffer, sizeof(USHORT));
	memcpy_s(&size, sizeof(USHORT), &buffer[sizeof(USHORT)], sizeof(USHORT));
	if (size > 0)
	{
		pData = &buffer[sizeof(USHORT) * 2];
	}
	if (command < ProtocolHeader::Keep_Alive || command >= ProtocolHeader::DefineEndProtocol)
	{
		return;
	}

	switch (command)
	{
	case ProtocolHeader::Keep_Alive:
		Server::Instance()->Echo(packet);
		trace0("[%d] KEEP ALIVE Packet.", GetCurrentThreadId());
		break;
	case ProtocolHeader::RequestLogin:				// login(manager)
		ProcessProtocolRequestLogin(pData);
		break;
	case ProtocolHeader::RequestSetMasterKey:		// ������Ű, ��ū, ��ȭ��ȣ ����
		ProcessProtocolRequestSetMasterKey(pData);
		break;
	case ProtocolHeader::RequestGetEventList:		// �Ϸù�ȣ ������ �̺�Ʈ ����Ʈ ��û(������Ű ����)
		ProcessProtocolRequestGetEventList(pData);
		break;
	case ProtocolHeader::RequestGetRecentEventList: // ���� �ֱ��� �̺�Ʈ ����Ʈ ��û(����¡ ����)
		ProcessProtocolRequestGetRecentEventList(pData);
		break;
	case ProtocolHeader::RequestAddNewSystem:		// �ǹ�, ���� �� �ý��� �߰�(������ Ű ����)
		ProcessProtocolRequestAddNewSystem(pData);
		break;
	case ProtocolHeader::RequestModSystem:			// ����
		ProcessProtocolRequestModSystem(pData);
		break;
	case ProtocolHeader::RequestDelSystem:			// ����
		ProcessProtocolRequestDelSystem(pData);
		break;
	case ProtocolHeader::RequestGetSystemList:		// �ý��� ��� ��û
		ProcessProtocolRequestGetSystemList(pData);
		break;
	case ProtocolHeader::RequestGetUserList:		// �ش� �ý��ۿ� ��ϵ� ���� ��� ��û
		ProcessProtocolRequestGetUserList(pData);
		break;
	case ProtocolHeader::RequestDelUser:			// ���� ����
		ProcessProtocolRequestDelUser(pData);
		break;
	default:
		Server::Instance()->Echo(packet);
		break;
	}
}

void Server::ProcessProtocolRequestLogin(BYTE* pData)	// login(manager)
{
	ProtocolRequestLogin* pReq = (ProtocolRequestLogin*)pData;
	ProtocolResponse* pRes = NULL;

}

void Server::ProcessProtocolRequestSetMasterKey(BYTE* pData)	// ������Ű, ��ū, ��ȭ��ȣ ����
{
	ProtocolRequestSetMasterKey* pReq = (ProtocolRequestSetMasterKey*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestGetEventList(BYTE* pData)	// �Ϸù�ȣ ������ �̺�Ʈ ����Ʈ ��û(������Ű ����)
{
	ProtocolRequestGetEventList* pReq = (ProtocolRequestGetEventList*)pData;
	ProtocolResponseGetEventList* pRes = NULL;
}

void Server::ProcessProtocolRequestGetRecentEventList(BYTE* pData)	// ���� �ֱ��� �̺�Ʈ ����Ʈ ��û(����¡ ����)
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetRecentEventList* pRes = NULL;
}

void Server::ProcessProtocolRequestAddNewSystem(BYTE* pData)	// �ǹ�, ���� �� �ý��� �߰�(������ Ű ����)
{
	ProtocolRequestAddNewSystem* pReq = (ProtocolRequestAddNewSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestModSystem(BYTE* pData)		// ����
{
	ProtocolRequestModSystem* pReq = (ProtocolRequestModSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestDelSystem(BYTE* pData)		// ����
{
	ProtocolRequestDelSystem* pReq = (ProtocolRequestDelSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestGetSystemList(BYTE* pData)	// �ý��� ��� ��û
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetSystemList* pRes = NULL;
}

void Server::ProcessProtocolRequestGetUserList(BYTE* pData)	// �ش� �ý��ۿ� ��ϵ� ���� ��� ��û
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetUserList* pRes = NULL;
}

void Server::ProcessProtocolRequestDelUser(BYTE* pData)		// ���� ����
{
	ProtocolRequestAddEvent* pReq = (ProtocolRequestAddEvent*)pData;
	ProtocolResponse* pRes = NULL;
}
