#include "ClockDebug.h"

#include "imgui.h"

ClockDebug::ClockDebug(P4Boy::Clock::shared_ptr clock) : _clock(clock)
{

}

void ClockDebug::MenuToolBarUpdate()
{
    if (ImGui::BeginMenu("Clock"))
    {
        if (ImGui::MenuItem("Settings", nullptr, _show))
        {
            _show = !_show;
        }
        ImGui::EndMenu();
    }
}
namespace {
    void ShowField(char const* fieldName, const float& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text("%2.2f", value);
        ImGui::TableNextRow();
    }
}

void ClockDebug::WindowUpdate()
{
    if (_show)
    {
        if (ImGui::BeginTable("TableClock", 2))
        {
            ShowField("Frenquency (Mghz)", _clock->GetCurrentFequencyMhz());
            ImGui::EndTable();
        }
        ImGui::SliderFloat("Target clock (MHz)", &targetClock , 1, 24, "%.2f");
        _clock->SetTickDuration(std::chrono::nanoseconds((long)1000/(long)targetClock));
    }
}
