#pragma once

#pragma pack(push, 1)

class ProtocolHeader
{
public:
	ProtocolHeader() {protocol=0;size=0;};
	struct 
	{
		unsigned protocol : 16;
		unsigned size : 16;
	};

	enum ProtocolCommand
	{
		Keep_Alive = 1,

		RequestLogin,			// login(manager)
		ResponseLogin, 

		// mobile protocol ----------------------------------------

		RequestSetMasterKey,	// ������Ű, ��ū, ��ȭ��ȣ ����
		ResponseSetMasterKey,

		RequestGetEventList,	// �Ϸù�ȣ ������ �̺�Ʈ ����Ʈ ��û(������Ű ����)
		ResponseGetEventList, 

		RequestGetRecentEventList, // ���� �ֱ��� �̺�Ʈ ����Ʈ ��û(����¡ ����)
		ResponseGetRecentEventList, 

		// manager protocol ----------------------------------------

		RequestAddNewSystem,	// �ǹ�, ���� �� �ý��� �߰�(������ Ű ����)
		ResponseAddNewSystem, 

		RequestModSystem,		// ����
		ResponseModSystem, 

		RequestDelSystem,		// ����
		ResponseDelSystem, 

		RequestGetSystemList,	// �ý��� ��� ��û
		ResponseGetSystemList, 

		RequestGetUserList,		// �ش� �ý��ۿ� ��ϵ� ���� ��� ��û
		ResponseGetUserList, 

		RequestDelUser,			// ���� ����
		ResponseDelUser, 

		// sender protocol --------------------------------------------

		RequestAddEvent,		// �̺�Ʈ �߻�
		ResponseAddEvent,

		DefineEndProtocol
	};
};

//------------- COMMON ---------------------------------------------------

// common request
class ProtocolRequest : public ProtocolHeader
{
public:
	ProtocolRequest() 
	{
		memset(this, 0, sizeof(*this));
		protocol = 0;
		size   = sizeof(*this)-sizeof(ProtocolHeader);
	}
	int nValue;
};

// common response
class ProtocolResponse : public ProtocolHeader
{
public:
	ProtocolResponse() 
	{
		memset(this, 0, sizeof(*this));
		protocol = 0;
		size   = sizeof(*this)-sizeof(ProtocolHeader);
	}
	int nResult;
};

//------------------------------------------------------------------------

// login(manager) - ResponseLogin(1: success, 0: fail)
class ProtocolRequestLogin : public ProtocolHeader
{
public:
	ProtocolRequestLogin()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestLogin;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	WCHAR szID[32];
	WCHAR szPW[32];
};

// mobile protocol ----------------------------------------

// ������Ű, ��ū, ��ȭ��ȣ ����
// response - ResponseSetMasterKey(1: success, 0: fail)
class ProtocolRequestSetMasterKey : public ProtocolHeader
{
public:
	ProtocolRequestSetMasterKey()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestSetMasterKey;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	WCHAR szMasterKey[32];
	WCHAR szToken[256];
	WCHAR szPhoneNo[16];
};

//--------------------------------------------------------------------------------------
// �Ϸù�ȣ ������ �̺�Ʈ ����Ʈ ��û(������Ű ����)
class ProtocolRequestGetEventList : public ProtocolHeader
{
public:
	ProtocolRequestGetEventList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseGetEventList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	int nSequenceNo;
	WCHAR szMasterKey[32];
};
class ProtocolResponseGetEventList : public ProtocolHeader
{
public:
	ProtocolResponseGetEventList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseGetEventList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	struct eventInfo
	{
		int nSequenceNo;
		WCHAR szEvent[32];
	};
	int nCount;
	eventInfo info[0];
};
//--------------------------------------------------------------------------------------
// request - RequestGetRecentEventList (nValue: nSequenceNo)
// ���� �ֱ��� �̺�Ʈ ����Ʈ ��û(����¡ ����)
class ProtocolResponseGetRecentEventList : public ProtocolHeader
{
public:
	ProtocolResponseGetRecentEventList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseGetRecentEventList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	struct eventInfo
	{
		int nSequenceNo;
		WCHAR szEvent[32];
	};
	int nCount;
	eventInfo info[0];
};
//--------------------------------------------------------------------------------------
// �ǹ�, ���� �� �ý��� �߰�(������ Ű ����)
// response - ResponseAddNewSystem (1: success, 0: fail)
class ProtocolRequestAddNewSystem : public ProtocolHeader
{
public:
	ProtocolRequestAddNewSystem()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestAddNewSystem;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	WCHAR szSystemName[32];
	WCHAR szMasterKey[32];
};
//--------------------------------------------------------------------------------------
// ����
// response - ResponseModSystem(1: success, 0: fail, 2: �̹� ���� ��ϵǾ� ����)
class ProtocolRequestModSystem : public ProtocolHeader
{
public:
	ProtocolRequestModSystem()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestModSystem;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	int nSequenceNo;
	WCHAR szSystemName[32];
	WCHAR szMasterKey[32];
};
//--------------------------------------------------------------------------------------
// ����
// response - ResponseDelSystem(1: success, 0: fail)
class ProtocolRequestDelSystem : public ProtocolHeader
{
public:
	ProtocolRequestDelSystem()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestDelSystem;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	int nSequenceNo;
};
//--------------------------------------------------------------------------------------
// request - RequestGetSystemList
// �ý��� ��� ��û
class ProtocolResponseGetSystemList : public ProtocolHeader
{
public:
	ProtocolResponseGetSystemList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseGetSystemList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	struct systemInfo
	{
		int nSequenceNo;
		WCHAR szSystemName[32];
		WCHAR szMasterKey[32];
	};
	int nCount;
	systemInfo info[0];
};
//,
//--------------------------------------------------------------------------------------
//request - RequestGetUserList (nSequenceNo)
// �ش� �ý��ۿ� ��ϵ� ���� ��� ��û
class ProtocolResponseGetUserList : public ProtocolHeader
{
public:
	ProtocolResponseGetUserList()
	{
		memset(this, 0, sizeof(*this));
		protocol = ResponseGetUserList;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	struct userInfo
	{
		int nSequenceNo;
		WCHAR szMasterKey[32];
		WCHAR szToken[256];
		WCHAR szPhoneNo[16];
	};
	int nCount;
	userInfo info[0];
};
//--------------------------------------------------------------------------------------
//request - RequestDelUser (nSequenceNo)
// ���� ����
//response - ResponseDelUser (1: success, 0: fail)
//--------------------------------------------------------------------------------------
// �̺�Ʈ �߻�
class ProtocolRequestAddEvent : public ProtocolHeader
{
public:
	ProtocolRequestAddEvent()
	{
		memset(this, 0, sizeof(*this));
		protocol = RequestAddEvent;
		size = sizeof(*this) - sizeof(ProtocolHeader);
	}
	WCHAR szMasterKey[32];
	WCHAR szEvent[32];
};
//response - ResponseAddEvent (nSequenceNo: success, 0: fail)

#pragma pack(pop, 1)