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

		RequestSetMasterKey,	// 마스터키, 토큰, 전화번호 전송
		ResponseSetMasterKey,

		RequestGetEventList,	// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
		ResponseGetEventList, 

		RequestGetRecentEventList, // 가장 최근의 이벤트 리스트 요청(페이징 포함)
		ResponseGetRecentEventList, 

		// manager protocol ----------------------------------------

		RequestAddNewSystem,	// 건물, 단지 등 시스템 추가(마스터 키 생성)
		ResponseAddNewSystem, 

		RequestModSystem,		// 수정
		ResponseModSystem, 

		RequestDelSystem,		// 삭제
		ResponseDelSystem, 

		RequestGetSystemList,	// 시스템 목록 요청
		ResponseGetSystemList, 

		RequestGetUserList,		// 해당 시스템에 등록된 유저 목록 요청
		ResponseGetUserList, 

		RequestDelUser,			// 유저 삭제
		ResponseDelUser, 

		// sender protocol --------------------------------------------

		RequestAddEvent,		// 이벤트 발생
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

// 마스터키, 토큰, 전화번호 전송
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
// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
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
// 가장 최근의 이벤트 리스트 요청(페이징 포함)
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
// 건물, 단지 등 시스템 추가(마스터 키 생성)
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
// 수정
// response - ResponseModSystem(1: success, 0: fail, 2: 이미 폰이 등록되어 있음)
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
// 삭제
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
// 시스템 목록 요청
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
// 해당 시스템에 등록된 유저 목록 요청
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
// 유저 삭제
//response - ResponseDelUser (1: success, 0: fail)
//--------------------------------------------------------------------------------------
// 이벤트 발생
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