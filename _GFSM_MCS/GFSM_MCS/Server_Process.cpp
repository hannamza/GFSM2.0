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
	case ProtocolHeader::RequestSetMasterKey:		// 마스터키, 토큰, 전화번호 전송
		ProcessProtocolRequestSetMasterKey(pData);
		break;
	case ProtocolHeader::RequestGetEventList:		// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
		ProcessProtocolRequestGetEventList(pData);
		break;
	case ProtocolHeader::RequestGetRecentEventList: // 가장 최근의 이벤트 리스트 요청(페이징 포함)
		ProcessProtocolRequestGetRecentEventList(pData);
		break;
	case ProtocolHeader::RequestAddNewSystem:		// 건물, 단지 등 시스템 추가(마스터 키 생성)
		ProcessProtocolRequestAddNewSystem(pData);
		break;
	case ProtocolHeader::RequestModSystem:			// 수정
		ProcessProtocolRequestModSystem(pData);
		break;
	case ProtocolHeader::RequestDelSystem:			// 삭제
		ProcessProtocolRequestDelSystem(pData);
		break;
	case ProtocolHeader::RequestGetSystemList:		// 시스템 목록 요청
		ProcessProtocolRequestGetSystemList(pData);
		break;
	case ProtocolHeader::RequestGetUserList:		// 해당 시스템에 등록된 유저 목록 요청
		ProcessProtocolRequestGetUserList(pData);
		break;
	case ProtocolHeader::RequestDelUser:			// 유저 삭제
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

void Server::ProcessProtocolRequestSetMasterKey(BYTE* pData)	// 마스터키, 토큰, 전화번호 전송
{
	ProtocolRequestSetMasterKey* pReq = (ProtocolRequestSetMasterKey*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestGetEventList(BYTE* pData)	// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
{
	ProtocolRequestGetEventList* pReq = (ProtocolRequestGetEventList*)pData;
	ProtocolResponseGetEventList* pRes = NULL;
}

void Server::ProcessProtocolRequestGetRecentEventList(BYTE* pData)	// 가장 최근의 이벤트 리스트 요청(페이징 포함)
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetRecentEventList* pRes = NULL;
}

void Server::ProcessProtocolRequestAddNewSystem(BYTE* pData)	// 건물, 단지 등 시스템 추가(마스터 키 생성)
{
	ProtocolRequestAddNewSystem* pReq = (ProtocolRequestAddNewSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestModSystem(BYTE* pData)		// 수정
{
	ProtocolRequestModSystem* pReq = (ProtocolRequestModSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestDelSystem(BYTE* pData)		// 삭제
{
	ProtocolRequestDelSystem* pReq = (ProtocolRequestDelSystem*)pData;
	ProtocolResponse* pRes = NULL;
}

void Server::ProcessProtocolRequestGetSystemList(BYTE* pData)	// 시스템 목록 요청
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetSystemList* pRes = NULL;
}

void Server::ProcessProtocolRequestGetUserList(BYTE* pData)	// 해당 시스템에 등록된 유저 목록 요청
{
	ProtocolRequest* pReq = (ProtocolRequest*)pData;
	ProtocolResponseGetUserList* pRes = NULL;
}

void Server::ProcessProtocolRequestDelUser(BYTE* pData)		// 유저 삭제
{
	ProtocolRequestAddEvent* pReq = (ProtocolRequestAddEvent*)pData;
	ProtocolResponse* pRes = NULL;
}
