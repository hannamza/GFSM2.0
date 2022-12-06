#include "stdafx.h"
#include "SharedInformation.h"


CSharedInformation::CSharedInformation()
{
	m_dwTime = 0;
}

CSharedInformation::~CSharedInformation()
{
}

void CSharedInformation::SetReceiveTime(DWORD dwTime)
{
	m_dwTime = dwTime;
}

DWORD CSharedInformation::GetReceiveTime()
{
	return m_dwTime;
}

