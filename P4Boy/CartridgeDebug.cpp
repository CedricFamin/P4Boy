#include "CartridgeDebug.h"
#include "MBC.h"

#include "imgui.h"


ShowCartridge::ShowCartridge(P4Boy::Cartridge::shared_ptr cartrdige) : _cartridge(cartrdige) {}

void ShowCartridge::MenuToolBarUpdate()
{
    if (ImGui::BeginMenu("Cartridge"))
    {
        if (ImGui::MenuItem("Basic Information", nullptr, _show))
        {
            _show = !_show;
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

    void ShowField(char const* fieldName, const uint8_t& value)
    {
        ImGui::TableNextColumn();
        ImGui::Text(fieldName);
        ImGui::TableNextColumn();
        ImGui::Text("%d", value);
        ImGui::TableNextRow();
    }
}
void ShowCartridge::WindowUpdate()
{
    if (!_show)
        return;

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("========== Cartridge ==========").x) / 2.f);
    ImGui::Text("========== Cartridge ==========");

    if (ImGui::BeginTable("table1", 3))
    {
        ShowField("Rom Name:", _cartridge->GetTitle());
        ShowField("Cartridge type:", _cartridge->GetCartridgeType());
        ShowField("Manufacturer Code:", _cartridge->GetManufacturerCode());

        ShowField("Current RAM Bank:", _cartridge->GetMbcInterface()->GetCurrentRamBank());
        ShowField("Current ROM Bank:", _cartridge->GetMbcInterface()->GetCurrentRomBank());

        ShowField("RAM size:", _cartridge->GetRamSize());
        ShowField("NB RAM bank:", _cartridge->GetRamBankNB());
        ShowField("ROM size:", _cartridge->GetRomSize());
        ShowField("NB ROM bank:", _cartridge->GetRomBankNB());
        ShowField("GCB Flag:", _cartridge->GetGCBFlag());

        ImGui::EndTable();
    }
}