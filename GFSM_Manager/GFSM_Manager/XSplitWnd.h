#pragma once

#define		SPLIT_TICKNESS			5
#define		SPLIT_LIMIT				130


#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#define ULW_COLORKEY            0x00000001
#define ULW_ALPHA               0x00000002
#define ULW_OPAQUE              0x00000004 

typedef BOOL(WINAPI* SLWA)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

// CXSplitWnd 대화 상자입니다.

class CXSplitWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CXSplitWnd)

public:
	CXSplitWnd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CXSplitWnd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_POPUP_DLG };
#endif

public:
	void SetSplitWnd(CWnd* pLeftWnd, CWnd* pRightWnd);
	void SetSplitIndex(int nIndex);

	void Reposition(int nX);
	void CheckShowWindow();
	void SetSplitLimit(int nValue);
	void SetSplitTickSize(int nValue);

private:
	CWnd* m_pParent, *m_pLeftWnd, *m_pRightWnd;
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
