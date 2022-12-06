#pragma once

#include "YSplitWnd.h"
#include "MainTopDlg.h"
#include "MainBottomDlg.h"

// CMainFrameDlg ��ȭ �����Դϴ�.

class CMainFrameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrameDlg)

public:
	CMainFrameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainFrameDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_COMMON_CHILD_DLG };

public:
	CMainTopDlg* m_pTopDlg;
	CMainBottomDlg* m_pBottomDlg;
	CYSplitWnd* m_pSplit;

public:
	void MoveNotify();
	void MovingNotify();

private:
	bool m_bInit;

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
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
