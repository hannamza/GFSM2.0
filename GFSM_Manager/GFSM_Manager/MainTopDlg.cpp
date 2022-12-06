// MainTopDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MainTopDlg.h"
#include "afxdialogex.h"
#include "ManagerInputDlg.h"
#include "MessagePopup.h"
#include "ManagerModDlg.h"

// CMainTopDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMainTopDlg, CDialogEx)

CMainTopDlg::CMainTopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainTopDlg::IDD, pParent)
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
	CCommonState::Instance()->m_nManager = -1;
	m_bLock = false;
}

CMainTopDlg::~CMainTopDlg()
{
	m_listManager.DeleteAllItems();
}

void CMainTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_MOD, m_btnMod);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
	DDX_Control(pDX, IDC_LIST_MANAGER, m_listManager);
}


BEGIN_MESSAGE_MAP(CMainTopDlg, CDialogEx)
	ON_MESSAGE(REDISPLAY_MSG, OnRedisplayMsg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMainTopDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CMainTopDlg::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMainTopDlg::OnBnClickedButtonDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MANAGER, &CMainTopDlg::OnLvnItemchangedListManager)
END_MESSAGE_MAP()


// CMainTopDlg �޽��� ó�����Դϴ�.

const TCHAR* lpszHeaderManager[] = { _T("No"), _T("���̵�"), _T("����� ����"), _T("���"), NULL };
const int colWidthListManager[] = { 15, 50, 50, 80, 0 };

BOOL CMainTopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetBackgroundColor(RGB(255, 255, 255));

	CCommonState::Instance()->m_pManagerWnd = this;

	m_btnAdd.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnAdd.SetIcon(Global.GetIcon(IDX_ICON_ADD, ICON16), NULL, 5);

	m_btnMod.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnMod.SetIcon(Global.GetIcon(IDX_ICON_SETUP, ICON16), NULL, 5);

	m_btnDel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE2), Global.GetRGB(IDX_RGB_MASK));
	m_btnDel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	CXListCtrl::InitList(m_listManager, lpszHeaderManager);
	CXListCtrl::ResizeListColumn(m_listManager, colWidthListManager);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMainTopDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMainTopDlg::PreTranslateMessage(MSG* pMsg)
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

HBRUSH CMainTopDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CMainTopDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_btnAdd.Invalidate();
	m_btnMod.Invalidate();
	m_btnDel.Invalidate();
}

void CMainTopDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMainTopDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMainTopDlg::OnSize(UINT nType, int cx, int cy)
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

			m_listManager.MoveWindow(0, 62, rect.right - 5, rect.bottom - 62);
		}
	}
}

void CMainTopDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CCommonDisplay::DrawCaption(_pDC, L"���� ���", RGB(40, 40, 40), m_font, 0, 0, CRect(18, 8, 100, 34), DT_TOP | DT_LEFT | DT_SINGLELINE);
	ReleaseDC(_pDC);
}

void CMainTopDlg::ReListup()
{
	m_listManager.DeleteAllItems();

	CString sTemp;
	ProtocolResponseGetManagerList::managerInfo* pInfo;
	int nCount = CManagerManager::Instance()->GetCount();
	//_T("No"), _T("���̵�"), _T("����� ����"), _T("���")
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pInfo = CManagerManager::Instance()->GetManagerInfo(nIndex);
		if (pInfo) {
			sTemp.Format(L"%d", nIndex + 1);
			m_listManager.InsertItem(nIndex, sTemp, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listManager.SetItemText(nIndex, 1, CCommonFunc::Utf8ToWCHAR(pInfo->szID), RGB(80, 80, 80), RGB(255, 255, 255));
			sTemp.Format(L"%d", pInfo->nUserLimit);
			m_listManager.SetItemText(nIndex, 2, sTemp, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listManager.SetItemText(nIndex, 3, CCommonFunc::Utf8ToWCHAR(pInfo->szEtc), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listManager.SetItemData(nIndex, (DWORD)pInfo);
		}
	}
}

LRESULT CMainTopDlg::OnRedisplayMsg(WPARAM wParam, LPARAM lParam)
{
	int nType = wParam;
	int nValue = lParam;
	switch (nType)
	{
	case 0: 
		m_bLock = true;
		ReListup();
		CCommonState::Instance()->m_nManager = -1;
		break;
	case 1:
		m_bLock = true;
		m_listManager.DeleteAllItems();
		CCommonState::Instance()->m_nManager = -1;
		break;
	}

	m_bLock = false;
	return 0;
}

void CMainTopDlg::OnLvnItemchangedListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	if (m_bLock) {
		return;
	}

	int nSelect = m_listManager.GetCurSel();
	if (nSelect < 0)
		return;
	if (CManagerManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetManagerList::managerInfo* pInfo = CManagerManager::Instance()->GetManagerInfo(nSelect);
	if (pInfo && CCommonState::Instance()->m_nManager != pInfo->nSeq) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, pInfo->nSeq);
		CCommonState::Instance()->m_nManager = pInfo->nSeq;
		CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);
	}
}

void CMainTopDlg::OnBnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (CCommonState::Instance()->m_nCurrentWorksite <= 0) {
		CMessagePopup dlg(L"���� ����", L"���� �߰��Ͻ� ���� ��ü�� �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CManagerInputDlg dlg;
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sID, sPW, sEtc;
	int  nLimit;
	dlg.GetInputValue(sID, sPW, sEtc, nLimit);
	if (sID.GetLength() < 4)
	{
		CMessagePopup dlg(L"���� ����", L"���̵�� 4�ڸ� �� �̻� �Է��ϼž� �մϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (sPW.GetLength() < 4)
	{
		CMessagePopup dlg(L"���� ����", L"��й�ȣ�� 4�ڸ� �� �̻� �Է��ϼž� �մϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit <= 0)
	{
		CMessagePopup dlg(L"���� ����", L"�ִ� ����� �� �Է��� �ٸ��� �־� �ּ���.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit > 30)
	{
		CMessagePopup dlg(L"���� ����", L"����� ����(�ִ� ����� ��)�� �ִ� 30����� �Է� �����մϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestAddManager(CCommonState::Instance()->m_nCurrentWorksite, nLimit, sID, sPW, sEtc);
	CCommonState::Instance()->m_nManager = -1;
}

void CMainTopDlg::OnBnClickedButtonMod()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelect = m_listManager.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"���� ����", L"���� �����Ͻ� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CManagerManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetManagerList::managerInfo* pInfo = CManagerManager::Instance()->GetManagerInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CManagerModDlg dlg;
	dlg.SetManagerValue(CCommonFunc::Utf8ToWCHAR(pInfo->szID), L"", CCommonFunc::Utf8ToWCHAR(pInfo->szEtc), pInfo->nUserLimit);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CString sID, sPW, sEtc;
	int  nLimit;
	dlg.GetManagerValue(sID, sPW, sEtc, nLimit);
	/*if (sID.GetLength() < 4)
	{
		CMessagePopup dlg(L"���� ����", L"���̵�� 4�ڸ� �� �̻� �Է��ϼž� �մϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}*/
	if (nLimit <= 0)
	{
		CMessagePopup dlg(L"���� ����", L"�ִ� ����� �� �Է��� �ٸ��� �־� �ּ���.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit > 100)
	{
		CMessagePopup dlg(L"���� ����", L"�ִ� ����� ���� �ִ� 100����� �Է� �����մϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestModManager(pInfo->nSeq, nLimit, sID, sPW, sEtc);
	CCommonState::Instance()->m_nManager = -1;
}

void CMainTopDlg::OnBnClickedButtonDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nSelect = m_listManager.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"���� ����", L"���� �����Ͻ� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CManagerManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetManagerList::managerInfo* pInfo = CManagerManager::Instance()->GetManagerInfo(nSelect);
	if (!pInfo) {
		return;
	}

	CString sText;
	sText.Format(L"������ [%s] ������ �����Ͻðڽ��ϱ�?", CCommonFunc::Utf8ToWCHAR(pInfo->szID));
	CMessagePopup dlg(L"���� ����", sText, MB_OKCANCEL, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelManager(pInfo->nSeq);
	CCommonState::Instance()->m_nManager = -1;
	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);
}
