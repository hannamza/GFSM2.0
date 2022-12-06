// Worksite.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Worksite.h"
#include "afxdialogex.h"
#include "GFSM_Manager.h"

// CWorksite 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWorksite, CDialogEx)

CWorksite::CWorksite(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DLG, pParent)
{
	m_font.CreateFont(15, // nHeight 
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
		_T("굴림")); // lpszFacename 

	m_font2.CreateFont(12, // nHeight 
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
		_T("굴림")); // lpszFacename 

	m_font3.CreateFont(12, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("굴림")); // lpszFacename 

	m_bMouseRbtn = false;
	m_bMouseLbtn = false;

	m_nIndex = -1;

	m_pSite = NULL;

	m_bMouseOn = false;
	m_bSelected = false;

	m_pWnd = pParent;
}

CWorksite::~CWorksite()
{
}

void CWorksite::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWorksite, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CWorksite 메시지 처리기입니다.


BOOL CWorksite::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CWorksite::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CWorksite::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else
		{
			GetParent()->PreTranslateMessage(pMsg);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CWorksite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CWorksite::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

void CWorksite::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CWorksite::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CWorksite::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CWorksite::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		POINT point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		CRect rect;
		GetClientRect(&rect);
		if (!rect.PtInRect(point))
		{
			if (m_bMouseOn)
			{
				KillTimer(1);
				m_bMouseOn = false;
				Redisplay();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------------------

void CWorksite::SetWorksite(pWorkSite pSite)
{
	m_pSite = pSite;
}

void CWorksite::SetItemIndex(int nIndex)
{
	m_nIndex = nIndex;
}

int CWorksite::GetItemIndex()
{
	return m_nIndex;
}

void CWorksite::SetSelectedState(bool bValue)
{
	m_bSelected = bValue;
	Redisplay();
}

bool CWorksite::GetSelectedState()
{
	return m_bSelected;
}

bool CWorksite::FindSearchWord(CString sValue)
{
	if (m_pSite)
	{
		CString sText = m_pSite->szSystemName;
		if (sText.Find(sValue, 0) >= 0)
		{
			return true;
		}
	}
	return false;
}

void CWorksite::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	if (!m_pSite) {
		return;
	}

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	if (m_bSelected) {
		CCommonDisplay::DrawRect(&memDC, true, RGB(207, 238, 245), RGB(207, 238, 245), rect, 2);
	}
	else {
		CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect, 2);
	}
	CCommonDisplay::DrawCaption(&memDC, m_pSite->szLocation, RGB(120, 120, 120), m_font2, 0, 0, CRect(10, 6, rect.Width() - 10, 24), DT_LEFT | DT_TOP | DT_SINGLELINE);
	CCommonDisplay::DrawCaption(&memDC, m_pSite->szSystemName, RGB(80, 80, 80), m_font, 0, 0, CRect(10, 24, rect.Width() - 10, 49), DT_LEFT | DT_TOP | DT_SINGLELINE);
	CCommonDisplay::DrawCaption(&memDC, m_pSite->szEtc, RGB(120, 120, 120), m_font3, 0, 0, CRect(10, 46, rect.Width() - 15, 62), DT_LEFT | DT_TOP | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CWorksite::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseLbtn && m_nIndex >= 0)
	{
		m_bMouseLbtn = true;
		m_bMouseOn = false;
		SetCapture();
		Redisplay();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CWorksite::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bMouseLbtn && m_nIndex >= 0)
	{
		m_pWnd->PostMessage(WM_LBUTTON_BUTTONlIST_CLICK, 0, m_nIndex);
	}
	if (m_bMouseLbtn)
	{
		m_bMouseLbtn = false;
		ReleaseCapture();
		Redisplay();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CWorksite::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseRbtn && m_nIndex >= 0)
	{
		m_bMouseRbtn = true;
		m_bMouseOn = false;
		SetCapture();
		Redisplay();
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CWorksite::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bMouseRbtn && m_nIndex >= 0)
	{
		m_pWnd->PostMessageW(WM_RBUTTON_BUTTONlIST_CLICK, m_nIndex, 0);
	}
	if (m_bMouseRbtn)
	{
		m_bMouseRbtn = false;
		ReleaseCapture();
		Redisplay();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CWorksite::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bMouseRbtn && !m_bMouseLbtn)
	{
		CRect rect;
		GetClientRect(&rect);
		if (rect.PtInRect(point))
		{
			if (!m_bMouseOn)
			{
				m_bMouseOn = true;
				Redisplay();
			}
			SetTimer(1, 50, NULL);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
