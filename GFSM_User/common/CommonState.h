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
	int m_nWorksiteSeq;
	int m_nLimitCount;
	char m_szID[32];
	bool m_bLoginResult;
	CString m_sID;
	CWnd* m_pWnd;
	CWnd* m_pManagerWnd;
	CWnd* m_pUserWnd;

private:
	bool m_bResult[ProtocolHeader::DefineEndProtocol];
};

