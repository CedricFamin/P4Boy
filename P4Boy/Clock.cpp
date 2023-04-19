#include "Clock.h"

namespace P4Boy
{
    Clock::Clock(std::chrono::nanoseconds const& tickDuration)
    {
        _tickDuration = tickDuration;
        _nextClockTick = std::chrono::high_resolution_clock::now() + _tickDuration.load();
        _computeFrequencyStart = std::chrono::high_resolution_clock::now();
    }

    void Clock::Wait()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        if (currentTime < _nextClockTick)
            std::this_thread::sleep_for(_nextClockTick - currentTime);
        
        _nextClockTick += _tickDuration;


        auto on_date = std::chrono::high_resolution_clock::now();

        ++_tick;
        if (_tick >= NB_TICK_COMPUTE_FREQUENCY)
        {
            auto computeFrequencyEnd = std::chrono::high_resolution_clock::now();
            _frequency = (_tick * 1000.0f / (std::chrono::duration_cast<std::chrono::nanoseconds>(computeFrequencyEnd - _computeFrequencyStart).count()));
            _computeFrequencyStart = computeFrequencyEnd;
            _tick = 0;
        }
    }

    float Clock::GetCurrentFequencyMhz() const
    {
        return _frequency;
    }


    void Clock::SetTickDuration(std::chrono::nanoseconds duration)
    {
        _tickDuration = duration;
    }
}