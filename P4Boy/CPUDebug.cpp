#include "CPUDebug.h"

#include "imgui.h"


ShowCPURegisters::ShowCPURegisters(P4Boy::CPU::shared_ptr cpu, P4Boy::Clock::shared_ptr clock) : _cpu(cpu), _clock(clock) {}

void ShowCPURegisters::MenuToolBarUpdate()
{
    if (ImGui::BeginMenu("CPU"))
    {
        if (ImGui::MenuItem("CPU Info", nullptr, _show))
        {
            _show = !_show;
        }
        if (ImGui::MenuItem("Logs"))
        {
            _cpu->debugShowInstructions = !_cpu->debugShowInstructions;
        }
        ImGui::EndMenu();
    }
}

namespace {
    void ShowField(char const* fieldName, const std::string& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text(value.c_str());
        ImGui::TableNextRow();
    }

    void ShowField(char const* fieldName, const uint16_t& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text("%d", value);
        ImGui::TableNextRow();
    }

    void ShowField(char const* fieldName, const uint8_t& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text("%d", value);
        ImGui::TableNextRow();
    }

    void ShowField(char const* fieldName, const float& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text("%2.2f", value);
        ImGui::TableNextRow();
    }
}

void ShowCPURegisters::WindowUpdate()
{
    if (!_show) return;
    
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("========== CPU ==========").x) / 2.f);
    ImGui::Text("========== CPU ==========");

    if (ImGui::BeginTable("TableCPU", 2))
    {
        ShowField("Frenquency (Mghz)", _clock->GetCurrentFequencyMhz());
        ShowField("", "");

        ShowField("Registers", "");

        ShowField("AF", _cpu->AF);
        ShowField("A", _cpu->AF.A);
        ShowField("F", _cpu->AF.F);
        ShowField("BC", _cpu->BC);
        ShowField("B", _cpu->BC.B);
        ShowField("C", _cpu->BC.C);
        ShowField("DE", _cpu->DE);
        ShowField("D", _cpu->DE.D);
        ShowField("E", _cpu->DE.E);
        ImGui::EndTable();
    }
}