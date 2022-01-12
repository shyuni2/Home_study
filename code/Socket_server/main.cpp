#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> // 윈속 헤더파일
#include <iostream>

#pragma comment (lib, "ws2_32.lib") // 윈속환경설정 및 초기화

using namespace std;

// 모든 윈속 프로그램에서 소켓을 호출하기전에 반드시 윈속 초기화 함수를 제일 먼저 호출해야함
// 윈속 초기화 함수 : int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
// wVersionRequested 에는 MAKEWORD(2,2) 사용, lpWSAData는  WSADATA 타입의 구조체에 대한 포인터
// 프로그램을 종료할때는 윈속 종료 함수를 호출해야함
// 윈속 종료함수 : int WSACleanup(void)

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // 윈속 초기화 함수가 널이 아닐때
	{
		// 널이면 생성 못하니까 리턴해줌
		return;
	}
	// 1. 소켓 생성, 소켓 만드는건 서버랑 클라랑 똑같음
	SOCKET sock = socket
	(
		AF_INET, // 주소체계 지정, 인터넷 프로토콜
		SOCK_STREAM, // 소켓타입 지정, STREAM이면 TCP, DGRAM쓰면 UDP, 둘중 하나 사용하면됨
		0 // 사용할 프로토콜 지정, 0쓰면 두번째 인자에서 사용하는걸 보고 자동으로 지정해줌
	);
	SOCKADDR_IN sa; // AF_INET인경우 사용하는 구조체
	ZeroMemory(&sa, sizeof(sa)); // 메모리 초기화는 항상 해준다
	sa.sin_family = AF_INET; // 항상 AF_INET사용
	sa.sin_port = htons(10000); // 10000 포트번호로 접속한다, 서버랑 클라랑 일치해야함
	sa.sin_addr.s_addr = htonl(INADDR_ANY); // 서버는 다수의 클라가 접속하기때문에 아이피 지정안하고 내 컴퓨터로 들어오는애는 다 받겠다라는 매크로함수 INADDR_ANY

	// 서버에선 세개의 함수를 써야함 bind, listen, accept
	int Ret = bind(sock, (sockaddr*)&sa, sizeof(sa)); 
	if (Ret == SOCKET_ERROR) return;
	Ret = listen(sock, SOMAXCONN); // 내가 듣는데 접속한사람이 동시다발적이면 SOMAXCONN만큼 들어주겠다 라는 매크로
	if (Ret == SOCKET_ERROR) return;

	SOCKADDR_IN ClientAddr;
	int Len = sizeof(ClientAddr);
	cout << "서버 가동중....." << endl;

	// 서버는 이작업이 반복 되어야 실행하자마자 꺼지는 경우를 없앨수 있음
	while (1)
	{
		SOCKET ClientSock = accept(sock, (sockaddr*)&ClientAddr, &Len);	// 소켓으로 연결해서 소켓으로 대화해야함, accept가 반환타입 : 소켓 accept가 반환되면 누가 접속했는지 알려줌, 
		// accept(클라이언트 접속을 수용할목적으로 만든 소켓, 소켓주소 구조체 변수를 정의, 소켓 구조체의 크기를 나타내는 정수형 변수의 주소)
		if (ClientSock == SOCKET_ERROR)
		{
			int Error = WSAGetLastError();
			cout << "ErrorCode  = " << Error << endl;
			if (Error != WSAEWOULDBLOCK)
			{
				break;
			}
			continue;
		}
		cout << "ip = " << inet_ntoa(ClientAddr.sin_addr) // ip를 스트링으로 변환
			<< "port = " << ntohs(ClientAddr.sin_port) << " " << endl; // 네트워크바이트정렬된애를 호스트정렬바이트로 출력하는거니까 변환

		u_long on = 1;
		ioctlsocket(ClientSock, FIONBIO, &on); // 클라소켓을 넌블로킹으로 만든다
		
		while (1)
		{
			char szRecvBuffer[256] = { 0, };
			int RecvByte = recv(sock, szRecvBuffer, 256, 0);
			if (RecvByte == 0) // RecvByte가 0이면 클라 나간거
			{
				cout << "ip = " << inet_ntoa(ClientAddr.sin_addr)
					<< "port = " << ntohs(ClientAddr.sin_port)
					<< "==> Log Out" << endl;
				break;
			}
			if (RecvByte == SOCKET_ERROR) // RecvByte가 0이면 클라 나간거
			{
				int iError = WSAGetLastError();
				cout << "ErrorCode = " << iError << endl;
				// 넌블로킹으로 반환될떈 작업이없어서 반환된거
				if (iError != WSAEWOULDBLOCK)
				{
					break;
				}
			}
			else
			{
				cout << szRecvBuffer << "받음" << endl;
				int iSendByte = send(ClientSock, szRecvBuffer, RecvByte, 0);
				cout << iSendByte << "보냄." << endl;
			}
		}
		closesocket(ClientSock); // 소켓 닫기
	}
	WSACleanup();
}