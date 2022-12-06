// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_User.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_PW, m_editPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnConfirmClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCancelClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnFindClick)

	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CLoginDlg 메시지 처리기입니다.

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	this->SetWindowPos(NULL, 0, 0, 420, 380, SWP_NOMOVE);

	m_editID.MoveWindow(146, 118, 194, 28);
	m_editPW.MoveWindow(146, 179, 194, 28);

	m_btnConfirm.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnFind.Create(IDB_BMP_FIND, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);

	m_btnConfirm.MoveWindow(67, 236, 132, 44);
	m_btnCancel.MoveWindow(218, 236, 132, 44);
	m_btnFind.MoveWindow(67, 292, 283, 34);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			OnConfirmClick();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush B;
		B.CreateStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT); // 투명 설정 

		return B;
	}
	return m_brush;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LONG ExtendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	BYTE byAlphaValue = 0; // 0 ~ 255 투명값 지정[0 - 완전 투명, 255 - 불투명]
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(252, 252, 252), byAlphaValue, LWA_COLORKEY);

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
	m_btnFind.Invalidate();
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CLoginDlg::OnConfirmClick()
{
	GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);

	OnOK();
}

void CLoginDlg::OnCancelClick()
{
	OnCancel();
}

void CLoginDlg::OnFindClick()
{
	CString sName, sPhone, sMsg;
	CReadWriteState state;
	state.SetFileName(L"cs.ini");
	state.ReadText(L"고객상담연락처", L"NAME", sName);
	state.ReadText(L"고객상담연락처", L"PHONE", sPhone);
	sMsg.Format(L"\n\n아래 연락처로 연락 주시기 바랍니다.\n\n%s: %s", sName, sPhone);
	CMessagePopup dlg(L"아이디/패스워드 찾기", sMsg, MB_OK);
	UINT nResult = dlg.DoModal();
}

void CLoginDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	//CBitmap bitmap;
	//CDC memDC;
	//bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	//memDC.CreateCompatibleDC(_pDC);
	//memDC.SelectObject(bitmap);

	////CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_LOGINBG);
	//CCommonDisplay::DrawColorBg(&memDC, RGB(252, 252, 252), rect);
	//CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_LOGIN, 0, 0);

	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_LOGIN, 0, 0);

	//bitmap.DeleteObject();
	//::DeleteObject(memDC);

	/*_pDC->SetBkMode(TRANSPARENT);
	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_LOGIN, 0, 0);*/

	ReleaseDC(_pDC);
}

LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnNcHitTest(point);
	/*int nHit = CDialogEx::OnNcHitTest(point);
	if (nHit == HTCLIENT) {
		nHit = HTCAPTION;
	}

	return nHit;*/
}

void CLoginDlg::n_SetDlgAlpha(int chAlpha)     // 다이얼로그 투명도 조절
{
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	SetLayer pSetLayer = (SetLayer)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	if (pSetLayer == NULL)
	{
		return;
	}

	SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | 0x80000);
	pSetLayer(this->m_hWnd, 0, chAlpha, LWA_ALPHA);
}

