
// GFSM_UserDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "XListCtrl.h"

// CGFSM_UserDlg 대화 상자
class CGFSM_UserDlg : public CDialogEx
{
// 생성입니다.
public:
	CGFSM_UserDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_USER_DIALOG };
#endif

private:
	bool m_bInit;
	bool m_bDestroy;
	CFont m_font;
	int m_nUserSeq;
	CString m_sMessage;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	void Redisplay();
	void ReListup();


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAddClick();
	afx_msg void OnModClick();
	afx_msg void OnDelClick();
	afx_msg void OnMiniClick();
	afx_msg void OnCloseClick();

	afx_msg LRESULT OnSocketRecvMsg(WPARAM, LPARAM);
	afx_msg LRESULT OnRedisplayMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CEdit m_editEtc;
	CEdit m_editName;
	CEdit m_editPhone;
	CXListCtrl m_listUser;
	CImageBtn m_btnAdd;
	CImageBtn m_btnMod;
	CImageBtn m_btnDel;
	CImageBtn m_btnMini;
	CImageBtn m_btnClose;
	afx_msg void OnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult);
};
