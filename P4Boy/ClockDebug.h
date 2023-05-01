#pragma once

#pragma once

#include "Clock.h"

class ClockDebug
{
public:

    ClockDebug(P4Boy::Clock::shared_ptr clock);

    void MenuToolBarUpdate();

    void WindowUpdate();
private:
    P4Boy::Clock::shared_ptr _clock;
    bool _show = false;
    float targetClock = 4.194304;
};

