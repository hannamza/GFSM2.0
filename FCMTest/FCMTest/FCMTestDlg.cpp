
// FCMTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "FCMTest.h"
#include "FCMTestDlg.h"
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
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnDestroy();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_DESTROY()
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFCMTestDlg 대화 상자



CFCMTestDlg::CFCMTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FCMTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFCMTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_F, m_radioF);
	DDX_Control(pDX, IDC_RADIO_G, m_radioG);
	DDX_Control(pDX, IDC_RADIO_S, m_radioS);
	DDX_Control(pDX, IDC_RADIO_T, m_radioT);
	DDX_Control(pDX, IDC_RADIO_R, m_ridioR);
}

BEGIN_MESSAGE_MAP(CFCMTestDlg, CDialogEx)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FIRE, &CFCMTestDlg::OnBnClickedButtonFire)
	ON_BN_CLICKED(IDC_BUTTON_GAS, &CFCMTestDlg::OnBnClickedButtonGas)
	ON_BN_CLICKED(IDC_BUTTON_SPY, &CFCMTestDlg::OnBnClickedButtonSpy)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CFCMTestDlg::OnBnClickedButtonLine)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RECOVERY, &CFCMTestDlg::OnBnClickedButtonRecovery)
END_MESSAGE_MAP()


// CFCMTestDlg 메시지 처리기

BOOL CFCMTestDlg::OnInitDialog()
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
	CDeviceInfo::New();
	CDeviceInfo::Instance()->SetDeviceInfo();

	CEventSend::New();
	CEventSend::Instance()->InitEvent();

	CCommonState::New();

	CString sID;
	int nIdx;
	CReadWriteState state;
	state.SetFileName(L"setup.ini");
	state.ReadState(L"GFSM", L"SEQ", nIdx);
	state.ReadState(L"GFSM", L"ID", sID);

	CCommonState::Instance()->m_pWnd = this;
	CCommonState::Instance()->m_nIdx = nIdx;
	strcpy_s(CCommonState::Instance()->m_szID, CCommonFunc::WcharToUtf8(sID.GetBuffer(0)));

	m_radioF.SetCheck(true);

	CClientInterface::New();
	CClientInterface::Instance()->TryConnection("160.202.162.3", 10234);

	SetTimer(99, 30 * 1000, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFCMTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CEventSend::Delete();
	CDeviceInfo::Delete();

	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
}

BOOL CFCMTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFCMTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 99) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFCMTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFCMTestDlg::OnPaint()
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
HCURSOR CFCMTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFCMTestDlg::OnBnClickedButtonFire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	STATE_DATA data;
	data.code1 = 'G';			// 고유코드 G
	data.code2 = 'X';			// 고유코드 X
	data.event_code = 'F';		// 명령코드 F, G, S(감시), T(단선), R
	data.recv_number[0] = '0';	// 수신기 번호 ASCII
	data.recv_number[1] = '0';	// 수신기 번호 ASCII
	data.unit_number[0] = '0';	// 유닛 번호 ASCII
	data.unit_number[1] = '0';	// 유닛 번호 ASCII
	data.seperate = '-';		// 구분
	data.type_number = '1';		// 계통번호 ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// 발생정보 N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonGas()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	STATE_DATA data;
	data.code1 = 'G';			// 고유코드 G
	data.code2 = 'X';			// 고유코드 X
	data.event_code = 'G';		// 명령코드 F, G, S(감시), T(단선), R
	data.recv_number[0] = '0';	// 수신기 번호 ASCII
	data.recv_number[1] = '0';	// 수신기 번호 ASCII
	data.unit_number[0] = '0';	// 유닛 번호 ASCII
	data.unit_number[1] = '0';	// 유닛 번호 ASCII
	data.seperate = '-';		// 구분
	data.type_number = '1';		// 계통번호 ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// 발생정보 N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonSpy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	STATE_DATA data;
	data.code1 = 'G';			// 고유코드 G
	data.code2 = 'X';			// 고유코드 X
	data.event_code = 'S';		// 명령코드 F, G, S(감시), T(단선), R
	data.recv_number[0] = '0';	// 수신기 번호 ASCII
	data.recv_number[1] = '0';	// 수신기 번호 ASCII
	data.unit_number[0] = '0';	// 유닛 번호 ASCII
	data.unit_number[1] = '0';	// 유닛 번호 ASCII
	data.seperate = '-';		// 구분
	data.type_number = '1';		// 계통번호 ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// 발생정보 N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	STATE_DATA data;
	data.code1 = 'G';			// 고유코드 G
	data.code2 = 'X';			// 고유코드 X
	data.event_code = 'T';		// 명령코드 F, G, S(감시), T(단선), R
	data.recv_number[0] = '0';	// 수신기 번호 ASCII
	data.recv_number[1] = '0';	// 수신기 번호 ASCII
	data.unit_number[0] = '0';	// 유닛 번호 ASCII
	data.unit_number[1] = '0';	// 유닛 번호 ASCII
	data.seperate = '-';		// 구분
	data.type_number = '1';		// 계통번호 ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// 발생정보 N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}


void CFCMTestDlg::OnBnClickedButtonRecovery()
{
	char sType = 'R';
	if (m_radioF.GetCheck())
	{
		sType = 'F';
	}
	else if (m_radioG.GetCheck())
	{
		sType = 'G';
	}
	else if (m_radioS.GetCheck())
	{
		sType = 'S';
	}
	else if (m_radioT.GetCheck())
	{
		sType = 'T';
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	STATE_DATA data;
	data.code1 = 'G';			// 고유코드 G
	data.code2 = 'X';			// 고유코드 X
	data.event_code = sType;	// 명령코드 F, G, S(감시), T(단선), R
	data.recv_number[0] = '0';	// 수신기 번호 ASCII
	data.recv_number[1] = '0';	// 수신기 번호 ASCII
	data.unit_number[0] = '0';	// 유닛 번호 ASCII
	data.unit_number[1] = '0';	// 유닛 번호 ASCII
	data.seperate = '-';		// 구분
	data.type_number = '1';		// 계통번호 ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'F';			// 발생정보 N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

LRESULT CFCMTestDlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseAddEvent:
		if (nValue == 1) { // success

		}
		else { // fail

		}
		break;
	default:
		break;
	}
	return 0;
}