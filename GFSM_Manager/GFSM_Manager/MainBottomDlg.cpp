// MainBottomDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MainBottomDlg.h"
#include "afxdialogex.h"
#include "UserInputDlg.h"
#include "MessagePopup.h"

// CMainBottomDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMainBottomDlg, CDialogEx)

CMainBottomDlg::CMainBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainBottomDlg::IDD, pParent)
{
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

	m_bInit = false;
	m_nUserSeq = -1;
}

CMainBottomDlg::~CMainBottomDlg()
{
	m_listUser.DeleteAllItems();
}

void CMainBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_MOD, m_btnMod);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
}


BEGIN_MESSAGE_MAP(CMainBottomDlg, CDialogEx)
	ON_MESSAGE(REDISPLAY_MSG, OnRedisplayMsg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMainBottomDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CMainBottomDlg::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMainBottomDlg::OnBnClickedButtonDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CMainBottomDlg::OnLvnItemchangedListUser)
END_MESSAGE_MAP()


// CMainBottomDlg �޽��� ó�����Դϴ�.

const TCHAR* lpszHeaderUser[] = { _T("No"), _T("�̸�"), _T("��ȭ��ȣ"), _T("���"), NULL };
const int colWidthListUser[] = { 15, 40, 60, 70, 0 };

BOOL CMainBottomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetBackgroundColor(RGB(255, 255, 255));

	CCommonState::Instance()->m_pUserWnd = this;

	m_btnAdd.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnAdd.SetIcon(Global.GetIcon(IDX_ICON_ADD, ICON16), NULL, 5);

	m_btnMod.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnMod.SetIcon(Global.GetIcon(IDX_ICON_SETUP, ICON16), NULL, 5);

	m_btnDel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnDel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	CXListCtrl::InitList(m_listUser, lpszHeaderUser);
	CXListCtrl::ResizeListColumn(m_listUser, colWidthListUser);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMainBottomDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMainBottomDlg::PreTranslateMessage(MSG* pMsg)
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

HBRUSH CMainBottomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CMainBottomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	Redisplay();

	m_btnAdd.Invalidate();
	m_btnMod.Invalidate();
	m_btnDel.Invalidate();
}

void CMainBottomDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMainBottomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMainBottomDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_bInit) {
		CRect rect;
		GetClientRect(&rect);
		if (rect.Width() >= 400) {
			m_btnAdd.MoveWindow(rect.right - 100 - 100 - 100, 25, 95, 32);
			m_btnMod.MoveWindow(rect.right - 100 - 100, 25, 95, 32);
			m_btnDel.MoveWindow(rect.right - 100, 25, 95, 32);

			m_listUser.MoveWindow(0, 62, rect.right - 5, rect.bottom - 62);
		}
	}
}

void CMainBottomDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CCommonDisplay::DrawCaption(_pDC, L"����� ���", RGB(40, 40, 40), m_font, 0, 0, CRect(18, 8, 150, 34), DT_TOP | DT_LEFT | DT_SINGLELINE);
	ReleaseDC(_pDC);
}

void CMainBottomDlg::ReListup()
{
	m_listUser.DeleteAllItems();

	CString sTemp;
	ProtocolResponseGetUserList::userInfo* pInfo;
	int nCount = CUserManager::Instance()->GetCount();
	//_T("No"), _T("�̸�"), _T("��ȭ��ȣ"), _T("���")
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pInfo = CUserManager::Instance()->GetUserInfo(nIndex);
		if (pInfo) {
			sTemp.Format(L"%d", nIndex + 1);
			m_listUser.InsertItem(nIndex, sTemp, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 1, CCommonFunc::Utf8ToWCHAR(pInfo->szName), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 2, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 3, CCommonFunc::Utf8ToWCHAR(pInfo->szEtc), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemData(nIndex, (DWORD)pInfo);
		}
	}
}

LRESULT CMainBottomDlg::OnRedisplayMsg(WPARAM wParam, LPARAM lParam)
{
	int nType = wParam;
	int nValue = lParam;
	switch (nType)
	{
	case 0: ReListup(); break;
	case 1: m_listUser.DeleteAllItems(); break;
	}

	return 0;
}

void CMainBottomDlg::OnBnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (CCommonState::Instance()->m_nManager <= 0) {
		CMessagePopup dlg(L"����� ����", L"���� �߰��Ͻ� ���� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	ProtocolResponseGetManagerList::managerInfo* pInfo = CManagerManager::Instance()->GetManagerInfoBySeq(CCommonState::Instance()->m_nManager);
	if (!pInfo) {
		return;
	}
	if (pInfo->nUserLimit <= CUserManager::Instance()->GetCount()) {
		CString sTemp;
		sTemp.Format(L"����� �� �ִ� �ִ� ����� ��(�� %d ��)�Դϴ�.\n\nȮ�� �� �ٽ� �õ��Ͽ� �ֽʽÿ�.", pInfo->nUserLimit);
		CMessagePopup dlg(L"����� ����", sTemp, MB_OK, this);
		dlg.DoModal();
		return;
	}

	CUserInputDlg dlg;
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sName, sPhone, sEtc;
	dlg.GetInputValue(sName, sPhone, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� ����", L"�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");
	if (sPhone.GetLength() < 8)
	{
		CMessagePopup dlg(L"����� ����", L"��ȭ��ȣ�� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestAddUser(CCommonState::Instance()->m_nManager, sName, sPhone, sEtc);
}

void CMainBottomDlg::OnBnClickedButtonMod()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
	{
		return;
	}

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CUserInputDlg dlg;
	dlg.SetInputValue(CCommonFunc::Utf8ToWCHAR(pInfo->szName), CCommonFunc::Utf8ToWCHAR(pInfo->szMobile), CCommonFunc::Utf8ToWCHAR(pInfo->szEtc));
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sName, sPhone, sEtc;
	dlg.GetInputValue(sName, sPhone, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� ����", L"�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");
	if (sPhone.GetLength() < 8)
	{
		CMessagePopup dlg(L"����� ����", L"��ȭ��ȣ�� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestModUser(pInfo->nSeq, sName, sPhone, sEtc);
}

void CMainBottomDlg::OnBnClickedButtonDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CString sText, sName = CCommonFunc::Utf8ToWCHAR(pInfo->szName);
	sText.Format(L"������ [%s:%s] ����ڸ� �����Ͻðڽ��ϱ�?", sName, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile));
	CMessagePopup dlg(L"����� ����", sText, MB_OKCANCEL, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelUser(pInfo->nSeq);
}


void CMainBottomDlg::OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
		return;
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (pInfo && m_nUserSeq != pInfo->nSeq) {
		m_nUserSeq = pInfo->nSeq;
	}
}
