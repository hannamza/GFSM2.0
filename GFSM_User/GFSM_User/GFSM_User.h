
// GFSM_User.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CGFSM_UserApp:
// �� Ŭ������ ������ ���ؼ��� GFSM_User.cpp�� �����Ͻʽÿ�.
//

class CGFSM_UserApp : public CWinApp
{
public:
	CGFSM_UserApp();

	ULONG_PTR m_gdiplusToken;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CGFSM_UserApp theApp;