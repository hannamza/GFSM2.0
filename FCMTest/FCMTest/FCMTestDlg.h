
// FCMTestDlg.h : 헤더 파일
//

#pragma once

#include "EventSend.h"
#include "afxwin.h"


// CFCMTestDlg 대화 상자
class CFCMTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CFCMTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FCMTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


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
