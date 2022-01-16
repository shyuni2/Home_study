#include <iostream>
#include <WinSock2.h>
#include <conio.h>
#include "Packet.h"
#pragma comment (lib,"ws2_32.lib")

HANDLE hConnectEvent; // 연결됐을때 이벤트
HANDLE hExecuteSemaphore; // 세마포어

