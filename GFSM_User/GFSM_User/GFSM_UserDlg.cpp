
// GFSM_UserDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "GFSM_User.h"
#include "GFSM_UserDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGFSM_UserDlg ��ȭ ����



CGFSM_UserDlg::CGFSM_UserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GFSM_USER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInit = false;

	m_font.CreateFont(16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 
	m_nUserSeq = 0;
	m_bDestroy = false;
}

void CGFSM_UserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ETC, m_editEtc);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
}

BEGIN_MESSAGE_MAP(CGFSM_UserDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnAddClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnModClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnDelClick)

	ON_COMMAND(IDC_COMMON_BUTTON4, OnMiniClick)
	ON_COMMAND(IDC_COMMON_BUTTON5, OnCloseClick)

	ON_MESSAGE(REDISPLAY_MSG, OnRedisplayMsg)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_MOVING()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CGFSM_UserDlg::OnItemchangedListUser)
END_MESSAGE_MAP()


// CGFSM_UserDlg �޽��� ó����
const TCHAR* lpszHeaderManager[] = { _T("NO"), _T("�̸�"), _T("��ȭ��ȣ"), _T("��� ����"), NULL };
const int colWidthListManager[] = { 6, 22, 25, 40, 0 };

BOOL CGFSM_UserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Log::Setup();
	Log::SetMainPointer(this);

	CClientInterface::New();

#ifndef LOCAL_TEST_MODE
	CClientInterface::Instance()->TryConnection("160.202.162.3", 10234);
#else
	CClientInterface::Instance()->TryConnection("127.0.0.1", 10234);
#endif

	CUserManager::New();
	CCommonState::New();
	CCommonState::Instance()->m_pUserWnd = this;

	m_sMessage = L"�α����Ͽ� �ֽʽÿ�.";

	SetWindowText(L"GFSM ����� ���� ���α׷�");

	Global.SetFont(IDX_FONT_SMALL, _T("����"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("����"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("����"), 11, TRUE);
	Global.SetFont(IDX_FONT_HEADER, _T("����"), 10, TRUE);

	CCommonState::Instance()->m_pWnd = this;

	this->SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOMOVE);
	CenterWindow();

	SetTimer(99, 5 * 1000, NULL);

	m_editName.MoveWindow(142, 200, 230, 20);
	m_editPhone.MoveWindow(474, 200, 230, 20);
	m_editEtc.MoveWindow(140, 240, 565, 85);
	m_listUser.MoveWindow(22, 371, 980, 376);

	m_btnAdd.Create(IDB_BMP_ADD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnMod.Create(IDB_BMP_MOD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnDel.Create(IDB_BMP_DEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnMini.Create(IDB_BMP_MAX, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON4);
	m_btnClose.Create(IDB_BMP_CLOSE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON5);

	m_btnMini.MoveWindow(925, 20, 28, 28);
	m_btnClose.MoveWindow(969, 20, 28, 28);

	m_btnAdd.MoveWindow(804, 185, 156, 44);
	m_btnMod.MoveWindow(804, 237, 156, 44);
	m_btnDel.MoveWindow(804, 289, 156, 44);

	CXListCtrl::InitList(m_listUser, lpszHeaderManager);
	CXListCtrl::ResizeListColumn(m_listUser, colWidthListManager);

	SetTimer(1, 100, NULL);

	m_bInit = true;
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CGFSM_UserDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_bDestroy = true;

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
	CUserManager::Delete();

	Log::Cleanup();
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CGFSM_UserDlg::OnPaint()
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

		Redisplay();

		m_editName.Invalidate();
		m_editPhone.Invalidate();
		m_editEtc.Invalidate();
		m_listUser.Invalidate();

		m_btnAdd.Invalidate();
		m_btnMod.Invalidate();
		m_btnDel.Invalidate();
		m_btnMini.Invalidate();
		m_btnClose.Invalidate();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CGFSM_UserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGFSM_UserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)
			{
				CWnd* pWnd = GetFocus();
				if (pWnd && pWnd->m_hWnd == m_editEtc.m_hWnd)
				{
					m_editEtc.SetSel(-1, 0);
					m_editEtc.ReplaceSel(L"\r\n");
				}

				return TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGFSM_UserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

void CGFSM_UserDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1) {
		KillTimer(1);

		CLoginDlg dlg;
		UINT nResult = dlg.DoModal();
		if (nResult != IDOK) {
			OnOK();
			return;
		}
		CCommonState::Instance()->m_bLoginResult = false;
		CCommonState::Instance()->m_sID = dlg.m_sID;
		CClientInterface::Instance()->ProcessRequestManagerInfo(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
		DWORD dwLastTime = GetTickCount();
		while (GetTickCount() - dwLastTime < 3000)
		{
			if (CCommonState::Instance()->m_bLoginResult) {
				break;
			}
			Sleep(1);
		}
		if (CCommonState::Instance()->m_nIdx > 0) {
			m_sMessage.Format(L"%s �� ȯ���մϴ�.        ", CCommonState::Instance()->m_sID);
			CDC* _pDC = GetDC();
			// message
			CCommonDisplay::DrawCaption(_pDC, m_sMessage, RGB(112, 112, 112), m_font, true, RGB(255, 255, 255), CRect(215, 36, 660, 57), DT_TOP | DT_LEFT | DT_SINGLELINE);
			ReleaseDC(_pDC);
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
		}
		else {
			CMessagePopup dlg(L"�α���", L"\n\n�α��� ���̵� �Ǵ� ��й�ȣ�� ���� �ʽ��ϴ�.\n\n�ٽ� �õ��Ͽ� �ֽʽÿ�.", MB_OK);
			dlg.DoModal();
			SetTimer(1, 10, NULL);
		}
	}
	else if (nIDEvent == 99) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CGFSM_UserDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CGFSM_UserDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

void CGFSM_UserDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

HBRUSH CGFSM_UserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CGFSM_UserDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CGFSM_UserDlg::OnRedisplayMsg(WPARAM wParam, LPARAM lParam)
{
	int nType = wParam;
	int nValue = lParam;
	switch (nType)
	{
	case 0: ReListup(); break;
	case 1: m_listUser.DeleteAllItems(); break;
	}

	return 0;
}

void CGFSM_UserDlg::ReListup()
{
	m_listUser.DeleteAllItems();

	CString sTemp;
	ProtocolResponseGetUserList::userInfo* pInfo;
	int nCount = CUserManager::Instance()->GetCount();
	//_T("No"), _T("�̸�"), _T("��ȭ��ȣ"), _T("���")
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pInfo = CUserManager::Instance()->GetUserInfo(nIndex);
		if (pInfo) {
			sTemp.Format(L"%d", nIndex + 1);
			m_listUser.InsertItem(nIndex, sTemp, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 1, CCommonFunc::Utf8ToWCHAR(pInfo->szName), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 2, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 3, CCommonFunc::Utf8ToWCHAR(pInfo->szEtc), RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemData(nIndex, (DWORD)pInfo);
		}
	}
}

void CGFSM_UserDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BG);

	// message
	CCommonDisplay::DrawCaption(&memDC, m_sMessage, RGB(112, 112, 112), m_font, 0, 0, CRect(215, 36, 660, 57), DT_TOP | DT_LEFT | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

LRESULT CGFSM_UserDlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseLogin:
		break;
	case ProtocolHeader::ResponseAddUser:
		if (nValue == 0 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"����� �߰�", L"\n\n����� �߰��� �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"����� �߰�", L"\n\n�̹� �����ϴ� ��ȭ��ȣ�Դϴ�.\n\n�ٸ� ��ȭ��ȣ�� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"����� �߰�", L"\n\n����� �߰��� �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseModUser:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseDelUser:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestGetUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CGFSM_UserDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialogEx::OnNcHitTest(point);
	if (nHit == HTCLIENT) {
		nHit = HTCAPTION;
	}

	return nHit;
}

void CGFSM_UserDlg::OnAddClick()
{
	if (CCommonState::Instance()->m_nLimitCount <= CUserManager::Instance()->GetCount()) {
		CString sTemp;
		sTemp.Format(L"\n\n����� �� �ִ� �ִ� ����� ��(�� %d ��)�Դϴ�.\n\nȮ�� �� �ٽ� �õ��Ͽ� �ֽʽÿ�.", CCommonState::Instance()->m_nLimitCount);
		CMessagePopup dlg(L"����� �߰�", sTemp, MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sName, sPhone, sEtc;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� �߰�", L"\n\n�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");
	if (sPhone.GetLength() < 8)
	{
		CMessagePopup dlg(L"����� �߰�", L"\n\n��ȭ��ȣ�� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"�̸�: %s\n\n��ȭ��ȣ: %s\n\n������: %s", sName, sPhone, sEtc);
	CMessagePopup dlg(L"����� �߰�", sText, L"�� ������ ����ڸ� ����Ͻðڽ��ϱ�?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestAddUser(CCommonState::Instance()->m_nIdx, sName, sPhone, sEtc);
}

void CGFSM_UserDlg::OnModClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
	{
		return;
	}
	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CString sName, sPhone, sEtc;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");
	if (sPhone.GetLength() < 8)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n��ȭ��ȣ�� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"�̸�: %s\n\n��ȭ��ȣ: %s\n\n������: %s", sName, sPhone, sEtc);
	CMessagePopup dlg(L"����� ����", sText, L"�� ������ ����ڸ� �����Ͻðڽ��ϱ�?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestModUser(pInfo->nSeq, sName, sPhone, sEtc);
}

void CGFSM_UserDlg::OnDelClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CString sText, sName = CCommonFunc::Utf8ToWCHAR(pInfo->szName);
	sText.Format(L"\n\n[%s : %s]\n\n������ ����ڸ� �����Ͻðڽ��ϱ�?", sName, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile));
	CMessagePopup dlg(L"����� ����", sText, MB_YESNO, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelUser(pInfo->nSeq);
}

void CGFSM_UserDlg::OnMiniClick()
{
	ShowWindow(SW_MINIMIZE);
}

void CGFSM_UserDlg::OnCloseClick()
{
	OnOK();
}


void CGFSM_UserDlg::OnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	if (m_bDestroy) {
		return;
	}

	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		m_nUserSeq = 0;

		SetDlgItemText(IDC_EDIT_NAME, L"");
		SetDlgItemText(IDC_EDIT_PHONE, L"");
		SetDlgItemText(IDC_EDIT_ETC, L"");
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	ProtocolResponseGetUserList::userInfo* pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	m_nUserSeq = pInfo->nSeq;

	SetDlgItemText(IDC_EDIT_NAME, CCommonFunc::Utf8ToWCHAR(pInfo->szName));
	SetDlgItemText(IDC_EDIT_PHONE, CCommonFunc::Utf8ToWCHAR(pInfo->szMobile));
	SetDlgItemText(IDC_EDIT_ETC, CCommonFunc::Utf8ToWCHAR(pInfo->szEtc));
}
