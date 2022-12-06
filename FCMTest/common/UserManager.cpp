#include "stdafx.h"
#include "UserManager.h"

CUserManager::CUserManager()
{
}

CUserManager::~CUserManager()
{
	ReleaseList();
}

bool CUserManager::InsertUser(ProtocolResponseGetUserList::userInfo* pUser)
{
	mSync.Enter();
	ProtocolResponseGetUserList::userInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pUser->nSeq)
		{
			mSync.Leave();
			return false;
		}
	}
	pInfo = new ProtocolResponseGetUserList::userInfo;
	memcpy_s(pInfo, sizeof(ProtocolResponseGetUserList::userInfo), pUser, sizeof(ProtocolResponseGetUserList::userInfo));

	m_list.AddTail(pInfo);
	mSync.Leave();
	return true;
}

bool CUserManager::DeleteUser(int nSeq)
{
	mSync.Enter();
	ProtocolResponseGetUserList::userInfo* pInfo = NULL;
	POSITION oldPos;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		oldPos = pos;
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == nSeq)
		{
			m_list.RemoveAt(oldPos);
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

bool CUserManager::ModifyUser(ProtocolResponseGetUserList::userInfo* pUser)
{
	mSync.Enter();
	ProtocolResponseGetUserList::userInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pUser->nSeq)
		{
			memcpy_s(pInfo, sizeof(ProtocolResponseGetUserList::userInfo), pUser, sizeof(ProtocolResponseGetUserList::userInfo));
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

int CUserManager::GetCount()
{
	mSync.Enter();
	int nCount = m_list.GetCount();
	mSync.Leave();
	return nCount;
}

ProtocolResponseGetUserList::userInfo* CUserManager::GetUserInfo(int nIndex)
{
	mSync.Enter();
	if (m_list.GetCount() <= nIndex) {
		mSync.Leave();
		return NULL;
	}
	ProtocolResponseGetUserList::userInfo* pInfo = m_list.GetAt(m_list.FindIndex(nIndex));
	mSync.Leave();

	return pInfo;
}

void CUserManager::ReleaseList()
{
	mSync.Enter();
	ProtocolResponseGetUserList::userInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_list.RemoveAll();
	mSync.Leave();
}
