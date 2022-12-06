
// GFSM_MCSDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define		LIMIT_RICH_LOG		(1024*1024*2)

// CGFSM_MCSDlg 대화 상자
class CGFSM_MCSDlg : public CDialogEx
{
// 생성입니다.
public:
	CGFSM_MCSDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_MCS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	void WriteFunc(CString strText);
	BOOL WriteLog(INT nMode, LPTSTR data, ...);

private:
	int m_nServerPort;
	int m_nLimitClient;
	BOOL m_bStartServer;


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
	afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
	CRichEditCtrl m_richEdit;
	CButton m_checkScroll;
};
