#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMainTopDlg 대화 상자입니다.

class CMainTopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTopDlg)

public:
	CMainTopDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainTopDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANAGER };

private:
	void Redisplay();
	void ReListup();

protected:
	CFont m_font;
	bool m_bInit;
	bool m_bLock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnRedisplayMsg(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonMod();
	afx_msg void OnBnClickedButtonDel();
	CSkinButton m_btnAdd;
	CSkinButton m_btnMod;
	CSkinButton m_btnDel;
	CXListCtrl m_listManager;
	afx_msg void OnLvnItemchangedListManager(NMHDR *pNMHDR, LRESULT *pResult);
};
