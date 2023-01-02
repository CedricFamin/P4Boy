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
        std::this_thread::sleep_until(_nextClockTick - std::chrono::nanoseconds(20));
        auto currentTime = std::chrono::high_resolution_clock::now();
        _nextClockTick += _tickDuration;
    }
    float Clock::GetCurrentFequencyMhz() const
    {
        return .0f;
    }
}