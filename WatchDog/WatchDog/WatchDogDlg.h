
// WatchDogDlg.h : ��� ����
//

#pragma once
#include <thread>
#include "ClientInterface.h"
#include "afxwin.h"

using namespace std;

#define		STATE_MSG		(WM_USER+1001)

// CWatchDogDlg ��ȭ ����
class CWatchDogDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CWatchDogDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WATCHDOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

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

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
