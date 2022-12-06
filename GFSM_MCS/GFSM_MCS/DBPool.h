#pragma once

#include "TSingleton.h"
#include "DBConnectionManager.h"
#include "CriticalSectionEx.h"
#include <queue>

class CDBPool : public TSingleton<CDBPool>
{
public:
	CDBPool();
	~CDBPool();

	void SetDBInformation(dbInformation info);

	bool StartDBPool(int nCount);
	bool StopDBPool();

	CDBConnectionManager* GetDbManager();
	void ReturnDBManager(CDBConnectionManager* pManager);

protected:
	void Destroy();

private:
	std::queue<CDBConnectionManager*> m_pool;
	CCriticalSectionEx m_sync;
	dbInformation m_info;
};

