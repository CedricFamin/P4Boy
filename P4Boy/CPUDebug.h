#pragma once

#include "CPU.h"
#include "Clock.h"

class ShowCPURegisters
{
public:

    ShowCPURegisters(P4Boy::CPU::shared_ptr cpu, P4Boy::Clock::shared_ptr clock);

    void MenuToolBarUpdate();

    void WindowUpdate();
private:
    P4Boy::CPU::shared_ptr _cpu;
    P4Boy::Clock::shared_ptr _clock;
    bool _show = false;
};

