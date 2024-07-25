
// GFSM_MCSDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GFSM_MCS.h"
#include "GFSM_MCSDlg.h"
#include "afxdialogex.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGFSM_MCSDlg 대화 상자



CGFSM_MCSDlg::CGFSM_MCSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GFSM_MCS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGFSM_MCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_richEdit);
	DDX_Control(pDX, IDC_CHECK_SCROLL, m_checkScroll);
}

BEGIN_MESSAGE_MAP(CGFSM_MCSDlg, CDialogEx)
	ON_MESSAGE(LOG_MESSAGE, OnLogMessage)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGFSM_MCSDlg::OnBnClickedButtonStart)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CGFSM_MCSDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CGFSM_MCSDlg 메시지 처리기

BOOL CGFSM_MCSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	
	//20230316 GBM start - git hub test
	int a = 0;
	//20230316 GBM end

	//20240628 GBM start - 타이틀 바에 버전 표시
	CString strTitle = _T("");
	strTitle.Format(_T("%s v%.1f"), CCommonFunc::CharToWCHAR(PROGRAM_NAME), PROGRAM_VERSION);
	SetWindowText(strTitle);
	//20240628 GBM end

	m_nServerPort = 10234;
	m_nLimitClient = 1000;
	m_bStartServer = false;
	m_checkScroll.SetCheck(true);

	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	CString sPort;
	sPort.Format(_T("%d"), m_nServerPort);
	SetDlgItemText(IDC_EDIT_PORT, sPort);

	Log::Setup();
	Log::SetMainPointer(this);

	Log::EnableTrace(true);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	SetTimer(1, 1000, NULL);

	MoveWindow(0, 0, 1200, 800);
	CenterWindow();

	Log::WriteLog(0, "Program started....\n");

	SetTimer(2, 500, NULL);

	dbInformation info;
	info.nPort = 3306;
	strcpy_s(info.szDBInfoName, "gfsm");
	strcpy_s(info.szDBName, "gfsm_db");
	strcpy_s(info.szID, "gfsm_manager");
	strcpy_s(info.szIP, "127.0.0.1");
	strcpy_s(info.szPW, "gfs1020..[!@]");

	CDBPool::New();
	CDBPool::Instance()->SetDBInformation(info);
	CDBPool::Instance()->StartDBPool(100);

	CEncDec::New();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGFSM_MCSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonStop();

	CDBPool::Delete();

	Log::WriteLog(0, "Program ended....\n");

	Log::Cleanup();

	CEncDec::Delete();
}

BOOL CGFSM_MCSDlg::WriteLog(INT nMode, LPTSTR data, ...)
{
	SYSTEMTIME	SystemTime;
	TCHAR		CurrentDate[32] = { 0, };
	TCHAR		CurrentFileName[MAX_PATH] = { 0, };
	FILE*		FilePtr = NULL;
	TCHAR		DebugLog[2048] = { 0, };

	GetLocalTime(&SystemTime);

	TCHAR szValue[64];
	switch (nMode)
	{
	case 0:
		_tcscpy_s(szValue, 64, _T("LOG"));
		break;
	case 1:
		_tcscpy_s(szValue, 64, _T("DEBUG"));
		break;
	case 2:
		_tcscpy_s(szValue, 64, _T("WARNING"));
		break;
	case 3:
		_tcscpy_s(szValue, 64, _T("ERROR"));
		break;
	default:
		return TRUE;
	}

	_sntprintf(CurrentDate, 32, _T("%02d/%02d %02d:%02d:%02d %03d"),
		/*SystemTime.wYear,*/
		SystemTime.wMonth,
		SystemTime.wDay, 
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond,
		SystemTime.wMilliseconds);

	_sntprintf(DebugLog, 2048, _T("%s  [ %s ]  %s"), CurrentDate, szValue, data);

	OutputDebugString(DebugLog);
	//_tprintf(_T("%s"), DebugLog);

	WriteFunc(DebugLog);

	return TRUE;
}

void CGFSM_MCSDlg::WriteFunc(CString strText)
{
	long lLength = m_richEdit.GetTextLengthEx(GTL_NUMCHARS);
	if (lLength > LIMIT_RICH_LOG)
	{
		m_richEdit.SetSel(0, LIMIT_RICH_LOG / 10);
		m_richEdit.ReplaceSel(L"");
	}

	CHARRANGE cr = { 0, 0 };
	m_richEdit.GetSel(cr);
	m_richEdit.SetSel(lLength, lLength);
	m_richEdit.ReplaceSel(strText);

	INT nMax = m_richEdit.GetScrollLimit(SB_VERT);
	if (m_checkScroll.GetCheck())
	{
		m_richEdit.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nMax), NULL);
		m_richEdit.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_ENDSCROLL, nMax), NULL);
	}
}

void CGFSM_MCSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGFSM_MCSDlg::OnPaint()
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
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGFSM_MCSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_MCSDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGFSM_MCSDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_richEdit.m_hWnd)
	{
		CRect rect1, rect2;
		GetClientRect(&rect1);
		rect2.SetRect(3, 70, rect1.Width() - 3, rect1.bottom - 3);
		m_richEdit.MoveWindow(rect2);
	}
}

void CGFSM_MCSDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*if (m_richEdit.m_hWnd)
	{
		CRect rect1, rect2;
		GetClientRect(&rect1);
		rect2.SetRect(3, 70, rect1.Width() - 3, rect1.bottom - 3);
		m_richEdit.MoveWindow(rect2);
	}*/
}

void CGFSM_MCSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		if (m_bStartServer)
		{
			int nAccept = Server::Instance()->GetNumClients();
			int nLimit = Server::Instance()->GetNumPostAccepts();

			CString sTemp;
			sTemp.Format(_T("%d"), nLimit);
			SetDlgItemText(IDC_EDIT_LIMIT, sTemp);
			sTemp.Format(_T("%d"), nAccept);
			SetDlgItemText(IDC_EDIT_CONNECT, sTemp);
		}
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		OnBnClickedButtonStart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CGFSM_MCSDlg::OnBnClickedButtonStart()
{
	Log::WriteLog(0, "Server starting....\n");
	CString sPort;
	GetDlgItemText(IDC_EDIT_PORT, sPort);
	m_nServerPort = _ttoi(sPort.GetBuffer(0));
	sPort.ReleaseBuffer();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Network::Initialize() == false)
	{
		ERROR_MSG("Network::Initialize() failed");
		return;
	}

	Server::New();

	if (Server::Instance()->Create(m_nServerPort, m_nLimitClient) == false)
	{
		ERROR_MSG("Server::Create() failed");
		Network::Deinitialize();
		return;
	}

	Sleep(100);
	m_bStartServer = true;

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(false);
	
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	Log::WriteLog(0, "Server started....\n");
}

void CGFSM_MCSDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Log::WriteLog(0, "Server stopping....\n");

	m_bStartServer = false;
	Sleep(100);

	Server::Delete();

	Network::Deinitialize();

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	Log::WriteLog(0, "Server stopped....\n");
}

LRESULT CGFSM_MCSDlg::OnLogMessage(WPARAM wParam, LPARAM lParam)
{
	CHAR* pText = (CHAR*)wParam;
	int nMode = (int)lParam;

	WriteLog(nMode, CCommonFunc::CharToTCHAR(pText));

	return 0;
}
