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

int g_nValue = 0;
void Client::WorkerProcessRecvPacket(BYTE* pPacket, int nSize)
{
	BYTE* buffer = m_recvBuffer, *pData = NULL;

	ProtocolHeader header;
	memcpy_s(&header, sizeof(ProtocolHeader), buffer, sizeof(ProtocolHeader));
	if (header.size > nSize) {
		g_nValue += header.size;
		TRACE("%d / %d\n", header.size, nSize);
		return;
	}
	if (header.size > 0) {
		pData = buffer;
	}
	if (header.protocol < ProtocolHeader::Keep_Alive || header.protocol >= ProtocolHeader::DefineEndProtocol)
	{
		return;
	}
	switch (header.protocol)
	{
	case ProtocolHeader::ResponseAddEvent:
		ProcessResponseAddEvent(pData, pPacket);

		TRACE("ProtocolHeader::ResponseAddEvent\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseAddEvent(BYTE* pData, BYTE* pPacket)
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;

	if (pRes->nResult == 1) {
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddEvent + 100, 1);
	}
	else
	{
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddEvent + 100, 0);
	}

	CCommonState::Instance()->m_bAddEventResult = true;
}
