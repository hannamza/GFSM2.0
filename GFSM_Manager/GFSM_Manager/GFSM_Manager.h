
// GFSM_Manager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#define BUFF_SIZE 4096

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CGFSM_ManagerApp:
// �� Ŭ������ ������ ���ؼ��� GFSM_Manager.cpp�� �����Ͻʽÿ�.
//

class CGFSM_ManagerApp : public CWinApp
{
public:
	CGFSM_ManagerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGFSM_ManagerApp theApp;