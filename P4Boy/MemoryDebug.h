#pragma once

#include <vector>

#include "MainBus.h"

class ShowMemory
{
public:

    ShowMemory(P4Boy::MainBus& mainBus);

    void MenuToolBarUpdate();

    void UpdateMemoryContent(std::shared_ptr<P4Boy::AddressRange> range);
    void WindowUpdate();
private:
    P4Boy::MainBus& _mainBus;
    std::vector<std::string> _memoryContent;
    std::string _currentRange;
    bool _show = false;
};