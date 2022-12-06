#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMainBottomDlg 대화 상자입니다.

class CMainBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainBottomDlg)

public:
	CMainBottomDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainBottomDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_USER };

private:
	CFont m_font;
	bool m_bInit;
	int m_nUserSeq;

protected:
	void Redisplay();
	void ReListup();

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
	CSkinButton m_btnAdd;
	CSkinButton m_btnMod;
	CSkinButton m_btnDel;
	CXListCtrl m_listUser;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonMod();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult);
};
