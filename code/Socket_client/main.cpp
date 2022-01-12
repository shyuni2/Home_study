#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment	(lib, "ws2_32.lib")

using namespace std;

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	// 1. ���Ͽ���
	SOCKET sock = socket
	(
		AF_INET, // �ּ�ü�� ���� : ���ͳ��������� ����ϰڴ�
		SOCK_STREAM, // ����Ÿ�� ���� : TCP ����ϰڴ�, STREAM ���� TCP, DGRAM ���� UDP ���� �ϳ� ����ϸ��
		0 // ����� �������� ���� : �ι�° ���ڿ��� ����ϴ°� ���� �ڵ����� �������ִ� ��������
	);
	SOCKADDR_IN sa; 
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000); // 10000������ ����, ������ ��Ʈ��ȣ�� ��ġ�ؾ���, 10000������ ���������ʰ� �Լ��� ��, htons : ȣ��Ʈ����Ʈ�� ��Ʈ��ũ ����Ʈ short�� �ٲ��ִ� �Լ� 
	// ����Ʈ ���� �տ������� ������ �ڿ��� ���� ������?
	sa.sin_addr.s_addr = inet_addr("192.168.219.12"); // ���ڿ��� ������ �ٲ������,���ڿ��� 4����Ʈ�� �ٲ��ִ� �Լ� ���� inet_addr, ��Ʈ��ũ���� ������ �������� ��ȯ���ִ°��Է�

	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));
	int SendCount = 3;
	while (SendCount-- > 0)
	{
		char szBuffer[] = "�ȳ��ϼ���."; // ���⿡ �Է��ϴ� ���ڴ� Ŭ�� �����°�, �������� �������ڸ��� ������ �޼����� ����� ����
		int SendByte = send(sock, szBuffer, sizeof(szBuffer), 0);
		if (SendByte == SOCKET_ERROR)
		{
			std::cout << "������ ���������.." << endl;
			break;
		}
		char szRecvBuffer[256] = { 0, };
		int RecvByte = recv(sock, szRecvBuffer, 256, 0);
		cout << szRecvBuffer;
		if (RecvByte == 0)
		{
			cout << "���������.." << endl;
			break;
		}
		if (RecvByte == SOCKET_ERROR)
		{
			cout << "������ ���������.." << endl;
			break;
		}
		Sleep(1000);
	}
	closesocket(sock); // Ŭ�󿡼��� ������ ����
	WSACleanup();
}