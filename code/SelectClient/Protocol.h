#pragma once
#include <windows.h>

#pragma pack(push, 1)

typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER ph;
	char msg[4096];
}UPACKET, *P_UPACKET;

struct ChatMsg
{	
	long index;
	char name[20]; // È«±æµ¿
	short damage;
	char message[256];// ¾È³çÇÏ¼¼¿©.	
};
#pragma pack(pop)

#define PACKET_HEADER_SIZE	 4
#define PACKET_CHAT_MSG      1000
