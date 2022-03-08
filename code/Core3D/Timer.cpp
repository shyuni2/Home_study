#include "Timer.h"

float g_fSecPerFrame = 0.0f;
float g_fGameTimer = 0.0f;

bool Timer::Init()
{
    m_fSecondPerFrame = 0.0f;
    m_fTimer = 0.0f;
    m_dwBeforeTime = timeGetTime();
    return false;
}
bool Timer::Frame()
{
    DWORD dwCurrentTime = timeGetTime();
    DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
    m_fSecondPerFrame = dwElapseTime / 1000.0f;
    m_fTimer += m_fSecondPerFrame;

    g_fSecPerFrame = m_fSecondPerFrame;
    g_fGameTimer = m_fTimer;

    m_fFPSTimer += m_fSecondPerFrame;
    if (m_fFPSTimer >= 1.0f)
    {
        m_iFPS = m_iFPSCounter;
        m_iFPSCounter = 0;
        m_fFPSTimer -= 1.0f;
    }
    m_iFPSCounter++;

    m_dwBeforeTime = dwCurrentTime;
    return false;
}
bool Timer::Render()
{
    return false;
}
bool Timer::Release()
{
    return false;
}
