#pragma once
#include <Windows.h>

#pragma pack(push,1) // 모든게 1바이트 패킹이 들어감, 이 처리가 되어야지 문제가 안됨 pack~pop사이가 1바이트가됨
// 앞으로 만들어내는 모든 구조체는 1바이트 패킹이 들어감 
// 구조체를 이용하더라도 반드시 바이트단위로 통신이 되어야함, 구조체 크기가 달라지면 문자열 전송하는데 심각한 오류가남 

typedef struct
{
	WORD len; // 길이
	WORD type; // 타입
}PACKET_HEADER;

typedef struct // 방법 1>
{
	PACKET_HEADER ph; // 패킷헤더가 반드시 존재해야함
	char msg[4096]; // 합산했을때 배열크기안에 못들어가면 문제가 있음, 패킷을 만들어내는 구조를 정의해놓고 사용
}UPACKET, * P_UPACKET;

// 이 규칙을 안따르면 통신이 안됨, 수업은 어떤 메세지던 이 UPACKET을 이용하자

struct ChatMsg // 방법 2> // 실제 메세지 처리가 됨 
{
	long index;
	char name[20]; // 홍길동 , 데이터베이스에 이름을 몇바이트로 지정했는지에 따라서 달라짐
	short damage;
	char message[2048]; // 안녕하세요! 같은 메세지를 담는다
};

#pragma pack(pop) // 밑에껀 구조체 크기로됨

#define PACKET_CHAT_MSG 1000
#define PACKET_HEADER_SIZE 4