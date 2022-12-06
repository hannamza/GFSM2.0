#if !defined(AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)
#define AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrayDialog.h : header file
//

// 트레이로 사용하기 편하게 만든 CDialogExEx 상속 받은 클래스(CDialogEx로 변경 가능)
// CDialogEx, CDialogExEx를 상속 받은 다이얼로그 클래스 대신에 사용하여 트레이를 쉽게 사용 가능

/////////////////////////////////////////////////////////////////////////////
// CTrayDialog dialog


class CTrayDialog : public CDialogEx
{

private:
	BOOL m_bMinimizeToTray;

	BOOL			m_bTrayIconVisible; // 트레이 상태를 저장
	NOTIFYICONDATA	m_nidIconData;		// 아이콘 데이터
	CMenu			m_mnuTrayMenu;		// 트레이 메뉴
protected:
	BOOL			m_bVisible;
	// Construction
public:
	// 메뉴 리소스 지정
	BOOL TraySetMenu(UINT nResourceID, UINT nDefaultPos = 0);
	BOOL TraySetMenu(HMENU hMenu,UINT nDefaultPos=0);
	BOOL TraySetMenu(LPCTSTR lpszMenuName,UINT nDefaultPos=0);
	// 트레이 아이콘, 툴팁 정보 업데이트
	BOOL TrayUpdate();
	// 트레이 상태로
	BOOL TrayShow();
	// 트레이에서 제거
	BOOL TrayHide();
	// 트레이 상태로 다이얼로그만 팝업
	BOOL ShowDialog();
	// 다이얼로그 숨기기
	BOOL HIdeDialog();
	// 트레이 툴팁 정보
	void TraySetToolTip(LPCTSTR lpszToolTip);
	// 트레이 아이콘 리소스 지정
	void TraySetIcon(HICON hIcon);
	void TraySetIcon(UINT nResourceID);
	void TraySetIcon(LPCTSTR lpszResourceName);
	// 트레이 상태 정보 가져오기
	BOOL TrayIsVisible();
	CTrayDialog(UINT uIDD,CWnd* pParent = NULL);   // standard constructor
	
	virtual void OnTrayLButtonDown(CPoint pt);
	virtual void OnTrayLButtonDblClk(CPoint pt);
	
	virtual void OnTrayRButtonDown(CPoint pt);
	virtual void OnTrayRButtonDblClk(CPoint pt);

	virtual void OnTrayMouseMove(CPoint pt);
	

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CTrayDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	
	private:
//	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnTrayIconNotifyMessage(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)
