
// GFSM_SENDER.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CGFSM_SENDERApp:
// �� Ŭ������ ������ ���ؼ��� GFSM_SENDER.cpp�� �����Ͻʽÿ�.
//

class CGFSM_SENDERApp : public CWinApp
{
public:
	CGFSM_SENDERApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGFSM_SENDERApp theApp;
