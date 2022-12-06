#pragma once

class CImageBtn : public CButton
{
public:
	CImageBtn(void);
	~CImageBtn(void);
private:
	BOOL	m_bTracking;
	BOOL	m_bHover;
	BOOL	m_bDown;
	BOOL	m_bBitmapType;

	CBitmap m_bmImage, *m_pBmImage;
	CSize	m_bmSize;

	CCriticalSectionEx	m_sync;

	INT				m_nDirection;

public:
	virtual BOOL Create(UINT nResourceID, CBitmap* pBitmap, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void ChangeBitmap(UINT nResource);

protected:
	void Draw(CDC& dc, const CRect& rect, int nPos);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

