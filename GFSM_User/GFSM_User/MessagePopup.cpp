// MessagePopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_User.h"
#include "MessagePopup.h"
#include "afxdialogex.h"


// CMessagePopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMessagePopup, CDialogEx)

CMessagePopup::CMessagePopup(CString sTitle, CString sCaption, int nType, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
{
	m_sTitle = sTitle;
	m_sCaption = sCaption;
	m_nType = nType;
	m_nStyle = 0;

	m_font.CreateFont(44, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_HEAVY, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("HY견고딕")); // lpszFacename 

	m_font2.CreateFont(16, // nHeight 
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
}

CMessagePopup::CMessagePopup(CString sTitle, CString sCaption1, CString sCaption2, int nType, UINT nStyle, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
{
	m_sTitle = sTitle;
	m_nStyle = nStyle;
	m_sCaption1 = sCaption1;
	m_sCaption2 = sCaption2;
	m_nType = nType;

	m_font.CreateFont(44, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_HEAVY, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("HY견고딕")); // lpszFacename 

	m_font2.CreateFont(16, // nHeight 
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
}

CMessagePopup::~CMessagePopup()
{
}

void CMessagePopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessagePopup, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnOkClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCancelClick)

	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMessagePopup 메시지 처리기입니다.


BOOL CMessagePopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_btnOk.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	this->SetWindowPos(NULL, 0, 0, 490, 348, SWP_NOMOVE);

	switch (m_nType)
	{
	case MB_OK:
		m_btnOk.MoveWindow(490/2 - 132/2, 251, 132, 44);
		m_btnCancel.ShowWindow(SW_HIDE);
		break;
	case MB_YESNO:
		m_btnOk.MoveWindow(102, 251, 132, 44);
		m_btnCancel.MoveWindow(253, 251, 132, 44);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMessagePopup::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMessagePopup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CMessagePopup::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LONG ExtendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	BYTE byAlphaValue = 0; // 0 ~ 255 투명값 지정[0 - 완전 투명, 255 - 불투명]
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(252, 252, 252), byAlphaValue, LWA_COLORKEY);

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMessagePopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	Redisplay();

	m_btnOk.Invalidate();
	m_btnCancel.Invalidate();
}

void CMessagePopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CMessagePopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush B;
		B.CreateStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT); // 투명 설정 

		return B;
	}
	return hbr;
}

void CMessagePopup::OnOkClick()
{
	OnOK();
}

void CMessagePopup::OnCancelClick()
{
	OnCancel();
}

void CMessagePopup::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	//CBitmap bitmap;
	//CDC memDC;
	//bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	//memDC.CreateCompatibleDC(_pDC);
	//memDC.SelectObject(bitmap);

	//CCommonDisplay::DrawColorBg(&memDC, RGB(252, 252, 252), rect);
	//CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_MSGBG, 0, 0);

	//// title
	//CCommonDisplay::DrawCaption(&memDC, m_sTitle, RGB(255, 111, 0), m_font, 0, 0, CRect(0, 40, rect.Width(), 200), DT_CENTER | DT_TOP);	// message
	//if (m_nStyle == 0) {
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 89, 455, 228), DT_CENTER | DT_VCENTER);
	//}
	//else {
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption1, RGB(80, 80, 80), m_font2, 0, 0, CRect(100, 89, 455, 200), m_nStyle);
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption2, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 200, 455, 228), DT_CENTER | DT_VCENTER);
	//}

	//_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_MSGBG, 0, 0);

	// title
	CCommonDisplay::DrawCaption(_pDC, m_sTitle, RGB(255, 111, 0), m_font, 0, 0, CRect(0, 40, rect.Width(), 200), DT_CENTER | DT_TOP);	// message
	if (m_nStyle == 0) {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 89, 455, 228), DT_CENTER | DT_VCENTER);
	}
	else {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption1, RGB(80, 80, 80), m_font2, 0, 0, CRect(100, 89, 455, 200), m_nStyle);
		CCommonDisplay::DrawCaption(_pDC, m_sCaption2, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 200, 455, 228), DT_CENTER | DT_VCENTER);
	}

	//bitmap.DeleteObject();
	//::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

LRESULT CMessagePopup::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnNcHitTest(point);
}
