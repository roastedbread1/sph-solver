#include "timer.h"

void TIMER::startTimer(Timer* timer, const std::string& name)
{
    timer->name = name;
    timer->start = std::chrono::high_resolution_clock::now();
}

void TIMER::endTimer(Timer* timer)
{
    auto end = std::chrono::high_resolution_clock::now();

    auto startMicro = std::chrono::time_point_cast<std::chrono::microseconds>(timer->start)
        .time_since_epoch()
        .count();
    auto endMicro = std::chrono::time_point_cast<std::chrono::microseconds>(end)
        .time_since_epoch()
        .count();

    auto duration = (endMicro - startMicro) * 0.001; // Convert to milliseconds
    std::cerr << "<" << timer->name << "> clock finished: " << duration << " ms" << std::endl;
}
