// MainFrameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MainFrameDlg.h"
#include "afxdialogex.h"


// CMainFrameDlg ��ȭ �����Դϴ�.

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


// CMainFrameDlg �޽��� ó�����Դϴ�.


BOOL CMainFrameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMainFrameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMainFrameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CMainFrameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

void CMainFrameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMainFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_pSplit) {
		m_pSplit->ShowWindow(SW_HIDE);
		SetTimer(1, 10, NULL);
	}
}
