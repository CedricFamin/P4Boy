#pragma once

#include "Cartridge.h"

class ShowCartridge
{
public:

    ShowCartridge(P4Boy::Cartridge::shared_ptr ptr);

    void MenuToolBarUpdate();

    void WindowUpdate();
private:
    P4Boy::Cartridge::shared_ptr _cartridge;
    bool _show = false;
};

