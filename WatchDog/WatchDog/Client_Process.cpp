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
	case ProtocolHeader::ResponseGetEventList:		// �Ϸù�ȣ ������ �̺�Ʈ ����Ʈ ��û(������Ű ����)
		ProtocolResponseGetEventList(pData);
		break;
	case ProtocolHeader::ResponseGetRecentEventList: // ���� �ֱ��� �̺�Ʈ ����Ʈ ��û(����¡ ����)
		ProtocolResponseGetRecentEventList(pData);
		break;
	case ProtocolHeader::ResponseAddNewSystem:		// �ǹ�, ���� �� �ý��� �߰�(������ Ű ����)
		ProtocolResponseAddNewSystem(pData);
		break;
	case ProtocolHeader::ResponseModSystem:			// ����
		ProtocolResponseModSystem(pData);
		break;
	case ProtocolHeader::ResponseDelSystem:			// ����
		ProtocolResponseDelSystem(pData);
		break;
	case ProtocolHeader::ResponseGetSystemList:		// �ý��� ��� ��û
		ProtocolResponseGetSystemList(pData);
		break;
	case ProtocolHeader::ResponseGetUserList:		// �ش� �ý��ۿ� ��ϵ� ���� ��� ��û
		ProtocolResponseGetUserList(pData);
		break;
	case ProtocolHeader::ResponseDelUser:			// ���� ����
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
