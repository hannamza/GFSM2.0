
// WatchDog.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CWatchDogApp:
// �� Ŭ������ ������ ���ؼ��� WatchDog.cpp�� �����Ͻʽÿ�.
//

class CWatchDogApp : public CWinApp
{
public:
	CWatchDogApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CWatchDogApp theApp;