#pragma once

#include <chrono>
#include <thread>

namespace P4Boy
{
    class Clock
    {
    public:
        Clock(std::chrono::nanoseconds const& tickDuration);

        void Wait();
        float GetCurrentFequencyMhz() const;
    protected:

    private:
        std::chrono::steady_clock::time_point _nextClockTick;
        std::chrono::nanoseconds _tickDuration;
    };
}
