#include "stdafx.h"
#include "WorksiteManager.h"

CWorksiteManager::CWorksiteManager()
{
}

CWorksiteManager::~CWorksiteManager()
{
	ReleaseList();
}

bool CWorksiteManager::InsertWorksite(pWorkSite site)
{
	mSync.Enter();
	pWorkSite pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSequenceNo == site->nSequenceNo)
		{
			mSync.Leave();
			return false;
		}
	}
	pInfo = new workSite;
	memcpy_s(pInfo, sizeof(workSite), site, sizeof(workSite));

	m_list.AddTail(pInfo);
	mSync.Leave();
	return true;
}

bool CWorksiteManager::DeleteWorksite(int nSeq)
{
	mSync.Enter();
	pWorkSite pInfo = NULL;
	POSITION oldPos;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		oldPos = pos;
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSequenceNo == nSeq)
		{
			m_list.RemoveAt(oldPos);
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

bool CWorksiteManager::ModifyWorksite(pWorkSite site)
{
	mSync.Enter();
	pWorkSite pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSequenceNo == site->nSequenceNo)
		{
			memcpy_s(pInfo, sizeof(workSite), site, sizeof(workSite));
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

int CWorksiteManager::GetCount()
{
	mSync.Enter();
	int nCount = m_list.GetCount();
	mSync.Leave();
	return nCount;
}

pWorkSite CWorksiteManager::GetWorkSite(int nIndex)
{
	mSync.Enter();
	if (m_list.GetCount() <= nIndex) {
		mSync.Leave();
		return NULL;
	}
	pWorkSite pInfo = m_list.GetAt(m_list.FindIndex(nIndex));
	mSync.Leave();

	return pInfo;
}

pWorkSite CWorksiteManager::GetWorkSiteBySeq(int nSeq)
{
	mSync.Enter();
	for (int nIndex = 0; nIndex < m_list.GetCount(); nIndex++)
	{
		pWorkSite pInfo = m_list.GetAt(m_list.FindIndex(nIndex));
		if (pInfo->nSequenceNo == nSeq) {
			mSync.Leave();
			return pInfo;
		}
	}
	mSync.Leave();

	return NULL;
}

void CWorksiteManager::ReleaseList()
{
	mSync.Enter();
	pWorkSite pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_list.RemoveAll();
	mSync.Leave();
}

void CWorksiteManager::GetSiteList(CList<pWorkSite, pWorkSite> & list)
{
	mSync.Enter();
	pWorkSite pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo) {
			list.AddTail(pInfo);
		}
	}
	mSync.Leave();
}

void CWorksiteManager::CopySiteList(CList<pWorkSite, pWorkSite> & list)
{
	mSync.Enter();
	pWorkSite pInfo = NULL, pTemp = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo) {
			pTemp = new workSite;
			memcpy_s(pTemp, sizeof(workSite), pInfo, sizeof(workSite));
			list.AddTail(pTemp);
		}
	}
	mSync.Leave();
}