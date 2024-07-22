
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

//20240627 GBM start - 관리자에 따른 수신기 타입 추가
enum {
	MANAGER_FACP_TYPE_F3_KOREAN,
	MANAGER_FACP_TYPE_GT1_KOREAN,
	MANAGER_FACP_TYPE_F3_ENGLISH,
	MANAGER_FACP_TYPE_GT1_ENGLISH
};
//20240627 GBM end

//20240704 GBM start - 이벤트 타입 구분을 위한 입력타입 enum 추가
enum {
	자탐감지기 = 1,
	A교차감지기,
	B교차감지기,
	일반감지기,
	화재용접점,
	열식아나로그,
	연식아나로그,
	열식주소형,
	연식주소형,
	감시비지속,
	감시지속,
	일반확인,
	전원확인,
	가스누출,
	사용안함,
	AN열식교차A,
	AN열식교차B,
	AN연식교차A,
	AN연식교차B,
	발신기화재,
	광센서감지기,
	CCTV,
	NMS
};
//20240704 GBM end

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


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

//20230207 GBM start - GBM 정의 추가
//20230207 GBM - 이벤트 테스트 플래그, 배포 시에는 주석처리
//#define EVENT_TEST_MODE	

//Push Message 시간 측정 테스트 플래그
#define PUSH_MESSAGE_TIME_MEASURE_MODE

//수신기 event 버퍼 구성
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

//수신기 event 명령 코드 enum
enum {
	COMMAND_FIRE,
	COMMAND_GAS,
	COMMAND_SURVEIL,
	COMMAND_CUT,
	COMMAND_RECOVER,
	COMMAND_TOTAL_COUNT
};

// 발생 정보 enum
enum {
	OCCUR_INFO_OCCUR,
	OCCUR_INFO_RECOVER,
	OCCUR_INFO_TOTAL_COUNT
};

//수신기 event 명령 코드 문자열
static const TCHAR* g_lpszCommandString[] = {
	_T("화재"),	//F
	_T("가스"),	//G
	_T("감시"),	//S
	_T("단선"),	//T
	_T("복구"),	//R
	NULL
};

//수신기 event 명령 코드
static const TCHAR g_lpszCommand[] = {
	_T('F'),	//화재
	_T('G'),	//가스
	_T('S'),	//감시
	_T('T'),	//단선
	_T('R'),	//복구
	NULL
};

//수신기 event 발생 문자열
static const TCHAR* g_lpszOccurInfoString[] = {
	_T("발생"),
	_T("복구"),
	NULL
};

//수신기 event 발생 코드
static const TCHAR g_lpszOccurInfo[] = {
	_T('N'),
	_T('F'),
	NULL
};

//수신기 최대 개수
#define RECEIVER_MAX_COUNT	32

//유닛 최대 개수
#define UNIT_MAX_COUNT	64

//계통 최대 개수
#define SYSTEM_MAX_COUNT	4

//회로 최대 개수
#define CIRCUIT_MAX_COUNT	253

//사용자(수신기 이벤트를 받을 인원) 최대 개수
#define USER_MAX_COUNT	20

#define PROGRAM_NAME	"GFSM 이벤트 관리자"

#define PROGRAM_VERSION	3.0

//20230207 GBM end

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장

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


