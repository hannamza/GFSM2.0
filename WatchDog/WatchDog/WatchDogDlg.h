
// WatchDogDlg.h : 헤더 파일
//

#pragma once
#include <thread>
#include "ClientInterface.h"
#include "afxwin.h"

using namespace std;

#define		STATE_MSG		(WM_USER+1001)

// CWatchDogDlg 대화 상자
class CWatchDogDlg : public CDialogEx
{
// 생성입니다.
public:
	CWatchDogDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WATCHDOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	thread*		m_Thread;
	int			m_nServerPort;
	CString		m_sPath;
	CString		m_sFileName;
	bool		m_bFirst;
		
	PROCESS_INFORMATION m_Info;

private:
	bool ExecuteCheckProgram(CString sPath);
	bool IsCheckProgramRunning();

public:
	void ServerCheckLoop();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnStateMsg(WPARAM wParam, LPARAM lParam);
	CListBox m_listHistory;
};
