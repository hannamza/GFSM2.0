#pragma once

#include "TSingleton.h"

class CCommonState : public TSingleton<CCommonState>
{
public:
	CCommonState();
	~CCommonState();

public:
	void SetResult(int nIndex, bool bValue);
	bool GetResult(int nIndex);

public:
	int m_nIdx;
	char m_szID[32];
	bool m_bLoginResult;
	int m_nCurrentWorksite;
	int m_nManager;
	CWnd* m_pWnd;
	CWnd* m_pManagerWnd;
	CWnd* m_pUserWnd;

private:
	bool m_bResult[ProtocolHeader::DefineEndProtocol];
};

