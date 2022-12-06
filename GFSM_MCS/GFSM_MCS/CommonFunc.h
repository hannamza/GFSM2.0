#pragma once


#define		CHAR_LEN_SIZE		(100*1024)

static INT g_nFilePath = 0;
static CHAR g_szChar[CHAR_LEN_SIZE];
static TCHAR g_tszChar[CHAR_LEN_SIZE];
static WCHAR g_wszChar[CHAR_LEN_SIZE];

class CCommonFunc
{
public:
	CCommonFunc(void);
	~CCommonFunc(void);

public:
	static CHAR* WCharToChar(WCHAR* wszText);
	static CHAR* WCharToUtf8(WCHAR* wszText);
	static TCHAR* CharToTCHAR(char *str);
	static WCHAR* CharToWCHAR(char *str);
	static WCHAR* Utf8ToWCHAR(char* utf8);
	static UINT GetAtoUI(char* pValue);

public:
	static BOOL		GetRealFilePath(CString sFileName, CString & sPath);
	static CString	GetMakePath(CString sName);
	static CString	GetCurrentModuleFileName();

	static time_t	MakeTimeT(int YYYY, int MM, int DD, int hh, int mi, int ss);

	static CSize	GetTextExtent(CDC* _pDC, CString sText);
	static BOOL		CreateDirectory(LPCTSTR szDir);

	static CString	GetVersionInfo(HMODULE hLib, CString csEntry);
};
