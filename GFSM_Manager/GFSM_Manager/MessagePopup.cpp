// MessagePopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MessagePopup.h"
#include "afxdialogex.h"


// CMessagePopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMessagePopup, CSkinDialog)

CMessagePopup::CMessagePopup(CString sTitle, CString sCaption, int nType, CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_DIALOG_MESSAGE, pParent)
{
	m_sTitle = sTitle;
	m_sCaption = sCaption;
	m_nType = nType;
}

CMessagePopup::~CMessagePopup()
{
}

void CMessagePopup::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_btnOk);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_staticText);
}


BEGIN_MESSAGE_MAP(CMessagePopup, CSkinDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CMessagePopup::OnBnClickedButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CMessagePopup::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CMessagePopup 메시지 처리기입니다.


BOOL CMessagePopup::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	CRect rect;
	GetClientRect(&rect);

	SetWindowText(m_sTitle);
	CDC* _pDC = GetDC();
	CString sText;
	int nIndex = 0;
	CSize size, sz;
	size.cx = 0;
	while (AfxExtractSubString(sText, m_sCaption, nIndex, _T('\n')))
	{
		sz = CCommonFunc::GetTextExtent(_pDC, sText);
		if (size.cx < sz.cx) {
			size.SetSize(sz.cx, sz.cy);
		}
		++nIndex;
	}
	ReleaseDC(_pDC);

	ClientToScreen(&rect);
	rect.right = rect.left + size.cx + 30;
	rect.bottom = rect.top + 250 + 50;
	MoveWindow(rect);

	CenterWindow();

	GetClientRect(&rect);

	m_staticText.MoveWindow(10, 50, rect.Width() - 20, 250);

	switch (m_nType)
	{
	case MB_OKCANCEL:
		m_btnOk.MoveWindow(rect.Width() / 2 - 120 - 10, 160, 120, 36);
		m_btnCancel.MoveWindow(rect.Width() / 2 + 10, 160, 120, 36);
		break;
	case MB_YESNO:
		m_btnOk.SetWindowTextW(L"예");
		m_btnCancel.SetWindowTextW(L"아니오");
		m_btnOk.MoveWindow(rect.Width() / 2 - 120 - 10, 160, 120, 36);
		m_btnCancel.MoveWindow(rect.Width() / 2 + 10, 160, 120, 36);
		break;
	case MB_OK:
		m_btnOk.MoveWindow(rect.Width() / 2 - 60, 160, 120, 36);
		m_btnCancel.ShowWindow(SW_HIDE);
		break;
	}

	m_staticText.SetWindowTextW(m_sCaption);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMessagePopup::OnDestroy()
{
	CSkinDialog::OnDestroy();

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

	return CSkinDialog::PreTranslateMessage(pMsg);
}


HBRUSH CMessagePopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CMessagePopup::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CSkinDialog::OnEraseBkgnd(pDC);
}


void CMessagePopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CSkinDialog::OnPaint()을(를) 호출하지 마십시오.
}


void CMessagePopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CSkinDialog::OnTimer(nIDEvent);
}


void CMessagePopup::OnBnClickedButtonConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);*/

	OnOK();
}


void CMessagePopup::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
