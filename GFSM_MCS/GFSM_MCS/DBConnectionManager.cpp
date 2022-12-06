#include "StdAfx.h"
#include "DBConnectionManager.h"


DWORD WINAPI CheckerThreadDB(LPVOID parameter)
{
	CDBConnectionManager *Owner = (CDBConnectionManager*) parameter;
	Owner->CheckerThreadDB();
	return 0;
}

VOID CDBConnectionManager::CheckerThreadDB()
{
	SetEvent(mStartupEventHandle);
	m_bThread = TRUE;

	TIME_SLEEP(2000, m_bThread);

	while(m_bThread)
	{
		if(ConnectMysql(&m_pMasterDB, m_masterDBInfo.szIP, m_masterDBInfo.szID, m_masterDBInfo.szPW, m_masterDBInfo.szDBName, m_masterDBInfo.nPort))
		{
			m_bConnected = TRUE;
			return;
		}

		TIME_SLEEP(5000, m_bThread);

		/*if(!m_pMasterDB)
			continue;

		if(!MysqlSelectDBNamesUTF())//mysql_ping(m_pMasterDB))
			TRACE("DB Connection disconnected!!!\n");
		else TRACE("DB Connection state!!!\n");*/
	}
}

CDBConnectionManager::CDBConnectionManager(void)
{
	m_pMasterDB			= NULL;		// DB 
	m_bConnected		= FALSE;	// 연결 여부

	mStartupEventHandle = NULL;
	m_hThreadHandle		= NULL;
	m_bThread			= FALSE;

	m_bRetryUTF			= FALSE;
	m_dwLastUTFSet		= 0;

	memset(&m_masterDBInfo,	0x00,	sizeof(dbInformation));
}

CDBConnectionManager::~CDBConnectionManager(void)
{
}

BOOL CDBConnectionManager::DBConnectMysql(PCSTR pSvrIP, PCSTR pID, PCSTR pPass, PCSTR pDB, UINT nPort)
{
	m_masterDBInfo.nPort = nPort;
	strcpy(m_masterDBInfo.szIP		,pSvrIP	);
	strcpy(m_masterDBInfo.szID		,pID	);
	strcpy(m_masterDBInfo.szPW		,pPass	);
	strcpy(m_masterDBInfo.szDBName	,pDB	);

	return TRUE;
}

BOOL CDBConnectionManager::Begin()
{
	if(ConnectMysql(&m_pMasterDB, m_masterDBInfo.szIP, m_masterDBInfo.szID, m_masterDBInfo.szPW, m_masterDBInfo.szDBName, m_masterDBInfo.nPort))
	{
		m_bConnected = TRUE;
		return TRUE;
	}
	
	// 최초 연결이 안되면 최초 연결이 될때까지 연결 시도
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		return FALSE;
	}
	m_hThreadHandle = CreateThread(NULL, 0, ::CheckerThreadDB, this, 0, NULL);
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return FALSE;
}

VOID CDBConnectionManager::End()
{
	m_bThread = FALSE;

	if(m_pMasterDB) DisconnectDB(m_pMasterDB);
}

BOOL CDBConnectionManager::ConnectMysql(MYSQL** pMysql, PCSTR pSvrIP, PCSTR pID, PCSTR pPass, PCSTR pDB, UINT nPort)
{
	*pMysql = mysql_init(NULL);
	if(*pMysql == NULL) return FALSE;

	my_bool chOn = 1;
	mysql_options(*pMysql, MYSQL_OPT_RECONNECT, (PCHAR)&chOn);

	if(mysql_real_connect(*pMysql, pSvrIP, pID, pPass, pDB, nPort, NULL, 131074) == NULL)
	{
		trace0("ERROR: %s\n", mysql_error(*pMysql));
		*pMysql = NULL;
		return FALSE;
	}

	m_dwLastUTFSet = GetTickCount();
	if(mysql_query(*pMysql, "set names utf8") != 0)
	{
		trace0("ERROR: %s\n", mysql_error(*pMysql));
		return FALSE;
	}

	return TRUE;
}

MYSQL* CDBConnectionManager::GetConnectDB()
{
	if(m_bConnected) return m_pMasterDB;
	else return NULL;
}

VOID CDBConnectionManager::SetMysqlError(MYSQL *pMysql)
{
	strcpy_s(m_szErrorMsg, 256, mysql_error(pMysql));
}

VOID CDBConnectionManager::DisconnectDB(MYSQL* pMysql)
{
	if(pMysql)
	{
		m_bConnected = FALSE;
		mysql_close(pMysql);
		pMysql = NULL;
	}
}

MYSQL_RES* CDBConnectionManager::MysqlSelectQuery(CHAR* szQuery)
{
	MYSQL_RES *pRes;

	MYSQL* pSql = GetConnectDB();
	if(!pSql)
	{
		return NULL;
	}

	if(m_bRetryUTF || GetTickCount() - m_dwLastUTFSet >= RETRY_UTF8_TIME) // 재 연결이 되면 utf8 설정을 해준다
	{
		mysql_query(pSql, "set names utf8");
		m_dwLastUTFSet = GetTickCount();
		m_bRetryUTF = FALSE;
	}

	if(mysql_query(pSql, szQuery) != 0)
	{
		m_bRetryUTF = TRUE;
		return NULL;
	}

	pRes = mysql_store_result(pSql);
	if(pRes == NULL)
	{
		return NULL;
	}

	if(mysql_num_rows(pRes) == 0)
	{
		ReleaseSelectQuery(pRes);
		return NULL;
	}

	return pRes;
}


VOID CDBConnectionManager::ReleaseSelectQuery(MYSQL_RES *pContentRes)
{
	MYSQL* pSql = GetConnectDB();
	if(pContentRes != NULL)
	{
		mysql_free_result(pContentRes);
		int n = mysql_next_result(pSql);
	}
}

BOOL CDBConnectionManager::MysqlSelectDBNamesUTF()
{
	MYSQL* pSql = GetConnectDB();
	if(!pSql) return FALSE;

	m_dwLastUTFSet = GetTickCount();
	if(mysql_query(pSql, "set names utf8") != 0)
		return FALSE;

	return TRUE;
}

BOOL CDBConnectionManager::MysqlExcuteQuery(CHAR* szQuery)
{
	MYSQL_RES *pRes;
	MYSQL* pSql = GetConnectDB();

	if(!pSql)
	{
		return FALSE;
	}

	if(mysql_query(pSql, szQuery) != 0)
	{
		m_bRetryUTF = TRUE;
		return FALSE;
	}

	pRes = mysql_store_result(pSql);
	if(pRes != NULL)
	{
		mysql_free_result(pRes);
		int n = mysql_next_result(pSql);
	}

	return TRUE;
}
