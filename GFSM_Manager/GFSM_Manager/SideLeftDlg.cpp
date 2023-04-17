// SideLeftDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "SideLeftDlg.h"
#include "afxdialogex.h"
#include "WorksiteInputDlg.h"
#include "MessagePopup.h"

// CSideLeftDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSideLeftDlg, CDialogEx)

CSideLeftDlg::CSideLeftDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSideLeftDlg::IDD, pParent)
{
	m_pListCtrl = NULL;

	m_font.CreateFont(16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 

	m_font2.CreateFont(15, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 
}

CSideLeftDlg::~CSideLeftDlg()
{
	if (m_pListCtrl)
	{
		m_pListCtrl->DestroyWindow();
	}
}

void CSideLeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_MOD, m_btnMod);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_editSearch);
}


BEGIN_MESSAGE_MAP(CSideLeftDlg, CDialogEx)
	ON_MESSAGE(WM_LBUTTON_BUTTONlIST_CLICK, OnLWorksiteClick)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSideLeftDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSideLeftDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CSideLeftDlg::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CSideLeftDlg::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CSideLeftDlg �޽��� ó�����Դϴ�.


BOOL CSideLeftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetBackgroundColor(RGB(255, 255, 255));

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CWorksiteListCtrl))) {
		if (NULL != (m_pListCtrl = (CWorksiteListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	m_pListCtrl->MoveWindow(10, 40, 371, 540);

	m_pListCtrl->ShowWindow(SW_SHOW);

	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnSearch.SetIcon(Global.GetIcon(IDX_ICON_SEARCH, ICON16), NULL, 5);

	m_btnAdd.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnAdd.SetIcon(Global.GetIcon(IDX_ICON_ADD, ICON16), NULL, 5);

	m_btnMod.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnMod.SetIcon(Global.GetIcon(IDX_ICON_SETUP, ICON16), NULL, 5);

	m_btnDel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnDel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	CRect rect;
	GetClientRect(&rect);

	m_editSearch.MoveWindow(70, 44, 150, 25);
	m_btnSearch.MoveWindow(230, 41, 95, 32);
	m_btnAdd.MoveWindow(28, 84, 95, 32);
	m_btnMod.MoveWindow(129, 84, 95, 32);
	m_btnDel.MoveWindow(230, 84, 95, 32);

	m_btnMod.EnableWindow(false);
	m_btnDel.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSideLeftDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CSideLeftDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			if (pMsg->hwnd == m_editSearch.m_hWnd) {
				OnBnClickedButtonSearch();
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CSideLeftDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CSideLeftDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CSideLeftDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSideLeftDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSideLeftDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_pListCtrl) {
		CRect rect;
		GetClientRect(&rect);
		rect.top += 135;
		rect.left += 1;
		rect.right -= 1;
		rect.bottom -= 1;
		m_pListCtrl->MoveWindow(rect);
	}
}

void CSideLeftDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	/*CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);*/

	CCommonDisplay::DrawCaption(_pDC, L"��ü ���", RGB(40, 40, 40), m_font, 0, 0, CRect(18, 8, 100, 34), DT_TOP | DT_LEFT | DT_SINGLELINE);
	CCommonDisplay::DrawCaption(_pDC, L"�˻�", RGB(80, 80, 80), m_font2, 0, 0, CRect(28, 49, 60, 70), DT_TOP | DT_LEFT | DT_SINGLELINE);

	if (m_pListCtrl) {
		m_pListCtrl->GetClientRect(&rect);
		m_pListCtrl->ClientToScreen(&rect);
		ScreenToClient(&rect);
		rect.left -= 1;
		rect.right += 1;
		rect.top -= 1;
		rect.bottom += 1;
		CCommonDisplay::DrawRect(_pDC, false, RGB(140, 140, 140), 0, rect);
	}

	//_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	//bitmap.DeleteObject();
	ReleaseDC(_pDC);
	//::DeleteObject(memDC);
}

LRESULT CSideLeftDlg::OnLWorksiteClick(WPARAM wParam, LPARAM lParam)
{
	int nWorksite = (int)lParam;
	if (CCommonState::Instance()->m_nCurrentWorksite != nWorksite) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, nWorksite);
		CCommonState::Instance()->m_nCurrentWorksite = nWorksite;
		CCommonState::Instance()->m_nManager = -1;
		CCommonState::Instance()->m_pManagerWnd->PostMessage(REDISPLAY_MSG, 1, 0);
		CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);
		m_btnMod.EnableWindow(true);
		m_btnDel.EnableWindow(true);
	}

	return 0;
}

void CSideLeftDlg::OnBnClickedButtonSearch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sSearch;
	GetDlgItemText(IDC_EDIT_SEARCH, sSearch);
	if (m_pListCtrl) {
		m_pListCtrl->SetSearchWord(sSearch);
	}
	SetDlgItemText(IDC_EDIT_SEARCH, L"");
	m_editSearch.SetFocus();
}

void CSideLeftDlg::OnBnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CWorksiteInputDlg dlg;
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sName, sLoc, sEtc;
	dlg.GetInputValue(sName, sLoc, sEtc);
	if (sName.GetLength() == 0)
	{
		CMessagePopup dlg(L"��ü���� �߰�", L"��ü���� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}

	CClientInterface::Instance()->ProcessRequestAddNewSystem(sName, sLoc, sEtc);

	CCommonState::Instance()->m_pManagerWnd->PostMessage(REDISPLAY_MSG, 1, 0);
	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);

	CCommonState::Instance()->m_nCurrentWorksite = -1;
	CCommonState::Instance()->m_nManager = -1;

	m_pListCtrl->SetSelectedItem(-1);
	m_btnMod.EnableWindow(false);
	m_btnDel.EnableWindow(false);
}

void CSideLeftDlg::OnBnClickedButtonMod()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelected = m_pListCtrl->GetSelectedItemSeq();
	if (nSelected < 0) {
		CMessagePopup dlg(L"��ü ����", L"���� �����Ͻ� ��ü�� �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	pWorkSite pValue = CWorksiteManager::Instance()->GetWorkSiteBySeq(nSelected);
	if (!pValue) {
		return;
	}

	CWorksiteInputDlg dlg;
	dlg.SetInputValue(pValue->szSystemName, pValue->szLocation, pValue->szEtc);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sName, sLoc, sEtc;
	dlg.GetInputValue(sName, sLoc, sEtc);
	if (sName.GetLength() == 0)
	{
		CMessagePopup dlg(L"��ü���� �߰�", L"��ü���� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}

	CClientInterface::Instance()->ProcessRequestModSystem(pValue->nSequenceNo, sName, sLoc, sEtc);

	CCommonState::Instance()->m_pManagerWnd->PostMessage(REDISPLAY_MSG, 1, 0);
	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);

	CCommonState::Instance()->m_nCurrentWorksite = -1;
	CCommonState::Instance()->m_nManager = -1;
	m_pListCtrl->SetSelectedItem(-1);
	m_btnMod.EnableWindow(false);
	m_btnDel.EnableWindow(false);
}

void CSideLeftDlg::OnBnClickedButtonDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelected = m_pListCtrl->GetSelectedItemSeq();
	if (nSelected < 0) {
		CMessagePopup dlg(L"��ü ����", L"���� �����Ͻ� ��ü�� �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	pWorkSite pValue = CWorksiteManager::Instance()->GetWorkSiteBySeq(nSelected);
	if (!pValue) {
		return;
	}
	CString sTemp;
	sTemp.Format(L"[%s] ������ �����Ͻðڽ��ϱ�?", pValue->szSystemName);
	// �޽��� �˾�
	CMessagePopup dlg(L"��ü���� ����", sTemp, MB_OKCANCEL, this);
	UINT nResult = dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}

	CClientInterface::Instance()->ProcessRequestDelSystem(pValue->nSequenceNo);

	CCommonState::Instance()->m_pManagerWnd->PostMessage(REDISPLAY_MSG, 1, 0);
	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);

	CCommonState::Instance()->m_nCurrentWorksite = -1;
	CCommonState::Instance()->m_nManager = -1;
	m_pListCtrl->SetSelectedItem(-1);
	m_btnMod.EnableWindow(false);
	m_btnDel.EnableWindow(false);
}
