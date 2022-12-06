// PosVSliderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PosVSliderDlg.h"
#include "GFSM_Manager.h"

// CPosVSliderDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPosVSliderDlg, CDialogEx)

CPosVSliderDlg::CPosVSliderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DLG, pParent)
{
	m_bMouseDown = FALSE;
	m_nRange = 100;
	m_nHeight = 100;
	m_nPos = 0;
	m_nMiddle = 100;
	m_dRatio = 1.0;
	m_nTop = 0;
	m_nDownPos = 0;
}

CPosVSliderDlg::~CPosVSliderDlg()
{
}

void CPosVSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPosVSliderDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPosVSliderDlg 메시지 처리기입니다.


BOOL CPosVSliderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(33, 35, 34));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPosVSliderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CPosVSliderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CPosVSliderDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

HBRUSH CPosVSliderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


void CPosVSliderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect, rt;
	GetClientRect(&rect);
	rt.SetRect(0, m_nTop, rect.Width(), m_nTop + m_nMiddle);
	if(rt.PtInRect(point))
	{
		m_nDownPos = point.y;
		m_bMouseDown = TRUE;
		SetCapture();
	}
	else if (m_nTop > point.y)
	{
		CalculateValue(100, 0, 1);
	}
	else if (m_nTop + m_nMiddle < point.y)
	{
		CalculateValue(0, 100, 1);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CPosVSliderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bMouseDown)
	{
		CalculateValue(m_nDownPos, point.y, 1);
		m_bMouseDown = FALSE;
		ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CPosVSliderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	
	if(m_bMouseDown)
	{
		CalculateValue(m_nDownPos, point.y);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CPosVSliderDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CPosVSliderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CPosVSliderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CPosVSliderDlg::Redisplay()
{
	if (m_dRatio >= 1.0)
	{
		return;
	}
	CDC* _pDC = GetDC();
	if(!_pDC) return;

	CRect rect;
	GetClientRect(&rect);
	CBitmap bitmap;
	CDC memDC;

	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_S_M, 0, 0, rect.Width(), rect.Height());
	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_S_T, 0, 0, rect.Width(), 5);
	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_S_B, 0, rect.Height() - 5, rect.Width(), 5);

	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_M_M, 0, m_nTop, rect.Width(), m_nMiddle);
	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_M_T, 0, m_nTop, rect.Width(), 5);
	CCommonDisplay::DrawImage(&memDC, IDB_BITMAP_M_B, 0, m_nTop + m_nMiddle - 5, rect.Width(), 5);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CPosVSliderDlg::SetValue(INT nRange, INT nHeight, INT nPos)
{
	m_nRange = nRange;
	m_nHeight = nHeight;
	m_nPos = nPos;

	m_dRatio = (double)m_nHeight / (double)m_nRange;

	if (m_dRatio >= 1.0)
	{
		return;
	}

	m_nMiddle = (INT)((double)m_nHeight * m_dRatio);
	if (m_nMiddle < 50)
	{
		m_nMiddle = 50;
	}
	int nTerm = m_nHeight - m_nMiddle;
	int nBottom = m_nRange - (nHeight + m_nPos);
	if (nBottom < 0)
	{
		nBottom = 0;
	}
	nBottom *= m_dRatio;
	m_nTop = nTerm - nBottom;

	Redisplay();
}

DWORD g_dwScrollTime = 0;
void CPosVSliderDlg::CalculateValue(INT nDownPos, INT nCurrentPos, INT nType/* = 0*/)
{
	if (nType == 0 && GetTickCount() - g_dwScrollTime < 50)
	{
		return;
	}
	g_dwScrollTime = GetTickCount();
	double dRatio = (double)m_nRange / (double)m_nHeight;
	int nValue = (INT)((double)(nCurrentPos - nDownPos) * dRatio);

	GetParent()->PostMessageW(SLIDER_INFO, nType, nValue);

	return;
}
