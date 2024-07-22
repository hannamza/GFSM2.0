
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

//20240627 GBM start - �����ڿ� ���� ���ű� Ÿ�� �߰�
enum {
	MANAGER_FACP_TYPE_F3_KOREAN,
	MANAGER_FACP_TYPE_GT1_KOREAN,
	MANAGER_FACP_TYPE_F3_ENGLISH,
	MANAGER_FACP_TYPE_GT1_ENGLISH
};
//20240627 GBM end

//20240704 GBM start - �̺�Ʈ Ÿ�� ������ ���� �Է�Ÿ�� enum �߰�
enum {
	��Ž������ = 1,
	A����������,
	B����������,
	�Ϲݰ�����,
	ȭ�������,
	���ľƳ��α�,
	���ľƳ��α�,
	�����ּ���,
	�����ּ���,
	���ú�����,
	��������,
	�Ϲ�Ȯ��,
	����Ȯ��,
	��������,
	������,
	AN���ı���A,
	AN���ı���B,
	AN���ı���A,
	AN���ı���B,
	�߽ű�ȭ��,
	������������,
	CCTV,
	NMS
};
//20240704 GBM end

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <string>
using namespace std;

#define		SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define		TIME_SLEEP(t, b) { DWORD cT=GetTickCount(); while(b && GetTickCount() - cT < t) Sleep(1); }

#include "DeviceInfo.h"
#include "TSingleton.h"
#include "SerialComm.h"
#include "CriticalSectionEx.h"
#include "Log.h"
#include "CommonFunc.h"
#include "Network.h"
#include "ClientInterface.h"
#include "CommonProtocol.h"
#include "CommonState.h"
#include "ReadWriteState.h"

#include "Global.h"
#include "SkinButton.h"
#include "SkinDialog.h"

#define STX       0x02
#define ETX       0x03
#define NEW_INPUT 0x0A

extern HANDLE G_hShutdown_Event;
extern CGlobal Global;

//20230207 GBM start - GBM ���� �߰�
//20230207 GBM - �̺�Ʈ �׽�Ʈ �÷���, ���� �ÿ��� �ּ�ó��
//#define EVENT_TEST_MODE	

//Push Message �ð� ���� �׽�Ʈ �÷���
#define PUSH_MESSAGE_TIME_MEASURE_MODE

//���ű� event ���� ����
enum {
	SI_EVENT_BUF_COMPANY_1,
	SI_EVENT_BUF_COMPANY_2,
	SI_EVENT_BUF_COMMAND,
	SI_EVENT_BUF_FIRE_RECEIVER_1,
	SI_EVENT_BUF_FIRE_RECEIVER_2,
	SI_EVENT_BUF_UNIT_1,
	SI_EVENT_BUF_UNIT_2,
	SI_EVENT_BUF_DASH,
	SI_EVENT_BUF_SYSTEM,
	SI_EVENT_BUF_CIRCUIT_1,
	SI_EVENT_BUF_CIRCUIT_2,
	SI_EVENT_BUF_CIRCUIT_3,
	SI_EVENT_BUF_OCCUR_INFO,
	SI_EVENT_BUF_EOP_1,
	SI_EVENT_BUF_EOP_2,
	SI_EVENT_BUF_SIZE
};

//���ű� event ��� �ڵ� enum
enum {
	COMMAND_FIRE,
	COMMAND_GAS,
	COMMAND_SURVEIL,
	COMMAND_CUT,
	COMMAND_RECOVER,
	COMMAND_TOTAL_COUNT
};

// �߻� ���� enum
enum {
	OCCUR_INFO_OCCUR,
	OCCUR_INFO_RECOVER,
	OCCUR_INFO_TOTAL_COUNT
};

//���ű� event ��� �ڵ� ���ڿ�
static const TCHAR* g_lpszCommandString[] = {
	_T("ȭ��"),	//F
	_T("����"),	//G
	_T("����"),	//S
	_T("�ܼ�"),	//T
	_T("����"),	//R
	NULL
};

//���ű� event ��� �ڵ�
static const TCHAR g_lpszCommand[] = {
	_T('F'),	//ȭ��
	_T('G'),	//����
	_T('S'),	//����
	_T('T'),	//�ܼ�
	_T('R'),	//����
	NULL
};

//���ű� event �߻� ���ڿ�
static const TCHAR* g_lpszOccurInfoString[] = {
	_T("�߻�"),
	_T("����"),
	NULL
};

//���ű� event �߻� �ڵ�
static const TCHAR g_lpszOccurInfo[] = {
	_T('N'),
	_T('F'),
	NULL
};

//���ű� �ִ� ����
#define RECEIVER_MAX_COUNT	32

//���� �ִ� ����
#define UNIT_MAX_COUNT	64

//���� �ִ� ����
#define SYSTEM_MAX_COUNT	4

//ȸ�� �ִ� ����
#define CIRCUIT_MAX_COUNT	253

//�����(���ű� �̺�Ʈ�� ���� �ο�) �ִ� ����
#define USER_MAX_COUNT	20

#define PROGRAM_NAME	"GFSM �̺�Ʈ ������"

#define PROGRAM_VERSION	3.0

//20230207 GBM end

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��

#define		WM_TRAY_NOTIFICATION		(WM_USER+1003)





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


