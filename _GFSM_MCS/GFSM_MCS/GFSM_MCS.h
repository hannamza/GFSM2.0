
// GFSM_MCS.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CGFSM_MCSApp:
// �� Ŭ������ ������ ���ؼ��� GFSM_MCS.cpp�� �����Ͻʽÿ�.
//

class CGFSM_MCSApp : public CWinApp
{
public:
	CGFSM_MCSApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGFSM_MCSApp theApp;