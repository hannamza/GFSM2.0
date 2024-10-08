
// GFSM_ManagerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GFSM_Manager.h"
#include "GFSM_ManagerDlg.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include "MessagePopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CSkinDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CSkinDialog)
END_MESSAGE_MAP()


// CGFSM_ManagerDlg 대화 상자



CGFSM_ManagerDlg::CGFSM_ManagerDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_GFSM_MANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSideLeftDlg = NULL;
	m_pMainFrameDlg = NULL;
	m_pSplit = NULL;

	m_bInit = false;
}

void CGFSM_ManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGFSM_ManagerDlg, CSkinDialog)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CGFSM_ManagerDlg 메시지 처리기

BOOL CGFSM_ManagerDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Log::Setup();
	Log::SetMainPointer(this);

	CClientInterface::New();
#ifndef LOCAL_TEST_MODE
	CClientInterface::Instance()->TryConnection(/*"192.168.1.222"*/"160.202.162.3", 10234);
#else
	CClientInterface::Instance()->TryConnection(/*"192.168.1.222"*/"127.0.0.1", 10234);
#endif
	 
	CWorksiteManager::New();
	CManagerManager::New();
	CUserManager::New();

	CCommonState::New();

	// 타이틀 바에 버전 표시
	CString strTitle = _T("");
	strTitle.Format(_T("%s v%.1f"), CCommonFunc::CharToWCHAR(PROGRAM_NAME), PROGRAM_VERSION);
	SetWindowText(strTitle);

	Global.SetFont(IDX_FONT_SMALL, _T("돋움"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("돋움"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("돋움"), 11, TRUE);

	m_pSideLeftDlg = new CSideLeftDlg(this);
	m_pSideLeftDlg->Create(IDD_DIALOG_LEFTSIDE, this);
	m_pMainFrameDlg = new CMainFrameDlg(this);
	m_pMainFrameDlg->Create(IDD_COMMON_CHILD_DLG, this);

	m_pSplit = new CXSplitWnd(this);
	m_pSplit->Create(IDD_COMMON_POPUP_DLG, this);
	m_pSplit->SetSplitWnd(m_pSideLeftDlg, m_pMainFrameDlg);
	::SetWindowPos(m_pSplit->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pSplit->ShowWindow(SW_SHOW);

	MoveWindow(0, 0, 1024, 768);
	CenterWindow();

	m_pSideLeftDlg->ShowWindow(SW_SHOW);
	m_pMainFrameDlg->ShowWindow(SW_SHOW);

	CPoint point(350, 0);
	ClientToScreen(&point);
	m_pSplit->Reposition(point.x);

	m_bInit = true;

	CCommonState::Instance()->m_pWnd = this;
	SetTimer(2, 100, NULL);

	SetTimer(99, 30 * 1000, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGFSM_ManagerDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	SAFE_DELETE(m_pSideLeftDlg);
	SAFE_DELETE(m_pMainFrameDlg);
	SAFE_DELETE(m_pSplit);

	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();

	CUserManager::Delete();
	CManagerManager::Delete();
	CWorksiteManager::Delete();

	Log::Cleanup();
}

void CGFSM_ManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CSkinDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGFSM_ManagerDlg::OnPaint()
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
		CSkinDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGFSM_ManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_ManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CSkinDialog::PreTranslateMessage(pMsg);
}

HBRUSH CGFSM_ManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CGFSM_ManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CSkinDialog::OnEraseBkgnd(pDC);
}

void CGFSM_ManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}

	CRect rect, rt;
	m_pSideLeftDlg->GetClientRect(&rt);
	m_pSideLeftDlg->ClientToScreen(&rt);

	m_pSplit->Reposition(rt.right);
}

void CGFSM_ManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		KillTimer(1);
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, 0);
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);

		CLoginDlg dlg;
		UINT nResult = dlg.DoModal();
		if (nResult == IDCANCEL) {
			EndDialog(0);
			return;
		}
		else
		{
			CCommonState::Instance()->SetResult(ProtocolHeader::ResponseAdminLogin, false);
			CCommonState::Instance()->m_bLoginResult = false;
			CClientInterface::Instance()->ProcessRequestLogin(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
			DWORD dwLastTime = GetTickCount();
			while (GetTickCount() - dwLastTime <= 3000)
			{
				if (CCommonState::Instance()->GetResult(ProtocolHeader::ResponseAdminLogin)) {
					break;
				}
				if (CCommonState::Instance()->m_bLoginResult) {
					break;
				}
				Sleep(1);
			}
			/*if (CCommonState::Instance()->GetResult(ProtocolHeader::ResponseAdminLogin)) {
				CCommonState::Instance()->SetResult(ProtocolHeader::ResponseGetSystemList, false);
				CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
				SetTimer(1, 10, NULL);
			}
			else {*/
			if(!CCommonState::Instance()->GetResult(ProtocolHeader::ResponseAdminLogin)) {
				SetTimer(2, 10, NULL);
			}
		}
	}
	else if (nIDEvent == 100) {
		if (!GetAsyncKeyState(VK_LBUTTON)) {
			KillTimer(1);
			if (m_pSplit) {
				m_pSplit->CheckShowWindow();
			}
		}
	}
	else if (nIDEvent == 99) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
	}

	CSkinDialog::OnTimer(nIDEvent);
}

void CGFSM_ManagerDlg::OnMove(int x, int y)
{
	CSkinDialog::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}

	if (m_pSplit) {
		CRect rect, rt;
		m_pSideLeftDlg->GetClientRect(&rt);
		m_pSideLeftDlg->ClientToScreen(&rt);

		m_pSplit->Reposition(rt.right);
	}
	if (m_pMainFrameDlg) {
		m_pMainFrameDlg->MoveNotify();
	}
}


void CGFSM_ManagerDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CSkinDialog::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pSplit) {
		m_pSplit->ShowWindow(SW_HIDE);
		SetTimer(100, 10, NULL);
	}
	if (m_pMainFrameDlg) {
		m_pMainFrameDlg->MovingNotify();
	}
}


void CGFSM_ManagerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	CSkinDialog::OnGetMinMaxInfo(lpMMI);
}

LRESULT CGFSM_ManagerDlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseAdminLogin:
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		break;
	case ProtocolHeader::ResponseGetSystemList: // 시스템 목록 수신
		if (m_pSideLeftDlg) {
			m_pSideLeftDlg->Invalidate();
		}
		break;
	case ProtocolHeader::ResponseAddNewSystem: // 시스템 추가 수신
		if (nValue == 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}
		else if(nValue == -1) {
			CMessagePopup dlg(L"업체 추가", L"이미 존재하는 업체 명입니다.\n\n다른 이름으로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"업체 추가", L"업체 추가에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModSystem:
		if (nValue == 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}

		if (nValue == -1) {
			CMessagePopup dlg(L"업체 수정", L"이미 존재하는 업체 명입니다.\n\n다른 이름으로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"업체 수정", L"업체 수정에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelSystem:
		if (nValue == 1) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}

		if (nValue == 0) {
			CMessagePopup dlg(L"업체 삭제", L"업체 삭제에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseAddManager:
		if (nValue == 0 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"계정 추가", L"이미 존재하는 계정(아이디/비밀번호)입니다.\n\n다른 아이디로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"계정 추가", L"계정 추가에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModManager:
		if (nValue == 1 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"계정 추가", L"이미 존재하는 계정(아이디/비밀번호)입니다.\n\n다른 아이디로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"계정 수정", L"계정 수정에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelManager:
		if (nValue == 1 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"계정 삭제", L"계정 삭제에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseAddUser:
		if (nValue == 0 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"사용자 추가", L"이미 존재하는 전화번호입니다.\n\n다른 전화번호로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"사용자 추가", L"사용자 추가에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModUser:
		if (nValue == 1 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 수정", L"사용자 수정에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelUser:
		if (nValue == 1 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 삭제", L"사용자 삭제에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}
	return 0;
}