#pragma once
#include "afxwin.h"

typedef BOOL(WINAPI *SetLayer)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
#define LWA_COLORKEY            0x01
#define LWA_ALPHA               0x02

// CLoginDlg 대화 상자입니다.

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

public:
	CString m_sID, m_sPW;
	CBrush m_brush;

private:
	void Redisplay();

	void n_SetDlgAlpha(int chAlpha);     // 다이얼로그 투명도 조절

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConfirmClick();
	afx_msg void OnCancelClick();
	afx_msg void OnFindClick();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editID;
	CEdit m_editPW;
	CImageBtn m_btnConfirm;
	CImageBtn m_btnCancel;
	CImageBtn m_btnFind;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
