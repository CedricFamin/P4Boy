#include "LCDDebug.h"

#include "imgui.h"

LCDDebug::LCDDebug(P4Boy::LCD::shared_ptr lcd) :
	_lcd(lcd)
{

}

void LCDDebug::MenuToolBarUpdate()
{
    if (ImGui::BeginMenu("LCD"))
    {
        if (ImGui::MenuItem("Show Tile Map", nullptr, _show))
        {
            _show = !_show;
        }
        ImGui::EndMenu();
    }
}

void LCDDebug::WindowUpdate()
{
    if (_show)
    {
        static int tileX = 0;
		static int tileY = 0;
        ImGui::SliderInt("Tile X", &tileX, 0, 32);
        ImGui::SliderInt("Tile Y", &tileY, 0, 32);
		uint16_t tileAddress = _lcd->GetTileAddress(_lcd->GetTileMapAddress(), _lcd->GetTileDataAddress(), tileX, tileY);
		if (tileAddress == 0)
		{
			ImGui::Text("No tile");
			return;
		}
		ImGui::Text("Tile address: %x", tileAddress);

		P4Boy::Tile<8> tile;
		_lcd->GetTilePixels(tileAddress, tile);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 pBase = ImGui::GetCursorScreenPos();
		for (uint8_t y = 0; y < 8; ++y)
		{
			for (uint8_t x = 0; x <= 8; ++x)
			{
				uint8_t color = tile.UnpackGetColor(x, y);
				ImVec2 p0{pBase.x + x * 8, pBase.y + y * 8};
				ImVec2 p1{p0.x + 8, p0.y + 8};
				draw_list->AddRectFilled(p0, p1, _lcd->GetColors(color) + 0xFF000000);
			}
		}
    }
}

