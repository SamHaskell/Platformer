#pragma once

#include "core/defines.hpp"
#include "core/singleton.hpp"

#include <fstream>
#include <chrono>

/*
    Credit to Prof. Dave Churchill at Memorial University for this profiler.
    Designed to be used with google chromes tracing capabilities.
    Take the output json and load it into chrome://tracing to view.
*/

#define PROFILING 1
#ifdef PROFILING
    #define PROFILE_SCOPE(name) \
        ProfileTimer timer##__LINE__(name);

    #define PROFILE_FUNCTION() \
        PROFILE_SCOPE(__FUNCTION__);
#else
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif

struct ProfileResult
{
    std::string Name = "Default";
    u64 Start = 0;
    u64 End = 0;
    size_t ThreadID = 0;
};

class ProfileTimer
{
public:
    ProfileTimer(const std::string &name)
    {
        m_Result.Name = name;
        Start();
    }

    ~ProfileTimer()
    {
        Stop();
    }

    void Start();
    void Stop();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_STP;
    bool m_Stopped = false;
    ProfileResult m_Result;
};

class Profiler : public Singleton<Profiler>
{
public:
    Profiler()
    {
        m_Output = std::ofstream(m_OutputPath);
        WriteHeader();
    }

    ~Profiler()
    {
        WriteFooter();
    }

    void WriteProfile(const ProfileResult &r);

    void WriteHeader();
    void WriteFooter();

private:
    std::string m_OutputPath = "../results.json";
    std::ofstream m_Output;
    size_t m_ProfileCount = 0;
    std::mutex m_Lock;
};