
// GFSM_SENDERDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EventSend.h"

// CGFSM_SENDERDlg ��ȭ ����
class CGFSM_SENDERDlg : public CSkinDialog
{
// �����Դϴ�.
public:
	CGFSM_SENDERDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_SENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

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
	//�̺�Ʈ �׽�Ʈ ��ɿ� ����
	BYTE m_ringBuffer[(SI_EVENT_BUF_SIZE + 1) * 20000];
	int m_nBufPos = 0;

	//������ ���
	BOOL m_bAdminMode;
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
