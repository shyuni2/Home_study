#include "ServerObj.h"

ServerObj::ServerObj()
{
	InitializeCriticalSection(&m_CS);
	m_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
}
ServerObj::~ServerObj()
{
	DeleteCriticalSection(&m_CS);
	CloseHandle(m_hKillEvent);
}