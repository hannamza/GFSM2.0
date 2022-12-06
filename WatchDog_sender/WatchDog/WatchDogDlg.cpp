
// WatchDogDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "WatchDog.h"
#include "WatchDogDlg.h"
#include "afxdialogex.h"

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


// CWatchDogDlg ��ȭ ����



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
	// ������ ����
	while (g_bThread)
	{
		TIME_SLEEP(1000, g_bThread);
		pDlg->ServerCheckLoop();
	}
}


// CWatchDogDlg �޽��� ó����

BOOL CWatchDogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	SetWindowText(_T("Sender ���� ���α׷�"));

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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWatchDogDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWatchDogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	OnBnClickedButtonStop();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CSharedInformation::Delete();

	Log::WriteLog(0, "Program ended....\n");
	Log::Cleanup();
}

BOOL CWatchDogDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWatchDogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//SetDlgItemText(IDC_STATIC_STATE, _T("���� ���� ����"));
}

LRESULT CWatchDogDlg::OnStateMsg(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int nState = (int)wParam;
	switch (nState)
	{
	case 0:
		sText = _T("���� ��� ����");
		break;
	case 1:
		sText = _T("���� ���� ����");
		break;
	case 2:
		sText = _T("������ ���α׷��� ������ �� �����ϴ�.");
		break;
	case 3:
		sText = _T("���α׷��� ������ �Դϴ�.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		break;
	case 4:
		sText = _T("���α׷��� ����Ǿ����ϴ�. ������մϴ�.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		break;
	case 5:
	{
		sText = _T("������ ���� �̻����� �ٽ� �����մϴ�.");
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		SetTimer(2, 10, NULL);
		CTime time = CTime::GetCurrentTime();
		sText.Format(_T("[RESTART]%04d/%02d/%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		m_listHistory.InsertString(0, sText);
	}
	break;
	case 6:
	{
		sText = _T("���� ���� �̻����� �ٽ� �����մϴ�.");
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
		PostMessage(STATE_MSG, 2, 0); // _T("������ ���α׷��� ������ �� �����ϴ�.")
		TIME_SLEEP(1000, g_bThread);
		return;
	}
	PostMessage(STATE_MSG, 3, 0); // _T("���α׷��� ������ �Դϴ�.")

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
			PostMessage(STATE_MSG, 4, 0); // _T("���α׷��� ����Ǿ����ϴ�. ������մϴ�.")
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
		//		PostMessage(STATE_MSG, 5, 0); // _T("������ ���� �̻����� �ٽ� �����մϴ�.")
		//		CCommonFunc::KillProcess(m_sFileName);
		//		memset(&m_Info, 0x00, sizeof(m_Info));
		//		TIME_SLEEP(1000, g_bThread);
		//	}
		//	else
		//	{
		//		++g_checkCount;
		//		PostMessage(STATE_MSG, 6, 0); // _T("������ ���� �̻����� �ٽ� �����մϴ�.")
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
}
