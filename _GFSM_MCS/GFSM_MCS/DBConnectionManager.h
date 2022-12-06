#pragma once

#include <mysql.h>

#define		RETRY_UTF8_TIME		(10*1000)

typedef struct DB_INFORMATION
{
	char szDBInfoName[32];
	char szDBName[32];
	char szIP[32];
	char szID[32];
	char szPW[32];
	int  nPort;
}dbInformation, *pDBInformation;

class CDBConnectionManager
{
public:
	CDBConnectionManager(void);
	~CDBConnectionManager(void);

	MYSQL*		GetConnectDB();
	BOOL		Begin();
	VOID		End();

	BOOL		DBConnectMysql(PCSTR pSvrIP, PCSTR pID, PCSTR pPass, PCSTR pDB, UINT nPort);

	MYSQL_RES*	MysqlSelectQuery(CHAR* szQuery);
	BOOL		MysqlExcuteQuery(CHAR* szQuery);
	VOID		ReleaseSelectQuery(MYSQL_RES *pContentRes);

	VOID		CheckerThreadDB();

protected:
	BOOL		ConnectMysql(MYSQL** pMysql, PCSTR pSvrIP, PCSTR pID, PCSTR pPass, PCSTR pDB, UINT nPort);

	VOID		SetMysqlError(MYSQL *pMysql);
	VOID		DisconnectDB(MYSQL* pMysql);

	BOOL		MysqlSelectDBNamesUTF();

protected:
	BOOL		m_bRetryUTF;
	DWORD		m_dwLastUTFSet;

public:
	BOOL		m_bConnected;
	CHAR		m_szErrorMsg[512];
	
	MYSQL*		m_pMasterDB;
	dbInformation m_masterDBInfo;

	HANDLE		m_hThreadHandle;
	HANDLE		mStartupEventHandle;
	BOOL		m_bThread;
};
