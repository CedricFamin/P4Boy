#include "MemoryDebug.h"

#include <sstream>
#include <iomanip>

#include "imgui.h"

ShowMemory::ShowMemory(P4Boy::MainBus& mainBus) : _mainBus(mainBus) {}

void ShowMemory::MenuToolBarUpdate()
{

    if (ImGui::BeginMenu("Memory"))
    {
        if (ImGui::MenuItem("Show Memory", nullptr, _show))
        {
            _show = !_show;
        }
        ImGui::EndMenu();
    }
}

void ShowMemory::UpdateMemoryContent(std::shared_ptr<P4Boy::AddressRange> range)
{
    _memoryContent.clear();
    if (!range)
    {
        _currentRange = "";

    }
    std::stringstream ss;
    uint8_t nbHex = 0;
    uint32_t addr = range->From();
    for (; addr <= range->To(); ++addr)
    {
        if (nbHex % 16 == 0)
        {
            ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << addr << ": ";
        }
        ss << std::setfill('0') << std::setw(2) << std::hex << uint16_t(_mainBus.Get_8b(addr)) << " ";
        ++nbHex;

        if (nbHex % 16 == 0)
        {
            _memoryContent.push_back(ss.str());
            ss.str("");
        }
    }
    _memoryContent.push_back(ss.str());

    std::stringstream currentRange;
    currentRange << "" << std::setfill('0') << std::setw(4) << std::hex << range->From() << " - ";
    currentRange << "" << std::setfill('0') << std::setw(4) << std::hex << range->To() << " : ";
    currentRange << range->Name();
    _currentRange = currentRange.str();
}
void ShowMemory::WindowUpdate()
{
    if (_show)
    {
        ImGui::Begin("Show Memory");


        if (ImGui::BeginCombo("Address type", _currentRange.c_str()))
        {
            for (auto const& range : _mainBus.Ranges())
            {
                std::stringstream ss;
                ss << "" << std::setfill('0') << std::setw(4) << std::hex << range->From() << " - ";
                ss << "" << std::setfill('0') << std::setw(4) << std::hex << range->To() << " : ";
                ss << range->Name();

                if (ImGui::Selectable(ss.str().c_str()))
                {
                    UpdateMemoryContent(range);
                }
            }
            ImGui::EndCombo();
        }

        for (auto& line : _memoryContent)
            ImGui::Text(line.c_str());


        ImGui::End();
    }
}