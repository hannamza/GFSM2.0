
// GFSM_ManagerDlg.h : 헤더 파일
//

#pragma once

#include "SideLeftDlg.h"
#include "MainFrameDlg.h"
#include "XSplitWnd.h"

// CGFSM_ManagerDlg 대화 상자
class CGFSM_ManagerDlg : public CSkinDialog
{
// 생성입니다.
public:
	CGFSM_ManagerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_MANAGER_DIALOG };
#endif

private:
	CSideLeftDlg* m_pSideLeftDlg;
	CMainFrameDlg* m_pMainFrameDlg;
	CXSplitWnd* m_pSplit;

	bool m_bInit;

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
	afx_msg LRESULT OnSocketRecvMsg(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
