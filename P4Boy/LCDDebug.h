#pragma once

#include "LCD.h"

class LCDDebug
{
public:

    LCDDebug(P4Boy::LCD::shared_ptr cpu);

    void MenuToolBarUpdate();

    void WindowUpdate();
private:
    P4Boy::LCD::shared_ptr _lcd;
    bool _show = false;
};

