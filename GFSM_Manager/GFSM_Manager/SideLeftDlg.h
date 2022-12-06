#pragma once

#include "WorksiteListCtrl.h"
#include "afxwin.h"

// CSideLeftDlg 대화 상자입니다.

class CSideLeftDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSideLeftDlg)

public:
	CSideLeftDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSideLeftDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LEFTSIDE};

protected:
	CWorksiteListCtrl* m_pListCtrl;
	CFont m_font, m_font2;

private:
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonMod();
	afx_msg void OnBnClickedButtonDel();
	afx_msg LRESULT OnLWorksiteClick(WPARAM wParam, LPARAM lParam);
	CSkinButton m_btnSearch;
	CSkinButton m_btnAdd;
	CSkinButton m_btnMod;
	CSkinButton m_btnDel;
	CEdit m_editSearch;
};
