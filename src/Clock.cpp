#include "Clock.h"

Clock::Clock()
{
    Restart();
}

void Clock::Restart()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
}

float Clock::Elapsed() const
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(endTime - m_StartTime);
    return elapsedTime.count();
}

float Clock::Stamp()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(endTime - m_StartTime);
    m_StartTime = endTime;
    return elapsedTime.count();
}
