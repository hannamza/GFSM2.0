
// LogRemoverDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "LogRemover.h"
#include "LogRemoverDlg.h"
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


// CLogRemoverDlg ��ȭ ����



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
	DWORD dwTerm = (60 * 1000) * (60); // 1 �ð�����
	while (g_bThread)
	{
		DWORD ddd = GetTickCount();
		if (GetTickCount() - dwLastTime > dwTerm)
		{
			CTime CurTime = CTime::GetCurrentTime();
			CTime tDel;
			tDel = CurTime - CTimeSpan(nTerm, 0, 0, 0); // ��, ��, ��, ��

			pDlg->DeleteLogfiles(sPath, tDel, 0, 0, 0);
			dwLastTime = GetTickCount();
		}
		Sleep(1);
	}
}

// CLogRemoverDlg �޽��� ó����

BOOL CLogRemoverDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	SetDlgItemText(IDC_EDIT_FOLDER, _T("log"));
	SetDlgItemText(IDC_EDIT_SAVEDAY, _T("30"));

	m_Thread = NULL;

	SetTimer(1, 1000, NULL);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CLogRemoverDlg::OnPaint()
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
HCURSOR CLogRemoverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLogRemoverDlg::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogRemoverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1) {
		KillTimer(1);
		OnBnClickedButtonStart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL DeleteDirectory(CString lpDirPath)
{
	if (lpDirPath.GetLength() <= 3)// ��ΰ� ���� ��� �ǵ��ư���.
	{
		return FALSE;
	}
	BOOL bRval = FALSE;
	int nRval = 0;
	CString szNextDirPath = _T("");
	CString szRoot = _T("");
	CFileFind find;

	// ������ ���� �ϴ� �� Ȯ�� �˻�
	bRval = find.FindFile(lpDirPath);
	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();
		// . or .. �� ��� ���� �Ѵ�.
		if (find.IsDots() == TRUE)
		{
			continue;
		}
		// Directory �� ���
		if (find.IsDirectory())
		{
			szNextDirPath.Format(_T("%s\\*.*"), find.GetFilePath());
			// Recursion function ȣ ��           
			DeleteDirectory(szNextDirPath);
		}
		// file�� ���          
		else
		{
			// ���� ����
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
		if (finder.IsDirectory()) // ����
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