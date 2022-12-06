#pragma once

#include "TSingleton.h"
#include <afxtempl.h>

typedef struct WORKSITE_INFO
{
	WORKSITE_INFO() {
		//memset(this, 0x00, sizeof(*this));
		nSequenceNo = -1;
	}
	int nSequenceNo;
	WCHAR szSystemName[64];
	WCHAR szLocation[256];
	WCHAR szEtc[256];
	WCHAR szRegdate[32];
}workSite, *pWorkSite;

class CWorksiteManager : public TSingleton<CWorksiteManager>
{
public:
	CWorksiteManager();
	~CWorksiteManager();

public:
	bool InsertWorksite(pWorkSite site);
	bool DeleteWorksite(int nSeq);
	bool ModifyWorksite(pWorkSite site);

	void ReleaseList();
	int  GetCount();
	pWorkSite GetWorkSite(int nIndex);
	pWorkSite GetWorkSiteBySeq(int nSeq);
	void GetSiteList(CList<pWorkSite, pWorkSite> & list);
	void CopySiteList(CList<pWorkSite, pWorkSite> & list);

private:
	CList<pWorkSite, pWorkSite> m_list;
	CCriticalSectionEx mSync;
};

