#include "stdafx.h"
#include "DeviceInfo.h"


CDeviceInfo::CDeviceInfo()
{
}

CDeviceInfo::~CDeviceInfo()
{
}

void CDeviceInfo::SetDeviceInfo()
{
	CFileFind finder;
	CString strFileName;

	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);

	strFileName.Format(_T("%s\\Data\\*"), szPath);
	//strFileName = theApp.GetAppPath() + L"\\Data\\*";

	BOOL bWorking = finder.FindFile(strFileName);
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		strFileName = finder.GetFileName();

		if ("." == strFileName ||
			".." == strFileName)
		{
			continue;
		}

		if (_T("CRTDATA") == strFileName.Left(7))
		{
			TCHAR szPath[2048];
			GetModuleFileName(NULL, szPath, 2048);
			PathRemoveFileSpec(szPath);

			strFileName.Format(_T("%s\\Data\\%s"), szPath, strFileName);
			//strFileName = theApp.GetAppPath() + L"\\Data\\" + strFileName;
	
			GetDeviceInfo(strFileName);
		}
	}
}

CString CDeviceInfo::GetDeviceName(CString strAddress)
{
	CString strValue;
	
	m_Info.Lookup(strAddress, strValue);

	return strValue;
}

void CDeviceInfo::GetDeviceInfo(CString strFileName)
{
	FILE *fp;

	char szBuf[BUFF_SIZE];
	char szTemp[BUFF_SIZE];

	CString strTemp;

	CString strLineNum;
	CString strLineName;
//	CString strOutputType;
	CString strLineType;

	CString strDeviceNum;
	CString strDeviceName;

	memset(szBuf, 0x00, sizeof(szBuf));

	fopen_s(&fp, (CStringA)strFileName, "r");

	if (NULL == fp)
	{
		//theApp.WriteLog(L"%s File Open Error!", strFileName);
		TRACE(L"%s File Open Error!", strFileName);
		return ;
	}

	while (NULL != fgets(szBuf, sizeof(szBuf), fp))
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		strncpy(szTemp, szBuf, 8);

		strLineNum = (LPSTR)szTemp;

		strTemp = strtok(szBuf, ",");
		strLineType = strtok(NULL, ","); //해당 번호의 설비의 종류 구분을 위해 추가 : 1 ~ 9 - 화재 / 10 ~ 13 - 감시 / 14 - 가스
		strTemp = strtok(NULL, ",");
		strLineName = strtok(NULL, ",");

		m_Info.SetAt(strLineNum, strLineType + ":" + strLineName);
	}

	fclose(fp);
}