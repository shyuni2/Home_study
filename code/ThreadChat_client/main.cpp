#include <iostream>
#include <WinSock2.h>
#include <conio.h>
#include "Packet.h"
#pragma comment (lib,"ws2_32.lib")

HANDLE hConnectEvent; // ��������� �̺�Ʈ
HANDLE hExecuteSemaphore; // ��������

