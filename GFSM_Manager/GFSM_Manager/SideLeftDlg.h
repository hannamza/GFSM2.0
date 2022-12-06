#pragma once

#include "WorksiteListCtrl.h"
#include "afxwin.h"

// CSideLeftDlg ��ȭ �����Դϴ�.

class CSideLeftDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSideLeftDlg)

public:
	CSideLeftDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSideLeftDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LEFTSIDE};

protected:
	CWorksiteListCtrl* m_pListCtrl;
	CFont m_font, m_font2;

private:
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
