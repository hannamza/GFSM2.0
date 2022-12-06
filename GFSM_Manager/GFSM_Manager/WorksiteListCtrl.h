#pragma once

#include <afxtempl.h>
#include "Worksite.h"
#include "PosVSliderDlg.h"

// CWorksiteListCtrl ���Դϴ�.

class CWorksiteListCtrl : public CScrollView
{
	DECLARE_DYNCREATE(CWorksiteListCtrl)

protected:
	CWorksiteListCtrl();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CWorksiteListCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	INT		m_nScrollY, m_nOldScrollY;
	INT		m_nScrollX, m_nOldScrollX;
	INT		m_Range;

	CFont	m_font;
	CString	m_sSearch;

	INT		m_nShowIndex;
	INT		m_nPos, m_nScrollPos;

	CPosVSliderDlg* m_pSlider;

	CCriticalSectionEx m_listSync, m_displaySync;

	CList<CDialogEx*, CDialogEx*> m_listWorksite;

protected:
	void Redisplay();
	void SetScrollSize();
	void RedisplayWorksite(INT nIndex);

public:
	void SetSearchWord(CString sSearch);
	void SetSelectedItem(int nIndex);
	int  GetSelectedItemSeq();
	int  GetSelectedItemIndex();
	int  GetItemIndex();

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnLWorksiteClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRWorksiteClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


