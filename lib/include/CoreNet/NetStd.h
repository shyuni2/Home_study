#pragma once
#include <WinSock2.h>
#include <windows.h> // 윈도우 헤더랑 윈속헤더랑 순서가 중요함
#include <vector>
#include <list>
#include <iostream>
#pragma comment	(lib, "ws2_32.lib")
// 충돌은 서버에서 필요해서 클라엔 필요없다

using namespace std;