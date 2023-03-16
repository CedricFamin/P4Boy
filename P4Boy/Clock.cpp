#include "Clock.h"

namespace P4Boy
{
    Clock::Clock(std::chrono::nanoseconds const& tickDuration)
    {
        _tickDuration = tickDuration;
        _nextClockTick = std::chrono::high_resolution_clock::now() + _tickDuration;
    }

    void Clock::Wait()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        if (currentTime < _nextClockTick)
            std::this_thread::sleep_for(_nextClockTick - currentTime);
        
        _nextClockTick += _tickDuration;
    }
    float Clock::GetCurrentFequencyMhz() const
    {
        return .0f;
    }
}