// MainTopDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "MainTopDlg.h"
#include "afxdialogex.h"
#include "ManagerInputDlg.h"
#include "MessagePopup.h"
#include "ManagerModDlg.h"

// CMainTopDlg 대화 상자입니다.

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
		_T("굴림")); // lpszFacename 

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


// CMainTopDlg 메시지 처리기입니다.

const TCHAR* lpszHeaderManager[] = { _T("No"), _T("아이디"), _T("사용자 제한"), _T("비고"), NULL };
const int colWidthListManager[] = { 15, 50, 50, 80, 0 };

BOOL CMainTopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainTopDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMainTopDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CMainTopDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_btnAdd.Invalidate();
	m_btnMod.Invalidate();
	m_btnDel.Invalidate();
}

void CMainTopDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMainTopDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMainTopDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

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
	CCommonDisplay::DrawCaption(_pDC, L"계정 목록", RGB(40, 40, 40), m_font, 0, 0, CRect(18, 8, 100, 34), DT_TOP | DT_LEFT | DT_SINGLELINE);
	ReleaseDC(_pDC);
}

void CMainTopDlg::ReListup()
{
	m_listManager.DeleteAllItems();

	CString sTemp;
	ProtocolResponseGetManagerList::managerInfo* pInfo;
	int nCount = CManagerManager::Instance()->GetCount();
	//_T("No"), _T("아이디"), _T("사용자 제한"), _T("비고")
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CCommonState::Instance()->m_nCurrentWorksite <= 0) {
		CMessagePopup dlg(L"계정 관리", L"먼저 추가하실 상위 업체를 선택하여 주십시오.", MB_OK, this);
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
		CMessagePopup dlg(L"계정 관리", L"아이디는 4자리 수 이상 입력하셔야 합니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (sPW.GetLength() < 4)
	{
		CMessagePopup dlg(L"계정 관리", L"비밀번호는 4자리 수 이상 입력하셔야 합니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit <= 0)
	{
		CMessagePopup dlg(L"계정 관리", L"최대 사용자 수 입력을 바르게 넣어 주세요.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit > 30)
	{
		CMessagePopup dlg(L"계정 관리", L"사용자 제한(최대 사용자 수)은 최대 30명까지 입력 가능합니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestAddManager(CCommonState::Instance()->m_nCurrentWorksite, nLimit, sID, sPW, sEtc);
	CCommonState::Instance()->m_nManager = -1;
}

void CMainTopDlg::OnBnClickedButtonMod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSelect = m_listManager.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"계정 관리", L"먼저 수정하실 계정을 선택하여 주십시오.", MB_OK, this);
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
		CMessagePopup dlg(L"계정 관리", L"아이디는 4자리 수 이상 입력하셔야 합니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}*/
	if (nLimit <= 0)
	{
		CMessagePopup dlg(L"계정 관리", L"최대 사용자 수 입력을 바르게 넣어 주세요.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (nLimit > 100)
	{
		CMessagePopup dlg(L"계정 관리", L"최대 사용자 수는 최대 100명까지 입력 가능합니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CClientInterface::Instance()->ProcessRequestModManager(pInfo->nSeq, nLimit, sID, sPW, sEtc);
	CCommonState::Instance()->m_nManager = -1;
}

void CMainTopDlg::OnBnClickedButtonDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSelect = m_listManager.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"계정 관리", L"먼저 삭제하실 계정을 선택하여 주십시오.", MB_OK, this);
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
	sText.Format(L"선택한 [%s] 계정을 삭제하시겠습니까?", CCommonFunc::Utf8ToWCHAR(pInfo->szID));
	CMessagePopup dlg(L"계정 관리", sText, MB_OKCANCEL, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelManager(pInfo->nSeq);
	CCommonState::Instance()->m_nManager = -1;
	CCommonState::Instance()->m_pUserWnd->PostMessage(REDISPLAY_MSG, 1, 0);
}
