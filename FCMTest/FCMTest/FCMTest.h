
// FCMTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFCMTestApp:
// �� Ŭ������ ������ ���ؼ��� FCMTest.cpp�� �����Ͻʽÿ�.
//

class CFCMTestApp : public CWinApp
{
public:
	CFCMTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFCMTestApp theApp;