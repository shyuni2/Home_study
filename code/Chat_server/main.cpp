#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h> // 윈속 헤더파일
#include <list>
#pragma comment	(lib, "ws2_32.lib")

using namespace std;

// 모든 윈속 프로그램에서 소켓을 호출하기전에 반드시 윈속 초기화 함수를 제일 먼저 호출해야함
// 윈속 초기화 함수 : int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
// wVersionRequested 에는 MAKEWORD(2,2) 사용, lpWSAData는  WSADATA 타입의 구조체에 대한 포인터
// 프로그램을 종료할때는 윈속 종료 함수를 호출해야함
// 윈속 종료함수 : int WSACleanup(void)

struct User // 유저 설정
{
	SOCKET user_sock;
	SOCKADDR_IN user_addr;
	string user_name;
	short user_port;
	void set(SOCKET Usock, SOCKADDR_IN Uaddr)
	{
		user_sock = Usock;
		user_addr = Uaddr;
		user_name = inet_ntoa(Uaddr.sin_addr);
		user_port = ntohs(Uaddr.sin_port);
	}
};

void main()
{
	WSADATA WSA; // WSADAD 구조체 이름을 WSA로 설정
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0) // 윈속초기화함수 WSAStartup가 0이 아닐때
	{
		return;
	}
	// 서버의 흐름
	// 1. listen()(클라가 접속하는걸 들어야하기때문) 이라는 소켓을 생성
	// 2.	-> 바인딩 bind 작업(클라에선 없음) : 소켓에 전화번호를 연결한다(클라에선 입력만하면됨)
	// 3.	-> 전화올거를 기다리는상태
	// 4.	-> 전화오면 받음 / accept() 상태가됨 서버와 클라이언트가 연결됨 어떤사람이 전화했는지 연결했는지 알수있음
	// 5.	-> recv, send작업하면됨

	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0); // socket(소켓의 주소체계, 소켓타입 , 사용할 프로토콜)
														 // AF_INET : 인터넷 프로토콜 사용, SOCK_STREAM : TCP사용, 두번째 인자에서 사용하는걸 보고 자동으로 타입 정해주니 헷갈리지 않게 0으로 넣으면 됨
	SOCKADDR_IN SAddr; // 사용할 프로토콜의 종류에 맞는 소켓 구조체 사용
					   // 함수 인자로 사용할때는 항상 주소값을 전달하며 반드시 포인터형으로 반환해서 사용해야한다
	ZeroMemory(&SAddr, sizeof(SAddr)); // 일단 메모리초기화를 해줌
	SAddr.sin_family = AF_INET; // 주소체계를 나타내며 항상 AF_INET을 사용한다
	SAddr.sin_port = htons(10000); // 10000번으로 접속, 서버랑 포트번호가 일치해야함, 10000번으로 직접넣지않고 htons함수를 쓴 이유 : 호스트바이트를 네트워크바이트 short로 바꿔줘야함, 바이트 정렬함수
	SAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 아이피주소를 나타냄, 서버는 다수의 클라가 접속해야하기때문에 아이피 지정안하고, INADDR_ANY 내컴퓨터로 들어오는애 다받겠다 라는 매크로함수

	// 바인드 작업
	int ret = bind(ListenSock, (sockaddr*)&SAddr, sizeof(SAddr)); // bind(클라이언트 접속을 수용할 목적으로 만든 소켓, 소켓주소 구조체, 소켓주소 구조체 길이
																 // 성공 = 0, 실패 = SOCKET_ERROR
	if (ret == SOCKET_ERROR) return; // 바인드함수 리턴값이 에러면 리턴
	// 리슨 작업
	ret = listen(ListenSock, SOMAXCONN); // listen(bind함수에 의해 아이피주소와 지역포트번호가 설정된 소켓, 접속가능한 클라이언트 개수)
										  // SOMAXCONN : 지원가능한 최대값 사용
	if (ret == SOCKET_ERROR) return; // 리슨함수 리턴값이 에러면 리턴

	SOCKADDR_IN ClientAddr; // 위에서도 썼지만 함수인자로 사용할때는 항상 주소값을 전달하며 반드시 포인터형으로 반환해서 사용해야함
	int AddrC = sizeof(ClientAddr); // 클라이언트주소를 담는 변수

	cout << "서버 가동중 >" << endl; // 서버가 연결됐을때 출력하는 메세지

	// 논블로킹 소켓 만들기
	u_long on = 1; // u_long : 바이트 정렬함수
	ioctlsocket(ListenSock, FIONBIO, &on); // ioctlsocket(논블로킹으로 만들 소켓, 소켓에대한 수행할 동작을 의미하는 커맨드, 0이면 블로킹모드가 되고 0이아닌값을 넣으면 논블로킹모드로 설정)

	// 바이트 정렬함수란? 바이트 정렬방식이 틀릴경우 잘못된 데이터가 전달될 가능성이 있다 
	// 이러한 모호함을 없애기 위해서 아이피주소와 포트번호의 바이트 정렬 방식은 빅엔디안으로 통일되어있다. 
	// 그런데, 인텔 프로세스에서는 바이트 정렬을 리틀엔디안 방식으로 하고있어서 아이피주소와 포트번호를 정렬하는 방식과 맞지 않다.
	// 따라서 빅엔디안을 네트워크 바이트 정렬이라고하고 시스템이 사용하는 정렬 방식을 호스트 바이트 정렬이라 한다.
	// 바이트 정렬 함수를 보면 htonl() 이라고 되어있는데 h는 호스트 n은 네트워크 l,s는 long형, short형 이다 상황에따라서 맞게 사용하면 됨

	// 블로킹, 논블로킹이란 ? 
	// 블로킹은 어떤 시스템을 콜했을때 네트워크 시스템이 동작완료할때까지 멈춰있음
	// 논블로킹은 시스템콜에 대하여 네트워크 시스템이 즉시처리할수없는 경우라도 시스템콜이 리턴되어 블록되지않게 함

	list<User> userlist; // 유저리스트를 알기위해 사용, 배열을사용하면 빠졌을때 상황을 고려해야하기 때문에 list사용함

	//서버는 이작업이 반복되어야 실행하자마자 꺼지는 경우를 없앨수있음
	while (1)
	{
		SOCKET ClientSock = accept(ListenSock, (sockaddr*)&ClientAddr, &AddrC); // accept(클라이언트 접속을 수용할목적으로 만든 소켓, 소켓주소 구조체 변수를 정의, 소켓 구조체의 크기를 나타내는 정수형 변수의 주소)

		// 클라이언트 소켓 오류관련 코드
		if (ClientSock == SOCKET_ERROR) // 클라이언트 소켓이 에러이면
		{
			int Error = WSAGetLastError(); // 소켓 오류코드
			if (Error != WSAEWOULDBLOCK) // 
			{
				cout << "Error Code = " << Error << endl; // 오류코드 출력, 현재 error가 int로 되어있음 char로 해서 문자로 받을수있을지 다 빌드하고 고민해보자
				break;
			}
		}
		else // 클라이언트 소켓이 에러가 아니면
		{
			User user;
			user.set(ClientSock, ClientAddr);
			userlist.push_back(user); // 유저리스트 뒤에 유저 푸쉬백
			cout << user.user_name << ", ip = " << inet_ntoa(ClientAddr.sin_addr) << "port = " << ntohs(ClientAddr.sin_port) << " ==> 접속성공" << endl; // inet_ntoa : 네트워크 바이트 정렬 방식의 4바이트 주소를 문자열주소로 표현
			// 논블로킹소켓 만들기
			u_long on = 1;
			ioctlsocket(ClientSock, FIONBIO, &on);
			cout << userlist.size() << " 명 접속중 >" << endl;
		}

		// 유저리스트 관리 코드
		if (userlist.size() > 0) // 유저가 한명이라도 있을때
		{
			list <User> ::iterator Sockiter; // 소켓반복자생성
			for (Sockiter = userlist.begin(); Sockiter != userlist.end(); ) // for문에서 Sockiter가 ++되면 나가고 지울때 고려해야함, erase의 반환값 중요
			{
				User user = *Sockiter; // 소켓반복자 포인터 설정
				char szRecvBuffer[256] = { 0, };
				int RecvByte = recv(user.user_sock, szRecvBuffer, 256, 0); // recv : 데이터 주고받는 함수
																	 // recv(통신대상과 연결된 소켓, 받은 데이터를 저장할 버퍼(char형이어야함), 데이터버퍼의 크기, 특별한 옵션을 주기위함 - 안쓰면 0)
																	 // 성공 : 받은 바이트수 또는 0, 실패 : SOCKET_ERROR
				if (RecvByte == 0) // RecvByte가 0이면 클라 나간거
				{
					closesocket(user.user_sock);
					Sockiter = userlist.erase(Sockiter);
					cout << user.user_name << ", ip = " << inet_ntoa(ClientAddr.sin_addr) << "port = " << ntohs(ClientAddr.sin_port) << " ==> 접속종료" << endl;
					continue; // 계속 진행 되어야 하니까 컨티뉴
				}
				if (RecvByte == SOCKET_ERROR) // 리시브함수가 에러일때
				{
					int Error = WSAGetLastError();
					if (Error != WSAEWOULDBLOCK) // 에러가 WSAEWOULDBLOCK이 아니면
					{
						Sockiter = userlist.erase(Sockiter);
						cout << user.user_name << ", ip = " << inet_ntoa(ClientAddr.sin_addr) << "port = " << ntohs(ClientAddr.sin_port) << " ==> 비정상 접속종료" << endl;
					}
					else // 에러가 WSAEWOULDBLOCK이면
					{
						Sockiter++;
					}
				}
				else // 리시브함수가 에러가 아닐때
				{
					list<User> ::iterator Senditer; // 샌드반복자 생성
					for (Senditer = userlist.begin(); Senditer != userlist.end(); ) // 샌드반복자가 유저리스트의 처음에서 끝까지갈때 
					{
						User user = *Senditer; // 샌드반복자 포인터 설정
						cout << szRecvBuffer << "받음" << endl; // 사용자가 입력한 버퍼 출력 함수
						int SendByte = send(user.user_sock, szRecvBuffer, RecvByte, 0); // send : 데이터를 받는 함수
																				  // send(통신할 대상과 연결된 소켓, 보낼데이터를 담고있는 버퍼, 보낼 데이터 크기, 특별한 옵션을 주기위함 - 안쓰면 0)
																				  // 성공 : 받은 바이트수 또는 0, 실패 : SOCKET_ERROR
						cout << user.user_sock << ":" << SendByte << "보냄" << endl;

						if (SendByte == SOCKET_ERROR) // 리시브함수가 에러일때
						{
							int Error = WSAGetLastError();
							cout << "Error Code = " << Error << endl; // 에러코드 출력하고
							if (Error != WSAEWOULDBLOCK) // 에러가 WSAEWOULDBLOCK이 아니면
							{
								closesocket(user.user_sock);
								Senditer = userlist.erase(Senditer);
								cout << user.user_name << ", ip = " << inet_ntoa(ClientAddr.sin_addr) << "port = " << ntohs(ClientAddr.sin_port) << " ==> 비정상 접속종료" << endl;
							}
							else // 에러가 WSAEWOULDBLOCK이면
							{
								Senditer++;
							}
						}
						if (Sockiter != userlist.end()) // 유저리스트 마지막 뒤에는 없으니까
						{
							Sockiter++;
						}

					}
				}
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup(); // 윈속 종료함수
}
