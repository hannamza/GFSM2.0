#include "stdafx.h"
#include "DBPool.h"


CDBPool::CDBPool()
{
	memset(&m_info, 0x00, sizeof(dbInformation));
}

CDBPool::~CDBPool()
{
	Destroy();
}

void CDBPool::SetDBInformation(dbInformation info)
{
	m_sync.Enter();
	memcpy_s(&m_info, sizeof(dbInformation), &info, sizeof(dbInformation));
	m_sync.Leave();
}

bool CDBPool::StopDBPool()
{
	Destroy();
	return true;
}

bool CDBPool::StartDBPool(int nCount)
{
	CDBConnectionManager* pManager;
	m_sync.Enter();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pManager = new CDBConnectionManager();
		pManager->DBConnectMysql(m_info.szIP, m_info.szID, m_info.szPW, m_info.szDBName, m_info.nPort);
		if (pManager->Begin())
		{
			Log::WriteLog(0, "[%d/%d] DB Connected.", nIndex+1, nCount);
			TRACE("DB Connected!\n");
		}
		m_pool.push(pManager);
	}
	m_sync.Leave();

	return true;
}

CDBConnectionManager* CDBPool::GetDbManager()
{
	CDBConnectionManager* pManager;
	m_sync.Enter();
	if (m_pool.empty())
	{
		m_sync.Leave();
		return NULL;
	}
	pManager = m_pool.front();
	//pManager->End();
	//SAFE_DELETE(pManager);
	m_pool.pop();
	m_sync.Leave();

	return pManager;
}

void CDBPool::ReturnDBManager(CDBConnectionManager* pManager)
{
	m_sync.Enter();

	m_pool.push(pManager);

	m_sync.Leave();
}

void CDBPool::Destroy()
{
	CDBConnectionManager* pManager;
	m_sync.Enter();
	while (!m_pool.empty())
	{
		pManager = m_pool.front();
		pManager->End();
		SAFE_DELETE(pManager);
		m_pool.pop();
	}
	m_sync.Leave();
}