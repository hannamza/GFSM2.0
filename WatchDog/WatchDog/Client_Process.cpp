#include "stdafx.h"
#include "Client.h"
#include "ClientMan.h"

#include "Log.h"
#include "Network.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/pool/singleton_pool.hpp>

using namespace std;

void Client::WorkerProcessRecvPacket(BYTE* pPacket, int nSize)
{
	BYTE* buffer = m_recvBuffer, *pData = NULL;

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
		CSharedInformation::Instance()->SetReceiveTime(GetTickCount());
		TRACE("Keep_Alive [%d]\n", GetTickCount());
		break;
	case ProtocolHeader::ResponseLogin:				// login(manager)
		ProtocolResponseLogin(pData);
		break;
	case ProtocolHeader::ResponseGetEventList:		// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
		ProtocolResponseGetEventList(pData);
		break;
	case ProtocolHeader::ResponseGetRecentEventList: // 가장 최근의 이벤트 리스트 요청(페이징 포함)
		ProtocolResponseGetRecentEventList(pData);
		break;
	case ProtocolHeader::ResponseAddNewSystem:		// 건물, 단지 등 시스템 추가(마스터 키 생성)
		ProtocolResponseAddNewSystem(pData);
		break;
	case ProtocolHeader::ResponseModSystem:			// 수정
		ProtocolResponseModSystem(pData);
		break;
	case ProtocolHeader::ResponseDelSystem:			// 삭제
		ProtocolResponseDelSystem(pData);
		break;
	case ProtocolHeader::ResponseGetSystemList:		// 시스템 목록 요청
		ProtocolResponseGetSystemList(pData);
		break;
	case ProtocolHeader::ResponseGetUserList:		// 해당 시스템에 등록된 유저 목록 요청
		ProtocolResponseGetUserList(pData);
		break;
	case ProtocolHeader::ResponseDelUser:			// 유저 삭제
		ProtocolResponseDelUser(pData);
		break;
	default:
		m_recvBuffer[nSize] = '\0';
		trace0("OnRecv() : %s", m_recvBuffer);
		break;
	}
}

void Client::ProtocolResponseLogin(BYTE* pData)
{

}

void Client::ProtocolResponseGetEventList(BYTE* pData)
{

}

void Client::ProtocolResponseGetRecentEventList(BYTE* pData)
{

}

void Client::ProtocolResponseAddNewSystem(BYTE* pData)
{

}

void Client::ProtocolResponseModSystem(BYTE* pData)
{

}

void Client::ProtocolResponseDelSystem(BYTE* pData)
{

}

void Client::ProtocolResponseGetSystemList(BYTE* pData)
{

}

void Client::ProtocolResponseGetUserList(BYTE* pData)
{

}

void Client::ProtocolResponseDelUser(BYTE* pData)
{

}
