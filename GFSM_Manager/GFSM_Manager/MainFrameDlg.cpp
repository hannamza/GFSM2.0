// MainFrameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MainFrameDlg.h"
#include "afxdialogex.h"


// CMainFrameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainFrameDlg, CDialogEx)

CMainFrameDlg::CMainFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainFrameDlg::IDD, pParent)
{
	m_pTopDlg = NULL;
	m_pBottomDlg = NULL;
	m_pSplit = NULL;

	m_bInit = false;
}

CMainFrameDlg::~CMainFrameDlg()
{
	SAFE_DELETE(m_pTopDlg);
	SAFE_DELETE(m_pBottomDlg);
	SAFE_DELETE(m_pSplit);
}

void CMainFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainFrameDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_WM_MOVING()
END_MESSAGE_MAP()


// CMainFrameDlg 메시지 처리기입니다.


BOOL CMainFrameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetBackgroundColor(RGB(255, 255, 255));

	m_pTopDlg = new CMainTopDlg(this);
	m_pBottomDlg = new CMainBottomDlg(this);
	m_pSplit = new CYSplitWnd(this);

	m_pTopDlg->Create(IDD_DIALOG_MANAGER, this);
	m_pBottomDlg->Create(IDD_DIALOG_USER, this);
	m_pSplit->Create(IDD_COMMON_POPUP_DLG, this);

	m_pSplit->SetSplitWnd(m_pTopDlg, m_pBottomDlg);
	::SetWindowPos(m_pSplit->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_pTopDlg->ShowWindow(SW_SHOW);
	m_pBottomDlg->ShowWindow(SW_SHOW);
	m_pSplit->ShowWindow(SW_SHOW);

	CPoint point(0, 250);
	ClientToScreen(&point);
	m_pSplit->Reposition(point.y);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainFrameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMainFrameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CMainFrameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CMainFrameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

void CMainFrameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		if (!GetAsyncKeyState(VK_LBUTTON)) {
			KillTimer(1);
			if (m_pSplit) {
				m_pSplit->CheckShowWindow();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMainFrameDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMainFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}

	CRect rect, rt;
	m_pTopDlg->GetClientRect(&rt);
	m_pTopDlg->ClientToScreen(&rt);

	m_pSplit->Reposition(rt.bottom);
}

void CMainFrameDlg::MoveNotify()
{
	if (!m_bInit) {
		return;
	}

	CRect rect, rt;
	m_pTopDlg->GetClientRect(&rt);
	m_pTopDlg->ClientToScreen(&rt);

	m_pSplit->Reposition(rt.bottom);
}

void CMainFrameDlg::MovingNotify()
{
	if (m_pSplit) {
		m_pSplit->ShowWindow(SW_HIDE);
		SetTimer(1, 10, NULL);
	}
}

void CMainFrameDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (!m_bInit) {
		return;
	}

	CRect rect, rt;
	m_pTopDlg->GetClientRect(&rt);
	m_pTopDlg->ClientToScreen(&rt);

	m_pSplit->Reposition(rt.bottom);
}


void CMainFrameDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pSplit) {
		m_pSplit->ShowWindow(SW_HIDE);
		SetTimer(1, 10, NULL);
	}
}
