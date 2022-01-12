#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#pragma comment	(lib, "ws2_32.lib")

using namespace std;

void main()
{
	WSADATA wsa; // WSADAD 구조체 이름을 wsa로 설정
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // 윈속초기화함수 WSAStartup가 0이 아닐때
	{
		return;
	}
	// 클라이언트의 흐름
	// 1. 소켓생성
	// 2.	-> 서버로 연결시도 connect 
	// 3.	-> recv, send 이용해 소켓으로 데이터 송수신

	SOCKET ConnectSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000); // 포트번호 10000
	sa.sin_addr.s_addr = inet_addr("192.168.0.117"); // 서버 아이피로 설정, 내 아이피 192.168.0.117, inet_ntoa : 네트워크 바이트 정렬 방식의 4바이트 주소를 문자열주소로 표현

	int Ret = connect(ConnectSock, (sockaddr*)&sa, sizeof(sa)); // 커넥트함수의 리턴 설정
	if (Ret == SOCKET_ERROR) // 커넥트가 오류나면 리턴
	{
		return;
	}
	// 커넥트가 오류가 아니면 서버와 연결성공
	cout << "접속 성공 >" << endl;

	u_long on = 1;
	ioctlsocket(ConnectSock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int End = 0;

	while (1)
	{
		if (_kbhit() == 1) // _kbhit : 키보드가 눌렸는지 체크해주는 함수
		{
			int KValue = _getche(); // _getche : 키보드 입력받고 출력해주는 함수
			if (KValue == '\r' && strlen(szBuffer) == 0)
			{
				break;
			}
			if (KValue == '\r') // 엔터 입력을 받으면
			{
				int SendByte = send(ConnectSock, szBuffer, End, 0); // send(통신할 대상과 연결된 소켓, 보낼데이터를 담고있는 버퍼, 보낼 데이터 크기, 특별한 옵션을 주기위함 - 안쓰면 0)
				if (SendByte == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						cout << "비정상 서버 종료" << endl;
					}
				}
				End = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256); // 메모리 초기화
			}
			else // 엔터 입력 받지않으면
			{
				szBuffer[End++] = KValue;
			}
		}
		else
		{
			char szRecvBuffer[256] = { 0, };
			int RecvByte = recv(ConnectSock, szBuffer, 256, 0);

			if (RecvByte == 0)
			{
				cout << "서버종료" << endl;
				break;
			}
			if (RecvByte == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					cout << "비정상 서버종료" << endl;
					break;
				}
			}
			else
			{
				cout << "\n" << szRecvBuffer;
				ZeroMemory(szRecvBuffer, sizeof(char) * 256);
			}
		}
	}
	cout << "접속종료" << endl;
	closesocket(ConnectSock);
	WSACleanup();
}