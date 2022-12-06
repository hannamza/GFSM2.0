// WorksiteInputDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "WorksiteInputDlg.h"
#include "afxdialogex.h"


// CWorksiteInputDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWorksiteInputDlg, CSkinDialog)

CWorksiteInputDlg::CWorksiteInputDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_DIALOG_WORKSITE, pParent)
{

}

CWorksiteInputDlg::~CWorksiteInputDlg()
{
}

void CWorksiteInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_btnOk);
	DDX_Control(pDX, IDC_EDIT_ETC, m_editEtc);
	DDX_Control(pDX, IDC_EDIT_WORKSITE, m_editWorksite);
	DDX_Control(pDX, IDC_EDIT_LOC, m_editLoc);
}


BEGIN_MESSAGE_MAP(CWorksiteInputDlg, CSkinDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CWorksiteInputDlg::OnBnClickedButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CWorksiteInputDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CWorksiteInputDlg �޽��� ó�����Դϴ�.


BOOL CWorksiteInputDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	/*CRect rect;
	GetClientRect(&rect);

	m_btnOk.MoveWindow(rect.Width() - 7 - 120, 160, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() - 17 - 240, 160, 120, 36);*/

	CRect rect, rt;
	GetDlgItem(IDC_EDIT_ETC)->GetClientRect(&rt);
	GetDlgItem(IDC_EDIT_ETC)->GetClientRect(&rt);
	GetDlgItem(IDC_EDIT_ETC)->ClientToScreen(&rt);
	ScreenToClient(&rt);

	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rt.right + 60, rt.bottom + 56 + 70, SWP_NOMOVE);

	GetClientRect(&rect);

	m_btnOk.MoveWindow(rect.Width() / 2 - 120 - 10, rt.bottom + 15, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() / 2 + 10, rt.bottom + 15, 120, 36);


	m_editWorksite.SetLimitText(50);
	m_editLoc.SetLimitText(50);
	m_editEtc.SetLimitText(400);

	SetDlgItemText(IDC_EDIT_WORKSITE, m_sName);
	SetDlgItemText(IDC_EDIT_LOC, m_sLoc);
	SetDlgItemText(IDC_EDIT_ETC, m_sEtc);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CWorksiteInputDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CWorksiteInputDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			if (pMsg->hwnd == m_editEtc.m_hWnd) {
				CString sText;
				GetDlgItemText(IDC_EDIT_ETC, sText);
				sText += L"\r\n";

				m_editEtc.SetSel(0, -1);
				m_editEtc.ReplaceSel(sText);
				m_editEtc.SetFocus();
			}
			return TRUE;
		}
	}

	return CSkinDialog::PreTranslateMessage(pMsg);
}


HBRUSH CWorksiteInputDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CWorksiteInputDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CSkinDialog::OnEraseBkgnd(pDC);
}

void CWorksiteInputDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CSkinDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

void CWorksiteInputDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CSkinDialog::OnTimer(nIDEvent);
}

void CWorksiteInputDlg::GetInputValue(CString & sName, CString & sLoc, CString & sEtc)
{
	sName = m_sName;
	sLoc = m_sLoc;
	sEtc = m_sEtc;
}

void CWorksiteInputDlg::SetInputValue(CString sName, CString sLoc, CString sEtc)
{
	m_sName = sName;
	m_sLoc = sLoc;
	m_sEtc = sEtc;
}

void CWorksiteInputDlg::OnBnClickedButtonConfirm()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItemText(IDC_EDIT_WORKSITE, m_sName);
	GetDlgItemText(IDC_EDIT_LOC, m_sLoc);
	GetDlgItemText(IDC_EDIT_ETC, m_sEtc);

	OnOK();
}


void CWorksiteInputDlg::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
