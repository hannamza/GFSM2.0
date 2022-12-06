#pragma once

#include "XSplitWnd.h"

// CYSplitWnd 대화 상자입니다.

class CYSplitWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CYSplitWnd)

public:
	CYSplitWnd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CYSplitWnd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_POPUP_DLG };
#endif

public:
	void SetSplitWnd(CWnd* pTopWnd, CWnd* pBottomWnd);
	void SetSplitIndex(int nIndex);
	void CheckShowWindow();
	void SetSplitLimit(int nValue);
	void SetSplitTickSize(int nValue);

	void Reposition(int nY);

private:
	CWnd* m_pParent, *m_pTopWnd, *m_pBottomWnd;
	bool m_bMouseDown;
	bool m_bInit;
	int m_nDownPoint;
	int m_nIndex;
	COLORREF m_color, m_moveColor;
	int m_nSplitLimit;
	int m_nTickSize;

protected:
	void SetTransparency(int percent);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
