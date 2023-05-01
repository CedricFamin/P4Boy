#pragma once

#include <chrono>
#include <thread>

#include "Utility.h"

namespace P4Boy
{
    class Clock
    {
    public:
        typedef std::shared_ptr<Clock> shared_ptr;

        static const int NB_TICK_COMPUTE_FREQUENCY = 1500000;
    public:
        Clock(std::chrono::nanoseconds const& tickDuration);

        void Wait();
        float GetCurrentFequencyMhz() const;
        void SetTickDuration(std::chrono::nanoseconds duration);
        std::chrono::nanoseconds GetTickDuration() const { return _tickDuration; }

        void Reset();
    protected:

    private:
        std::chrono::steady_clock::time_point   _nextClockTick;
        std::chrono::steady_clock::time_point   _computeFrequencyStart;
        std::atomic<std::chrono::nanoseconds>   _tickDuration;
        std::atomic<float>                      _frequency = .0f;
        unsigned int                            _tick = 0;
    };
}
