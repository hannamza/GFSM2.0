#include "stdafx.h"
#include "EventSend.h"
//#include "HttpClient.h"

#include <Wininet.h>

#pragma comment(lib, "wininet.lib")
//#pragma comment(lib, "HttpStack.lib")

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

CEventSend::CEventSend()
{
	m_pThread = NULL;
	m_pWnd = NULL;
	m_nIDCount = 0;
}

CEventSend::~CEventSend()
{
	StopSendEventThread();

	m_sync.Enter();
	BYTE* pData;
	while (!m_sendQueue.empty())
	{
		pData = m_sendQueue.front();
		SAFE_DELETE(pData);

		m_sendQueue.pop();
	}
	m_sync.Leave();
}

bool CEventSend::InitEvent()
{
	mIDSync.Enter();
	CReadWriteState state;
	state.SetFileName(L"Setup.ini");
	if (!state.ReadState(L"ID", L"ID_CNT", m_nIDCount)) {
		m_nIDCount = 0;
	}
	CString strField_Name;
	CString sValue;
	for (int i = 0; i < m_nIDCount; i++)
	{
		strField_Name.Format(L"ID%d", i + 1);
		if (!state.ReadState(L"ID", strField_Name, sValue)) {
			sValue = L"";
		}
		m_IDList[i] = sValue;
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
	BYTE* pBuffer = new BYTE[16];
	memset(pBuffer, 0x00, sizeof(BYTE) * 16);
	memcpy_s(pBuffer, 13, pData, 13);

	m_sync.Enter();
	m_sendQueue.push(pBuffer);
	m_sync.Leave();
}

void CEventSend::SetMainWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;
}

void CEventSend::StartSendEventThread()
{
	g_bThread = true;
	m_pThread = new thread(&func_thread, this);
}

void CEventSend::StopSendEventThread()
{
	if (!m_pThread)
	{
		return;
	}
	g_bThread = false;
	m_pThread->join();

	SAFE_DELETE(m_pThread);
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

//void CEventSend::SendFcmData(BYTE* pData)
//{
//	CHttpClient clsApi;
//	//CHttpHeader clsHeader;
//	//HTTP_HEADER_LIST clsHeaderList;
//	std::string strSendBody, strRecvContentType, strRecvBody;
//
//	//InitNetwork();
//
//	clsHeader.m_strName = "Authorization";
//
//	// QQQ: FCM 클라우드 메시징에서 서버키를 가져와서 아래의 {server key} 를 수정한다.
//	clsHeader.m_strValue = "key={server key}";
//	clsHeaderList.push_back(clsHeader);
//
//	// QQQ: 안드로이드 앱의 FCM token 으로 아래의 {device token} 을 수정한다.
//	strSendBody = "{ "
//		"  \"registration_ids\" : [\"{device token}\"], "
//		"  \"data\" : { \"dName\" : \"dValue\" } "
//		"}";
//
//	if (clsApi.DoPost("https://fcm.googleapis.com/fcm/send", &clsHeaderList, "application/json", strSendBody.c_str(), strSendBody.length(), strRecvContentType, strRecvBody))
//	{
//		printf("recv[%s]\n", strRecvBody.c_str());
//	}
//	else
//	{
//		printf("DoPost error\n");
//	}
//}

void CEventSend::SendThreadLoop()
{
	BYTE *pData;
	char szSendData[4000];
	CString  strUni;
	char strUtf8[4000] = { 0, };
	int nLen;
	char* pSendData;
	char* mID;
	char szTitle[16], szBody[128];

	CTime   currTime;

	CString strPosition;

	CString strFACPNum;
	CString strUnitNum;
	CString strLoopNum;
	CString strLineNum;
	CString strOccurType;

	CString strName;
	CString strAddress;

	CString strFirst;
	CString strSecond;

	CString strHeader;

	CString strDisplay;
	CString strDisplay2;

	CString strInputType;

	CString strType;
	CString sTitle;

	LPVOID lpOutputBuffer = NULL;

	char szLog[512];

	while (g_bThread)
	{
		if (m_sendQueue.empty())
		{
			Sleep(1);
			continue;
		}
		memset(szSendData, 0, 4000);
		memset(strUtf8, 0x00, 4000);

		pData = m_sendQueue.front();
		currTime = CTime::GetCurrentTime();

		if ('R' == pData[2])
		{
			strUni = L"R";
		}
		else
		{
			if ('F' != pData[2] &&   //화재
				'G' != pData[2] &&   //가스
				'S' != pData[2] &&   //감시
				'T' != pData[2])     //단선
			{
				strUni = L"";

				m_sendQueue.pop();
				SAFE_DELETE(pData);

				continue;
			}

			strFACPNum.Format(L"%c%c", pData[3], pData[4]);
			strUnitNum.Format(L"%c%c", pData[5], pData[6]);
			strLoopNum.Format(L"%c", pData[8]);
			strLineNum.Format(L"%c%c%c", pData[9], pData[10], pData[11]);

			strAddress.Format(L"%02s%02s%s%03s", strFACPNum, strUnitNum, strLoopNum, strLineNum);
			strType = CDeviceInfo::Instance()->GetDeviceName(strAddress).Left(1);
			strName = CDeviceInfo::Instance()->GetDeviceName(strAddress).Mid(3);

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

			strUni.Format(L"%s%s%d/%02d/%02d %02d:%02d:%02dx%s-%s-%s-%sx%s",
				strFirst, strSecond,
				currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
				currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond(),
				strFACPNum, strUnitNum, strLoopNum, strLineNum,
				strName);

			strDisplay2.Format(L"%d/%02d/%02d %02d:%02d:%02d %s %s (%s-%s-%s-%s) : %s",
				currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
				currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond(),
				strPosition, strOccurType,
				strFACPNum, strUnitNum, strLoopNum, strLineNum,
				strName);
		}

		if ("R" == strUni)
		{
			/*sprintf_s(szLog, "Rx%d/%02d/%02d %02d:%02d:%02d 수신기 복구", currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
				currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());*/
			strUni.Format(L"Rx%d/%02d/%02d %02d:%02d:%02dx수신기 복구", currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(),
				currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());
			strcpy_s(szLog, CCommonFunc::WCharToChar(strUni.GetBuffer(0)));
			Log::Trace(szLog);
		}
		else
		{
			strcpy_s(szLog, CCommonFunc::WCharToChar(strDisplay.GetBuffer(0)));
			Log::Trace(szLog);
		}
		nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);

		BOOL bJason = true;
		if(bJason) {
			strcpy_s(szBody, CCommonFunc::WcharToUtf8(strName.GetBuffer(0)));
			strcpy_s(szTitle, CCommonFunc::WcharToUtf8(sTitle.GetBuffer(0)));
		}

		CClientInterface::Instance()->ProcessRequestAddEvent(CCommonState::Instance()->m_nIdx, CCommonState::Instance()->m_szID, strUtf8);

		// utf8 -> urlencode
		pSendData = qURLencode(strUtf8);

		mIDSync.Enter();
		for (int i = 0; i < ID_CNT; i++)
		{
			if ("" != m_IDList[i])
			{
				HANDLE hConnect = InternetOpen(L"FCM", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
				if (hConnect == NULL)
				{
					mIDSync.Leave();
					return;
				}

				HANDLE hHttp = InternetConnect(hConnect, L"fcm.googleapis.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
				if (hHttp == NULL)
				{
					InternetCloseHandle(hConnect);
					hHttp = NULL;
					mIDSync.Leave();
					return;
				}

				HANDLE hReq = HttpOpenRequest(hHttp, L"POST", L"/fcm/send", L"HTTP/1.1", NULL, NULL,
					INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
				if (hReq == NULL)
				{
					InternetCloseHandle(hConnect);
					InternetCloseHandle(hHttp);
					mIDSync.Leave();
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
				//strHeader += "AAAAwk5zZ_M:APA91bFWjKNy4rdKizSuO_G7n9yGz3bHzu_MYlStJsEPDnw_Qz212aIZb1tqZQgeGFFi4MqW5nSIrQbK4D3BDvjaxRpDpnpceufQiuKwcuw1xayHirO5LJtUc5kZtkxc2lMQwj20uY5w";
				strHeader += "AAAAfiAPpoM:APA91bEeX02UhoaqaGvTPffwhhp1y7VAY1PFDFiMfkANhYoEZqrSunBoaBGKoXKvnljDGrksIjUPniz7w2bCN7Lp9GABQovcTsbMbab_yYBXrvtb9DXBvIODfeopk4DLbsYJRgD9eDO4";
				strHeader += "\r\n\r\n";

				HttpAddRequestHeaders(hReq, strHeader, strHeader.GetAllocLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

				nLen = WideCharToMultiByte(CP_UTF8, 0, m_IDList[i], lstrlenW(m_IDList[i]), NULL, 0, NULL, NULL);
				WideCharToMultiByte(CP_UTF8, 0, m_IDList[i], lstrlenW(m_IDList[i]), strUtf8, nLen, NULL, NULL);

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
					/*sprintf_s(szSendData, 4000, "{ \"data\":{ \"event\": \"%s\" }, \
							\"notification\": { \"title\": \"%s\", \"body\": \"%s\" } \
							\"to\": \"%s\", \"priority\": \"high\" }", 
						pSendData, szTitle, szBody, mID);*/
				}
				if (!HttpSendRequest(hReq, NULL, 0, (LPVOID)szSendData, strlen(szSendData))) {
					TRACE("HttpSendRequest failed.\n");
				}

				free(mID);

				::InternetCloseHandle(hReq);
				::InternetCloseHandle(hHttp);
				::InternetCloseHandle(hConnect);
			}
		}
		mIDSync.Leave();

		free(pSendData);

		m_sendQueue.pop();

		SAFE_DELETE(pData);
	}
}