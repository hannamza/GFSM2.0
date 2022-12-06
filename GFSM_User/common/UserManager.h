#pragma once

#include "TSingleton.h"
#include <afxtempl.h>

class CUserManager : public TSingleton<CUserManager>
{
public:
	CUserManager();
	~CUserManager();

public:
	bool InsertUser(ProtocolResponseGetUserList::userInfo* pInfo);
	bool DeleteUser(int nSeq);
	bool ModifyUser(ProtocolResponseGetUserList::userInfo* pInfo);

	void ReleaseList();
	int  GetCount();
	ProtocolResponseGetUserList::userInfo* GetUserInfo(int nIndex);


private:
	CList<ProtocolResponseGetUserList::userInfo*, ProtocolResponseGetUserList::userInfo*> m_list;
	CCriticalSectionEx mSync;
};

