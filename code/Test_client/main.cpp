#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <time.h>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")

using namespace std;

HANDLE hExecuteSemaphore; // 세마포어 핸들
HANDLE hConnectEvent; // 이벤트 핸들

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1. 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2. 패킷 전송 : 운영체제 sendbuffer(short바이트), recvbuffer
	char* pMsg = (char*)&packet;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pMsg[SendSize], packet.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < packet.ph.len);
	return SendSize;
}
int SendPacket(SOCKET sock, char* msg, WORD type)
{
	// 1. 패킷 생성
	Packet tPacket(type);
	tPacket << 999 << "이승현" << (short)50 << msg;
	
	Packet tPacketTest(tPacket);
	ChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacketTest >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int SendSize = 0;
	do {
		int SendByte = send(sock, &pData[SendSize], tPacket.m_uPacket.ph.len - SendSize, 0);
		if (SendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		SendSize += SendByte;
	} while (SendSize < tPacket.m_uPacket.ph.len);
	return SendSize;
}
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//패킷헤더 받기
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << " 종료 " << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				cout << " 비정상 접속종료" << endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// 데이터 받기
	int RecvSize = 0;
	do {
		int RecvByte = recv(sock, recvPacket.msg, recvPacket.ph.len - PACKET_HEADER_SIZE - RecvSize, 0);
		RecvSize += RecvByte;
		if (RecvByte == 0)
		{
			closesocket(sock);
			cout << " 퇴장" << endl;
			return -1;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				cout << " 비정상 접속종료됨" << endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (RecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);
	return 1;
}

// 방법 1, win api 사용
// 보내는 스레드
DWORD WINAPI SendThread(LPVOID IpThreadParameter)
{
	SOCKET sock = (SOCKET)IpThreadParameter;
	char szBuffer[256] = { 0, };
	while (1)
	{
		DWORD dwRet = WaitForSingleObject(hConnectEvent, INFINITE); // INFINITE 로 하면 이벤트 무한정 대기
		if (dwRet != WAIT_OBJECT_0) // WAIT_OBJECT_0 : 기다리던 이벤트가 시그널 된 경우
		{
			break;
		}
		ZeroMemory(szBuffer, sizeof(char) * 256);
		fgets(szBuffer, 256, stdin); // 키보드로 입력받아라, 엔터안치면 반환 안치는 함수
		if (strlen(szBuffer) == 0) // 버퍼의 길이가 없으면 종료
		{
			cout << "정상 종료" << endl;
			break;
		}
		int SendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
		if (SendByte < 0)
		{
			cout << "비정상 종료" << endl;
			break;
		}
	}
	return 0; // 리턴할때 종료해주는게 좋음
} // 이함수를 벗어나는 일이 발생하면 쓰레드 종료

// 받는 스레드
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		DWORD dwRet = WaitForSingleObject(hConnectEvent, INFINITE); // INFINITE 로 하면 이벤트 무한정 대기
		if (dwRet != WAIT_OBJECT_0) // WAIT_OBJECT_0 : 기다리던 이벤트가 시그널 된 경우
		{
			break;
		}
		UPACKET packet;
		int ret = RecvPacketHeader(sock, packet);
		if (ret < 0) continue; // 이제는 여기있으면 종료 되니까 continue
		if (ret == 1)
		{
			int ret = RecvPacketData(sock, packet);
			if (ret < 0) break;
			if (ret == 0) continue;
			// 메세지 처리
			Packet data;
			data.m_uPacket = packet;
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			data >> recvdata.index >> recvdata.name >> recvdata.age >> recvdata.message;
			cout << "\n" << "[" << recvdata.name << "]" << recvdata.message;
			time_t t = time(NULL);
			cout << ctime(&t);
		}
	}
	return 0;
}

void main()
{
	// 입장할때 메세지
	//cout << "채팅방에서 사용할 이름을 입력해주세요 : " << endl;
	
	cout << "안녕하세요 채팅방에 입장합니다" << endl;

	cout << "잠시만 기다려주세요" << endl;

	hConnectEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	// CreateEvent (보안속성 구조에대한 포인터, false면 자동리셋이벤트 true면 수동리셋이벤트, 이벤트 객체의 초기상태를 시그널 상태로 하고싶으면 true 논시그널상태로 하고싶으면 false, 이벤트 객체이름)
	// 자동 리셋 이벤트 : 대기상태가 종료되면 자동으로 비신호 상태가 됨
	// 수동 리셋 이벤트 : 스레드가 비신호상태로 만들때까지 신호를 유지함
	hExecuteSemaphore = CreateSemaphore(NULL, 3, 3, L"Execute"); // 세마포어는 카운트가 가능함
	// CreateSemaphore (NULL, 초기값 설정, 최대 카운트값, 생성되는 세마포어에 이름을 줄경우 사용)
	if (WaitForSingleObject(hExecuteSemaphore, 0) == WAIT_TIMEOUT)
	{
		CloseHandle(hExecuteSemaphore);
		MessageBox(NULL, L"이미 3개의 인스턴스가 실행중입니다.", L"알림", MB_OK);
		return;
	}
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// 쓰레드 생성, 위치는 상관없는데 소켓이 있어야 스레드가 생성될수 있기때문에 스레드 밑에 만들자
	// win api방법
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(
		0, // 보안속성, 0은 기본사이즈만 쓰겠다는 의미
		0, // 스택 크기 지정, 0은 기본사이즈만 쓰겠다는 의미
		SendThread, // 반환, 
		(LPVOID)sock, // 소켓을 넘겨줘야 send를 함
		0,// 쓰레드 만들자마자 일시킬래 or 대기시켜놓고 원하는 시점에 일시킬래? 정할수있는 자리, 0을 넣으면 쓰레드를 만들자마자 SendTread를 실행
		&ThreadIdSend // 생성된 쓰레드에대한 인덱스를 반환해준다 , 종업원의 번호라고 생각
	);
	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0, // 0은 기본사이즈만 쓰겠다는 의미
		RecvThread, // 반환, 
		(LPVOID)sock, // 소켓을 넘겨줘야 recv를 함
		0,// 쓰레드 만들자마자 일시킬래 or 대기시켜놓고 원하는 시점에 일시킬래? 정할수있는 자리, 0을 넣으면 쓰레드를 만들자마자 RecvTread를 실행
		&ThreadIdRecv // 생성된 쓰레드에대한 인덱스를 반환해준다 , 종업원의 번호라고 생각
	);

	Sleep(1000); // 스위칭이 되게 대기함수

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1);
	sa.sin_addr.s_addr = inet_addr("192.168.0.87");
	int ret = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	cout << "입장 성공!" << endl;

	SetEvent(hConnectEvent); // 이벤트 세팅

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on); // 여기까진 동일함

	// 메인스레드 작업
	while (1)
	{
		// 나중에 관리 코드가 생길수있음
		Sleep(1);
	}
	cout << "접속종료" << endl;

	CloseHandle(hThreadSend);
	closesocket(sock);
	WSACleanup();

	CloseHandle(hConnectEvent); // 이벤트 종료
}