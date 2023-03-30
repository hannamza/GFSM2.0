
// GFSM_SENDERDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "GFSM_SENDER.h"
#include "GFSM_SENDERDlg.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include "ReadWriteState.h"
#include "DlgEventTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGFSM_SENDERDlg ��ȭ ����



CGFSM_SENDERDlg::CGFSM_SENDERDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_GFSM_SENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bInit = false;
	m_bKilled = false;
	m_bAutoLogin = false;
}

void CGFSM_SENDERDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_richLog);
	DDX_Control(pDX, IDC_COMBO1, m_comboPort);
	DDX_Control(pDX, IDC_CHECK_SCROLL, m_checkScroll);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btnSend);
	DDX_Control(pDX, IDC_EDIT_DIRECT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_BUTTON_EVENT, m_btnEvent);
}

BEGIN_MESSAGE_MAP(CGFSM_SENDERDlg, CSkinDialog)
	ON_MESSAGE(LOG_MESSAGE, OnLogMessage)
	ON_MESSAGE(WM_TRAY_NOTIFICATION, OnTrayNotification)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGFSM_SENDERDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CGFSM_SENDERDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CGFSM_SENDERDlg::OnBnClickedButtonSend)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID__EXIT, &CGFSM_SENDERDlg::OnMenuExit)
	ON_COMMAND(ID__START, &CGFSM_SENDERDlg::OnMenuStart)
	ON_COMMAND(ID__STOP, &CGFSM_SENDERDlg::OnMenuStop)
	ON_COMMAND(ID__POPUP, &CGFSM_SENDERDlg::OnMenuPopup)
	ON_UPDATE_COMMAND_UI(ID__START, &CGFSM_SENDERDlg::OnMenuUpdateStart)
	ON_UPDATE_COMMAND_UI(ID__STOP, &CGFSM_SENDERDlg::OnMenuUpdateStop)
	ON_UPDATE_COMMAND_UI(ID__POPUP, &CGFSM_SENDERDlg::OnMenuUpdatePopup)
	ON_UPDATE_COMMAND_UI(ID__EXIT, &CGFSM_SENDERDlg::OnMenuUpdateExit)
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON_EVENT, &CGFSM_SENDERDlg::OnBnClickedButtonEvent)
END_MESSAGE_MAP()


// CGFSM_SENDERDlg �޽��� ó����

BOOL CGFSM_SENDERDlg::OnInitDialog()
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
	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	m_bStarted = false;

	Log::Setup();
	Log::SetMainPointer(this);
	//Log::EnableTrace(false);

	m_checkScroll.SetCheck(true);

	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);
	CString strINI_Name;
	strINI_Name.Format(_T("%s\\Setup.ini"), szPath);

	TCHAR szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	GetPrivateProfileString(L"COMM", L"PORT", L"COM1", szTemp, BUFF_SIZE, strINI_Name);

	CString sText;
	bool bFind = false;
	for (int nIndex = 1; nIndex < 20; nIndex++)
	{
		sText.Format(L"COM%d", nIndex);
		m_comboPort.InsertString(m_comboPort.GetCount(), sText);
		if (sText.CompareNoCase(szTemp) == 0)
		{
			bFind = true;
			m_comboPort.SetCurSel(nIndex - 1);
		}
	}
	if (!bFind)
	{
		m_comboPort.SetCurSel(0);
	}

	//20230207 GBM start - test ���� ������ ���� �ʿ�
#ifndef EVENT_TEST_MODE
	strcpy_s(m_szServerIP, "160.202.162.3");/*"192.168.1.222"*/
#else
	strcpy_s(m_szServerIP, "127.0.0.1");
#endif
	//20230207 GBM end

	CClientInterface::New();
	CClientInterface::Instance()->TryConnection(m_szServerIP, 10234);

	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	m_btnStart.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnStart.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

	m_btnStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnStop.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	m_editInput.ShowWindow(SW_HIDE);
	m_btnSend.ShowWindow(SW_HIDE);

#ifndef EVENT_TEST_MODE
	//m_btnEvent.ShowWindow(SW_HIDE);
#endif

	CCommonState::New();

	Reposition();

	INT nTemp = 0;
	CReadWriteState state;
	CString sTemp;
	state.SetFileName(L"Setup.ini");
	if (state.ReadState(L"INFO", L"LOGIN", nTemp) && nTemp == 1)
	{
		state.ReadState(L"INFO", L"ID", sTemp);
		strcpy_s(CCommonState::Instance()->m_szID, CCommonFunc::WCharToChar(sTemp.GetBuffer(0)));

		state.ReadState(L"INFO", L"INDEX", nTemp);
		CCommonState::Instance()->m_nIdx = nTemp;

		CCommonState::Instance()->m_bLoginResult = true;

		ShowWindow(SW_HIDE);
		SetTimer(3, 10, NULL);
	}
	else
	{
		SetTimer(1, 10, NULL);
	}
	m_bInit = true;

	Log::Trace("Program started...");

	SetTimer(99, 1000, NULL);

	if (CCommonState::Instance()->m_nIdx == 0)
	{
		m_bAutoLogin = true;
		ShowWindow(SW_SHOW);
	}
	else
	{
		SetTimer(20, 100, NULL);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CGFSM_SENDERDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGFSM_SENDERDlg::OnPaint()
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
HCURSOR CGFSM_SENDERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_SENDERDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CGFSM_SENDERDlg::OnDestroy()
{
	CSkinDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bKilled) {
		CCommonFunc::KillProcess(L"Sender ���α׷� ����.exe");
	}

	OnBnClickedButtonStop();

	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();

	Log::Cleanup();
}

BOOL CGFSM_SENDERDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CSkinDialog::OnEraseBkgnd(pDC);
}

void CGFSM_SENDERDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1)
	{
		KillTimer(1);
		CCommonState::Instance()->m_bLoginResult = false;
		DWORD dwCheckTime = GetTickCount();
		while (TRUE)
		{
			CLoginDlg dlg;
			UINT nResult = dlg.DoModal();
			if (nResult == IDOK)
			{
				CClientInterface::Instance()->ProcessRequestLogin(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
				strcpy_s(CCommonState::Instance()->m_szID, CCommonFunc::WCharToChar(dlg.m_sID.GetBuffer(0)));
				dwCheckTime = GetTickCount();
				while (GetTickCount() - dwCheckTime <= 2500)
				{
					Sleep(1);
					if (CCommonState::Instance()->m_bLoginResult && CCommonState::Instance()->m_nIdx > 0)
					{
						CReadWriteState state;
						state.SetFileName(L"Setup.ini");
						state.WriteState(L"INFO", L"LOGIN", 1);
						state.WriteState(L"INFO", L"ID", dlg.m_sID);
						state.WriteState(L"INFO", L"INDEX", CCommonState::Instance()->m_nIdx);
						break;
					}
				}
			}
			else
			{
				EndDialog(0);
				return;
			}
			if (CCommonState::Instance()->m_nIdx > 0)
			{
				break;
			}
		}
		SetTimer(3, 10, NULL);
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		OnBnClickedButtonStop();
		Log::Trace("���� ����(Serial ���� ����) - ���� �Ϸ�");

		/*CEventSend::New();
		CEventSend::Instance()->SetMainWnd(this);
		CEventSend::Instance()->InitEvent();

		OnBnClickedButtonSend();*/
	}
	else if (nIDEvent == 3)
	{
		KillTimer(3);
		RegistryTrayIcon();
		OnBnClickedButtonStart();
	}
	else if (nIDEvent == 10) {
		OnBnClickedButtonStart();
	}
	else if (nIDEvent == 20) {
		KillTimer(20);
		m_bAutoLogin = true;
	}
	else if (nIDEvent == 99)
	{
		if (GetTickCount() - CCommonState::Instance()->m_dwLastRecv >= 2500) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
		}
		if (GetTickCount() - CCommonState::Instance()->m_dwLastRecv > 7 * 1000) {
			KillTimer(99);

			m_bKilled = true;
			OnMenuExit();
			return;
			/*CClientInterface::Delete();
			CClientInterface::New();
			CClientInterface::Instance()->TryConnection(m_szServerIP, 10234);
			g_nTest = 0;
			SetTimer(99, 1000, NULL);
			return;
		}
		++g_nTest;*/
		}
	}

	CSkinDialog::OnTimer(nIDEvent);
}

HBRUSH CGFSM_SENDERDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}
	return m_brush;

	return hbr;
}

void CGFSM_SENDERDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	UINT swState;
	if (m_bInit)
	{
		if (!IsWindowVisible())
		{
			swState = SW_HIDE;
			RegistryTrayIcon();
		}
		else
		{
			if (IsIconic())
			{
				swState = SW_MINIMIZE;
				RegistryTrayIcon();
			}
			else
			{
				if (IsZoomed())
				{
					swState = SW_MAXIMIZE;
				}
				else
				{
					// not hidden, minimized or zoomed, so we are a normal visible window
					// last ShowWindow flag could have been SW_RESTORE, SW_SHOW, SW_SHOWNA, etc
					// no way to tell
					swState = SW_SHOW;
				}
			}
		}
	}

	if (m_bInit)
	{
		Reposition();
	}
}

void CGFSM_SENDERDlg::Reposition()
{
	CRect rect;
	GetClientRect(&rect);
	m_btnStart.MoveWindow(rect.right - 20 - 240 - 10, 15, 120, 36);
	m_btnStop.MoveWindow(rect.right - 20 - 120, 15, 120, 36);
	m_richLog.MoveWindow(10, 70, rect.Width() - 20, rect.Height() - 75);

	m_btnStart.Invalidate();
	m_btnStop.Invalidate();
}

void CGFSM_SENDERDlg::RegistryTrayIcon()
{
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.uID = 0;
	nid.hWnd = GetSafeHwnd();

	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_tcscpy(nid.szTip, L"GFSM �̺�Ʈ ������");
	nid.uCallbackMessage = WM_TRAY_NOTIFICATION;

	BOOL bRet = ::Shell_NotifyIcon(NIM_ADD, &nid); //Ʈ���� ������ ���
	ShowWindow(SW_HIDE); //������ ���߱�
}

DWORD g_dwErrorTime = 0;
LRESULT CGFSM_SENDERDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 999 && lParam == 999) { // ���� ����
		if (GetTickCount() - g_dwErrorTime >= 7500) {
			OnBnClickedButtonStop();
			SetTimer(10, 5000, NULL);
			g_dwErrorTime = GetTickCount();
		}
	}
	switch (lParam)
	{
	case WM_RBUTTONDOWN:
	{
		CPoint ptMouse;
		::GetCursorPos(&ptMouse);

		CMenu menu;
		menu.LoadMenu(IDR_MENU_TRAY);
		CMenu *pMenu = menu.GetSubMenu(0); //Ȱ��ȭ �� �޴� ����
		if (m_bStarted)
		{
			pMenu->EnableMenuItem(ID__START, MF_DISABLED);
			pMenu->EnableMenuItem(ID__STOP, MF_ENABLED);
		}
		else
		{
			pMenu->EnableMenuItem(ID__START, MF_ENABLED);
			pMenu->EnableMenuItem(ID__STOP, MF_DISABLED);
		}
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, AfxGetMainWnd());
	}
	break;

	case WM_LBUTTONDBLCLK:
	{
		NOTIFYICONDATA nid;
		ZeroMemory(&nid, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.uID = 0;
		nid.hWnd = GetSafeHwnd();

		BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid); //Ʈ���̾����� ����
		if (!bRet)
		{
			Log::Trace("Ʈ���� ������ ���� ����");
			return -1;
		}
		ShowWindow(SW_SHOW); //������ Ȱ��ȭ
		ShowWindow(SW_RESTORE);
	}
	break;
	}
	return 0;
}

void CGFSM_SENDERDlg::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_bStarted)
	{
		return;
	}

	Log::Trace("Sender start...");

	CDeviceInfo::New();
	CDeviceInfo::Instance()->SetDeviceInfo();

	G_hShutdown_Event = CreateEvent(NULL, TRUE, FALSE, NULL);

	CEventSend::New();
	CEventSend::Instance()->SetMainWnd(this);
	CEventSend::Instance()->InitEvent();

	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);
	CString strINI_Name;
	strINI_Name.Format(_T("%s\\Setup.ini"), szPath);

	CString strTemp;
	m_comboPort.GetLBText(m_comboPort.GetCurSel(), strTemp);
	WritePrivateProfileString(L"COMM", L"PORT", strTemp, strINI_Name);

	Log::Trace("COM PORT : [COM %d], 9600, 8, 0, 1, 0", m_comboPort.GetCurSel() + 1);

	CSerialComm::New();
	CSerialComm::Instance()->SetMainWnd(this);
	CSerialComm::Instance()->SetSerialCommPort(strTemp);
	//CSerialComm::Instance()->SetSerialBaudRate(GetPrivateProfileInt(L"COMM", L"BAUDRATE", 9600, strINI_Name));
	CSerialComm::Instance()->SetSerialBaudRate(GetPrivateProfileInt(L"COMM", L"BAUDRATE", 38400, strINI_Name));
	CSerialComm::Instance()->SetSerialDataBit(GetPrivateProfileInt(L"COMM", L"DATABIT", 8, strINI_Name));
	CSerialComm::Instance()->SetSerialParityBit(GetPrivateProfileInt(L"COMM", L"PARITY", 0, strINI_Name));
	CSerialComm::Instance()->SetSerialStopBit(GetPrivateProfileInt(L"COMM", L"STOPBIT", 1, strINI_Name));
	CSerialComm::Instance()->SetFACPNum(GetPrivateProfileInt(L"COMM", L"FACP_NUM", 0, strINI_Name));
	if (!CSerialComm::Instance()->Init())
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);

		SetTimer(2, 3000, NULL);
		m_bStarted = true;

		Log::Trace("���� ����(Serial ���� ����) - ���� ��.....");

		return;
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);

	m_bStarted = true;
}

void CGFSM_SENDERDlg::OnBnClickedButtonStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bStarted)
	{
		return;
	}
	Log::Trace("Sender stop...");

	CEventSend::Delete();
	CDeviceInfo::Delete();

	::SetEvent(G_hShutdown_Event);
	WaitForSingleObject(CSerialComm::Instance()->m_hThreadWatchComm, INFINITE);

	CSerialComm::Instance()->Close();
	CSerialComm::Delete();

	m_bStarted = false;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
}

void CGFSM_SENDERDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BYTE data[BUFF_SIZE];

	memset(data, 0x00, sizeof(data));

	wchar_t* wchar_str;     //ù��° �ܰ�(CString to wchar_t*)�� ���� ����
	int nLen;

	CString strText;
	GetDlgItem(IDC_EDIT_DIRECT_INPUT)->GetWindowTextW(strText);

	strText.TrimLeft();
	strText.TrimRight();

	/*strText = _T("1234567");

	if (strText.GetLength() < 5)
	{
		return;
	}*/

	wchar_str = strText.GetBuffer(strText.GetLength());

	nLen = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, (LPSTR)(data + 2), nLen, 0, 0);

	data[0] = 'G';
	data[1] = 'X';
	data[13] = 0x0D;
	data[14] = 0x0A;
	data[15] = 0x00;

	BYTE *pData = new BYTE[nLen + 7];
	memset(pData, 0x00, nLen + 7);
	memcpy(pData, data, nLen + 6);

	strText.ReleaseBuffer();

	CEventSend::Instance()->SendEvent(pData);
}

#define		LIMIT_RICH_LOG		(1024*1024*2)
LRESULT CGFSM_SENDERDlg::OnLogMessage(WPARAM wParam, LPARAM lParam)
{
	CHAR * pLog = (CHAR*)wParam;
	if (strlen(pLog) > 512)
	{
		return 0;
	}
	TCHAR szLog[512];

	CTime time = CTime::GetCurrentTime();
	_stprintf_s(szLog, _T("[%d/%d %02d:%02d:%02d] %s"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), CCommonFunc::CharToTCHAR(pLog));
	//_tcscpy_s(szLog, CCommonFunc::CharToTCHAR(pLog));

	OutputDebugString(szLog);

	long lLength = m_richLog.GetTextLengthEx(GTL_NUMCHARS);
	if (lLength > LIMIT_RICH_LOG)
	{
		m_richLog.SetSel(0, LIMIT_RICH_LOG / 10);
		m_richLog.ReplaceSel(L"");
	}

	CHARRANGE cr = { 0, 0 };
	m_richLog.GetSel(cr);
	m_richLog.SetSel(lLength, lLength);
	m_richLog.ReplaceSel(szLog);

	INT nMax = m_richLog.GetScrollLimit(SB_VERT);
	if (m_checkScroll.GetCheck())
	{
		m_richLog.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nMax), NULL);
		m_richLog.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_ENDSCROLL, nMax), NULL);
	}

	return 0;
}


void CGFSM_SENDERDlg::OnMenuExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.uID = 0;
	nid.hWnd = GetSafeHwnd();

	BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid); //Ʈ���̾����� ����
	
	OnOK();
}

void CGFSM_SENDERDlg::OnMenuStart()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedButtonStart();
}

void CGFSM_SENDERDlg::OnMenuStop()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedButtonStop();
}

void CGFSM_SENDERDlg::OnMenuPopup()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.uID = 0;
	nid.hWnd = GetSafeHwnd();

	BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid); //Ʈ���̾����� ����
	if (!bRet)
	{
		Log::Trace("Ʈ���� ������ ���� ����");
		return ;
	}
	ShowWindow(SW_SHOW); //������ Ȱ��ȭ
	ShowWindow(SW_RESTORE);
}

void CGFSM_SENDERDlg::OnMenuUpdateStart(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}

void CGFSM_SENDERDlg::OnMenuUpdateStop(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}

void CGFSM_SENDERDlg::OnMenuUpdatePopup(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}

void CGFSM_SENDERDlg::OnMenuUpdateExit(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}


void CGFSM_SENDERDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CSkinDialog::OnWindowPosChanging(lpwndpos);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bAutoLogin) {
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}
}


void CGFSM_SENDERDlg::OnBnClickedButtonEvent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDlgEventTest dlg;
	if (dlg.DoModal() == IDOK)
	{
		BYTE* pData = new BYTE[SI_EVENT_BUF_SIZE];
		memset(pData, NULL, SI_EVENT_BUF_SIZE);
		memcpy(pData, dlg.m_eventBuf, SI_EVENT_BUF_SIZE);

		Sleep(500);

		CEventSend::Instance()->SendEvent(pData);

		CString strBuf = _T("");
		for (int i = 0; i < SI_EVENT_BUF_SIZE; i++)
		{
			strBuf += pData[i];
		}

		Log::Trace("Event Test - [%s] EventQueue Added!", CCommonFunc::WCharToChar(strBuf.GetBuffer(0)));
	}
}
