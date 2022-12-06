
// FCMTestDlg.h : ��� ����
//

#pragma once

#include "EventSend.h"
#include "afxwin.h"


// CFCMTestDlg ��ȭ ����
class CFCMTestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CFCMTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FCMTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


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
	LRESULT OnSocketRecvMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonFire();
	afx_msg void OnBnClickedButtonGas();
	afx_msg void OnBnClickedButtonSpy();
	afx_msg void OnBnClickedButtonLine();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRecovery();
	CButton m_radioF;
	CButton m_radioG;
	CButton m_radioS;
	CButton m_radioT;
	CButton m_ridioR;
};
