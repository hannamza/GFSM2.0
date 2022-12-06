#include "stdafx.h"
#include "ManagerManager.h"

CManagerManager::CManagerManager()
{
}

CManagerManager::~CManagerManager()
{
	ReleaseList();
}

bool CManagerManager::InsertManager(ProtocolResponseGetManagerList::managerInfo* pManager)
{
	mSync.Enter();
	ProtocolResponseGetManagerList::managerInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pManager->nSeq)
		{
			mSync.Leave();
			return false;
		}
	}
	pInfo = new ProtocolResponseGetManagerList::managerInfo;
	memcpy_s(pInfo, sizeof(ProtocolResponseGetManagerList::managerInfo), pManager, sizeof(ProtocolResponseGetManagerList::managerInfo));

	m_list.AddTail(pInfo);
	mSync.Leave();
	return true;
}

bool CManagerManager::DeleteManager(int nSeq)
{
	mSync.Enter();
	ProtocolResponseGetManagerList::managerInfo* pInfo = NULL;
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

bool CManagerManager::ModifyManager(ProtocolResponseGetManagerList::managerInfo* pManager)
{
	mSync.Enter();
	ProtocolResponseGetManagerList::managerInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pManager->nSeq)
		{
			memcpy_s(pInfo, sizeof(ProtocolResponseGetManagerList::managerInfo), pManager, sizeof(ProtocolResponseGetManagerList::managerInfo));
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

int CManagerManager::GetCount()
{
	mSync.Enter();
	int nCount = m_list.GetCount();
	mSync.Leave();
	return nCount;
}

ProtocolResponseGetManagerList::managerInfo* CManagerManager::GetManagerInfo(int nIndex)
{
	mSync.Enter();
	if (m_list.GetCount() <= nIndex) {
		mSync.Leave();
		return NULL;
	}
	ProtocolResponseGetManagerList::managerInfo* pInfo = m_list.GetAt(m_list.FindIndex(nIndex));
	mSync.Leave();

	return pInfo;
}

ProtocolResponseGetManagerList::managerInfo* CManagerManager::GetManagerInfoBySeq(int nSeq)
{
	mSync.Enter();
	ProtocolResponseGetManagerList::managerInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == nSeq)
		{
			mSync.Leave();
			return pInfo;
		}
	}
	mSync.Leave();
	return NULL;
}

void CManagerManager::ReleaseList()
{
	mSync.Enter();
	ProtocolResponseGetManagerList::managerInfo* pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_list.RemoveAll();
	mSync.Leave();
}
