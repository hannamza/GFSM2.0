// WorksiteListCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WorksiteListCtrl.h"
#include "GFSM_Manager.h"


int nWorksiteTerm = 1;
int nWorksiteHeight = 64;
int nWorksiteWidth = 150;

// CWorksiteListCtrl

IMPLEMENT_DYNCREATE(CWorksiteListCtrl, CScrollView)

CWorksiteListCtrl::CWorksiteListCtrl()
{
	m_nMapMode = MM_TEXT;

	m_nScrollX = 0;
	m_nScrollY = 0;

	m_font.CreateFont(12, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 

	m_nShowIndex = 0;
	m_nPos = -1;
	m_nScrollPos = -1;
	m_Range = 100;

	m_pSlider = NULL;
}

CWorksiteListCtrl::~CWorksiteListCtrl()
{
}


BEGIN_MESSAGE_MAP(CWorksiteListCtrl, CScrollView)
	ON_MESSAGE(WM_LBUTTON_BUTTONlIST_CLICK, OnLWorksiteClick)
	ON_MESSAGE(WM_RBUTTON_BUTTONlIST_CLICK, OnRWorksiteClick)
	ON_MESSAGE(SLIDER_INFO, OnSliderInfo)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWorksiteListCtrl �׸����Դϴ�.

void CWorksiteListCtrl::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	ShowScrollBar(SB_BOTH, FALSE);

	m_pSlider = new CPosVSliderDlg(this);
	m_pSlider->Create(IDD_COMMON_CHILD_DLG, this);
}

void CWorksiteListCtrl::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CWorksiteListCtrl �����Դϴ�.

#ifdef _DEBUG
void CWorksiteListCtrl::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorksiteListCtrl::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWorksiteListCtrl �޽��� ó�����Դϴ�.


BOOL CWorksiteListCtrl::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	while (!m_listWorksite.IsEmpty())
	{
		CWorksite * pSite = (CWorksite*)m_listWorksite.RemoveHead();
		SAFE_DELETE(pSite);
	}

	SAFE_DELETE(m_pSlider);

	return CScrollView::DestroyWindow();
}


void CWorksiteListCtrl::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


HBRUSH CWorksiteListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

BOOL CWorksiteListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CScrollView::PreTranslateMessage(pMsg);
}


void CWorksiteListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(&rect);

	int nHeight = rect.Height();
	int nWorksiteCount = ((nHeight / nWorksiteHeight) * 2) + 2;

	int nWorkSiteListCount = CWorksiteManager::Instance()->GetCount();

	for (int nIndex = m_listWorksite.GetCount(); nIndex < nWorkSiteListCount; nIndex++)
	{
		CWorksite* pSite = new CWorksite(this);
		pSite->Create(IDD_COMMON_CHILD_DLG, this);
		pSite->ShowWindow(SW_HIDE);
		m_listWorksite.AddTail(pSite);
	}

	if (m_pSlider)
	{
		m_pSlider->MoveWindow(rect.Width() - 13, 0, 13, rect.Height());

		if (rect.Height() >= m_Range)
		{
			m_pSlider->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pSlider->ShowWindow(SW_SHOW);
		}

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);

		SetScrollSize();
	}

	ShowScrollBar(SB_BOTH, FALSE);
}

void CWorksiteListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	/*if (nIDEvent == 9)
	{
		KillTimer(9);
		Redisplay();
	}*/

	CScrollView::OnTimer(nIDEvent);
}


void CWorksiteListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	nPos = si.nTrackPos;

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CWorksiteListCtrl::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CWorksiteListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	/*CRect rect;
	GetClientRect(&rect);
	CCommonDisplay::DrawRect(pDC, true, RGB(140, 140, 140), RGB(140, 140, 140), rect);*/

	return  CScrollView::OnEraseBkgnd(pDC);
}


void CWorksiteListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CScrollView::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}


BOOL CWorksiteListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CRect rect;
	GetClientRect(&rect);

	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	int nTotalHeight = nWorksiteCount * (nWorksiteTerm + nWorksiteHeight) - nWorksiteTerm;
	int nSign = zDelta / abs(zDelta);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * 50);// (nWorksiteHeight + nWorksiteTerm));
	if (nTotalHeight < rect.Height() + nPos)
	{
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0)
	{
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		m_nScrollPos = nPos;
		Redisplay();

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
	}

	return CScrollView::OnMouseWheel(nFlags, 0, pt);
}

LRESULT CWorksiteListCtrl::OnLWorksiteClick(WPARAM wParam, LPARAM lParam)
{
	int nWorksite = (int)lParam;
	GetParent()->PostMessageW(WM_LBUTTON_BUTTONlIST_CLICK, wParam, lParam);

	m_displaySync.Enter();
	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	CWorksite* pSite;
	for (int nIndex = 0; nIndex < nWorksiteCount; nIndex++)
	{
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
		if (pSite->GetItemIndex() == nWorksite && !pSite->GetSelectedState()) {
			pSite->SetSelectedState(true);
		}
		else if (pSite->GetItemIndex() != nWorksite && pSite->GetSelectedState()) {
			pSite->SetSelectedState(false);
		}
	}
	m_displaySync.Leave();

	return 0;
}

LRESULT CWorksiteListCtrl::OnRWorksiteClick(WPARAM wParam, LPARAM lParam)
{
	int nWorksite = (int)wParam;
	//GetParent()->PostMessageW(WM_RBUTTON_BUTTONlIST_CLICK, wParam, lParam);

	return 0;
}

LRESULT CWorksiteListCtrl::OnSliderInfo(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nValue = (int)lParam;

	CRect rect;
	GetClientRect(&rect);

	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	int nLeafCount = 0;

	int nTotalHeight = nWorksiteCount * (nWorksiteTerm + nWorksiteHeight) - nWorksiteTerm;

	int nPos = m_nScrollPos + nValue;
	if (nTotalHeight < rect.Height() + nPos)
	{
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0)
	{
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		Redisplay();

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
	}
	if (nType == 1)
	{
		m_nScrollPos = m_nPos;
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------

void CWorksiteListCtrl::SetScrollSize()
{
	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();

	int nTotalHeight = nWorksiteCount * (nWorksiteTerm + nWorksiteHeight) - nWorksiteTerm;
	if (nTotalHeight < 0) {
		nTotalHeight = 0;
	}
	if (m_Range == nTotalHeight) {
		return;
	}

	CSize sizeTotal;
	sizeTotal.cx = 0, sizeTotal.cy = nTotalHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);
	m_Range = nTotalHeight;

	ShowScrollBar(SB_BOTH, FALSE);

	CRect rect;
	GetClientRect(&rect);

	if (rect.Height() >= m_Range)
	{
		m_pSlider->ShowWindow(SW_HIDE);

		if (m_nPos != 0)
		{
			SetScrollPos(SB_VERT, 0, TRUE);
			m_nPos = 0;
			m_nScrollPos = 0;
			Redisplay();
		}
	}
	else
	{
		m_pSlider->ShowWindow(SW_SHOW);
	}
	m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
}

void CWorksiteListCtrl::RedisplayWorksite(INT nIndex)
{
	m_displaySync.Enter();

	if (nIndex < 0 || nIndex >= CWorksiteManager::Instance()->GetCount())
	{
		m_displaySync.Leave();
		return;
	}

	CWorksite* pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
	if (pSite)
	{
		pSite->Invalidate();
	}

	m_displaySync.Leave();
}

void CWorksiteListCtrl::SetSearchWord(CString sSearch)
{
	m_sSearch = sSearch;
	Redisplay();
}

void CWorksiteListCtrl::SetSelectedItem(int nValue)
{
	m_displaySync.Enter();
	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	CWorksite* pSite;
	for (int nIndex = 0; nIndex < nWorksiteCount; nIndex++)
	{
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
		if (nValue == pSite->GetItemIndex()) {
			pSite->SetSelectedState(true);
		}
		else if (pSite->GetSelectedState()) {
			pSite->SetSelectedState(false);
		}
	}
	m_displaySync.Leave();
}

int CWorksiteListCtrl::GetSelectedItemSeq()
{
	m_displaySync.Enter();
	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	CWorksite* pSite;
	for (int nIndex = 0; nIndex < nWorksiteCount; nIndex++)
	{
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
		if (pSite->GetSelectedState()) {
			m_displaySync.Leave();
			return pSite->GetItemIndex();
		}
	}
	m_displaySync.Leave();
	return -1;
}

int CWorksiteListCtrl::GetSelectedItemIndex()
{
	m_displaySync.Enter();
	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	CWorksite* pSite;
	for (int nIndex = 0; nIndex < nWorksiteCount; nIndex++)
	{
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
		if (pSite->GetSelectedState()) {
			m_displaySync.Leave();
			return nIndex;
		}
	}
	m_displaySync.Leave();
	return -1;
}

void CWorksiteListCtrl::Redisplay()
{
	CString sSearch = m_sSearch;
	sSearch.Trim(L" ");
	if (sSearch.GetLength() == 0) {
		SetScrollSize();
	}

	m_displaySync.Enter();

	CRect rect;
	GetClientRect(&rect);

	int nWorksiteCount = CWorksiteManager::Instance()->GetCount();
	int nPos = 0, nLineH = 0;
	int nWorksiteIndex = 0;
	CWorksite* pSite = nullptr;
	pWorkSite pValue;
	int nCount = 0;

	CDC* _pDC = GetDC();

	for (int nIndex = 0; nIndex < nWorksiteCount; nIndex++)
	{
		if (sSearch.GetLength() > 0) {
			pValue = CWorksiteManager::Instance()->GetWorkSite(nIndex);
			CString sText = pValue->szSystemName;
			if (sText.Find(sSearch, 0) < 0) {
				continue;
			}
		}
		++nCount;
		if (nPos > m_nPos + rect.Height())
		{
			nPos += nWorksiteHeight + nWorksiteTerm;
			continue;
		}

		pValue = CWorksiteManager::Instance()->GetWorkSite(nIndex);
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nWorksiteIndex));
		pSite->SetItemIndex(pValue->nSequenceNo);
		pSite->SetWorksite(pValue);

		int nTerm = 13;
		if (rect.Height() >= m_Range) {
			nTerm = 0;
		}
		nLineH = nPos - m_nPos + nWorksiteHeight;
		CCommonDisplay::DrawLine(_pDC, 0, nLineH, rect.Width() - nTerm, nLineH,	PS_SOLID, 1, RGB(140, 140, 140));

		pSite->MoveWindow(0, nPos - m_nPos, rect.Width() - nTerm, nWorksiteHeight);
		pSite->ShowWindow(SW_SHOW);
		pSite->Invalidate();
		++nWorksiteIndex;

		nPos += nWorksiteHeight + nWorksiteTerm;
	}

	for (int nIndex = nWorksiteIndex; nIndex < m_listWorksite.GetCount(); nIndex++)
	{
		pSite = (CWorksite*)m_listWorksite.GetAt(m_listWorksite.FindIndex(nIndex));
		pSite->SetItemIndex(-1);
		pSite->SetWorksite(NULL);
		pSite->ShowWindow(SW_HIDE);
	}

	if (sSearch.GetLength() > 0) {
		int nTotalHeight = nCount * (nWorksiteTerm + nWorksiteHeight) - nWorksiteTerm;
		CSize sizeTotal;
		if (nTotalHeight < 0) {
			nTotalHeight = 0;
		}
		if (m_Range != nTotalHeight)
		{
			sizeTotal.cx = 0, sizeTotal.cy = nTotalHeight;
			SetScrollSizes(MM_TEXT, sizeTotal);
			m_Range = nTotalHeight;

			if (rect.Height() >= m_Range)
			{
				m_pSlider->ShowWindow(SW_HIDE);
			}
			else
			{
				m_pSlider->ShowWindow(SW_SHOW);
			}
		}
	}
	ShowScrollBar(SB_BOTH, FALSE);

	ReleaseDC(_pDC);

	m_pSlider->Invalidate();

	m_displaySync.Leave();
}

