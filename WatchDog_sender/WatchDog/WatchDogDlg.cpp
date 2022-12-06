
// WatchDogDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WatchDog.h"
#include "WatchDogDlg.h"
#include "afxdialogex.h"

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


// CWatchDogDlg 대화 상자



CWatchDogDlg::CWatchDogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WATCHDOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWatchDogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_listHistory);
}

BEGIN_MESSAGE_MAP(CWatchDogDlg, CDialogEx)
	ON_MESSAGE(STATE_MSG, OnStateMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CWatchDogDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CWatchDogDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


bool g_bThread = false;
void func_thread(CWatchDogDlg* pDlg)
{
	// 쓰레드 메인
	while (g_bThread)
	{
		TIME_SLEEP(1000, g_bThread);
		pDlg->ServerCheckLoop();
	}
}


// CWatchDogDlg 메시지 처리기

BOOL CWatchDogDlg::OnInitDialog()
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
	SetWindowText(_T("Sender 감시 프로그램"));

	m_Info.hProcess = NULL;
	m_Info.hThread = NULL;
	m_Info.dwProcessId = 0L;
	m_Info.dwThreadId = 0L;

	m_Thread = NULL;
	m_bFirst = true;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	SetDlgItemText(IDC_EDIT_EXE, _T("GFSM_SENDER.exe"));
	SetDlgItemText(IDC_EDIT_PORT, _T("10234"));
	m_nServerPort = 10234;

	Log::Setup();
	Log::SetMainPointer(this);

	Log::EnableTrace(true);

	CSharedInformation::Instance()->New();

	SetTimer(1, 10, NULL);

	SetTimer(3, 1000, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CWatchDogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWatchDogDlg::OnPaint()
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
HCURSOR CWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWatchDogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	OnBnClickedButtonStop();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSharedInformation::Delete();

	Log::WriteLog(0, "Program ended....\n");
	Log::Cleanup();
}

BOOL CWatchDogDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWatchDogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		KillTimer(1);
		MoveWindow(0, 0, 456, 310);
		PostMessage(STATE_MSG, 0, 0);
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		OnBnClickedButtonStop();
		Sleep(1000);
		OnBnClickedButtonStart();
	}
	else if (nIDEvent == 3)
	{
		KillTimer(3);
		OnBnClickedButtonStart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CWatchDogDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Thread)
	{
		return;
	}

	GetDlgItemText(IDC_EDIT_EXE, m_sFileName);

	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);

	m_sPath.Format(_T("%s\\%s"), szPath, m_sFileName);

	g_bThread = true;
	m_Thread = new thread(&func_thread, this);

	CString sTemp;
	GetDlgItemText(IDC_EDIT_PORT, sTemp);
	m_nServerPort = _ttoi(sTemp.GetBuffer(0));
	sTemp.ReleaseBuffer();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_EXE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
}

void CWatchDogDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_Thread)
	{
		return;
	}
	g_bThread = false;
	TRACE("1111111111111\n");
	m_Thread->join();
	TRACE("2222222222222\n");

	SAFE_DELETE(m_Thread);


	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_EXE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);

	PostMessage(STATE_MSG, 1, 0);
	//SetDlgItemText(IDC_STATIC_STATE, _T("감시 중지 상태"));
}

LRESULT CWatchDogDlg::OnStateMsg(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int nState = (int)wParam;
	switch (nState)
	{
	case 0:
		sText = _T("감시 대기 상태");
		break;
	case 1:
		sText = _T("감시 중지 상태");
		break;
	case 2:
		sText = _T("감시할 프로그램을 실행할 수 없습니다.");
		break;
	case 3:
		sText = _T("프로그램이 실행중 입니다.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		break;
	case 4:
		sText = _T("프로그램이 종료되었습니다. 재실행합니다.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		break;
	case 5:
	{
		sText = _T("서버의 상태 이상으로 다시 실행합니다.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		SetTimer(2, 10, NULL);
		CTime time = CTime::GetCurrentTime();
		sText.Format(_T("[RESTART]%04d/%02d/%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		m_listHistory.InsertString(0, sText);
	}
	break;
	case 6:
	{
		sText = _T("연결 상태 이상으로 다시 접속합니다.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		SetTimer(2, 10, NULL);
		CTime time = CTime::GetCurrentTime();
		sText.Format(_T("[CHECK]%04d/%02d/%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		m_listHistory.InsertString(0, sText);
	}
	break;
	default:
		return 0;
	}
	SetDlgItemText(IDC_STATIC_STATE, sText);
	return 0;
}

int g_checkCount = 0;
void CWatchDogDlg::ServerCheckLoop()
{
	DWORD dwLastTime = GetTickCount(), dwRecvTime = 0, dwCheckTime = 0;
	ProtocolRequest keep_alive;
	keep_alive.protocol = ProtocolHeader::Keep_Alive;
	CString sText;

	// 1. execute program
	if (!ExecuteCheckProgram(m_sPath))
	{
		PostMessage(STATE_MSG, 2, 0); // _T("감시할 프로그램을 실행할 수 없습니다.")
		TIME_SLEEP(1000, g_bThread);
		return;
	}
	PostMessage(STATE_MSG, 3, 0); // _T("프로그램이 실행중 입니다.")

	if (!m_bFirst) {
		CTime time = CTime::GetCurrentTime();
		CString sTime;
		sTime.Format(L"%04d/%02d/%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		m_listHistory.InsertString(0, sTime);
	}
	m_bFirst = false;

	CSharedInformation::Instance()->SetReceiveTime(GetTickCount());
	while (g_bThread)
	{
		Sleep(1);
		// 2. server running check
		if (!IsCheckProgramRunning())
		{
			PostMessage(STATE_MSG, 4, 0); // _T("프로그램이 종료되었습니다. 재실행합니다.")
			TIME_SLEEP(100, g_bThread);
			return;
		}
		// 3. send packet
		/*if (GetTickCount() - dwLastTime >= 500)
		{
			CClientInterface::Instance()->Send((BYTE*)&keep_alive, sizeof(ProtocolRequest));
			dwLastTime = GetTickCount();
		}*/
		// 4. check response
		//dwRecvTime = GetTickCount() - CSharedInformation::Instance()->GetReceiveTime();
		//if (dwRecvTime >= 5 * 1000)
		//{
		//	if (g_checkCount > 0)
		//	{
		//		PostMessage(STATE_MSG, 5, 0); // _T("서버의 상태 이상으로 다시 실행합니다.")
		//		CCommonFunc::KillProcess(m_sFileName);
		//		memset(&m_Info, 0x00, sizeof(m_Info));
		//		TIME_SLEEP(1000, g_bThread);
		//	}
		//	else
		//	{
		//		++g_checkCount;
		//		PostMessage(STATE_MSG, 6, 0); // _T("서버의 상태 이상으로 다시 실행합니다.")
		//		TIME_SLEEP(1000, g_bThread);
		//	}
		//	return;
		//}
		//else
		//{
		//	if (dwCheckTime > dwRecvTime)
		//	{
		//		g_checkCount = 0;
		//	}
		//	dwCheckTime = dwRecvTime;
		//}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWatchDogDlg::ExecuteCheckProgram(CString sPath)
{
	STARTUPINFO sinfo;
	if (IsCheckProgramRunning())
		return true;

	memset(&sinfo, 0x00, sizeof(sinfo));
	if (CreateProcess(NULL, sPath.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &m_Info))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CWatchDogDlg::IsCheckProgramRunning()
{
	unsigned long l;
	if (m_Info.hProcess)
	{
		GetExitCodeProcess(m_Info.hProcess, &l);
		if (l == STILL_ACTIVE)
		{
			return true;
		}
	}
	return false;
}

void CWatchDogDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
}
