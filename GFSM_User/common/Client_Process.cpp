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
	case ProtocolHeader::ResponseLogin:		// ����: ������ �α���
		ProcessResponseManagerLogin(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAdminLogin\n");
		break;
	case ProtocolHeader::ResponseManagerInfo:
		ProcessResponseManagerInfo(pData, pPacket);
		TRACE("ProtocolHeader::ResponseManagerInfo\n");
		break;
	case ProtocolHeader::ResponseGetUserList:		// ����: ����� ��� ��û
		ProcessResponseGetUserList(pData, pPacket);
		TRACE("ProtocolHeader::ResponseGetUserList\n");
		break;
	case ProtocolHeader::ResponseAddUser:			// ����: ����� �߰�
		ProcessResponseAddUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAddUser\n");
		break;
	case ProtocolHeader::ResponseModUser:			// ����: ����� ����
		ProcessResponseModUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseModUser\n");
		break;
	case ProtocolHeader::ResponseDelUser:			// ����: ����� ����
		ProcessResponseDelUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseDelUser\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseManagerLogin(BYTE* pData, BYTE* pPacket)		// ����: ������ �α���
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult > 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestLogin, true);
		CCommonState::Instance()->m_nIdx = pRes->nResult;
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::RequestLogin + 100, 0);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestLogin, false);
	}
	CCommonState::Instance()->m_bLoginResult = true;
}

void Client::ProcessResponseManagerInfo(BYTE* pData, BYTE* pPacket)		// ����: ������ �α���
{
	ProtocolResponseManagerInfo* pRes = (ProtocolResponseManagerInfo*)pData;
	if (pRes->nResult > 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestManagerInfo, true);
		CCommonState::Instance()->m_nWorksiteSeq = pRes->nWorkSiteSeq;
		CCommonState::Instance()->m_nLimitCount = pRes->nLimitUser;
		CCommonState::Instance()->m_nIdx = pRes->nManagerSeq;
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::RequestManagerInfo + 100, 0);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::RequestManagerInfo, false);
	}
	CCommonState::Instance()->m_bLoginResult = true;
}

void Client::ProcessResponseGetUserList(BYTE* pData, BYTE* pPacket)		// ����: ����� ��� ��û
{
	ProtocolResponseGetUserList* pList = (ProtocolResponseGetUserList*)pData;
	int nCount = pList->nCount;

	CUserManager::Instance()->ReleaseList();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CUserManager::Instance()->InsertUser(&pList->info[nIndex]);
	}
	CCommonState::Instance()->SetResult(ProtocolHeader::ResponseGetUserList, true);

	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 0, 0);
}

void Client::ProcessResponseAddUser(BYTE* pData, BYTE* pPacket)	 		// ����: ����� �߰�
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddUser, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddUser, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddUser + 100, pRes->nResult);
}

void Client::ProcessResponseModUser(BYTE* pData, BYTE* pPacket)	 		// ����: ����� ����
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModUser, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModUser, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseModUser + 100, pRes->nResult);
}

void Client::ProcessResponseDelUser(BYTE* pData, BYTE* pPacket) 		// ����: ����� ����
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelUser, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelUser, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseDelUser + 100, pRes->nResult);
}
