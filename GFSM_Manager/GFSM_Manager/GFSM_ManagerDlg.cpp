
// GFSM_ManagerDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CGFSM_ManagerDlg ��ȭ ����



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


// CGFSM_ManagerDlg �޽��� ó����

BOOL CGFSM_ManagerDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

	// Ÿ��Ʋ �ٿ� ���� ǥ��
	CString strTitle = _T("");
	strTitle.Format(_T("%s v%.1f"), CCommonFunc::CharToWCHAR(PROGRAM_NAME), PROGRAM_VERSION);
	SetWindowText(strTitle);

	Global.SetFont(IDX_FONT_SMALL, _T("����"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("����"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("����"), 11, TRUE);

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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CGFSM_ManagerDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CGFSM_ManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CSkinDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CGFSM_ManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_ManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

BOOL CGFSM_ManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CSkinDialog::OnEraseBkgnd(pDC);
}

void CGFSM_ManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	case ProtocolHeader::ResponseGetSystemList: // �ý��� ��� ����
		if (m_pSideLeftDlg) {
			m_pSideLeftDlg->Invalidate();
		}
		break;
	case ProtocolHeader::ResponseAddNewSystem: // �ý��� �߰� ����
		if (nValue == 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}
		else if(nValue == -1) {
			CMessagePopup dlg(L"��ü �߰�", L"�̹� �����ϴ� ��ü ���Դϴ�.\n\n�ٸ� �̸����� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"��ü �߰�", L"��ü �߰��� �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModSystem:
		if (nValue == 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}

		if (nValue == -1) {
			CMessagePopup dlg(L"��ü ����", L"�̹� �����ϴ� ��ü ���Դϴ�.\n\n�ٸ� �̸����� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"��ü ����", L"��ü ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelSystem:
		if (nValue == 1) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetSystemList, CCommonState::Instance()->m_nIdx);
		}

		if (nValue == 0) {
			CMessagePopup dlg(L"��ü ����", L"��ü ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseAddManager:
		if (nValue == 0 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"���� �߰�", L"�̹� �����ϴ� ����(���̵�/��й�ȣ)�Դϴ�.\n\n�ٸ� ���̵�� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"���� �߰�", L"���� �߰��� �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModManager:
		if (nValue == 1 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"���� �߰�", L"�̹� �����ϴ� ����(���̵�/��й�ȣ)�Դϴ�.\n\n�ٸ� ���̵�� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"���� ����", L"���� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelManager:
		if (nValue == 1 && CCommonState::Instance()->m_nCurrentWorksite > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetManagerList, CCommonState::Instance()->m_nCurrentWorksite);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"���� ����", L"���� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseAddUser:
		if (nValue == 0 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"����� �߰�", L"�̹� �����ϴ� ��ȭ��ȣ�Դϴ�.\n\n�ٸ� ��ȭ��ȣ�� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"����� �߰�", L"����� �߰��� �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModUser:
		if (nValue == 1 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelUser:
		if (nValue == 1 && CCommonState::Instance()->m_nManager > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nManager);
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}
	return 0;
}