#include "stdafx.h"
#include "CommonDisplay.h"


CCommonDisplay::CCommonDisplay(void)
{
}


CCommonDisplay::~CCommonDisplay(void)
{
}

void CCommonDisplay::DrawLine(CDC* _pDC, INT nX, INT nY, INT nToX, INT nToY, INT nPenStyle, INT nWidth, COLORREF color)
{
	CPen pen(nPenStyle, nWidth, color);
	CBrush brush(color);
	CPen* oldPen = _pDC->SelectObject(&pen);
	CBrush* oldBrush = _pDC->SelectObject(&brush);
	int nOldBckMode = _pDC->SetBkMode(TRANSPARENT);

	_pDC->MoveTo(nX, nY);
	_pDC->LineTo(nToX, nToY);

	_pDC->SelectObject(oldPen);
	_pDC->SelectObject(oldBrush);
	_pDC->SetBkMode(nOldBckMode);
}

void CCommonDisplay::DrawRect(CDC* _pDC, BOOL bBg, COLORREF penColor, COLORREF bgColor, CRect& rect, INT nPS/*=1*/)
{
	CPen pen(PS_SOLID, nPS, penColor);
	CBrush brush(bgColor);
	CPen* oldPen = _pDC->SelectObject(&pen);
	CBrush* oldBrush = NULL;
	HGDIOBJ oldObj = NULL;
	if (bBg) oldBrush = _pDC->SelectObject(&brush);
	else oldObj = _pDC->SelectObject(GetStockObject(NULL_BRUSH));

	_pDC->Rectangle(&rect);

	_pDC->SelectObject(oldPen);
	if (bBg) _pDC->SelectObject(oldBrush);
	else _pDC->SelectObject(oldObj);
}

void CCommonDisplay::DrawCaption(CDC* _pDC, CString sText, COLORREF textColor, CFont & font, BOOL bBkColor, COLORREF bkColor, CRect & rect, UINT nFormat)
{
	// 텍스트 컬러 
	COLORREF tOldColor = _pDC->SetTextColor(textColor);
	// 폰트 설정
	HGDIOBJ hOldFont = _pDC->SelectObject(&font);
	COLORREF tOldBkColor;
	int nBkMode;
	if(bBkColor)
		tOldBkColor = _pDC->SetBkColor(bkColor);
	else nBkMode = _pDC->SetBkMode(TRANSPARENT);

	_pDC->DrawText(sText, rect, nFormat);
	//sText.ReleaseBuffer();

	_pDC->SelectObject(hOldFont);
	_pDC->SetTextColor(tOldColor);
	if(bBkColor) _pDC->SetBkColor(tOldBkColor);
	else _pDC->SetBkMode(nBkMode);
}

void CCommonDisplay::GetTextExtentSize(CDC* _pDC, CString sText, CFont & font, CSize & size)
{
	try{
	// 폰트 설정
	HGDIOBJ hOldFont = _pDC->SelectObject(&font);

	size = _pDC->GetTextExtent(sText);
	//sText.ReleaseBuffer();

	_pDC->SelectObject(hOldFont);
	}
	catch(...)
	{
		return;
	}
}

void CCommonDisplay::DrawColorBg(CDC* _pDC, COLORREF color, CRect & rect)
{
	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* oldPen = _pDC->SelectObject(&pen);
	CBrush* oldBrush = _pDC->SelectObject(&brush);

	_pDC->Rectangle(&rect);

	_pDC->SelectObject(oldPen);
	_pDC->SelectObject(oldBrush);
}

void CCommonDisplay::DrawImage(CDC* _pDC, UINT nResource, INT nX/*=0*/, INT nY/*=0*/, INT nWidth/*=0*/, INT nHeight/*=0*/, INT nSrcX/*=0*/, INT nSrcY/*=0*/, INT nSrcWidth/*=0*/, INT nSrcHeight/*=0*/)
{
	HGDIOBJ hOldBmp;
	BITMAP bm;

	CBitmap* pBitmap = NULL, image;
	if (image.LoadBitmap(nResource))
		pBitmap = &image;
	else ASSERT(0);

	CDC memDC;
	memDC.CreateCompatibleDC(_pDC);
	hOldBmp = memDC.SelectObject(*pBitmap);

	if(CBitmap::FromHandle(*pBitmap))
		(CBitmap::FromHandle(*pBitmap))->GetBitmap(&bm);

	if(nWidth == 0)
		nWidth = bm.bmWidth;
	if(nHeight == 0)
		nHeight = bm.bmHeight;

	if(nSrcWidth == 0)
		nSrcWidth = bm.bmWidth;
	if(nSrcHeight == 0)
		nSrcHeight = bm.bmHeight;

	_pDC->StretchBlt(nX, nY, nWidth, nHeight, &memDC, nSrcX, nSrcY, nSrcWidth, nSrcHeight, SRCCOPY);

	memDC.SelectObject(hOldBmp);
	::DeleteObject(memDC);
}

void CCommonDisplay::DrawImageTrans(CDC* _pDC, UINT nResource, COLORREF clrTrans, INT nX/*=0*/, INT nY/*=0*/, INT nWidth/*=0*/, INT nHeight/*=0*/, INT nSrcX/*=0*/, INT nSrcY/*=0*/, INT nSrcWidth/*=0*/, INT nSrcHeight/*=0*/)
{
	HGDIOBJ hOldBmp;
	BITMAP bm;

	CBitmap* pBitmap = NULL, image;
	if (image.LoadBitmap(nResource))
		pBitmap = &image;
	else ASSERT(0);

	CDC memDC;
	memDC.CreateCompatibleDC(_pDC);
	hOldBmp = memDC.SelectObject(pBitmap);

	if(CBitmap::FromHandle(*pBitmap))
		(CBitmap::FromHandle(*pBitmap))->GetBitmap(&bm);

	if(nWidth == 0)
		nWidth = bm.bmWidth;
	if(nHeight == 0)
		nHeight = bm.bmHeight;

	if(nSrcWidth == 0)
		nSrcWidth = bm.bmWidth;
	if(nSrcHeight == 0)
		nSrcHeight = bm.bmHeight;

	_pDC->TransparentBlt(nX, nY, nWidth, nHeight, &memDC, nSrcX, nSrcY, nSrcWidth, nSrcHeight, clrTrans);

	memDC.SelectObject(hOldBmp);
	::DeleteObject(memDC);
}

void CCommonDisplay::DrawImageGrade(CDC* _pDC, UINT nResource, INT nGrade, INT nIndex, INT nX, INT nY, INT nWidth, INT nHeight)
{
	HGDIOBJ hOldBmp;
	BITMAP bm;

	CBitmap* pBitmap = NULL, image;
	if(image.LoadBitmap(nResource))
		pBitmap = &image;
	else ASSERT(0);

	CDC memDC;
	memDC.CreateCompatibleDC(_pDC);
	hOldBmp = memDC.SelectObject(*pBitmap);

	if(CBitmap::FromHandle(*pBitmap))
		(CBitmap::FromHandle(*pBitmap))->GetBitmap(&bm);

	INT nSrcWidth = bm.bmWidth;
	INT nSrcHeight = bm.bmHeight / nGrade;

	_pDC->StretchBlt(nX, nY, nWidth, nHeight, &memDC, 0, nSrcHeight * nIndex, nSrcWidth, nSrcHeight, SRCCOPY);

	pBitmap->DeleteObject();
	DeleteObject(image);
	memDC.SelectObject(hOldBmp);
	::DeleteObject(memDC);
}

void CCommonDisplay::DrawImageGdiplusBMP(CDC* _pDC, UINT nResource, INT nX, INT nY, INT nWidth, INT nHeight, INT nSrcX, INT nSrcY, INT nSrcWidth, INT nSrcHeight)
{
	HINSTANCE h111 = AfxGetApp()->m_hInstance;
	HINSTANCE h222 = AfxGetInstanceHandle();
	Bitmap* pBitmap = Bitmap::FromResource(/*AfxGetApp()->m_hInstance*/AfxGetInstanceHandle(), MAKEINTRESOURCE(nResource));
	if (!pBitmap)
	{
		return;
	}

	if(nWidth == 0) nWidth = pBitmap->GetWidth();
	if(nHeight == 0) nHeight = pBitmap->GetHeight();
	if(nSrcWidth == 0) nSrcWidth = pBitmap->GetWidth();
	if(nSrcHeight == 0) nSrcHeight = pBitmap->GetHeight();

	Graphics graphics(_pDC->m_hDC);
	graphics.DrawImage(pBitmap, Rect(nX, nY, nWidth, nHeight), nSrcX, nSrcY, nSrcWidth, nSrcHeight, UnitPixel);
}

void CCommonDisplay::DrawImageGdiplusBMPTrans(CDC* _pDC, UINT nResource, INT nX, INT nY, INT nWidth, INT nHeight, INT nSrcX, INT nSrcY, INT nSrcWidth, INT nSrcHeight, COLORREF color)
{
	Bitmap* pBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(nResource));
	if (!pBitmap)
	{
		return;
	}

	if (nWidth == 0) nWidth = pBitmap->GetWidth();
	if (nHeight == 0) nHeight = pBitmap->GetHeight();
	if (nSrcWidth == 0) nSrcWidth = pBitmap->GetWidth();
	if (nSrcHeight == 0) nSrcHeight = pBitmap->GetHeight();

	Graphics graphics(_pDC->m_hDC);
	//graphics.SetSmoothingMode(SmoothingModeHighQuality);

	ImageAttributes imAtt;
	imAtt.SetColorKey(Color(color), Color(color), ColorAdjustTypeBitmap);

	graphics.DrawImage(pBitmap, Rect(nX, nY, nWidth, nHeight), nSrcX, nSrcY, nSrcWidth, nSrcHeight, UnitPixel, &imAtt);
}

void CCommonDisplay::DrawImageIcon(CDC* _pDC, UINT nResource, INT nX/*=0*/, INT nY/*=0*/, INT nWidth/*=0*/, INT nHeight/*=0*/, INT nSrcX/*=0*/, INT nSrcY/*=0*/, INT nSrcWidth/*=0*/, INT nSrcHeight/*=0*/)
{
	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(nResource));
	if (!hIcon)
	{
		return;
	}
	ICONINFO ii;
	::ZeroMemory(&ii, sizeof(ICONINFO));
	bool bRetValue = ::GetIconInfo(hIcon, &ii);

	DWORD dwWidth = (DWORD)(ii.xHotspot * 2);
	DWORD dwHeight = (DWORD)(ii.yHotspot * 2);
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	if (nWidth == 0) nWidth = dwWidth;
	if (nHeight == 0) nHeight = dwHeight;
	if (nSrcWidth == 0) nSrcWidth = dwWidth;
	if (nSrcHeight == 0) nSrcHeight = dwHeight;

	CPoint pt(nX, nY);
	CSize size(nWidth, nHeight);
	_pDC->DrawState(pt, size, hIcon, DSS_NORMAL, (HBRUSH)NULL);

	DestroyIcon(hIcon);
}

void CCommonDisplay::DrawImageGdiplusPNG(CDC* _pDC, UINT nResource, INT nX, INT nY, INT nWidth, INT nHeight, INT nSrcX, INT nSrcY, INT nSrcWidth, INT nSrcHeight)
{
	HRSRC hResource = FindResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(nResource), TEXT("PNG"));
	if (!hResource) return;

	DWORD imageSize = SizeofResource(AfxGetApp()->m_hInstance, hResource);
	HGLOBAL hGlobal = LoadResource(AfxGetApp()->m_hInstance, hResource);
	LPVOID pData = LockResource(hGlobal);
	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	LPSTREAM pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image image(pStream);
	pStream->Release();

	if (nWidth == 0) nWidth = image.GetWidth();
	if (nHeight == 0) nHeight = image.GetHeight();
	if (nSrcWidth == 0) nSrcWidth = image.GetWidth();
	if (nSrcHeight == 0) nSrcHeight = image.GetHeight();

	REAL rTransparency = 1.0f;    // 0.1f ~ 1.0f
	ColorMatrix colorMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rTransparency, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ia;
	ia.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	Graphics graphics(_pDC->m_hDC);
	graphics.DrawImage(&image, Rect(nX, nY, nWidth, nHeight), nSrcX, nSrcY, nSrcWidth, nSrcHeight, UnitPixel, &ia);
}
