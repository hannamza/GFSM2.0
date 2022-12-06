// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_SENDER.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 대화 상자입니다.

//#ifdef _DEBUG
//IMPLEMENT_DYNAMIC(CLoginDlg, CObject)
//#elif
IMPLEMENT_DYNAMIC(CLoginDlg, CSkinDialog)
//#endif

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_LOGIN_DIALOG, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CSkinDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CLoginDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLoginDlg::OnBnClickedButtonCancel)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기입니다.


BOOL CLoginDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_btnOK.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnOK.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	/*CRect rect;
	GetClientRect(&rect);

	m_btnOK.MoveWindow(rect.Width() / 2 - 120 - 30, 175, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() / 2 + 30, 175, 120, 36);*/

	CRect rect, rt;
	GetDlgItem(IDC_EDIT_PW)->GetClientRect(&rt);
	GetDlgItem(IDC_EDIT_PW)->GetClientRect(&rt);
	GetDlgItem(IDC_EDIT_PW)->ClientToScreen(&rt);
	ScreenToClient(&rt);

	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rt.right + 40, rt.bottom + 56 + 70, SWP_NOMOVE);

	GetClientRect(&rect);

	m_btnOK.MoveWindow(rect.Width() / 2 - 120 - 10, rt.bottom + 15, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() / 2 + 10, rt.bottom + 15, 120, 36);

	//m_bSystemMenu = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLoginDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonOk();
		}
	}

	return CSkinDialog::PreTranslateMessage(pMsg);
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}

	return m_brush;

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CSkinDialog::OnTimer(nIDEvent);
}

void CLoginDlg::OnBnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);

	OnOK();
}

void CLoginDlg::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
