#pragma once
#include <queue>
#include <thread>
using namespace std;

#define		ID_CNT			5

typedef struct STATE_DATA
{
	char code1;
	char code2;
	char event_code;
	char recv_number[2];
	char unit_number[2];
	char seperate;
	char type_number;
	char line_number[3];
	char event;
}stateData, *pStateData;

class CEventSend : public TSingleton<CEventSend>
{
public:
	CEventSend();
	~CEventSend();

public:
	bool InitEvent();
	void SendEvent(BYTE* pData);
	void SetMainWnd(CWnd* pWnd);

protected:
	queue<BYTE*>m_sendQueue;
	thread* m_pThread;
	CCriticalSectionEx m_sync, mIDSync;
	CString m_IDList[100];
	int m_nIDCount;
	CWnd* m_pWnd;

private:
	//void SendFcmData(BYTE* pData);
	void StartSendEventThread();
	void StopSendEventThread();

public:
	void SendThreadLoop();
};

