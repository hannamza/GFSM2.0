#pragma once

class CCommonDisplay
{
public:
	CCommonDisplay(void);
	~CCommonDisplay(void);

	static void DrawLine(CDC* _pDC, INT nX, INT nY, INT nToX, INT nToY, INT nPenStyle, INT nWidth, COLORREF color);
	static void DrawRect(CDC* _pDC, BOOL bBg, COLORREF penColor, COLORREF bgColor, CRect& rect, INT nPS = 1);
	static void DrawCaption(CDC* _pDC, CString sText, COLORREF textColor, CFont & font, BOOL bBkColor, COLORREF bkColor, CRect & rect, UINT nFormat);
	static void GetTextExtentSize(CDC* _pDC, CString sText, CFont & font, CSize & size);
	static void DrawColorBg(CDC* _pDC, COLORREF color, CRect & rect);
	static void DrawImage(CDC* _pDC, UINT nResource, INT nX=0, INT nY=0, INT nWidth=0, INT nHeight=0, INT nSrcX=0, INT nSrcY=0, INT nSrcWidth=0, INT nSrcHeight=0);
	static void DrawImageTrans(CDC* _pDC, UINT nResource, COLORREF clrTrans, INT nX=0, INT nY=0, INT nWidth=0, INT nHeight=0, INT nSrcX=0, INT nSrcY=0, INT nSrcWidth=0, INT nSrcHeight=0);
	static void DrawImageGrade(CDC* _pDC, UINT nResource, INT nGrade, INT nIndex, INT nX, INT nY, INT nWidth, INT nHeight);
	static void DrawImageGdiplusBMP(CDC* _pDC, UINT nResource, INT nX=0, INT nY=0, INT nWidth=0, INT nHeight=0, INT nSrcX=0, INT nSrcY=0, INT nSrcWidth=0, INT nSrcHeight=0);
	static void DrawImageGdiplusBMPTrans(CDC* _pDC, UINT nResource, INT nX = 0, INT nY = 0, INT nWidth = 0, INT nHeight = 0, INT nSrcX = 0, INT nSrcY = 0, INT nSrcWidth = 0, INT nSrcHeight = 0, COLORREF color = RGB(0, 255, 0));
	static void DrawImageIcon(CDC* _pDC, UINT nResource, INT nX = 0, INT nY = 0, INT nWidth = 0, INT nHeight = 0, INT nSrcX = 0, INT nSrcY = 0, INT nSrcWidth = 0, INT nSrcHeight = 0);
	static void DrawImageGdiplusPNG(CDC* _pDC, UINT nResource, INT nX = 0, INT nY = 0, INT nWidth = 0, INT nHeight = 0, INT nSrcX = 0, INT nSrcY = 0, INT nSrcWidth = 0, INT nSrcHeight = 0);
};

