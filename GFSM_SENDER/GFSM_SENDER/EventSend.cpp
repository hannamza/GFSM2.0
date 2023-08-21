#include "stdafx.h"
#include "EventSend.h"

#include <Wininet.h>

#pragma comment(lib, "wininet.lib")

#define		SEND_DELAY		500

bool g_bThread = false;
void func_thread(CEventSend* pWnd)
{
	// 쓰레드 메인
	while (g_bThread)
	{
		TIME_SLEEP(1000, g_bThread);
		pWnd->SendThreadLoop();
	}
}

char* qURLencode(char *str)
{
	char *encstr, buf[2 + 1];
	unsigned char c;
	int i, j;

	if (str == NULL) return NULL;
	if ((encstr = (char *)malloc((strlen(str) * 3) + 1)) == NULL)
		return NULL;

	for (i = j = 0; str[i]; i++)
	{
		c = (unsigned char)str[i];
		if ((c >= '0') && (c <= '9')) encstr[j++] = c;
		else if (c == ' ') encstr[j++] = '+';    // 웹에 맞게 추가
		else if ((c >= 'A') && (c <= 'Z')) encstr[j++] = c;
		else if ((c >= 'a') && (c <= 'z')) encstr[j++] = c;
		else if ((c == '@') || (c == '.') || (c == '/') || (c == '\\')
			|| (c == '-') || (c == '_') || (c == ':'))
			encstr[j++] = c;
		else
		{
			sprintf(buf, "%02x", c);
			encstr[j++] = '%';
			encstr[j++] = buf[0];
			encstr[j++] = buf[1];
		}
	}
	encstr[j] = '\0';

	return encstr;
}

typedef struct {
	CEventSend* pDlg;
	int userIndex;
	CString sTitle;
	char* pSendData;
	char* szBody;
	char* szTitle;
	BOOL bJason;
} ALARM_INFO;

UINT SendAlarmThread(LPVOID param)
{
	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, endTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	ALARM_INFO* pAi;
	pAi = (ALARM_INFO*)param;
	CEventSend* pDlg = pAi->pDlg;
	int userIndex = pAi->userIndex;
	char* pSendData = pAi->pSendData;
 	CString sTitle = pAi->sTitle;
	char* szBody = pAi->szBody;
	char* szTitle = pAi->szTitle;
 	BOOL bJason = pAi->bJason;

	int nLen;
	char* mID = NULL;

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	char* szSendData = new char[4000];
	memset(szSendData, 0, 4000);
	char* strUtf8 = new char[4000];
	memset(strUtf8, 0, 4000);

	currTime = CTime::GetCurrentTime();

	//

	HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hConnect == NULL)
	{
		//SAFE_DELETE(pData);
		return 0;
	}

	HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hHttp == NULL)
	{
		InternetCloseHandle(hConnect);
		hHttp = NULL;
		//SAFE_DELETE(pData);
		return 0;
	}

	HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
		INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hReq == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hHttp);
		//SAFE_DELETE(pData);
		return 0;
	}

	//
	userInfo* pInfo = pDlg->m_list.GetAt(pDlg->m_list.FindIndex(userIndex));
	if (!pInfo) {
		return 0;
	}
	if (pInfo->nUseTime) {
		if (pInfo->nHour > currTime.GetHour() || pInfo->nEndHour < currTime.GetHour()
			|| (pInfo->nHour == currTime.GetHour() && pInfo->nMin > currTime.GetMinute())
			|| (pInfo->nEndHour == currTime.GetHour() && pInfo->nEndMin < currTime.GetMinute()))
		{
			return 0;
		}
	}
	if (!pInfo->nAlert) {
		return 0;
	}
	if (sTitle.Find(L"화재") >= 0 && pInfo->nFire == 0) {
		return 0;
	}
	if (sTitle.Find(L"가스") >= 0 && pInfo->nGas == 0) {
		return 0;
	}
	if (sTitle.Find(L"감시") >= 0 && pInfo->nSpy == 0) {
		return 0;
	}
	if (sTitle.Find(L"단선") >= 0 && pInfo->nLine == 0) {
		return 0;
	}

	if ("" != pInfo->szToken)//m_IDList[i])
	{
		if (!bJason) {
			strHeader += "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
		}
		else {
			strHeader += "Content-Type:application/json";
		}
		strHeader += "\r\n";
		strHeader += "Authorization:key=";
		strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
		strHeader += "\r\n\r\n";

		HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

		//
		int nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/pInfo->szToken, lstrlenW(pInfo->szToken), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, pInfo->szToken, lstrlenW(pInfo->szToken), strUtf8, nLen, NULL, NULL);

		char* mID = qURLencode(strUtf8);

		if (!bJason) {
			sprintf_s(szSendData, 4000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, mID);
		}
		else {			
			/*sprintf_s(szSendData, 4000, "{\"registration_ids\":[\"%s\"], \"priority\": \"high\", \*/
			sprintf_s(szSendData, 4000, "{\"to\":\"%s\", \"priority\": \"high\", \
							\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\", \"sound\" : \"default\"},\
							\"data\" : {\"event\":\"%s\"},\
								\"android\" : {	\"priority\":\"high\"},\
									\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
										\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
				, mID, szBody, szTitle, pSendData);

			Log::Trace("%d스레드 : %s", userIndex, szSendData);
		}
		//

		BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
		Log::Trace("%d 스레드 FCM Push Message 처리 완료! - 결과 : %d", userIndex, bSend);

		delete mID;
	}

	::InternetCloseHandle(hReq);
	::InternetCloseHandle(hHttp);
	::InternetCloseHandle(hConnect);
	//

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	double duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("%d 스레드 FCM Push Message 처리 시간 : %f", userIndex, duringTime);
#endif
	//20230320 GBM end

	//
	delete[] szSendData;
	delete[] strUtf8;
	//

	SetEvent(pDlg->m_hThread[userIndex]);

	//SwitchToThread();
	Sleep(1);

	return 0;
}

CEventSend::CEventSend()
{
	m_pThread = NULL;
	m_pWnd = NULL;
	m_nIDCount = 0;

	m_dwLine = 0;
	m_dwGas = 0;
	m_dwSpy = 0;

	m_nSendCount = 0;
}

CEventSend::~CEventSend()
{
	StopSendEventThread();

	m_sync.Enter();
	BYTE* pData;
	while (!m_sendQueue.empty())
	{
		pData = m_sendQueue.front();
		//SAFE_DELETE(pData);

		m_sendQueue.pop();
	}
	m_sync.Leave();

	mIDSync.Enter();
	ReleaseListUser();
	mIDSync.Leave();
}

void CEventSend::ReleaseListUser()
{
	userInfo* pInfo = NULL;
	while (!m_list.IsEmpty())
	{
		pInfo = m_list.RemoveHead();
		SAFE_DELETE(pInfo);
	}
}

bool CEventSend::InitEvent()
{
	mIDSync.Enter();
	CReadWriteState state;
	state.SetFileName(L"Setup.ini");
	if (!state.ReadState(L"ID", L"ID_CNT", m_nIDCount)) {
		m_nIDCount = 0;
	}

	ReleaseListUser();

	CString strField_Name;
	CString sValue;
	userInfo* pInfo = NULL;
	for (int i = 0; i < m_nIDCount; i++)
	{
		pInfo = new userInfo;
		strField_Name.Format(L"ID%d", i + 1);
		state.ReadState(strField_Name, L"TOKEN", pInfo->szToken);
		state.ReadState(strField_Name, L"PHONE", pInfo->szPhoneNo);
		state.ReadState(strField_Name, L"TYPE", pInfo->nMobileType);
		state.ReadState(strField_Name, L"FIRE", pInfo->nFire);
		state.ReadState(strField_Name, L"GAS", pInfo->nGas);
		state.ReadState(strField_Name, L"SPY", pInfo->nSpy);
		state.ReadState(strField_Name, L"LINE", pInfo->nLine);
		state.ReadState(strField_Name, L"ALERT", pInfo->nAlert);
		state.ReadState(strField_Name, L"HOUR", pInfo->nHour);
		state.ReadState(strField_Name, L"MIN", pInfo->nMin);
		state.ReadState(strField_Name, L"ENDHOUR", pInfo->nEndHour);
		state.ReadState(strField_Name, L"ENDMIN", pInfo->nEndMin);
		state.ReadState(strField_Name, L"USETIME", pInfo->nUseTime);

		m_list.AddTail(pInfo);
		//m_IDList[i] = sValue;
	}
	mIDSync.Leave();

	StartSendEventThread();
	return true;

	/*TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);
	CString strINI_Name;
	strINI_Name.Format(_T("%s\\Setup.ini"), szPath);
	CString strField_Name;
	WCHAR	szTemp[BUFF_SIZE];

	if (false == ::PathFileExists(strINI_Name))
	{
		return false;
	}
	GetPrivateProfileString(L"ID", L"ID_CNT", L"", szTemp, BUFF_SIZE, strINI_Name);
	m_nIDCount = _ttoi(szTemp);
	for (int i = 0; i < m_nIDCount; i++)
	{
		strField_Name.Format(L"ID%d", i + 1);
		memset(szTemp, 0x00, sizeof(szTemp));
		GetPrivateProfileString(L"ID", strField_Name, L"", szTemp, BUFF_SIZE, strINI_Name);
		m_IDList[i] = szTemp;
	}

	StartSendEventThread();
	return true;*/
}

void CEventSend::SendEvent(BYTE* pData)
{
	/*BYTE* pBuffer = new BYTE[16];
	memset(pBuffer, 0x00, sizeof(BYTE) * 16);
	memcpy_s(pBuffer, 13, pData, 13);*/

	m_sync.Enter();
	m_sendQueue.push(pData);
	m_sync.Leave();
}

void CEventSend::SetMainWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;
}

void CEventSend::StartSendEventThread()
{
	if (!g_bThread) {
		g_bThread = true;
		m_pThread = new thread(&func_thread, this);
	}
}

void CEventSend::StopSendEventThread()
{
	g_bThread = false;
	if (!m_pThread)
	{
		return;
	}
	m_pThread->join();

	SAFE_DELETE(m_pThread);
}

void CEventSend::SendThreadLoop()
{
	BYTE *pData;
	CTime   currTime;
	bool bQueue = false;

	CString strFACPNum;
	CString strUnitNum;
	CString strLoopNum;
	CString strLineNum;

	CString strType;
	CString strAddress;
	CString sName;

	m_dwLine = 0;
	m_dwGas = 0;
	m_dwSpy = 0;

	while (g_bThread)
	{
		CheckAndSend(); // 체크 후 푸시 알람 전송

		m_sync.Enter();
		bQueue = m_sendQueue.empty();
		m_sync.Leave();
		if (bQueue)
		{
			Sleep(1);
			continue;
		}
		m_sync.Enter();
		pData = m_sendQueue.front();
		m_sendQueue.pop();
		m_sync.Leave();

		strFACPNum.Format(L"%c%c", pData[3], pData[4]);
		strUnitNum.Format(L"%c%c", pData[5], pData[6]);
		strLoopNum.Format(L"%c", pData[8]);
		strLineNum.Format(L"%c%c%c", pData[9], pData[10], pData[11]);

		strAddress.Format(L"%02s%02s%s%03s", strFACPNum, strUnitNum, strLoopNum, strLineNum);
		strType = CDeviceInfo::Instance()->GetDeviceName(strAddress).Left(1);
		sName = CDeviceInfo::Instance()->GetDeviceName(strAddress).Mid(3);

		currTime = CTime::GetCurrentTime();

		if ('R' == pData[2]) // 수신기복구
		{
			SendAll(); // 이전 내역 모두 전송
			m_fixQueue.push(pData);
			m_dwFix = GetTickCount();
			ProcessEventQueue(m_fixQueue, m_dwFix, true);
		}
		else
		{
			if ('F' != pData[2] &&   //화재
				'G' != pData[2] &&   //가스
				'S' != pData[2] &&   //감시
				'T' != pData[2])     //단선
			{
				//SAFE_DELETE(pData);
				continue;
			}
			strUnitNum.Format(L"%c%c", pData[5], pData[6]);

			if ('T' == pData[2]) // 단선
			{
				m_lineQueue.push(pData);
				m_dwLine = GetTickCount();
				//ProcessEventQueue(m_lineQueue, m_dwLine, true); // test++
			}
			else if ('F' == pData[2])
			{
				if (strType == "0") // 화재
				{
					SendAll();
					m_fireQueue.push(pData);
					m_dwFire = GetTickCount();
					ProcessEventQueue(m_fireQueue, m_dwFire, true);
				}
				else // 감시
				{
					m_spyQueue.push(pData);
					m_dwSpy = GetTickCount();
					//ProcessEventQueue(m_spyQueue, m_dwSpy, true); // test++
				}

				if (L"63" == strUnitNum) // 감시
				{
					m_spyQueue.push(pData);
					m_dwSpy = GetTickCount();
					//ProcessEventQueue(m_spyQueue, m_dwSpy, true); // test++
				}
			}
			else if ('S' == pData[2]) // 감시
			{
				m_spyQueue.push(pData);
				m_dwSpy = GetTickCount();
				//ProcessEventQueue(m_spyQueue, m_dwSpy, true); // test++
			}
			else if ('G' == pData[2]) // 가스
			{
				m_gasQueue.push(pData);
				m_dwGas = GetTickCount();
				//ProcessEventQueue(m_gasQueue, m_dwGas, true); // test++
			}
			else
			{
				//SAFE_DELETE(pData);
				continue;
			}
		}

		//Sleep(1);	//20230403 GBM - test
	}
}

void CEventSend::ProcessEventQueue(queue<BYTE*> & queue, DWORD & dwValue, bool bSend /*= false*/)
{
	int nSize = queue.size();
	if (nSize == 0) {
		return;
	}
	char strUtf8[100*1024] = { 0, };
	memset(strUtf8, 0x00, 100 * 1024);
	CString strUni, sTitle, strName, strDisplay;
	int nLen;
	char szTemp[128];
	char szLog[512];
	BYTE* pData;
	BYTE* pDataSave = queue.front();

	int nSendCount = 0;
	while (!queue.empty()) {
		pData = queue.front();
		queue.pop();

		if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, 0)) {
			continue;
		}

		strcpy_s(szLog, CCommonFunc::WCharToChar(strUni.GetBuffer(0)));
		Log::Trace(szLog);

		if (!queue.empty()) {
			strUni += L"§";
		}
		memset(szTemp, 0x00, 128);
		nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), szTemp, nLen, NULL, NULL);
		
		strcat(strUtf8, szTemp);

		/*if (pDataSave != pData) {
			SAFE_DELETE(pData);
		}*/

		++nSendCount;

		if (strlen(strUtf8) + 256 >= 100 * 1024) {
			CClientInterface::Instance()->ProcessRequestAddEvent(CCommonState::Instance()->m_nIdx, CCommonState::Instance()->m_szID, strUtf8, nSendCount);
			nSendCount = 0;
			memset(strUtf8, 0x00, 100 * 1024);
			//Sleep(SEND_DELAY);
		}
	}
	if (strlen(strUtf8) > 0) {
		CClientInterface::Instance()->ProcessRequestAddEvent(CCommonState::Instance()->m_nIdx, CCommonState::Instance()->m_szID, strUtf8, nSendCount);
		nSendCount = 0;
		//Sleep(SEND_DELAY);
	}
	// 전송
	if (bSend) {
		if (GetTickCount() - CCommonState::Instance()->m_dwToken > 3000)
		{
			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			LARGE_INTEGER startTime, endTime;
			QueryPerformanceCounter(&startTime);
#endif
			//20230320 GBM end

			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserTokenList, CCommonState::Instance()->m_nIdx);

			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			QueryPerformanceCounter(&endTime);
			double duringTime;
			duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
			Log::Trace("사용자 리스트 요청에 걸린 시간 : %f", duringTime);
#endif
			//20230320 GBM end

			DWORD dwLastTime = GetTickCount();
			CCommonState::Instance()->m_dwToken = 0;
			while (g_bThread)
			{
				if (GetTickCount() - dwLastTime > 3000)
				{
					break;
				}
				if (CCommonState::Instance()->m_dwToken > 0)
				{
					break;
				}
				Sleep(1);
			}
		}

		if (m_nSendCount != LONG_MAX)
			InterlockedExchangeAdd(&m_nSendCount, 1);
		else
			m_nSendCount = 0;

		Log::Trace("SendCount = %d", m_nSendCount);

		//20230410 GBM start - 프로그램 기동 후 최초는 순차 전송 -> 20230420 GBM - 스레드간 토큰 값이 겹치는 현상을 버퍼 동적할당으로 해결 후 처음부터 병렬 전송으로 전송해도 OK ->
		//20230814 GBM - 사용자에게 전송을 사용자 별이 아닌 한번에 처리하는 함수
#if 1
		SendAlarmAtOnce(pDataSave, nSize - 1);
		//SendAlarmInParallel(pDataSave, nSize - 1);
#else
		SendAlarm(pDataSave, nSize - 1);
#endif
		//20230410 GBM end
	}

	dwValue = 0;

	//Sleep(1);
}

void CEventSend::CheckAndSend()
{
	BYTE* pData = NULL;
	// 단선
	int nSize = m_lineQueue.size();
	if ((m_dwLine > 0 && nSize > 0 && GetTickCount() - m_dwLine >= 3000) || nSize >= 1001) {
		ProcessEventQueue(m_lineQueue, m_dwLine, true);
	}
	// 가스
	nSize = m_gasQueue.size();
	if ((m_dwGas > 0 && nSize > 0 && GetTickCount() - m_dwGas >= 3000) || nSize >= 1001) {
		ProcessEventQueue(m_gasQueue, m_dwGas, true);
	}
	// 감시
	nSize = m_spyQueue.size();
	if ((m_dwSpy > 0 && nSize > 0 && GetTickCount() - m_dwSpy >= 3000) || nSize >= 1001) {
		ProcessEventQueue(m_spyQueue, m_dwSpy, true);
	}
}

void CEventSend::SendAll()
{
	BYTE* pData = NULL;
	// 단선
	int nSize = m_lineQueue.size();
	if (nSize > 0) {
		ProcessEventQueue(m_lineQueue, m_dwLine, true);
	}
	// 가스
	nSize = m_gasQueue.size();
	if (nSize > 0) {
		ProcessEventQueue(m_gasQueue, m_dwGas, true);
	}
	// 감시
	nSize = m_spyQueue.size();
	if (nSize > 0) {
		ProcessEventQueue(m_spyQueue, m_dwSpy, true);
	}
}

bool CEventSend::CheckClassify(BYTE* pData, CString & sUni, CString & sTitle, CString & sName, CString & sDisplay, int nSendCount)
{
	CString strFACPNum;
	CString strUnitNum;
	CString strLoopNum;
	CString strLineNum;

	CString strPosition;
	CString strFirst;
	CString strSecond;

	CString strAddress;
	CString strType;

	char szLog[512];
	CString sTemp;

	CTime   currTime = CTime::GetCurrentTime();

	if ('R' == pData[2])
	{
		sUni = L"R";
#ifndef ENGLISH_MODE
		sTitle = L"수신기 복구";
		sName = L"";
#else
		sTitle = L"FACP Restored";
		sName = L"";
#endif
	}
	else
	{
		if ('F' != pData[2] &&   //화재
			'G' != pData[2] &&   //가스
			'S' != pData[2] &&   //감시
			'T' != pData[2])     //단선
		{
			sUni = L"";
			//SAFE_DELETE(pData);
			return false;
		}

		strFACPNum.Format(L"%c%c", pData[3], pData[4]);	
		strUnitNum.Format(L"%c%c", pData[5], pData[6]);	
		strLoopNum.Format(L"%c", pData[8]);
		strLineNum.Format(L"%c%c%c", pData[9], pData[10], pData[11]);

		strAddress.Format(L"%02s%02s%s%03s", strFACPNum, strUnitNum, strLoopNum, strLineNum);
		strType = CDeviceInfo::Instance()->GetDeviceName(strAddress).Left(1);
		sName = CDeviceInfo::Instance()->GetDeviceName(strAddress).Mid(3);

		if ('R' == pData[2])
		{
#ifndef ENGLISH_MODE
			strPosition = L"수신기 복구";
#else
			strPosition = L"FACP Restored";
#endif
		}
		else if ('T' == pData[2])
		{
#ifndef ENGLISH_MODE
			strPosition = L"단선";
#else
			strPosition = L"Trouble";
#endif
			strSecond.Format(L"03x");
		}
		else if ('F' == pData[2])
		{
			if (strType == "0")
			{
#ifndef ENGLISH_MODE
				strPosition = L"화재";
#else
				strPosition = L"Alarm";
#endif
				strSecond.Format(L"00x");
			}
			else
			{
#ifndef ENGLISH_MODE
				strPosition = L"감시";
#else
				strPosition = L"Supervisory";
#endif
				strSecond.Format(L"02x");
			}

			if (L"63" == strUnitNum)
			{
#ifndef ENGLISH_MODE
				strPosition = L"감시";
#else
				strPosition = L"Supervisory";
#endif
				strSecond.Format(L"02x");
			}
		}
		else if ('S' == pData[2])
		{
#ifndef ENGLISH_MODE
			strPosition = L"감시";
#else
			strPosition = L"Supervisory";
#endif
			strSecond.Format(L"02x");
		}
		else if ('G' == pData[2])
		{
#ifndef ENGLISH_MODE
			strPosition = L"가스";
#else
			strPosition = L"Gas";
#endif
			strSecond.Format(L"01x");
		}
		else
		{
			return false;
		}

		sTitle = strPosition;
		if (pData[12] == 'N')
		{
#ifndef ENGLISH_MODE
			if (sTitle != L"수신기 복구") {
				sTitle += L" 발생";
			}
#endif
			strFirst.Format(L"Ax");
		}
		else
		{
#ifndef ENGLISH_MODE
			if (sTitle != L"수신기 복구") {
				sTitle += L" 복구";
			}
#else
			if (sTitle != L"FACP Restored") {
				sTitle += L" Restored";
			}
#endif
			strFirst.Format(L"Dx");
		}

		sUni.Format(L"%s%s%d/%02d/%02d %02d:%02d:%02dx%s-%s-%s-%sx%s",
			strFirst, strSecond,
			currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond(),
			strFACPNum, strUnitNum, strLoopNum, strLineNum,
			sName);
	}

	sTemp = L"";
	if (nSendCount >= 1) {
		sTemp.Format(L" (외 %d건)", nSendCount);
	}

	sTitle += sTemp;

	if ("R" == sUni)
	{
#ifndef ENGLISH_MODE
		sUni.Format(L"Rx%d/%02d/%02d %02d:%02d:%02dx수신기 복구", currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());
#else
		sUni.Format(L"Rx%d/%02d/%02d %02d:%02d:%02dxFACP Restored", currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());
#endif
		strcpy_s(szLog, CCommonFunc::WCharToChar(sUni.GetBuffer(0)));
		//Log::Trace(szLog);
	}
	else
	{
		strcpy_s(szLog, CCommonFunc::WCharToChar(sDisplay.GetBuffer(0)));
		//Log::Trace(szLog);
	}

	return true;
}

void CEventSend::SendAlarm(BYTE* pData, int nSendCount)
{
	//20233020 GBM start - test 1:초기화 루틴 분리, 2:기존 루틴 -> 20230420 GBM - 20인 테스트 시 잘 안되서 순차처리 방식으로 회귀
#if 0
	//
	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, roopStartTime, initEndTime, endTime;
	double duringTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	char szSendData[4000];
	
	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID;
	char szTitle[64], szBody[128];

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	memset(szSendData, 0, 4000);
	memset(strUtf8, 0x00, 4000);

	currTime = CTime::GetCurrentTime();

	CString strUni, sTitle, strName, strDisplay;

	if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, nSendCount)) {
		//SAFE_DELETE(pData);
		//20230320 GBM start - test
		Log::Trace("SendAlarm - CheckClassify 실패!");
		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("pData : [%s]", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
		//20230320 GBM end
		return;
	}

	//---------------------------------------------------------------------------------------
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

	BOOL bJason = true;
	if (bJason) {
		strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
		strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
	}

	mIDSync.Enter();
	int nCount = m_list.GetCount();
	mIDSync.Leave();

	// utf8 -> urlencode
	pSendData = qURLencode(strUtf8);

	mIDSync.Enter();
	nCount = m_list.GetCount();
	userInfo* pInfo = NULL;
	
	//

	HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hConnect == NULL)
	{
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hHttp == NULL)
	{
		InternetCloseHandle(hConnect);
		hHttp = NULL;
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	//
	HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
		INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hReq == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hHttp);
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}
	//

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&initEndTime);
	duringTime = CCommonFunc::GetPreciseTime(startTime, initEndTime);
	Log::Trace("FCM Push Message 초기화 시간 : %f", duringTime);
#endif
	//20230320 GBM end
	//

	for (int i = 0; i < nCount; i++)
	{
		pInfo = m_list.GetAt(m_list.FindIndex(i));
		if (!pInfo) {
			continue;
		}
		if (pInfo->nUseTime) {
			if (pInfo->nHour > currTime.GetHour() || pInfo->nEndHour < currTime.GetHour()
				|| (pInfo->nHour == currTime.GetHour() && pInfo->nMin > currTime.GetMinute())
				|| (pInfo->nEndHour == currTime.GetHour() && pInfo->nEndMin < currTime.GetMinute()))
			{
				continue;
			}
		}
		if (!pInfo->nAlert) {
			continue;
		}
		if (sTitle.Find(L"화재") >= 0 && pInfo->nFire == 0) {
			continue;
		}
		if (sTitle.Find(L"가스") >= 0 && pInfo->nGas == 0) {
			continue;
		}
		if (sTitle.Find(L"감시") >= 0 && pInfo->nSpy == 0) {
			continue;
		}
		if (sTitle.Find(L"단선") >= 0 && pInfo->nLine == 0) {
			continue;
		}

		if ("" != pInfo->szToken)//m_IDList[i])
		{
			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			QueryPerformanceCounter(&roopStartTime);
#endif
			//20230320 GBM end

			//
			if (!bJason) {
				strHeader += "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
			}
			else {
				strHeader += "Content-Type:application/json";
			}
			strHeader += "\r\n";
			strHeader += "Authorization:key=";
			strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
			strHeader += "\r\n\r\n";

			HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
			//

			//
			nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/pInfo->szToken, lstrlenW(pInfo->szToken), NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, pInfo->szToken, lstrlenW(pInfo->szToken), strUtf8, nLen, NULL, NULL);

			mID = qURLencode(strUtf8);

			if (!bJason) {
				sprintf_s(szSendData, 4000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, mID);
			}
			else {
				sprintf_s(szSendData, 4000, "{\"to\":\"%s\", \"priority\": \"high\", \
						\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\"},\
						\"data\" : {\"event\":\"%s\"},\
							\"android\" : {\"priority\":\"high\"},\
								\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
									\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
					, mID, szBody, szTitle, pSendData);
			}
			//

			BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
			Log::Trace("사용자 %d FCM Push Message 처리 완료! - 결과 : %d", i, bSend);

			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			QueryPerformanceCounter(&endTime);
			duringTime = CCommonFunc::GetPreciseTime(roopStartTime, endTime);
			Log::Trace("사용자 %d, FCM Push Message 처리 시간 : %f", i, duringTime);
#endif
			//20230320 GBM end
		}
	}

	free(mID);
	::InternetCloseHandle(hReq);
	::InternetCloseHandle(hHttp);
	::InternetCloseHandle(hConnect);
	free(pSendData);

	//SAFE_DELETE(pData);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("FCM Push Message 전체 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

	mIDSync.Leave();
#else
	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, endTime;
	double duringTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	char szSendData[4000];

	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID;
	char szTitle[64], szBody[128];

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	memset(szSendData, 0, 4000);
	memset(strUtf8, 0x00, 4000);

	currTime = CTime::GetCurrentTime();

	CString strUni, sTitle, strName, strDisplay;

	if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, nSendCount)) {
		//SAFE_DELETE(pData);
		//20230320 GBM start - test
		Log::Trace("SendAlarm - CheckClassify 실패!");
		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("pData : [%s]", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
		//20230320 GBM end
		return;
	}

	//---------------------------------------------------------------------------------------
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

	BOOL bJason = true;
	if (bJason) {
		strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
		strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
	}

	mIDSync.Enter();
	int nCount = m_list.GetCount();
	mIDSync.Leave();

	// utf8 -> urlencode
	pSendData = qURLencode(strUtf8);

	mIDSync.Enter();
	nCount = m_list.GetCount();
	userInfo* pInfo = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pInfo = m_list.GetAt(m_list.FindIndex(i));
		if (!pInfo) {
			continue;
		}
		if (pInfo->nUseTime) {
			if (pInfo->nHour > currTime.GetHour() || pInfo->nEndHour < currTime.GetHour()
				|| (pInfo->nHour == currTime.GetHour() && pInfo->nMin > currTime.GetMinute())
				|| (pInfo->nEndHour == currTime.GetHour() && pInfo->nEndMin < currTime.GetMinute()))
			{
				continue;
			}
		}
		if (!pInfo->nAlert) {
			continue;
		}
		if (sTitle.Find(L"화재") >= 0 && pInfo->nFire == 0) {
			continue;
		}
		if (sTitle.Find(L"가스") >= 0 && pInfo->nGas == 0) {
			continue;
		}
		if (sTitle.Find(L"감시") >= 0 && pInfo->nSpy == 0) {
			continue;
		}
		if (sTitle.Find(L"단선") >= 0 && pInfo->nLine == 0) {
			continue;
		}

		if ("" != pInfo->szToken)//m_IDList[i])
		{
			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			LARGE_INTEGER startTime, endTime;
			QueryPerformanceCounter(&startTime);
#endif
			//20230320 GBM end

			HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hConnect == NULL)
			{
				mIDSync.Leave();
				//SAFE_DELETE(pData);
				return;
			}

			HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
			if (hHttp == NULL)
			{
				InternetCloseHandle(hConnect);
				hHttp = NULL;
				mIDSync.Leave();
				//SAFE_DELETE(pData);
				return;
			}

			HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
				INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
			if (hReq == NULL)
			{
				InternetCloseHandle(hConnect);
				InternetCloseHandle(hHttp);
				mIDSync.Leave();
				//SAFE_DELETE(pData);
				return;
			}

			if (!bJason) {
				strHeader += "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
			}
			else {
				strHeader += "Content-Type:application/json";
			}
			strHeader += "\r\n";
			strHeader += "Authorization:key=";
			strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
			strHeader += "\r\n\r\n";

			HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

			nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/pInfo->szToken, lstrlenW(pInfo->szToken), NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, pInfo->szToken, lstrlenW(pInfo->szToken), strUtf8, nLen, NULL, NULL);

			mID = qURLencode(strUtf8);

			if (!bJason) {
				sprintf_s(szSendData, 4000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, mID);
			}
			else {
				sprintf_s(szSendData, 4000, "{\"to\":\"%s\", \"priority\": \"high\", \
							\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\", \"sound\" : \"default\"},\
							\"data\" : {\"event\":\"%s\"},\
								\"android\" : {	\"priority\":\"high\"},\
									\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
										\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
					, mID, szBody, szTitle, pSendData);
			}
			BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
			Log::Trace("사용자 %d, HttpSendRequest 성공여부 : %d", i, bSend);

			free(mID);

			::InternetCloseHandle(hReq);
			::InternetCloseHandle(hHttp);
			::InternetCloseHandle(hConnect);

			//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
			QueryPerformanceCounter(&endTime);
			double duringTime;
			duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
			Log::Trace("사용자 %d, FCM Push Message 처리 시간 : %f", i, duringTime);
#endif
			//20230320 GBM end
		}
	}
	free(pSendData);

	//SAFE_DELETE(pData);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("FCM Push Message 전체 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

	mIDSync.Leave();
#endif
//20230320 GBM end
}

void CEventSend::SendAlarmInParallel(BYTE* pData, int nSendCount)
{
	//
	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, endTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	char szSendData[4000];

	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID = NULL;
	char szTitle[64], szBody[128];

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	memset(szSendData, 0, 4000);
	memset(strUtf8, 0x00, 4000);

	currTime = CTime::GetCurrentTime();

	CString strUni, sTitle, strName, strDisplay;

	if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, nSendCount)) {
		//SAFE_DELETE(pData);
		//20230320 GBM start - test
		Log::Trace("SendAlarm - CheckClassify 실패!");
		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("pData : [%s]", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
		//20230320 GBM end
		return;
	}

	//---------------------------------------------------------------------------------------
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

	BOOL bJason = true;
	if (bJason) {
		strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
		strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
	}

	mIDSync.Enter();
	int nCount = m_list.GetCount();
	mIDSync.Leave();

	// utf8 -> urlencode
	pSendData = qURLencode(strUtf8);

	mIDSync.Enter();
	nCount = m_list.GetCount();

	ALARM_INFO ai[USER_MAX_COUNT];
	for (int i = 0; i < nCount; i++)
	{
		m_hThread[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		ai[i].pDlg = this;
		ai[i].userIndex = i;
 		ai[i].sTitle = sTitle;
		ai[i].pSendData = pSendData;
		ai[i].szBody = szBody;
		ai[i].szTitle = szTitle;
 		ai[i].bJason = bJason;

		CWinThread* pThread = ::AfxBeginThread(SendAlarmThread, &ai[i]);
		Sleep(250);		// FCM에서 Push Message 처리 시간을 위해 순차적 Delay를 줘서 핸드폰 알람 수신 안정화

	}

	//스레드가 모두 종료되길 기다린다.
	DWORD dw = WaitForMultipleObjects(nCount, m_hThread, true, 15000 + (250 * nCount));		// 테스트에서 나온 가장 Worst Time이 15초, 각 사용자별로 250밀리세컨트 Sleep 반영 시간
	if (dw != WAIT_OBJECT_0)
	{
		Log::Trace("스레드 대기 실패! dw : %d", dw);
	}

	//각 스레드 처리가 모두 끝나면 각 핸들을 닫는다.
	for (int i = 0; i < nCount; i++)
	{
		CloseHandle(m_hThread[i]);
	}

	free(mID);
	free(pSendData);

	//SAFE_DELETE(pData);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	double duringTime;
	duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("FCM Push Message 전체 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

	mIDSync.Leave();
}

//20230814 GBM start - 사용자에게 전송을 사용자 별이 아닌 한번에 처리하는 함수
void CEventSend::SendAlarmAtOnce(BYTE* pData, int nSendCount)
{
#if 1

#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, roopStartTime, initEndTime, endTime;
	double duringTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	//기존에 4000에서 registration_ids에 최대 1000명까지 가능하므로 토큰 한 문자열이 163, 따옴표 둘, 공백까지 이므로 166 * 1000 + 4000의 크기로 정한다.
	char szSendData[170000];

	char szRegistrationIds[166000];
	memset(szRegistrationIds, 0, 166000);

	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID;
	char szTitle[64], szBody[128];

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	memset(szSendData, 0, 170000);
	memset(strUtf8, 0x00, 4000);

	currTime = CTime::GetCurrentTime();

	CString strUni, sTitle, strName, strDisplay;

	if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, nSendCount)) {
		//SAFE_DELETE(pData);
		//20230320 GBM start - test
		Log::Trace("SendAlarm - CheckClassify 실패!");
		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("pData : [%s]", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
		//20230320 GBM end
		return;
	}

	//---------------------------------------------------------------------------------------
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

	BOOL bJason = true;
	if (bJason) {
		strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
		strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
	}

	mIDSync.Enter();
	int nCount = m_list.GetCount();
	mIDSync.Leave();

	// utf8 -> urlencode
	pSendData = qURLencode(strUtf8);

	mIDSync.Enter();
	nCount = m_list.GetCount();
	userInfo* pInfo = NULL;

	//

	HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hConnect == NULL)
	{
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hHttp == NULL)
	{
		InternetCloseHandle(hConnect);
		hHttp = NULL;
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	//
	HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
		INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hReq == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hHttp);
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}
	//

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&initEndTime);
	duringTime = CCommonFunc::GetPreciseTime(startTime, initEndTime);
	Log::Trace("FCM Push Message 초기화 시간 : %f", duringTime);
#endif
	//20230320 GBM end
	//
	CString strRegistrationIDs = _T("");

	for (int i = 0; i < nCount; i++)
	{
		pInfo = m_list.GetAt(m_list.FindIndex(i));
		if (!pInfo) {
			continue;
		}
		if (pInfo->nUseTime) {
			if (pInfo->nHour > currTime.GetHour() || pInfo->nEndHour < currTime.GetHour()
				|| (pInfo->nHour == currTime.GetHour() && pInfo->nMin > currTime.GetMinute())
				|| (pInfo->nEndHour == currTime.GetHour() && pInfo->nEndMin < currTime.GetMinute()))
			{
				continue;
			}
		}
		if (!pInfo->nAlert) {
			continue;
		}
		if (sTitle.Find(L"화재") >= 0 && pInfo->nFire == 0) {
			continue;
		}
		if (sTitle.Find(L"가스") >= 0 && pInfo->nGas == 0) {
			continue;
		}
		if (sTitle.Find(L"감시") >= 0 && pInfo->nSpy == 0) {
			continue;
		}
		if (sTitle.Find(L"단선") >= 0 && pInfo->nLine == 0) {
			continue;
		}

		CString strTemp = _T("");
		if ("" != pInfo->szToken)//m_IDList[i])
		{
			strTemp = pInfo->szToken;

			nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/strTemp, lstrlenW(strTemp), NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, strTemp, lstrlenW(strTemp), strUtf8, nLen, NULL, NULL);

			mID = qURLencode(strUtf8);
			strTemp.Format(_T("%s"), CCommonFunc::CharToTCHAR(mID));

			//
			strRegistrationIDs += "\"";
			strRegistrationIDs += strTemp;
			strRegistrationIDs += "\"";

			if (nCount != i + 1)
				strRegistrationIDs += ", ";

			free(mID);
		}
	}

	nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/strRegistrationIDs, lstrlenW(strRegistrationIDs), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strRegistrationIDs, lstrlenW(strRegistrationIDs), szRegistrationIds, nLen, NULL, NULL);


	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&roopStartTime);
#endif
	//20230320 GBM end

	//
	if (!bJason) {
		strHeader += "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
	}
	else {
		strHeader += "Content-Type:application/json";
	}
	strHeader += "\r\n";
	strHeader += "Authorization:key=";
	strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
	strHeader += "\r\n\r\n";

	HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
	//

	if (!bJason) {
		sprintf_s(szSendData, 170000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, szRegistrationIds);
	}
	else {
		//지금은 위에서 토큰 배열에 들어가는 따옴표까지 qURLencode하는데 따옴표를 변환하지 않고 문자열에 붙이고 시도해 볼 것
		sprintf_s(szSendData, 170000, "{\"registration_ids\": [%s], \"priority\": \"high\", \
					\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\", \"sound\" : \"default\"},\
					\"data\" : {\"event\":\"%s\"},\
						\"android\" : {\"priority\":\"high\"},\
							\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
								\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
			, szRegistrationIds, szBody, szTitle, pSendData);
	}
	//

	BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
	Log::Trace("FCM Push Message 처리 완료! - 결과 : %d", bSend);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	duringTime = CCommonFunc::GetPreciseTime(roopStartTime, endTime);
	Log::Trace("FCM Push Message 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

	//20230816 GBM start - 응답 받기
	DWORD dwByteRead = 0;
	DWORD bufSize = 170000;
	char* pszBuf = NULL;
	CString sRecv = _T("");
	CString strTemp = _T("");
	BOOL bRead;

	pszBuf = (char*)malloc(sizeof(char) * bufSize + 1);
	memset(pszBuf, '\0', bufSize + 1);

	do 
	{
		bRead = InternetReadFile(hReq, pszBuf, bufSize, &dwByteRead);

		if (bRead && (dwByteRead > 0))
		{
			strTemp.Format(_T("%s"), CCommonFunc::CharToTCHAR(pszBuf));
			sRecv += strTemp;
			memset(pszBuf, '\0', bufSize + 1);
		}
 	} while ((bRead == TRUE) && (dwByteRead > 0));

	Log::Trace("FCM Response : %s", CCommonFunc::WCharToChar(sRecv.GetBuffer(0)));

	free(pszBuf);

	//1. 응답 문자열에 "failed_registration_ids"가 있는 지를 확인
	//test
	//sRecv = _T("aaaaafailed_registration_ids : [\"123\", \"456\", \"789\"]aaaaa");
	
	BOOL bFailed = FALSE;
	int nFRIPos = -1;
	int nStartBracketPos = -1;
	int nEndBracketPos = -1;
	CString strFRIs = _T("");

	nFRIPos = sRecv.Find(_T("failed_registration_ids"));
	if (nFRIPos != -1)
	{
		bFailed = TRUE;
	}

	if (bFailed)
	{
		//2. 없으면 넘어가고 있다면 "failed_registration_ids" 위치를 얻음

		//3. 2에서 얻어진 위치부터 '['가 가장 먼저 나오는 위치를 얻음
		nStartBracketPos = sRecv.Find(_T("["));

		//4. 3에서 얻어진 위치부터 ']'가 가장 먼저 나오는 위치를 얻음
		nEndBracketPos = sRecv.Find(_T("]"));

		//5. 2와 3에서 얻어진 위치 안에 들어가는 문자열을 잘라서 추출
		strFRIs = sRecv.Mid(nStartBracketPos, nEndBracketPos - nStartBracketPos + 1);

		//6. 위에서 HttpSendRequest 문자열 만드는 걸 이용해서 registration_ids 정보를 5에서 얻은 문자열로 채우고 다시 HttpSendRequest를 실행
		memset(szRegistrationIds, 0, 166000);
		nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/strFRIs, lstrlenW(strFRIs), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, strFRIs, lstrlenW(strFRIs), szRegistrationIds, nLen, NULL, NULL);

		memset(szSendData, 0, 170000);
		if (!bJason) {
			sprintf_s(szSendData, 170000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, strFRIs);
		}
		else {
			sprintf_s(szSendData, 170000, "{\"registration_ids\": %s, \"priority\": \"high\", \
					\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\", \"sound\" : \"default\"},\
					\"data\" : {\"event\":\"%s\"},\
						\"android\" : {\"priority\":\"high\"},\
							\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
								\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
				, szRegistrationIds, szBody, szTitle, pSendData);
		}
		//

		BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
		Log::Trace("failed_registration_ids : %s", CCommonFunc::WCharToChar(strFRIs.GetBuffer(0)));
		Log::Trace("FCM Push Message 재처리 완료! - 결과 : %d", bSend);
	}

	//20230816 GBM end

	//free(mID);
	::InternetCloseHandle(hReq);
	::InternetCloseHandle(hHttp);
	::InternetCloseHandle(hConnect);
	free(pSendData);

	//SAFE_DELETE(pData);

	mIDSync.Leave();

#else

#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	LARGE_INTEGER startTime, roopStartTime, initEndTime, endTime;
	double duringTime;
	QueryPerformanceCounter(&startTime);
#endif
	//20230320 GBM end

	char szSendData[4000];

	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID;
	char szTitle[64], szBody[128];

	CTime   currTime;
	CString strHeader;
	CString strInputType;
	CString sTemp;
	LPVOID lpOutputBuffer = NULL;

	DWORD dwLastTime = 0;

	memset(szSendData, 0, 4000);
	memset(strUtf8, 0x00, 4000);

	currTime = CTime::GetCurrentTime();

	CString strUni, sTitle, strName, strDisplay;

	if (!CheckClassify(pData, strUni, sTitle, strName, strDisplay, nSendCount)) {
		//SAFE_DELETE(pData);
		//20230320 GBM start - test
		Log::Trace("SendAlarm - CheckClassify 실패!");
		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("pData : [%s]", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
		//20230320 GBM end
		return;
	}

	//---------------------------------------------------------------------------------------
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

	BOOL bJason = true;
	if (bJason) {
		strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
		strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
	}

	mIDSync.Enter();
	int nCount = m_list.GetCount();
	mIDSync.Leave();

	// utf8 -> urlencode
	pSendData = qURLencode(strUtf8);

	mIDSync.Enter();
	nCount = m_list.GetCount();
	userInfo* pInfo = NULL;

	//

	HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hConnect == NULL)
	{
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hHttp == NULL)
	{
		InternetCloseHandle(hConnect);
		hHttp = NULL;
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}

	//
	HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
		INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hReq == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hHttp);
		mIDSync.Leave();
		//SAFE_DELETE(pData);
		return;
	}
	//

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&initEndTime);
	duringTime = CCommonFunc::GetPreciseTime(startTime, initEndTime);
	Log::Trace("FCM Push Message 초기화 시간 : %f", duringTime);
#endif
	//20230320 GBM end
	//
	CString strRegistrationIDs = _T("");

	for (int i = 0; i < nCount; i++)
	{
		pInfo = m_list.GetAt(m_list.FindIndex(i));
		if (!pInfo) {
			continue;
		}
		if (pInfo->nUseTime) {
			if (pInfo->nHour > currTime.GetHour() || pInfo->nEndHour < currTime.GetHour()
				|| (pInfo->nHour == currTime.GetHour() && pInfo->nMin > currTime.GetMinute())
				|| (pInfo->nEndHour == currTime.GetHour() && pInfo->nEndMin < currTime.GetMinute()))
			{
				continue;
			}
		}
		if (!pInfo->nAlert) {
			continue;
		}
		if (sTitle.Find(L"화재") >= 0 && pInfo->nFire == 0) {
			continue;
		}
		if (sTitle.Find(L"가스") >= 0 && pInfo->nGas == 0) {
			continue;
		}
		if (sTitle.Find(L"감시") >= 0 && pInfo->nSpy == 0) {
			continue;
		}
		if (sTitle.Find(L"단선") >= 0 && pInfo->nLine == 0) {
			continue;
		}

		if ("" != pInfo->szToken)//m_IDList[i])
		{
			//
			strRegistrationIDs += "\"";
			strRegistrationIDs += pInfo->szToken;
			strRegistrationIDs += "\"";

			if (nCount != i + 1)
				strRegistrationIDs += ", ";
		}
	}


	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&roopStartTime);
#endif
	//20230320 GBM end

	//
	if (!bJason) {
		strHeader += "Content-Type:application/x-www-form-urlencoded;charset=UTF-8";
	}
	else {
		strHeader += "Content-Type:application/json";
	}
	strHeader += "\r\n";
	strHeader += "Authorization:key=";
	strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
	strHeader += "\r\n\r\n";

	HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
	//

	//
	nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/strRegistrationIDs, lstrlenW(strRegistrationIDs), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strRegistrationIDs, lstrlenW(strRegistrationIDs), strUtf8, nLen, NULL, NULL);

	mID = qURLencode(strUtf8);

	if (!bJason) {
		sprintf_s(szSendData, 4000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, mID);
	}
	else {
		//지금은 위에서 토큰 배열에 들어가는 따옴표까지 qURLencode하는데 따옴표를 변환하지 않고 문자열에 붙이고 시도해 볼 것
		sprintf_s(szSendData, 4000, "{\"registration_ids\": [%s], \"priority\": \"high\", \
					\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\"},\
					\"data\" : {\"event\":\"%s\"},\
						\"android\" : {\"priority\":\"high\"},\
							\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
								\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
			, mID, szBody, szTitle, pSendData);
	}
	//

	BOOL bSend = HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
	Log::Trace("FCM Push Message 처리 완료! - 결과 : %d", bSend);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	duringTime = CCommonFunc::GetPreciseTime(roopStartTime, endTime);
	Log::Trace("FCM Push Message 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

	free(mID);
	::InternetCloseHandle(hReq);
	::InternetCloseHandle(hHttp);
	::InternetCloseHandle(hConnect);
	free(pSendData);

	//SAFE_DELETE(pData);

	mIDSync.Leave();
#endif
}
//20230814 GBM end