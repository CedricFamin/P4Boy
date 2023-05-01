#pragma once

#include "CPU.h"
#include "Clock.h"

class ShowCPURegisters
{
public:

    ShowCPURegisters(P4Boy::CPU::shared_ptr cpu);

    void MenuToolBarUpdate();

    void WindowUpdate();
private:
    P4Boy::CPU::shared_ptr _cpu;
    bool _show = false;
};

