#include "stdafx.h"
#include "CommonState.h"


CCommonState::CCommonState()
{
	m_nIdx = 0;
	m_szID[0] = '\0';
	m_dwToken = 0;
	m_dwLastRecv = GetTickCount();
	m_nFacpType = 0;
}


CCommonState::~CCommonState()
{
}
