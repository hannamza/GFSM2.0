
// FCMTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "FCMTest.h"
#include "FCMTestDlg.h"
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


// CFCMTestDlg ��ȭ ����



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


// CFCMTestDlg �޽��� ó����

BOOL CFCMTestDlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CFCMTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CEventSend::Delete();
	CDeviceInfo::Delete();

	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
}

BOOL CFCMTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFCMTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CFCMTestDlg::OnPaint()
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
HCURSOR CFCMTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFCMTestDlg::OnBnClickedButtonFire()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	STATE_DATA data;
	data.code1 = 'G';			// �����ڵ� G
	data.code2 = 'X';			// �����ڵ� X
	data.event_code = 'F';		// ����ڵ� F, G, S(����), T(�ܼ�), R
	data.recv_number[0] = '0';	// ���ű� ��ȣ ASCII
	data.recv_number[1] = '0';	// ���ű� ��ȣ ASCII
	data.unit_number[0] = '0';	// ���� ��ȣ ASCII
	data.unit_number[1] = '0';	// ���� ��ȣ ASCII
	data.seperate = '-';		// ����
	data.type_number = '1';		// �����ȣ ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// �߻����� N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonGas()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	STATE_DATA data;
	data.code1 = 'G';			// �����ڵ� G
	data.code2 = 'X';			// �����ڵ� X
	data.event_code = 'G';		// ����ڵ� F, G, S(����), T(�ܼ�), R
	data.recv_number[0] = '0';	// ���ű� ��ȣ ASCII
	data.recv_number[1] = '0';	// ���ű� ��ȣ ASCII
	data.unit_number[0] = '0';	// ���� ��ȣ ASCII
	data.unit_number[1] = '0';	// ���� ��ȣ ASCII
	data.seperate = '-';		// ����
	data.type_number = '1';		// �����ȣ ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// �߻����� N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonSpy()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	STATE_DATA data;
	data.code1 = 'G';			// �����ڵ� G
	data.code2 = 'X';			// �����ڵ� X
	data.event_code = 'S';		// ����ڵ� F, G, S(����), T(�ܼ�), R
	data.recv_number[0] = '0';	// ���ű� ��ȣ ASCII
	data.recv_number[1] = '0';	// ���ű� ��ȣ ASCII
	data.unit_number[0] = '0';	// ���� ��ȣ ASCII
	data.unit_number[1] = '0';	// ���� ��ȣ ASCII
	data.seperate = '-';		// ����
	data.type_number = '1';		// �����ȣ ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// �߻����� N, F

	CEventSend::Instance()->SendEvent((BYTE*)&data);
}

void CFCMTestDlg::OnBnClickedButtonLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	STATE_DATA data;
	data.code1 = 'G';			// �����ڵ� G
	data.code2 = 'X';			// �����ڵ� X
	data.event_code = 'T';		// ����ڵ� F, G, S(����), T(�ܼ�), R
	data.recv_number[0] = '0';	// ���ű� ��ȣ ASCII
	data.recv_number[1] = '0';	// ���ű� ��ȣ ASCII
	data.unit_number[0] = '0';	// ���� ��ȣ ASCII
	data.unit_number[1] = '0';	// ���� ��ȣ ASCII
	data.seperate = '-';		// ����
	data.type_number = '1';		// �����ȣ ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'N';			// �߻����� N, F

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	STATE_DATA data;
	data.code1 = 'G';			// �����ڵ� G
	data.code2 = 'X';			// �����ڵ� X
	data.event_code = sType;	// ����ڵ� F, G, S(����), T(�ܼ�), R
	data.recv_number[0] = '0';	// ���ű� ��ȣ ASCII
	data.recv_number[1] = '0';	// ���ű� ��ȣ ASCII
	data.unit_number[0] = '0';	// ���� ��ȣ ASCII
	data.unit_number[1] = '0';	// ���� ��ȣ ASCII
	data.seperate = '-';		// ����
	data.type_number = '1';		// �����ȣ ASCII
	data.line_number[0] = '0';
	data.line_number[1] = '1';
	data.line_number[2] = '2';
	data.event = 'F';			// �߻����� N, F

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