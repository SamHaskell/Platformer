#include "perf/profiling.hpp"

#include <thread>

Profiler g_Profiler;

void ProfileTimer::Start()
{
    m_STP = std::chrono::high_resolution_clock::now();
}

void ProfileTimer::Stop()
{
    if (m_Stopped)
    {
        return;
    }

    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    m_Result.Start = time_point_cast<microseconds>(m_STP).time_since_epoch().count();
    m_Result.End = time_point_cast<microseconds>(now).time_since_epoch().count();
    m_Result.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

    Profiler::GetInstance().WriteProfile(m_Result);
}

void Profiler::WriteProfile(const ProfileResult &r)
{
    std::lock_guard<std::mutex> lock(m_Lock);

    if (m_ProfileCount++ > 0) { m_Output << ","; }

    std::string name = r.Name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_Output << "\n{";
    m_Output << "\"cat\":\"function\",";
    m_Output << "\"dur\":" << (r.End - r.Start) << ",";
    m_Output << "\"name\":\"" << name << "\",";
    m_Output << "\"ph\":\"X\",";
    m_Output << "\"pid\":0,";
    m_Output << "\"tid\":" << r.ThreadID << ",";
    m_Output << "\"ts\":" << r.Start;
    m_Output << "}";
}

void Profiler::WriteHeader()
{
    m_Output << "{\"otherData\": {},\"traceEvents\":[";
}

void Profiler::WriteFooter()
{
    m_Output << "]}";
}