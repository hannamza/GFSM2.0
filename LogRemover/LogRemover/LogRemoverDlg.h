
// LogRemoverDlg.h : ��� ����
//

#pragma once

#include <thread>
using namespace std;

// CLogRemoverDlg ��ȭ ����
class CLogRemoverDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CLogRemoverDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGREMOVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

protected:
	CString m_sPath;
	thread* m_Thread;

public:
	void DeleteLogfiles(CString sPath, CTime & tDel, int nYear, int nMon, int nType);

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};