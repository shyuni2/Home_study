#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> // ���� �������
#include <iostream>

#pragma comment (lib, "ws2_32.lib") // ����ȯ�漳�� �� �ʱ�ȭ

using namespace std;

// ��� ���� ���α׷����� ������ ȣ���ϱ����� �ݵ�� ���� �ʱ�ȭ �Լ��� ���� ���� ȣ���ؾ���
// ���� �ʱ�ȭ �Լ� : int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
// wVersionRequested ���� MAKEWORD(2,2) ���, lpWSAData��  WSADATA Ÿ���� ����ü�� ���� ������
// ���α׷��� �����Ҷ��� ���� ���� �Լ��� ȣ���ؾ���
// ���� �����Լ� : int WSACleanup(void)

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // ���� �ʱ�ȭ �Լ��� ���� �ƴҶ�
	{
		// ���̸� ���� ���ϴϱ� ��������
		return;
	}
	// 1. ���� ����, ���� ����°� ������ Ŭ��� �Ȱ���
	SOCKET sock = socket
	(
		AF_INET, // �ּ�ü�� ����, ���ͳ� ��������
		SOCK_STREAM, // ����Ÿ�� ����, STREAM�̸� TCP, DGRAM���� UDP, ���� �ϳ� ����ϸ��
		0 // ����� �������� ����, 0���� �ι�° ���ڿ��� ����ϴ°� ���� �ڵ����� ��������
	);
	SOCKADDR_IN sa; // AF_INET�ΰ�� ����ϴ� ����ü
	ZeroMemory(&sa, sizeof(sa)); // �޸� �ʱ�ȭ�� �׻� ���ش�
	sa.sin_family = AF_INET; // �׻� AF_INET���
	sa.sin_port = htons(10000); // 10000 ��Ʈ��ȣ�� �����Ѵ�, ������ Ŭ��� ��ġ�ؾ���
	sa.sin_addr.s_addr = htonl(INADDR_ANY); // ������ �ټ��� Ŭ�� �����ϱ⶧���� ������ �������ϰ� �� ��ǻ�ͷ� �����¾ִ� �� �ްڴٶ�� ��ũ���Լ� INADDR_ANY

	// �������� ������ �Լ��� ����� bind, listen, accept
	int Ret = bind(sock, (sockaddr*)&sa, sizeof(sa)); 
	if (Ret == SOCKET_ERROR) return;
	Ret = listen(sock, SOMAXCONN); // ���� ��µ� �����ѻ���� ���ôٹ����̸� SOMAXCONN��ŭ ����ְڴ� ��� ��ũ��
	if (Ret == SOCKET_ERROR) return;

	SOCKADDR_IN ClientAddr;
	int Len = sizeof(ClientAddr);
	cout << "���� ������....." << endl;

	// ������ ���۾��� �ݺ� �Ǿ�� �������ڸ��� ������ ��츦 ���ټ� ����
	while (1)
	{
		SOCKET ClientSock = accept(sock, (sockaddr*)&ClientAddr, &Len);	// �������� �����ؼ� �������� ��ȭ�ؾ���, accept�� ��ȯŸ�� : ���� accept�� ��ȯ�Ǹ� ���� �����ߴ��� �˷���, 
		// accept(Ŭ���̾�Ʈ ������ �����Ҹ������� ���� ����, �����ּ� ����ü ������ ����, ���� ����ü�� ũ�⸦ ��Ÿ���� ������ ������ �ּ�)
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
		cout << "ip = " << inet_ntoa(ClientAddr.sin_addr) // ip�� ��Ʈ������ ��ȯ
			<< "port = " << ntohs(ClientAddr.sin_port) << " " << endl; // ��Ʈ��ũ����Ʈ���ĵȾָ� ȣ��Ʈ���Ĺ���Ʈ�� ����ϴ°Ŵϱ� ��ȯ

		u_long on = 1;
		ioctlsocket(ClientSock, FIONBIO, &on); // Ŭ������� �ͺ��ŷ���� �����
		
		while (1)
		{
			char szRecvBuffer[256] = { 0, };
			int RecvByte = recv(sock, szRecvBuffer, 256, 0);
			if (RecvByte == 0) // RecvByte�� 0�̸� Ŭ�� ������
			{
				cout << "ip = " << inet_ntoa(ClientAddr.sin_addr)
					<< "port = " << ntohs(ClientAddr.sin_port)
					<< "==> Log Out" << endl;
				break;
			}
			if (RecvByte == SOCKET_ERROR) // RecvByte�� 0�̸� Ŭ�� ������
			{
				int iError = WSAGetLastError();
				cout << "ErrorCode = " << iError << endl;
				// �ͺ��ŷ���� ��ȯ�ɋ� �۾��̾�� ��ȯ�Ȱ�
				if (iError != WSAEWOULDBLOCK)
				{
					break;
				}
			}
			else
			{
				cout << szRecvBuffer << "����" << endl;
				int iSendByte = send(ClientSock, szRecvBuffer, RecvByte, 0);
				cout << iSendByte << "����." << endl;
			}
		}
		closesocket(ClientSock); // ���� �ݱ�
	}
	WSACleanup();
}