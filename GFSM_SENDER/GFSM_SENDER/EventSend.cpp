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
	char* pSendData;
	int userIndex;
	CTime currTime;
	HANDLE hReq;
	CString sTitle;
	char* strUtf8;
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
	char* pSendData = pAi->pSendData;
	int userIndex = pAi->userIndex;
	CTime currTime = pAi->currTime;
	HANDLE hReq = pAi->hReq;
	CString sTitle = pAi->sTitle;
	char* strUtf8 = pAi->strUtf8;
	char* szBody = pAi->szBody;
	char* szTitle = pAi->szTitle;
	BOOL bJason = pAi->bJason;
	
	ResetEvent(pDlg->m_hThread[userIndex]);

	char szSendData[4000];
	memset(szSendData, 0, 4000);

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

		//
		int nLen = WideCharToMultiByte(CP_UTF8, 0, /*m_IDList[i]*/pInfo->szToken, lstrlenW(pInfo->szToken), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, pInfo->szToken, lstrlenW(pInfo->szToken), strUtf8, nLen, NULL, NULL);

		char* mID = qURLencode(strUtf8);

		if (!bJason) {
			sprintf_s(szSendData, 4000, "&priority=high&%s%s&registration_id=%s", "data=", pSendData, mID);
		}
		else {
			sprintf_s(szSendData, 4000, "{\"to\":\"%s\", \"priority\": \"high\", \
						\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\"},\
						\"data\" : {\"event\":\"%s\"},\
							\"android\" : {	\"priority\":\"high\"},\
								\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
									\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
				, mID, szBody, szTitle, pSendData);
		}
		//

		HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));
	}
	//

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	double duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("%d 스레드 FCM Push Message 처리 시간 : %f", userIndex, duringTime);
#endif
	//20230320 GBM end

	SetEvent(pDlg->m_hThread[userIndex]);

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
			LARGE_INTEGER startTime, endTime;
			QueryPerformanceCounter(&startTime);
			//20230320 GBM end

			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserTokenList, CCommonState::Instance()->m_nIdx);

			//20230320 GBM start - test
			QueryPerformanceCounter(&endTime);
			double duringTime;
			duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
			Log::Trace("사용자 리스트 요청에 걸린 시간 : %f", duringTime);
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
		//SendAlarm(pDataSave, nSize - 1);
		SendAlarmInParallel(pDataSave, nSize - 1);
	}

	dwValue = 0;
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
	CString strOccurType;

	CString strDisplay2;

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
		sTitle = L"복구 완료";
		sName = L"수신기 복구";
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
			strPosition = L"복구 완료";
		}
		else if ('T' == pData[2])
		{
			strPosition = L"단선";
			strSecond.Format(L"03x");
		}
		else if ('F' == pData[2])
		{
			if (strType == "0")
			{
				strPosition = L"화재";
				strSecond.Format(L"00x");
			}
			else
			{
				strPosition = L"감시";
				strSecond.Format(L"02x");
			}

			if (L"63" == strUnitNum)
			{
				strPosition = L"감시";
				strSecond.Format(L"02x");
			}
		}
		else if ('S' == pData[2])
		{
			strPosition = L"감시";
			strSecond.Format(L"02x");
		}
		else if ('G' == pData[2])
		{
			strPosition = L"가스";
			strSecond.Format(L"01x");
		}
		else
		{
			return false;
		}

		sTitle = strPosition;
		if (pData[12] == 'N')
		{
			strOccurType.Format(L"%s", L"발생");
			if (sTitle != L"복구 완료") {
				sTitle += L" 발생";
			}
			strFirst.Format(L"Ax");
		}
		else
		{
			strOccurType.Format(L"%s", L"복구");
			if (sTitle != L"복구 완료") {
				sTitle += L" 복구";
			}
			strFirst.Format(L"Dx");
		}

		sUni.Format(L"%s%s%d/%02d/%02d %02d:%02d:%02dx%s-%s-%s-%sx%s",
			strFirst, strSecond,
			currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond(),
			strFACPNum, strUnitNum, strLoopNum, strLineNum,
			sName);

		strDisplay2.Format(L"%d/%02d/%02d %02d:%02d:%02d %s %s (%s-%s-%s-%s) : %s",
			currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond(),
			strPosition, strOccurType,
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
		sUni.Format(L"Rx%d/%02d/%02d %02d:%02d:%02dx수신기 복구", currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
			currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());
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
	//20233020 GBM start - test 1:초기화 루틴 분리, 2:기존 루틴
#if 1
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
							\"android\" : {	\"priority\":\"high\"},\
								\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
									\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
					, mID, szBody, szTitle, pSendData);
			}
			//

			HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));

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

	SAFE_DELETE(pData);

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
							\"notification\" : {\"body\" : \"%s\",\"title\" : \"%s\"},\
							\"data\" : {\"event\":\"%s\"},\
								\"android\" : {	\"priority\":\"high\"},\
									\"apns\" : {\"headers\":{\"apns-priority\":\"10\"}},\
										\"webpush\" : {\"headers\": {\"Urgency\": \"high\"}}}"
					, mID, szBody, szTitle, pSendData);
			}
			HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData));

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

	SAFE_DELETE(pData);

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
	//
	ALARM_INFO ai[USER_MAX_COUNT];

	for (int i = 0; i < nCount; i++)
	{
		m_hThread[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		ai[i].pDlg = this;
		ai[i].pSendData = pSendData;
		ai[i].userIndex = i;
		ai[i].currTime = currTime;
		ai[i].hReq = hReq;
		ai[i].sTitle = sTitle;
		ai[i].strUtf8 = strUtf8;
		ai[i].szBody = szBody;
		ai[i].szTitle = szTitle;
		ai[i].bJason = bJason;

		CWinThread* pThread = ::AfxBeginThread(SendAlarmThread, &ai[i]);
	}

	//스레드가 모두 종료되길 기다린다.
	DWORD dw = WaitForMultipleObjects(nCount, m_hThread, true, INFINITE);
	if (dw != WAIT_OBJECT_0)
	{
		Log::Trace("스레드 대기 실패!");
	}

	//각 스레드 처리가 모두 끝나면 각 핸들을 닫는다.
	for (int i = 0; i < nCount; i++)
	{
		CloseHandle(m_hThread[i]);
	}

	free(mID);
	::InternetCloseHandle(hReq);
	::InternetCloseHandle(hHttp);
	::InternetCloseHandle(hConnect);
	free(pSendData);

	SAFE_DELETE(pData);

	//20230320 GBM start - test
#ifdef PUSH_MESSAGE_TIME_MEASURE_MODE
	QueryPerformanceCounter(&endTime);
	double duringTime;
	duringTime = CCommonFunc::GetPreciseTime(startTime, endTime);
	Log::Trace("FCM Push Message 전체 처리 시간 : %f", duringTime);
#endif
	//20230320 GBM end

// 	for (int i = 0; i < nCount; i++)
// 	{
// 		SAFE_DELETE(hList[i]);
// 	}

	mIDSync.Leave();
}