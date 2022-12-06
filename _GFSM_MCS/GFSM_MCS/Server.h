#pragma once

#include <winsock2.h>
#include <vector>

#include "TSingleton.h"

class Client;
class Packet;
class IOEvent;

class Server :  public TSingleton<Server>
{
private:
	// Callback Routine
	static void CALLBACK IoCompletionCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PVOID Overlapped, ULONG IoResult, ULONG_PTR NumberOfBytesTransferred, PTP_IO Io);

	// Worker Thread Functions
	static void CALLBACK WorkerPostAccept(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context, PTP_WORK /* Work */);

	static void CALLBACK WorkerAddClient(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context);
	static void CALLBACK WorkerRemoveClient(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context);
	static void CALLBACK WorkerProcessRecvPacket(PTP_CALLBACK_INSTANCE /* Instance */, PVOID Context);

public:
	Server();
	virtual ~Server();

	bool Create(short port, int maxPostAccept);
	void Destroy();

	size_t GetNumClients();
	long GetNumPostAccepts();

private:
	void PostAccept();
	void PostRecv(Client* client);
	void PostSend(Client* client, Packet* packet);

	void OnAccept(IOEvent* event);
	void OnRecv(IOEvent* event, DWORD dwNumberOfBytesTransfered);
	void OnSend(IOEvent* event, DWORD dwNumberOfBytesTransfered);
	void OnClose(IOEvent* event);

	void AddClient(Client* client);
	void RemoveClient(Client* client);

	void Echo(Packet* packet);
	void Send(BYTE* pData, int nSize);

protected:
	static void ProcessProtocolRequestLogin(BYTE* pData);	// login(manager)
	static void ProcessProtocolRequestSetMasterKey(BYTE* pData);	// 마스터키, 토큰, 전화번호 전송
	static void ProcessProtocolRequestGetEventList(BYTE* pData);	// 일련번호 이후의 이벤트 리스트 요청(마스터키 포함)
	static void ProcessProtocolRequestGetRecentEventList(BYTE* pData);	// 가장 최근의 이벤트 리스트 요청(페이징 포함)
	static void ProcessProtocolRequestAddNewSystem(BYTE* pData);	// 건물, 단지 등 시스템 추가(마스터 키 생성)
	static void ProcessProtocolRequestModSystem(BYTE* pData);		// 수정
	static void ProcessProtocolRequestDelSystem(BYTE* pData);		// 삭제
	static void ProcessProtocolRequestGetSystemList(BYTE* pData);	// 시스템 목록 요청
	static void ProcessProtocolRequestGetUserList(BYTE* pData);	// 해당 시스템에 등록된 유저 목록 요청
	static void ProcessProtocolRequestDelUser(BYTE* pData);		// 유저 삭제

private:
	Server& operator=(Server& rhs);
	Server(const Server& rhs);

private:
	TP_IO* m_pTPIO;
	SOCKET m_listenSocket;

	TP_WORK* m_AcceptTPWORK; 

	typedef std::vector<Client*> ClientList;
	ClientList m_Clients;

	int	m_MaxPostAccept;
	volatile long m_NumPostAccept;

	CRITICAL_SECTION m_CSForClients;

	TP_CALLBACK_ENVIRON m_ClientTPENV;
	TP_CLEANUP_GROUP* m_ClientTPCLEAN;

	volatile bool m_ShuttingDown;
};
