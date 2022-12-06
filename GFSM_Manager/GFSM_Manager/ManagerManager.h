#pragma once

#include "TSingleton.h"
#include <afxtempl.h>

class CManagerManager : public TSingleton<CManagerManager>
{
public:
	CManagerManager();
	~CManagerManager();

public:
	bool InsertManager(ProtocolResponseGetManagerList::managerInfo* pInfo);
	bool DeleteManager(int nSeq);
	bool ModifyManager(ProtocolResponseGetManagerList::managerInfo* pInfo);

	void ReleaseList();
	int  GetCount();
	ProtocolResponseGetManagerList::managerInfo* GetManagerInfo(int nIndex);
	ProtocolResponseGetManagerList::managerInfo* GetManagerInfoBySeq(int nSeq);


private:
	CList<ProtocolResponseGetManagerList::managerInfo*, ProtocolResponseGetManagerList::managerInfo*> m_list;
	CCriticalSectionEx mSync;
};

