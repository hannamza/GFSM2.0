#include "StdAfx.h"
#include "ImageBtn.h"


CImageBtn::CImageBtn(void)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bDown = FALSE;
	m_nDirection = -1;
	m_pBmImage = NULL;
}

CImageBtn::~CImageBtn(void)
{
}

BEGIN_MESSAGE_MAP(CImageBtn, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CImageBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CImageBtn::Create(UINT nResourceID, CBitmap* pBitmap, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_bBitmapType = nResourceID;
	if(!m_bBitmapType)
		m_pBmImage = pBitmap;
	else
	{
		m_bmImage.LoadBitmap(nResourceID);
		m_pBmImage = &m_bmImage;
	}
	
	dwStyle |= BS_OWNERDRAW;
	
	BITMAP bm;
	if(!m_bBitmapType)
		m_pBmImage->GetBitmap(&bm);
	else m_bmImage.GetBitmap(&bm);

	m_bmSize.cx = bm.bmWidth;
	m_bmSize.cy = bm.bmHeight / 4;

	return CButton::Create(_T(""), dwStyle, rect, pParentWnd, nID);
}

void CImageBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
	int ySrc = 0;
	
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
		ySrc = 2;
	
	else if(lpDrawItemStruct->itemState & ODS_DISABLED)
		ySrc = 3;

	if(ySrc == 0)
		Draw(dc, CRect(lpDrawItemStruct->rcItem), 0);
	else Draw(dc, CRect(lpDrawItemStruct->rcItem), ySrc);  
	
	dc.Detach();
	::DeleteObject(&dc);
}

void CImageBtn::Draw(CDC& dc, const CRect& rect, int nPos)
{
	BOOL bHover = FALSE;
	if(nPos == 1) bHover = TRUE;
	CDC dcBitmap;
	CBitmap bitmap;
	dcBitmap.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()*4);

	HGDIOBJ hOldBmp = dcBitmap.SelectObject(bitmap);

	m_sync.Enter();
	CCommonDisplay::DrawImageGdiplusBMP(&dcBitmap, m_bBitmapType, 0, 0, rect.Width(), rect.Height()*4);
	//CCommonDisplay::DrawImage(&dcBitmap, m_pBmImage);
	m_sync.Leave();
	
	int x = rect.left;
	int y = rect.top;
	int width = rect.Width();
	int height = rect.Height();

	nPos *= height;//m_bmSize.cy;
	dc.StretchBlt(x, y, width, height, &dcBitmap, 0, nPos, width, height/*m_bmSize.cx, m_bmSize.cy*/, SRCCOPY);
	dcBitmap.SelectObject(hOldBmp);
	::DeleteObject(&dcBitmap);
}

void CImageBtn::ChangeBitmap(UINT nResource)
{
	m_sync.Enter();
	if (m_pBmImage) {
		m_pBmImage->DeleteObject();
	}

	m_bBitmapType = nResource;
	m_bmImage.LoadBitmap(nResource);
	m_pBmImage = &m_bmImage;
	m_sync.Leave();
}

void CImageBtn::OnMouseHover(UINT nFlags, CPoint point)
{
	if(IsWindowEnabled())
	{
		m_bDown = TRUE;

		CClientDC dc(this);
		CRect rect;
		GetClientRect(&rect),
		Draw(dc, rect, 1);
	}

	CButton::OnMouseHover(nFlags, point);
}

void CImageBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT	tme;
		tme.cbSize		= sizeof (tme);
		tme.hwndTrack	= m_hWnd;
		tme.dwFlags		=  TME_LEAVE | TME_HOVER;;
		tme.dwHoverTime	= 1;

		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CImageBtn::OnMouseLeave()
{
	m_bTracking	= FALSE;
	m_bDown = FALSE;

	if(IsWindowEnabled())
	{
		CClientDC dc(this);
		CRect rect;
		GetClientRect(&rect),
		Draw(dc, rect, 0);
	}

	CButton::OnMouseLeave();
}

void CImageBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CButton::OnLButtonDown(nFlags, point);
}

void CImageBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CButton::OnLButtonUp(nFlags, point);
}

void CImageBtn::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CButton::OnTimer(nIDEvent);
}


BOOL CImageBtn::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;//CButton::OnEraseBkgnd(pDC);
}
