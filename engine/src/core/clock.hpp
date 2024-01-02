#pragma once

#include "defines.hpp"
#include <chrono>

class Clock
{
public:
    Clock() = default;
    ~Clock() = default;

    inline void Start()
    {
        m_Now = std::chrono::high_resolution_clock::now();
        m_Started = true;
    }

    inline f64 Tick()
    {
        if (!m_Started)
        {
            return 0.0;
        }
        auto next = std::chrono::high_resolution_clock::now();
        f64 dt = std::chrono::duration<f64, std::chrono::seconds::period>(next - m_Now).count();
        m_Now = next;
        return dt;
    }

private:
    bool m_Started = false;
    std::chrono::steady_clock::time_point m_Now;
};