
// GFSM_SENDERDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EventSend.h"

// CGFSM_SENDERDlg 대화 상자
class CGFSM_SENDERDlg : public CSkinDialog
{
// 생성입니다.
public:
	CGFSM_SENDERDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_SENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	void Reposition();
	void RegistryTrayIcon();

private:
	bool m_bStarted;
	CBrush m_brush;
	bool m_bInit;
	bool m_bKilled;
	char m_szServerIP[32];
	bool m_bAutoLogin;

public:
	//이벤트 테스트 기능용 변수
	BYTE m_ringBuffer[(SI_EVENT_BUF_SIZE + 1) * 20000];
	int m_nBufPos = 0;

	//관리자 모드
	BOOL m_bAdminMode;
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
	afx_msg LRESULT OnTrayNotification(WPARAM, LPARAM);
	CRichEditCtrl m_richLog;
	CComboBox m_comboPort;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonSend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam);
	CButton m_checkScroll;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CSkinButton m_btnStart;
	CSkinButton m_btnStop;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btnSend;
	CEdit m_editInput;
	afx_msg void OnMenuExit();
	afx_msg void OnMenuStart();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuPopup();
	afx_msg void OnMenuUpdateStart(CCmdUI *pCmdUI);
	afx_msg void OnMenuUpdateStop(CCmdUI *pCmdUI);
	afx_msg void OnMenuUpdatePopup(CCmdUI *pCmdUI);
	afx_msg void OnMenuUpdateExit(CCmdUI *pCmdUI);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	CButton m_btnEvent;
	afx_msg void OnBnClickedButtonEvent();
};
