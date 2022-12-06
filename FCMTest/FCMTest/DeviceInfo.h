#include "stdafx.h"
#include "TSingleton.h"

#define BUFF_SIZE 4096

#pragma once
class CDeviceInfo : public TSingleton<CDeviceInfo>
{
public:
	CDeviceInfo();
	~CDeviceInfo();

private:
	CMap <CString, LPCTSTR, CString, CString> m_Info;

	void GetDeviceInfo(CString strFileName);

public:
	void SetDeviceInfo();

	CString GetDeviceName(CString strAddress);
};

