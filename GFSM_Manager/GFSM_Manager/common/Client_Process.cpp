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
	case ProtocolHeader::ResponseAdminLogin:		// 응답: 관리자 로그인
		ProcessResponseAdminLogin(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAdminLogin\n");
		break;
	case ProtocolHeader::ResponseAddNewSystem:		// 응답: 건물, 단지 등 시스템 추가
		ProcessResponseAddNewSystem(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAddNewSystem\n");
		break;
	case ProtocolHeader::ResponseModSystem:			// 응답: 시스템 수정
		ProcessResponseModSystem(pData, pPacket);
		TRACE("ProtocolHeader::ResponseModSystem\n");
		break;
	case ProtocolHeader::ResponseDelSystem:			// 응답: 시스템 삭제
		ProcessResponseDelSystem(pData, pPacket);
		TRACE("ProtocolHeader::ResponseDelSystem\n");
		break;
	case ProtocolHeader::ResponseGetSystemList:		// 응답: 시스템 목록 요청
		ProcessResponseGetSystemList(pData, pPacket);
		TRACE("ProtocolHeader::ResponseGetSystemList\n");
		break;
	case ProtocolHeader::ResponseGetManagerList:	// 응답: 관리자 목록 요청
		ProcessResponseGetManagerList(pData, pPacket);
		TRACE("ProtocolHeader::ResponseGetManagerList\n");
		break;
	case ProtocolHeader::ResponseAddManager:		// 응답: 관리자 추가
		ProcessResponseAddManager(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAddManager\n");
		break;
	case ProtocolHeader::ResponseModManager:		// 응답: 관리자 수정
		ProcessResponseModManager(pData, pPacket);
		TRACE("ProtocolHeader::ResponseModManager\n");
		break;
	case ProtocolHeader::ResponseDelManager:		// 응답: 관리자 삭제
		ProcessResponseDelManager(pData, pPacket);
		TRACE("ProtocolHeader::ResponseDelManager\n");
		break;
	case ProtocolHeader::ResponseGetUserList:		// 응답: 사용자 목록 요청
		ProcessResponseGetUserList(pData, pPacket);
		TRACE("ProtocolHeader::ResponseGetUserList\n");
		break;
	case ProtocolHeader::ResponseAddUser:			// 응답: 사용자 추가
		ProcessResponseAddUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseAddUser\n");
		break;
	case ProtocolHeader::ResponseModUser:			// 응답: 사용자 수정
		ProcessResponseModUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseModUser\n");
		break;
	case ProtocolHeader::ResponseDelUser:			// 응답: 사용자 삭제
		ProcessResponseDelUser(pData, pPacket);
		TRACE("ProtocolHeader::ResponseDelUser\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseAdminLogin(BYTE* pData, BYTE* pPacket)		// 응답: 관리자 로그인
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult > 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAdminLogin, true);
		CCommonState::Instance()->m_nIdx = pRes->nResult;
		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAdminLogin + 100, 0);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAdminLogin, false);
	}
	CCommonState::Instance()->m_bLoginResult = true;
}

void Client::ProcessResponseAddNewSystem(BYTE* pData, BYTE* pPacket)	// 응답: 건물, 단지 등 시스템 추가
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddNewSystem, true);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddNewSystem + 100, 0);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddNewSystem, false);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddNewSystem + 100, pRes->nResult);
	}
}

void Client::ProcessResponseModSystem(BYTE* pData, BYTE* pPacket)		// 응답: 시스템 수정
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 0) {
		//CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModSystem, true);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseModSystem + 100, 0);
	}
	else {
		//CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModSystem, false);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseModSystem + 100, pRes->nResult);
	}
}

void Client::ProcessResponseDelSystem(BYTE* pData, BYTE* pPacket)		// 응답: 시스템 삭제
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelSystem, true);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseDelSystem + 100, pRes->nResult);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelSystem, false);

		CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseDelSystem + 100, pRes->nResult);
	}
}

void Client::ProcessResponseGetSystemList(BYTE* pData, BYTE* pPacket)	// 응답: 시스템 목록 요청
{
	ProtocolResponseGetSystemList* pSystemList = (ProtocolResponseGetSystemList*) pData;
	if (pSystemList->nCount == 0) {
		return;
	}
	int nSystem = 0;
	CString sValue;
	/*
	1. nSequenceNo;
	2. szSystemName;
	3. szLocation;
	4. szEtc;
	5. szRegdate;
	*/
	CWorksiteManager::Instance()->ReleaseList();

	TRACE("ProcessResponseGetSystemList start(%d)\n", GetTickCount());

	workSite site;
	WCHAR *szList, *pTemp, *pValue;
	szList = new WCHAR[pSystemList->size + 1024];
	wcscpy_s(szList, pSystemList->size + 1024, CCommonFunc::Utf8ToWCHAR(pSystemList->szSystemInfo));
	int nPoint = 0;

	pTemp = szList;
	for (int nIndex = 0; nIndex < pSystemList->nCount; nIndex++)
	{
		for (int i = 0; i < 5; i++) {
			pValue = wcsstr(pTemp, L";");
			if (!pValue) {
				SAFE_DELETE(szList);
				return;
			}
			pValue[0] = L'\0';

			switch (i)
			{
			case 0:
				site.nSequenceNo = _wtoi(pTemp);
				break;
			case 1:
				_tcscpy_s(site.szSystemName, pTemp);
				break;
			case 2:
				_tcscpy_s(site.szLocation, pTemp);
				break;
			case 3:
				_tcscpy_s(site.szEtc, pTemp);
				break;
			case 4:
				_tcscpy_s(site.szRegdate, pTemp);
				CWorksiteManager::Instance()->InsertWorksite(&site);
				break;
			}
			pTemp = &pValue[1];
		}
	}
	TRACE("ProcessResponseGetSystemList complete(%d)\n", GetTickCount());
	SAFE_DELETE(szList);

	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseGetSystemList + 100, 0);
}

void Client::ProcessResponseGetManagerList(BYTE* pData, BYTE* pPacket)	// 응답: 관리자 목록 요청
{
	ProtocolResponseGetManagerList* pList = (ProtocolResponseGetManagerList*)pData;
	int nCount = pList->nCount;

	CManagerManager::Instance()->ReleaseList();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CManagerManager::Instance()->InsertManager(&pList->info[nIndex]);
	}
	CCommonState::Instance()->SetResult(ProtocolHeader::ResponseGetManagerList, true);

	CCommonState::Instance()->m_pManagerWnd->PostMessage(REDISPLAY_MSG, 0, 0);
}

void Client::ProcessResponseAddManager(BYTE* pData, BYTE* pPacket)		// 응답: 관리자 추가
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 0) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddManager, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAddManager, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseAddManager + 100, pRes->nResult);
}

void Client::ProcessResponseModManager(BYTE* pData, BYTE* pPacket)		// 응답: 관리자 수정
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModManager, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseModManager, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseModManager + 100, pRes->nResult);
}

void Client::ProcessResponseDelManager(BYTE* pData, BYTE* pPacket)		// 응답: 관리자 삭제
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;
	if (pRes->nResult == 1) {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelManager, true);
	}
	else {
		CCommonState::Instance()->SetResult(ProtocolHeader::ResponseDelManager, false);
	}
	CCommonState::Instance()->m_pWnd->PostMessage(SOCKET_RECV, ProtocolHeader::ResponseDelManager + 100, pRes->nResult);
}

void Client::ProcessResponseGetUserList(BYTE* pData, BYTE* pPacket)		// 응답: 사용자 목록 요청
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

void Client::ProcessResponseAddUser(BYTE* pData, BYTE* pPacket)	 		// 응답: 사용자 추가
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

void Client::ProcessResponseModUser(BYTE* pData, BYTE* pPacket)	 		// 응답: 사용자 수정
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

void Client::ProcessResponseDelUser(BYTE* pData, BYTE* pPacket) 		// 응답: 사용자 삭제
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
