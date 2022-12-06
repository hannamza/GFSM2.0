#pragma once

#include "TSingleton.h"

class CSharedInformation : public TSingleton<CSharedInformation>
{
public:
	CSharedInformation();
	~CSharedInformation();

public:
	void SetReceiveTime(DWORD dwTime);
	DWORD GetReceiveTime();

private:
	DWORD m_dwTime;
};

