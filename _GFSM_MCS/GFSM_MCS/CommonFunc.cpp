#include "StdAfx.h"
#include "CommonFunc.h"


CCommonFunc::CCommonFunc(void)
{
}

CCommonFunc::~CCommonFunc(void)
{
}

CHAR* CCommonFunc::WCharToChar(WCHAR* wszText)
{
	int l = WideCharToMultiByte(CP_ACP, 0, wszText, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, wszText, -1, g_szChar, CHAR_LEN_SIZE, 0, 0);
	return g_szChar;
}

TCHAR* CCommonFunc::CharToTCHAR(char *str)
{
#if defined(UNICODE) || defined(_UNICODE)
        int nLen = strlen(str) + 1;
        memset(g_tszChar, 0x00, nLen*sizeof(TCHAR));
        MultiByteToWideChar(CP_ACP, 0, str, -1, g_tszChar, nLen*sizeof(TCHAR));
#else
        int nLen = strlen(str) + 1;
        memset(g_tszChar, 0x00, nLen*sizeof(TCHAR));
        _tcscpy(g_tszChar, str);
#endif
        return g_tszChar;
}

WCHAR* CCommonFunc::CharToWCHAR(char *str)
{
    int nLen = strlen(str) + 1;
    memset(g_wszChar, 0x00, nLen*sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, g_wszChar, nLen*sizeof(WCHAR));

	return g_wszChar;
}

BOOL CCommonFunc::GetRealFilePath(CString sFileName, CString & sPath)
{
	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);

	sPath.Format(_T("%s\\%s"), szPath, sFileName);
	return TRUE;
}

CString CCommonFunc::GetMakePath(CString sName)
{
	CString sPath;
	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);

	sPath.Format(_T("%s\\%s"), szPath, sName);
	return sPath;
}

CString	CCommonFunc::GetCurrentModuleFileName()
{
	TCHAR szPath[2048];
	GetModuleFileName(NULL, szPath, 2048);
	CString sPath = szPath;
	return sPath.Right(sPath.GetLength() - sPath.ReverseFind(_T('\\')));
}

CSize CCommonFunc::GetTextExtent(CDC* _pDC, CString sText)
{
	CSize size = _pDC->GetTextExtent(sText);
	return size;
}

BOOL CCommonFunc::CreateDirectory(LPCTSTR szDir)
{
	CString strTemp(szDir);

	if (strTemp.GetLength() <= 3)
		return FALSE;

	if (strTemp[strTemp.GetLength() - 1] == _T('\\'))
		strTemp = strTemp.Left(strTemp.GetLength() - 1);

	DWORD dwAttr = ::GetFileAttributes(strTemp);
	if (dwAttr == -1)
	{
		// 하위디렉토리를 만든 다음 디렉토리를 만든다.
		CreateDirectory(strTemp.Left(strTemp.ReverseFind(_T('\\'))));

		BOOL bSucc = ::CreateDirectory(strTemp, NULL);
		if (!bSucc && ::GetLastError() != ERROR_ALREADY_EXISTS)
			;
		else if (!bSucc)
			return FALSE;
	}
	else if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
	{
	}
	return TRUE;
}

#pragma comment(lib, "version.lib")
CString CCommonFunc::GetVersionInfo(HMODULE hLib, CString csEntry)
{
	CString csRet;

	if (hLib == NULL)
		hLib = AfxGetResourceHandle();

	HRSRC hVersion = FindResource(hLib, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hVersion != NULL)
	{
		HGLOBAL hGlobal = LoadResource(hLib, hVersion);
		if (hGlobal != NULL)
		{

			LPVOID versionInfo = LockResource(hGlobal);
			if (versionInfo != NULL)
			{
				DWORD vLen, langD;
				BOOL retVal;

				LPVOID retbuf = NULL;
#ifdef _UNICODE
				static TCHAR fileEntry[256];
				wsprintf(fileEntry, _T("\\VarFileInfo\\Translation"));
#else
				static char fileEntry[256];
				sprintf(fileEntry, _T("\\VarFileInfo\\Translation"));
#endif


				retVal = VerQueryValue(versionInfo, fileEntry, &retbuf, (UINT *)&vLen);
				if (retVal && vLen == 4)
				{
					memcpy(&langD, retbuf, 4);
#ifdef _UNICODE
					wsprintf(fileEntry, _T("\\StringFileInfo\\%02X%02X%02X%02X\\%s"),
						(langD & 0xff00) >> 8, langD & 0xff, (langD & 0xff000000) >> 24,
						(langD & 0xff0000) >> 16, csEntry);
#else
					sprintf(fileEntry, _T("\\StringFileInfo\\%02X%02X%02X%02X\\%s"),
						(langD & 0xff00) >> 8, langD & 0xff, (langD & 0xff000000) >> 24,
						(langD & 0xff0000) >> 16, csEntry);
#endif
				}
				else
#ifdef _UNICODE
					wsprintf(fileEntry, L"\\StringFileInfo\\%04X04B0\\%s", GetUserDefaultLangID(), csEntry);
#else
					sprintf(fileEntry, "\\StringFileInfo\\%04X04B0\\%s", GetUserDefaultLangID(), (LPCTSTR)csEntry);
#endif

				if (VerQueryValue(versionInfo, fileEntry, &retbuf, (UINT *)&vLen))
#ifdef _UNICODE
					csRet = (TCHAR*)retbuf;
#else
					csRet = (char*)retbuf;
#endif
			}
		}

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return csRet;
}