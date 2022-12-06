
// LogRemoverDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "LogRemover.h"
#include "LogRemoverDlg.h"
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


// CLogRemoverDlg 대화 상자



CLogRemoverDlg::CLogRemoverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGREMOVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogRemoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogRemoverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CLogRemoverDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CLogRemoverDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()

bool g_bThread = false;
void func_thread(CLogRemoverDlg* pDlg, CString sPath, int nTerm)
{
	DWORD dwLastTime = 0;
	DWORD dwTerm = (60 * 1000) * (60); // 1 시간마다
	while (g_bThread)
	{
		DWORD ddd = GetTickCount();
		if (GetTickCount() - dwLastTime > dwTerm)
		{
			CTime CurTime = CTime::GetCurrentTime();
			CTime tDel;
			tDel = CurTime - CTimeSpan(nTerm, 0, 0, 0); // 일, 시, 분, 초

			pDlg->DeleteLogfiles(sPath, tDel, 0, 0, 0);
			dwLastTime = GetTickCount();
		}
		Sleep(1);
	}
}

// CLogRemoverDlg 메시지 처리기

BOOL CLogRemoverDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	SetDlgItemText(IDC_EDIT_FOLDER, _T("log"));
	SetDlgItemText(IDC_EDIT_SAVEDAY, _T("30"));

	m_Thread = NULL;

	SetTimer(1, 1000, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLogRemoverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLogRemoverDlg::OnPaint()
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
HCURSOR CLogRemoverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLogRemoverDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Thread)
	{
		return;
	}

	CString sFolder;
	GetDlgItemText(IDC_EDIT_FOLDER, sFolder);

	CString sPath;
	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);

	m_sPath.Format(_T("%s\\%s"), szPath, sFolder);

	GetDlgItemText(IDC_EDIT_SAVEDAY, sFolder);
	int nTerm = _ttoi(sFolder.GetBuffer(0));
	sFolder.ReleaseBuffer();

	g_bThread = true;
	m_Thread = new thread(&func_thread, this, m_sPath, nTerm);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_FOLDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SAVEDAY)->EnableWindow(FALSE);
}


void CLogRemoverDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_Thread)
	{
		return;
	}
	g_bThread = false;
	m_Thread->join();

	SAFE_DELETE(m_Thread);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_FOLDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SAVEDAY)->EnableWindow(TRUE);
}


BOOL CLogRemoverDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogRemoverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);
		OnBnClickedButtonStart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL DeleteDirectory(CString lpDirPath)
{
	if (lpDirPath.GetLength() <= 3)// 경로가 없는 경우 되돌아간다.
	{
		return FALSE;
	}
	BOOL bRval = FALSE;
	int nRval = 0;
	CString szNextDirPath = _T("");
	CString szRoot = _T("");
	CFileFind find;

	// 폴더가 존재 하는 지 확인 검사
	bRval = find.FindFile(lpDirPath);
	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();
		// . or .. 인 경우 무시 한다.
		if (find.IsDots() == TRUE)
		{
			continue;
		}
		// Directory 일 경우
		if (find.IsDirectory())
		{
			szNextDirPath.Format(_T("%s\\*.*"), find.GetFilePath());
			// Recursion function 호 출           
			DeleteDirectory(szNextDirPath);
		}
		// file일 경우          
		else
		{
			// 파일 삭제
			::DeleteFile(find.GetFilePath());
		}
	}
	szRoot = find.GetRoot();
	find.Close();
	bRval = RemoveDirectory(szRoot);
	return bRval;
}

void CLogRemoverDlg::DeleteLogfiles(CString sPath, CTime & tDel, int nYear, int nMon, int nType)
{
	CString sYear, sMon, sDay, sTemp, sValue;
	int nDay = 0;
	CFileFind finder;
	BOOL bRes;
	int nLogTime, nDelTime;
	int nTemp = 0;
	sTemp.Format(_T("%s\\*.*"), sPath);
	bRes = finder.FindFile(sTemp);
	while (bRes)
	{
		bRes = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}
		sValue = finder.GetFileName();
		nTemp = _ttoi(sValue.GetBuffer(0));
		if (finder.IsDirectory()) // 폴더
		{
			sValue = finder.GetFileName();
			nTemp = _ttoi(sValue.GetBuffer(0));
			if (nTemp == 0)
			{
				continue;
			}
			switch (nType)
			{
			case 0: // year
				sYear = finder.GetFileName();
				nYear = _ttoi(sYear.GetBuffer(0));
				sTemp.Format(_T("%s\\%s"), sPath, sYear);
				DeleteLogfiles(sTemp, tDel, nYear, 0, 1);
				break;
			case 1: // month
				sMon = finder.GetFileName();
				nMon = _ttoi(sMon.GetBuffer(0));
				sTemp.Format(_T("%s\\%s"), sPath, sMon);
				DeleteLogfiles(sTemp, tDel, nYear, nMon, 2);
				break;
			case 2: // day
				sDay = finder.GetFileName();
				nDay = _ttoi(sDay.GetBuffer(0));
				sValue.Format(_T("%s\\%s"), sPath, sDay);
				break;
			}

			sTemp.Format(_T("%04d%02d%02d"), nYear, nMon, nDay);
			nLogTime = _ttoi(sTemp.GetBuffer(0));
			sTemp.Format(_T("%04d%02d%02d"), tDel.GetYear(), tDel.GetMonth(), tDel.GetDay());
			nDelTime = _ttoi(sTemp.GetBuffer(0));

			if (nLogTime <= nDelTime)
			{
				// delete
				DeleteDirectory(sValue);
			}
		}
	}
}