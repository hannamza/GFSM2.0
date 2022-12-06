// YSplitWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "YSplitWnd.h"
#include "afxdialogex.h"


// CYSplitWnd 대화 상자입니다.

IMPLEMENT_DYNAMIC(CYSplitWnd, CDialogEx)

CYSplitWnd::CYSplitWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_POPUP_DLG, pParent)
{
	m_pParent = pParent;
	m_pTopWnd = NULL;
	m_pBottomWnd = NULL;

	m_bMouseDown = false;
	m_bInit = false;

	m_nIndex = 0;

	m_color = RGB(255, 255, 255);
	m_moveColor = RGB(80, 80, 80);

	m_nSplitLimit = SPLIT_LIMIT;
	m_nTickSize = SPLIT_TICKNESS;
}

CYSplitWnd::~CYSplitWnd()
{
}

void CYSplitWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYSplitWnd, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CYSplitWnd 메시지 처리기입니다.


BOOL CYSplitWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bInit = true;

	SetBackgroundColor(m_color);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CYSplitWnd::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CYSplitWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CYSplitWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CYSplitWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

void CYSplitWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}

	CRect rect, rect1;
	GetClientRect(&rect1);
	ClientToScreen(&rect1);
	AfxGetMainWnd()->GetClientRect(&rect);
	AfxGetMainWnd()->ClientToScreen(&rect);
	if (!rect.PtInRect(CPoint(rect1.left, rect1.bottom))) {
		ShowWindow(SW_HIDE);
	}
	else {
		ShowWindow(SW_SHOW);
	}
}

void CYSplitWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CYSplitWnd::SetTransparency(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  // 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(L"USER32.DLL"); // 인스턴스 얻음.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32, "SetLayeredWindowAttributes");
	//함수포인터 얻음.
	HWND hwnd = this->m_hWnd; //다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 150, LWA_ALPHA);
}

void CYSplitWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect, rt;
	GetClientRect(&rect);
	if (rect.PtInRect(point)) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
	}
	if (m_bMouseDown) {
		rect.top = point.y - m_nDownPoint;
		rect.bottom = rect.top + m_nTickSize;

		ClientToScreen(&rect);

		if (m_pTopWnd && m_pBottomWnd) {
			m_pTopWnd->GetClientRect(&rt);
			m_pTopWnd->ClientToScreen(&rt);
			if (rt.top + m_nSplitLimit > rect.top) {
				rect.top = rt.top + m_nSplitLimit;
				rect.bottom = rect.top + m_nTickSize;
			}
			m_pBottomWnd->GetClientRect(&rt);
			m_pBottomWnd->ClientToScreen(&rt);
			if (rt.bottom - m_nSplitLimit < rect.bottom) {
				rect.bottom = rt.bottom - m_nSplitLimit;
				rect.top = rect.bottom - m_nTickSize;
			}
		}

		MoveWindow(rect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CYSplitWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) || m_bMouseDown) {
		SetCapture();

		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		m_bMouseDown = true;

		m_nDownPoint = point.y - rect.top;

		ClientToScreen(&rect);
		TRACE("x: %d, y: %d, width: %d, height: %d\n", rect.left, rect.top, rect.Width(), rect.Height());

		SetTransparency(50);
		SetBackgroundColor(m_moveColor);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CYSplitWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bMouseDown) {
		m_bMouseDown = false;
		::ReleaseCapture();
		m_nDownPoint = 0;

		SetTransparency(100);
		SetBackgroundColor(m_color);

		CRect rect;
		GetClientRect(&rect);
		if (m_pTopWnd && m_pBottomWnd) {
			ClientToScreen(&rect);
			Reposition(rect.top);
		}

	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CYSplitWnd::SetSplitLimit(int nValue)
{
	m_nSplitLimit = nValue;
}

void CYSplitWnd::SetSplitTickSize(int nValue)
{
	m_nTickSize = nValue;
}

void CYSplitWnd::SetSplitWnd(CWnd* pTopWnd, CWnd* pBottomWnd)
{
	m_pTopWnd = pTopWnd;
	m_pBottomWnd = pBottomWnd;
}

void CYSplitWnd::SetSplitIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CYSplitWnd::Reposition(int nY)
{
	if (!m_pParent) {
		return;
	}
	CRect rt, rect, rect1;
	m_pParent->GetClientRect(&rt);
	m_pParent->ClientToScreen(&rt);

	rect.CopyRect(&rt);
	rect.bottom = nY;
	m_pParent->ScreenToClient(&rect);
	m_pTopWnd->MoveWindow(rect);

	rect.CopyRect(&rt);
	rect.top = nY + m_nTickSize;
	m_pParent->ScreenToClient(&rect);
	m_pBottomWnd->MoveWindow(rect);

	rt.SetRect(rt.left, nY, rt.right, nY + m_nTickSize);
	MoveWindow(rt);

	CheckShowWindow();
}

void CYSplitWnd::CheckShowWindow()
{
	CRect rect, rect1;

	GetClientRect(&rect1);
	ClientToScreen(&rect1);
	AfxGetMainWnd()->GetClientRect(&rect);
	AfxGetMainWnd()->ClientToScreen(&rect);
	if (!rect.PtInRect(CPoint(rect1.left, rect1.bottom))) {
		ShowWindow(SW_HIDE);
	}
	else {
		ShowWindow(SW_SHOW);
	}
}
