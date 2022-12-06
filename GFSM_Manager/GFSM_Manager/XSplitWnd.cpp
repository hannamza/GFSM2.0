// XSplitWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "XSplitWnd.h"
#include "afxdialogex.h"


// CXSplitWnd ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CXSplitWnd, CDialogEx)

CXSplitWnd::CXSplitWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_POPUP_DLG, pParent)
{
	m_pParent = pParent;
	m_pLeftWnd = NULL;
	m_pRightWnd = NULL;

	m_bMouseDown = false;
	m_bInit = false;

	m_nIndex = 0;

	m_color = RGB(255, 255, 255);
	m_moveColor = RGB(80, 80, 80);

	m_nSplitLimit = SPLIT_LIMIT;
	m_nTickSize = SPLIT_TICKNESS;
}

CXSplitWnd::~CXSplitWnd()
{
}

void CXSplitWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXSplitWnd, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CXSplitWnd �޽��� ó�����Դϴ�.


BOOL CXSplitWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_bInit = true;

	SetBackgroundColor(m_color);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CXSplitWnd::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CXSplitWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CXSplitWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CXSplitWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

void CXSplitWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

void CXSplitWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CXSplitWnd::SetTransparency(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  // �Լ������� ����, �ʱ�ȭ.
	HINSTANCE hmodUSER32 = LoadLibrary(L"USER32.DLL"); // �ν��Ͻ� ����.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32, "SetLayeredWindowAttributes");
	//�Լ������� ����.
	HWND hwnd = this->m_hWnd; //���̾�α��� �ڵ� ����.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 150, LWA_ALPHA);
}

void CXSplitWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect, rt;
	GetClientRect(&rect);
	if (rect.PtInRect(point)) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	}
	if (m_bMouseDown) {
		rect.left = point.x - m_nDownPoint;
		rect.right = rect.left + m_nTickSize;

		ClientToScreen(&rect);

		if (m_pLeftWnd && m_pRightWnd) {
			m_pLeftWnd->GetClientRect(&rt);
			m_pLeftWnd->ClientToScreen(&rt);
			if (rt.left + m_nSplitLimit > rect.left) {
				rect.left = rt.left + m_nSplitLimit;
				rect.right = rect.left + m_nTickSize;
			}
			m_pRightWnd->GetClientRect(&rt);
			m_pRightWnd->ClientToScreen(&rt);
			if (rt.right - m_nSplitLimit < rect.right) {
				rect.right = rt.right - m_nSplitLimit;
				rect.left = rect.right - m_nTickSize;
			}
		}

		MoveWindow(rect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CXSplitWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) || m_bMouseDown) {
		SetCapture();

		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		m_bMouseDown = true;

		m_nDownPoint = point.x - rect.left;

		ClientToScreen(&rect);
		TRACE("x: %d, y: %d, width: %d, height: %d\n", rect.left, rect.top, rect.Width(), rect.Height());

		SetTransparency(50);
		SetBackgroundColor(m_moveColor);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CXSplitWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bMouseDown) {
		m_bMouseDown = false;
		::ReleaseCapture();
		m_nDownPoint = 0;

		CRect rect;
		GetClientRect(&rect);
		if (m_pLeftWnd && m_pRightWnd) {
			ClientToScreen(&rect);
			Reposition(rect.left);
		}
		SetTransparency(100);
		SetBackgroundColor(m_color);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CXSplitWnd::SetSplitWnd(CWnd* pLeftWnd, CWnd* pRightWnd)
{
	m_pLeftWnd = pLeftWnd;
	m_pRightWnd = pRightWnd;
}

void CXSplitWnd::SetSplitLimit(int nValue)
{
	m_nSplitLimit = nValue;
}

void CXSplitWnd::SetSplitTickSize(int nValue)
{
	m_nTickSize = nValue;
}

void CXSplitWnd::SetSplitIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CXSplitWnd::Reposition(int nX)
{
	if (!m_pParent) {
		return;
	}
	CRect rt, rect, rect1;
	m_pParent->GetClientRect(&rt);
	m_pParent->ClientToScreen(&rt);

	rect.CopyRect(&rt);
	rect.right = nX;
	m_pParent->ScreenToClient(&rect);
	m_pLeftWnd->MoveWindow(rect);

	rect.CopyRect(&rt);
	rect.left = nX + m_nTickSize;
	m_pParent->ScreenToClient(&rect);
	m_pRightWnd->MoveWindow(rect);

	rt.SetRect(nX, rt.top, nX + m_nTickSize, rt.bottom);
	MoveWindow(rt);

	CheckShowWindow();
}

void CXSplitWnd::CheckShowWindow()
{
	CRect rect, rect1;

	GetClientRect(&rect1);
	ClientToScreen(&rect1);
	AfxGetMainWnd()->GetClientRect(&rect);
	AfxGetMainWnd()->ClientToScreen(&rect);
	if (!rect.PtInRect(CPoint(rect1.right, rect1.top))) {
		ShowWindow(SW_HIDE);
	}
	else {
		ShowWindow(SW_SHOW);
	}
}