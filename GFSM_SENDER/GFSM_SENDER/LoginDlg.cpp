// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_SENDER.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg ��ȭ �����Դϴ�.

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


// CLoginDlg �޽��� ó�����Դϴ�.


BOOL CLoginDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CLoginDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}

	return m_brush;

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CSkinDialog::OnTimer(nIDEvent);
}

void CLoginDlg::OnBnClickedButtonOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);

	OnOK();
}

void CLoginDlg::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
