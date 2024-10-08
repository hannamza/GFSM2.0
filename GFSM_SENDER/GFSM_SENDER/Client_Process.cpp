#include "stdafx.h"
#include "Client.h"
#include "ClientMan.h"

#include "Log.h"
#include "Network.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/pool/singleton_pool.hpp>
#include "GFSM_SENDERDlg.h"

using namespace std;

void Client::WorkerProcessRecvPacket(BYTE* pPacket, int nSize)
{
	BYTE* buffer = m_recvBuffer, *pData = NULL;

	USHORT command, size;
	memcpy_s(&command, sizeof(USHORT), buffer, sizeof(USHORT));
	memcpy_s(&size, sizeof(USHORT), &buffer[sizeof(USHORT)], sizeof(USHORT));
	if (size > 0)
	{
		pData = buffer;
	}
	if (command < ProtocolHeader::Keep_Alive || command >= ProtocolHeader::DefineEndProtocol)
	{
		return;
	}
	switch (command)
	{
	case ProtocolHeader::Keep_Alive:
		CCommonState::Instance()->m_dwLastRecv = GetTickCount();
		break;
	case ProtocolHeader::ResponseLogin:
		ProcessResponseLogin(pData);
		CCommonState::Instance()->m_dwLastRecv = GetTickCount();
		TRACE("ProtocolHeader::ResponseLogin\n");
		break;
	case ProtocolHeader::ResponseAddEvent:
		TRACE("ProtocolHeader::ResponseAddEvent\n");
		CCommonState::Instance()->m_dwLastRecv = GetTickCount();
		break;
	case ProtocolHeader::ResponseGetUserTokenList:
		ProcessResponseGetUserTokenList(pData);
		CCommonState::Instance()->m_dwLastRecv = GetTickCount();
		TRACE("ProtocolHeader::ResponseGetUserTokenList\n");
		break;
	case ProtocolHeader::ResponseGetFacpType:						// 수신기 타입
		ProcessResponseGetFacpType(pData);
		CCommonState::Instance()->m_dwLastRecv = GetTickCount();
		TRACE("ProtocolHeader::ProcessResponseGetFacpType\n");
		break;
	default:
		break;
	}

	m_recvBuffer[nSize] = '\0';
}

void Client::ProcessResponseGetUserTokenList(BYTE* pData)		// 응답: 사용자 목록 요청
{
	ProtocolResponseGetUserTokenList* pList = (ProtocolResponseGetUserTokenList*)pData;
	int nCount = pList->nCount;

	CReadWriteState state;
	state.SetFileName(L"Setup.ini");
	if (!state.WriteState(L"ID", L"ID_CNT", nCount)) {
		return;
	}

	CString strField_Name;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		/*strField_Name.Format(L"ID%d", nIndex + 1);
		state.WriteState(L"ID", strField_Name, CCommonFunc::Utf8ToWCHAR(pList->info[nIndex].szToken));
		state.WriteState(L"ID", strField_Name, CCommonFunc::Utf8ToWCHAR(pList->info[nIndex].szToken));*/
		strField_Name.Format(L"ID%d", nIndex + 1);
		state.WriteState(strField_Name, L"TOKEN", CCommonFunc::Utf8ToWCHAR(pList->info[nIndex].szToken));
		state.WriteState(strField_Name, L"TYPE", pList->info[nIndex].nMobileType);
		state.WriteState(strField_Name, L"FIRE", pList->info[nIndex].nFire);
		state.WriteState(strField_Name, L"GAS", pList->info[nIndex].nGas);
		state.WriteState(strField_Name, L"SPY", pList->info[nIndex].nSpy);
		state.WriteState(strField_Name, L"LINE", pList->info[nIndex].nLine);
		state.WriteState(strField_Name, L"ALERT", pList->info[nIndex].nAlert);
		state.WriteState(strField_Name, L"HOUR", pList->info[nIndex].nHour);
		state.WriteState(strField_Name, L"MIN", pList->info[nIndex].nMin);
		state.WriteState(strField_Name, L"ENDHOUR", pList->info[nIndex].nEndHour);
		state.WriteState(strField_Name, L"ENDMIN", pList->info[nIndex].nEndMin);
		state.WriteState(strField_Name, L"USETIME", pList->info[nIndex].nUseTime);
		state.WriteState(strField_Name, L"PHONE", CCommonFunc::Utf8ToWCHAR(pList->info[nIndex].szPhoneNo));
	}

	if (CEventSend::Instance()) {
		CEventSend::Instance()->InitEvent();
	}
	CCommonState::Instance()->m_dwToken = GetTickCount();
}

void Client::ProcessResponseLogin(BYTE* pData)
{
	ProtocolResponse* pReq = (ProtocolResponse*)pData;

	int idx = pReq->nResult;
	if (idx == 0)
	{
		Log::Trace("로그인 실패");
	}
	else
	{
		Log::Trace("로그인 성공");
		CCommonState::Instance()->m_nIdx = idx;
	}
	CCommonState::Instance()->m_bLoginResult = true;
}

void Client::ProcessResponseAddEvent(BYTE* pData)
{
	ProtocolResponse* pRes = (ProtocolResponse*)pData;

	int idx = pRes->nResult;
	if (idx == 0)
	{
		Log::Trace("이벤트 저장 실패");
	}
}

void Client::ProcessResponseGetFacpType(BYTE* pData)
{
	ProtocolResponseGetFacpType* pRes = (ProtocolResponseGetFacpType*)pData;

	int nFacpType = pRes->nFacpType;
	if (nFacpType >= MANAGER_FACP_TYPE_F3_KOREAN && nFacpType <= MANAGER_FACP_TYPE_GT1_ENGLISH)
	{
		CCommonState::Instance()->m_nFacpType = nFacpType;
		switch (nFacpType)
		{
		case MANAGER_FACP_TYPE_F3_KOREAN:
		{
			Log::Trace("수신기 타입 F3 / 한글 - 이벤트 이력과 알람 모두 전송합니다.");
			break;
		}
		case MANAGER_FACP_TYPE_GT1_KOREAN:
		{
			Log::Trace("수신기 타입 GT1 / 한글 - 이벤트 이력은 전송하지만 알람은 전송하지 않습니다.");
			break;
		}
		case MANAGER_FACP_TYPE_F3_ENGLISH:
		{
			Log::Trace("수신기 타입 F3 / 영어 - 이벤트 이력과 알람 모두 전송합니다.");
			break;
		}
		case MANAGER_FACP_TYPE_GT1_ENGLISH:
		{
			Log::Trace("수신기 타입 GT1 / 영어 - 이벤트 이력은 전송하지만 알람은 전송하지 않습니다.");
			break;
		}
		default:
		{
			Log::Trace("알 수 없는 수신기 타입입니다.");
			break;
		}
		}
	}
	else
	{
		CCommonState::Instance()->m_nFacpType = 0;
		Log::Trace("FacpType Out Of Range : %d", nFacpType);
	}
	
}
