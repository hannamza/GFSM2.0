#include "stdafx.h"
#include "CommonState.h"


CCommonState::CCommonState()
{
	m_nIdx = 0;
	m_nWorksiteSeq = 0;
	m_nLimitCount = 0;
	m_szID[0] = '\0';
	m_pWnd = NULL;
	m_pManagerWnd = NULL;
	m_pUserWnd = NULL;
}

CCommonState::~CCommonState()
{
}

void CCommonState::SetResult(int nIndex, bool bValue)
{
	m_bResult[nIndex] = bValue;
}

bool CCommonState::GetResult(int nIndex)
{
	return m_bResult[nIndex];
}
