
// GFSM_UserDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GFSM_User.h"
#include "GFSM_UserDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGFSM_UserDlg 대화 상자



CGFSM_UserDlg::CGFSM_UserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GFSM_USER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInit = false;

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
		_T("돋움")); // lpszFacename 
	m_nUserSeq = 0;
	m_bDestroy = false;
}

void CGFSM_UserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ETC, m_editEtc);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
}

BEGIN_MESSAGE_MAP(CGFSM_UserDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnAddClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnModClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnDelClick)

	ON_COMMAND(IDC_COMMON_BUTTON4, OnMiniClick)
	ON_COMMAND(IDC_COMMON_BUTTON5, OnCloseClick)

	ON_MESSAGE(REDISPLAY_MSG, OnRedisplayMsg)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_MOVING()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CGFSM_UserDlg::OnItemchangedListUser)
END_MESSAGE_MAP()


// CGFSM_UserDlg 메시지 처리기
const TCHAR* lpszHeaderManager[] = { _T("NO"), _T("이름"), _T("전화번호"), _T("담당 업무"), NULL };
const int colWidthListManager[] = { 6, 22, 25, 40, 0 };

BOOL CGFSM_UserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	Log::Setup();
	Log::SetMainPointer(this);

	CClientInterface::New();

#ifndef LOCAL_TEST_MODE
	CClientInterface::Instance()->TryConnection("160.202.162.3", 10234);
#else
	CClientInterface::Instance()->TryConnection("127.0.0.1", 10234);
#endif

	CUserManager::New();
	CCommonState::New();
	CCommonState::Instance()->m_pUserWnd = this;

	m_sMessage = L"로그인하여 주십시오.";

	SetWindowText(L"GFSM 사용자 관리 프로그램");

	Global.SetFont(IDX_FONT_SMALL, _T("돋움"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("돋움"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("돋움"), 11, TRUE);
	Global.SetFont(IDX_FONT_HEADER, _T("돋움"), 10, TRUE);

	CCommonState::Instance()->m_pWnd = this;

	this->SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOMOVE);
	CenterWindow();

	SetTimer(99, 5 * 1000, NULL);

	m_editName.MoveWindow(142, 200, 230, 20);
	m_editPhone.MoveWindow(474, 200, 230, 20);
	m_editEtc.MoveWindow(140, 240, 565, 85);
	m_listUser.MoveWindow(22, 371, 980, 376);

	m_btnAdd.Create(IDB_BMP_ADD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnMod.Create(IDB_BMP_MOD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnDel.Create(IDB_BMP_DEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnMini.Create(IDB_BMP_MAX, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON4);
	m_btnClose.Create(IDB_BMP_CLOSE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON5);

	m_btnMini.MoveWindow(925, 20, 28, 28);
	m_btnClose.MoveWindow(969, 20, 28, 28);

	m_btnAdd.MoveWindow(804, 185, 156, 44);
	m_btnMod.MoveWindow(804, 237, 156, 44);
	m_btnDel.MoveWindow(804, 289, 156, 44);

	CXListCtrl::InitList(m_listUser, lpszHeaderManager);
	CXListCtrl::ResizeListColumn(m_listUser, colWidthListManager);

	SetTimer(1, 100, NULL);

	m_bInit = true;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGFSM_UserDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_bDestroy = true;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
	CUserManager::Delete();

	Log::Cleanup();
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGFSM_UserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		Redisplay();

		m_editName.Invalidate();
		m_editPhone.Invalidate();
		m_editEtc.Invalidate();
		m_listUser.Invalidate();

		m_btnAdd.Invalidate();
		m_btnMod.Invalidate();
		m_btnDel.Invalidate();
		m_btnMini.Invalidate();
		m_btnClose.Invalidate();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGFSM_UserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_UserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)
			{
				CWnd* pWnd = GetFocus();
				if (pWnd && pWnd->m_hWnd == m_editEtc.m_hWnd)
				{
					m_editEtc.SetSel(-1, 0);
					m_editEtc.ReplaceSel(L"\r\n");
				}

				return TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGFSM_UserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

void CGFSM_UserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);

		CLoginDlg dlg;
		UINT nResult = dlg.DoModal();
		if (nResult != IDOK) {
			OnOK();
			return;
		}
		CCommonState::Instance()->m_bLoginResult = false;
		CCommonState::Instance()->m_sID = dlg.m_sID;
		CClientInterface::Instance()->ProcessRequestManagerInfo(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
		DWORD dwLastTime = GetTickCount();
		while (GetTickCount() - dwLastTime < 3000)
		{
			if (CCommonState::Instance()->m_bLoginResult) {
				break;
			}
			Sleep(1);
		}
		if (CCommonState::Instance()->m_nIdx > 0) {
			m_sMessage.Format(L"%s 님 환영합니다.        ", CCommonState::Instance()->m_sID);
			CDC* _pDC = GetDC();
			// message
			CCommonDisplay::DrawCaption(_pDC, m_sMessage, RGB(112, 112, 112), m_font, true, RGB(255, 255, 255), CRect(215, 36, 660, 57), DT_TOP | DT_LEFT | DT_SINGLELINE);
			ReleaseDC(_pDC);
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
		}
		else {
			CMessagePopup dlg(L"로그인", L"\n\n로그인 아이디 또는 비밀번호가 맞지 않습니다.\n\n다시 시도하여 주십시오.", MB_OK);
			dlg.DoModal();
			SetTimer(1, 10, NULL);
		}
	}
	else if (nIDEvent == 99) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CGFSM_UserDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CGFSM_UserDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

void CGFSM_UserDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

HBRUSH CGFSM_UserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CGFSM_UserDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CGFSM_UserDlg::OnRedisplayMsg(WPARAM wParam, LPARAM lParam)
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

void CGFSM_UserDlg::ReListup()
{
	m_listUser.DeleteAllItems();

	CString sTemp;
	ProtocolResponseGetUserList::userInfo* pInfo;
	int nCount = CUserManager::Instance()->GetCount();
	//_T("No"), _T("이름"), _T("전화번호"), _T("비고")
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

void CGFSM_UserDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BG);

	// message
	CCommonDisplay::DrawCaption(&memDC, m_sMessage, RGB(112, 112, 112), m_font, 0, 0, CRect(215, 36, 660, 57), DT_TOP | DT_LEFT | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

LRESULT CGFSM_UserDlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseLogin:
		break;
	case ProtocolHeader::ResponseAddUser:
		if (nValue == 0 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"사용자 추가", L"\n\n사용자 추가가 완료되었습니다.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"사용자 추가", L"\n\n이미 존재하는 전화번호입니다.\n\n다른 전화번호로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"사용자 추가", L"\n\n사용자 추가에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModUser:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"사용자 수정", L"\n\n사용자 수정이 완료되었습니다.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 수정", L"\n\n사용자 수정에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelUser:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"사용자 삭제", L"\n\n사용자 삭제가 완료되었습니다.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 삭제", L"\n\n사용자 삭제에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CGFSM_UserDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nHit = CDialogEx::OnNcHitTest(point);
	if (nHit == HTCLIENT) {
		nHit = HTCAPTION;
	}

	return nHit;
}

void CGFSM_UserDlg::OnAddClick()
{
	if (CCommonState::Instance()->m_nLimitCount <= CUserManager::Instance()->GetCount()) {
		CString sTemp;
		sTemp.Format(L"\n\n등록할 수 있는 최대 사용자 수(총 %d 명)입니다.\n\n확인 후 다시 시도하여 주십시오.", CCommonState::Instance()->m_nLimitCount);
		CMessagePopup dlg(L"사용자 추가", sTemp, MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sName, sPhone, sEtc;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"사용자 추가", L"\n\n이름은 필수 입력 사항입니다.", MB_OK, this);
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
		CMessagePopup dlg(L"사용자 추가", L"\n\n전화번호는 필수 입력사항입니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"이름: %s\n\n전화번호: %s\n\n담당업무: %s", sName, sPhone, sEtc);
	CMessagePopup dlg(L"사용자 추가", sText, L"이 정보로 사용자를 등록하시겠습니까?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestAddUser(CCommonState::Instance()->m_nIdx, sName, sPhone, sEtc);
}

void CGFSM_UserDlg::OnModClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"사용자 수정", L"\n\n먼저 수정하실 사용자 계정을 선택하여 주십시오.", MB_OK, this);
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
	CString sName, sPhone, sEtc;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"사용자 수정", L"\n\n이름은 필수 입력 사항입니다.", MB_OK, this);
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
		CMessagePopup dlg(L"사용자 수정", L"\n\n전화번호는 필수 입력사항입니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"이름: %s\n\n전화번호: %s\n\n담당업무: %s", sName, sPhone, sEtc);
	CMessagePopup dlg(L"사용자 수정", sText, L"이 정보로 사용자를 수정하시겠습니까?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestModUser(pInfo->nSeq, sName, sPhone, sEtc);
}

void CGFSM_UserDlg::OnDelClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"사용자 삭제", L"\n\n먼저 삭제하실 사용자 계정을 선택하여 주십시오.", MB_OK, this);
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
	sText.Format(L"\n\n[%s : %s]\n\n선택한 사용자를 삭제하시겠습니까?", sName, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile));
	CMessagePopup dlg(L"사용자 삭제", sText, MB_YESNO, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelUser(pInfo->nSeq);
}

void CGFSM_UserDlg::OnMiniClick()
{
	ShowWindow(SW_MINIMIZE);
}

void CGFSM_UserDlg::OnCloseClick()
{
	OnOK();
}


void CGFSM_UserDlg::OnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (m_bDestroy) {
		return;
	}

	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		m_nUserSeq = 0;

		SetDlgItemText(IDC_EDIT_NAME, L"");
		SetDlgItemText(IDC_EDIT_PHONE, L"");
		SetDlgItemText(IDC_EDIT_ETC, L"");
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	m_nUserSeq = pInfo->nSeq;

	SetDlgItemText(IDC_EDIT_NAME, CCommonFunc::Utf8ToWCHAR(pInfo->szName));
	SetDlgItemText(IDC_EDIT_PHONE, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile));
	SetDlgItemText(IDC_EDIT_ETC, CCommonFunc::Utf8ToWCHAR(pInfo->szEtc));
}
