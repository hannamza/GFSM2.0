// ManagerModDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "ManagerModDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CManagerModDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManagerModDlg, CSkinDialog)

CManagerModDlg::CManagerModDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_DIALOG_MODMANAGER, pParent)
{
	m_nLimit = 5;
}

CManagerModDlg::~CManagerModDlg()
{
}

void CManagerModDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_btnOk);
	DDX_Control(pDX, IDC_EDIT_ETC, m_editEtc);
	DDX_Control(pDX, IDC_CHECK_PW, m_checkPw);
	DDX_Control(pDX, IDC_CB_FACP_TYPE, m_ctrlCBFacpType);
}


BEGIN_MESSAGE_MAP(CManagerModDlg, CSkinDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CManagerModDlg::OnBnClickedButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CManagerModDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CManagerModDlg 메시지 처리기입니다.


BOOL CManagerModDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	/*CRect rect;
	GetClientRect(&rect);

	m_btnOk.MoveWindow(rect.Width() / 2 - 120 - 10, 195, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() / 2 + 10, 195, 120, 36);*/

	CRect rect, rt;
	GetDlgItem(IDC_EDIT_ETC)->GetClientRect(&rt);
	GetDlgItem(IDC_EDIT_ETC)->ClientToScreen(&rt);
	ScreenToClient(&rt);

	// 수신기 타입 추가
	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rt.right + 60, rt.bottom + 56 + 70 + 30, SWP_NOMOVE);

	GetClientRect(&rect);

	m_btnOk.MoveWindow(rect.Width() / 2 - 120 - 10, rt.bottom + 15 + 30, 120, 36);
	m_btnCancel.MoveWindow(rect.Width() / 2 + 10, rt.bottom + 15 + 30, 120, 36);

	m_ctrlCBFacpType.AddString(_T("F3 / 한글"));
	m_ctrlCBFacpType.AddString(_T("GT1 / 한글"));
	m_ctrlCBFacpType.AddString(_T("F3 / 영어"));
	m_ctrlCBFacpType.AddString(_T("GT1 / 영어"));
	m_ctrlCBFacpType.SetCurSel(m_nFacpType);

	/*m_editWorksite.SetLimitText(50);
	m_editLoc.SetLimitText(50);*/
	m_editEtc.SetLimitText(400);

	SetDlgItemText(IDC_EDIT_ID, m_sID);
	SetDlgItemText(IDC_EDIT_ETC, m_sEtc);
	CString sTemp;
	sTemp.Format(L"%d", m_nLimit);
	SetDlgItemText(IDC_EDIT_LIMIT, sTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CManagerModDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CManagerModDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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


HBRUSH CManagerModDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CManagerModDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CSkinDialog::OnEraseBkgnd(pDC);
}


void CManagerModDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CSkinDialog::OnPaint()을(를) 호출하지 마십시오.
}


void CManagerModDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CSkinDialog::OnTimer(nIDEvent);
}


void CManagerModDlg::OnBnClickedButtonConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);
	GetDlgItemText(IDC_EDIT_ETC, m_sEtc);
	CString sTemp;
	GetDlgItemText(IDC_EDIT_LIMIT, sTemp);
	m_nLimit = _ttoi(sTemp.GetBuffer(0));
	m_nFacpType = m_ctrlCBFacpType.GetCurSel();

	if (m_checkPw.GetCheck()) {
		if (m_sPW.GetLength() < 4) {
			CMessagePopup dlg(L"계정 관리", L"비밀번호는 4자리 수 이상 입력하셔야 합니다.", MB_OK, this);
			dlg.DoModal();
			return;
		}
	}
	else m_sPW = L"";

	OnOK();
}

void CManagerModDlg::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CManagerModDlg::SetManagerValue(CString sID, CString sPW, CString sEtc, int nLimit, int nFacpType)
{
	m_sID = sID;
	m_sPW = sPW;
	m_sEtc = sEtc;
	m_nLimit = nLimit;
	m_nFacpType = nFacpType;
}

void CManagerModDlg::GetManagerValue(CString & sID, CString & sPW, CString & sEtc, int & nLimit, int & nFacpType)
{
	sID = m_sID;
	sPW = m_sPW;
	sEtc = m_sEtc;
	nLimit = m_nLimit;
	nFacpType = m_nFacpType;
}
