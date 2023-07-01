#include "LCD.h"

#include "MainBus.h"
#include "AddressAction.h"
#include "CPU.h"


namespace P4Boy
{
	LCD::LCD() : _window(sf::VideoMode(160, 144), "P4Boy"), _renderTexture(), _renderImg(), _mainBus(nullptr)
	{
	}
	
	void LCD::ConnectAddressRange(MainBus& mainBus)
	{
		mainBus.AddDirectAccess(0xFF40, this->_LCDC, "LCDC");
		mainBus.AddDirectAccess(0xFF41, this->_LCDS, "LCDS");

		mainBus.AddDirectAccess(0xFF44, this->_LY, "LY");
		mainBus.AddDirectAccess(0xFF45, this->_LYC, "LYC");

		mainBus.AddDirectAccess(0xFF42, this->_SCY, "SCY");
		mainBus.AddDirectAccess(0xFF43, this->_SCX, "SCX");

		mainBus.AddDirectAccess(0xFF4A, this->_WY, "WY");
		mainBus.AddDirectAccess(0xFF4B, this->_WX, "WX");

		mainBus.AddDirectAccess(0xFF47, this->_BGP, "BGP");
		mainBus.AddDirectAccess(0xFF48, this->_OBP0, "OBP0");
		mainBus.AddDirectAccess(0xFF49, this->_OBP1, "OBP1");
		_mainBus = &mainBus;
	}

	void LCD::Reset()
	{
		_LCDC = 0;
		_LCDS = 0;
		_SCY = 0;
		_SCX = 0;
		_LY = 0;
		_LYC = 0;
		_WY = 0;
		_WX = 0;
		_BGP = 0;
		_OBP0 = 0;
		_OBP1 = 0;
	}

	uint16_t LCD::GetTileDataAddress() const
	{
		return _LCDC.BGWTileDataArea ? 0x8000 : 0x8800;
	}
	uint16_t LCD::GetTileMapAddress() const
	{
		return _LCDC.BGWTileMapArea ? 0x9C00 : 0x9800;
	}
	
	uint16_t LCD::GetTileAddress(uint16_t const& tileMapAddr, uint16_t const& tileDataAddr, uint8_t const& x, uint8_t const& y) const
	{
		uint16_t address = 0;

		uint8_t tileIndex = _mainBus->Get_8b(tileMapAddr + x + y * 32);
		if (!tileIndex)
			return 0;
		address = tileDataAddr + tileIndex * 16;
		if (_LCDC.BGWTileDataArea == 0)
		{
			address = tileDataAddr + (tileIndex - 128) * 16;
			if (tileIndex <= 127)
			{
				address = tileDataAddr + (tileIndex + 128) * 16;
			}
		}

		return address;
	}

	void LCD::DrawBackground()
	{
		uint16_t tile_data_addr = GetTileDataAddress();
		uint16_t tile_map_addr = GetTileMapAddress();

		static const sf::Color baseColors[] = { {0xE0, 0xF8, 0xD0}, {0x88, 0xC0, 0x70}, {0x34, 0x68, 0x56}, {0x08, 0x18, 0x28} };
		sf::Color colors[4] = {
			baseColors[_BGP.ColorIdx0]
			, baseColors[_BGP.ColorIdx1]
			, baseColors[_BGP.ColorIdx2]
			, baseColors[_BGP.ColorIdx3]
		};

		Tile<8> tile;

		if (_LCDC.BGWEnable_Priority)
		{
			for (uint8_t y = 0; y < 32; ++y)
			{
				for (uint8_t x = 0; x < 32; ++x)
				{
					uint16_t tileAddress = GetTileAddress(tile_map_addr, tile_data_addr, x, y);
					if (tileAddress == 0)
						continue;
					GetTilePixels(tileAddress, tile);

					for (uint8_t y_tile = 0; y_tile < 8; ++y_tile)
					{
						int coordY = y * 8 + y_tile - _SCY;
						for (uint8_t x_tile = 0; x_tile <= 8; ++x_tile)
						{
							int coordX = x * 8 + x_tile - _SCX;
							if (!(coordX >= 0 && coordX < 160 && coordY >= 0 && coordY < 144))
								continue;
							uint8_t color = tile.UnpackGetColor(x_tile, y_tile);
							_renderImg.setPixel(coordX, coordY, colors[color]);
						}
					}
				}
			}
		}
		
		if (_LCDC.WindowEnable)
		{
			_LCDC.WindowEnable = _LCDC.WindowEnable.Get();
		}
	}

	void LCD::DrawSprites()
	{
		if (!_LCDC.OBJEnable)
			return;
		uint16_t tile_data_addr = 0x8000;
		uint16_t oam_addr = 0xFE00;

		static const sf::Color baseColors[] = { {0xE0, 0xF8, 0xD0}, {0x88, 0xC0, 0x70}, {0x34, 0x68, 0x56}, {0x08, 0x18, 0x28} };
		sf::Color colors[4] = {
			baseColors[_BGP.ColorIdx0]
			, baseColors[_BGP.ColorIdx1]
			, baseColors[_BGP.ColorIdx2]
			, baseColors[_BGP.ColorIdx3]
		};
		
		for (uint8_t i = 0; i < 40; ++i)
		{
			uint16_t spriteAddr = oam_addr + i * 4;

			uint8_t yPos = _mainBus->Get_8b(spriteAddr + 0) - 16;
			uint8_t xPos = _mainBus->Get_8b(spriteAddr + 1) - 8;
			uint8_t tileIndex = _mainBus->Get_8b(spriteAddr + 2);
			uint8_t attributes = _mainBus->Get_8b(spriteAddr + 3);
			if (!tileIndex)
				continue;
			uint16_t tileAddr = tile_data_addr + tileIndex * 16;
			for (uint8_t y_tile = 0; y_tile < 8; ++y_tile)
			{
				uint8_t pixels1 = _mainBus->Get_8b(tileAddr + (y_tile * 2));
				uint8_t pixels2 = _mainBus->Get_8b(tileAddr + (y_tile * 2 + 1));
				for (uint8_t x_tile = 0; x_tile <= 8; ++x_tile)
				{
					uint8_t color = ((pixels1 >> (7 - x_tile)) & 1) | (((pixels2 >> (7 - x_tile)) & 1) << 1);
					int coordX = xPos + x_tile;
					int coordY = yPos + y_tile;
					if (coordX >= 0 && coordX < 160 && coordY >= 0 && coordY < 144)
					{
						_renderImg.setPixel(coordX, coordY, colors[color]);
					}
				}
			}
		}
	}
	void LCD::Tick()
	{
		static uint32_t MODE_TIMING[] = { 85, 456, 80, 291 };
		static uint32_t nextTickUpdate = 0;

		if (nextTickUpdate == 0)
		{
			Register_Interrupt requestInterrupt = 0;
			LCDMode currentMode = (LCDMode)_LCDS.Mode.Get();
			if (currentMode == LCDMode::OAM_Scan)
			{
				_LCDS.Mode = LCDMode::Drawing_Pixels;
			}
			else if (currentMode == LCDMode::Drawing_Pixels)
			{
				_LCDS.LYCCheckFlag = (int)(_LY == _LYC);
				if (_LY == _LYC && _LCDS.LYCCheckInterruptSource)
				{
					requestInterrupt.LCDStat = 1;
				}
				if (_LCDS.HblankInterruptSource)
				{
					requestInterrupt.LCDStat = 1;
				}
				_LCDS.Mode = LCDMode::Horizontal_Blank;
			}
			else if (currentMode == LCDMode::Horizontal_Blank)
			{
				_LY += 1;
				if (_LY >= 144)
				{
					_LCDS.Mode = LCDMode::Vertical_Blank;
					if (_LCDS.VblankInterruptSource)
					{
						requestInterrupt.LCDStat = 1;
					}
					requestInterrupt.VBlank = 1;
				}
				else _LCDS.Mode = LCDMode::OAM_Scan;
			}
			else if (currentMode == LCDMode::Vertical_Blank)
			{
				if (_LY >= 153)
				{
					_LCDS.Mode = LCDMode::OAM_Scan;
					_LCDS.VblankInterruptSource = 0;
					_LY = 0;

					_window.clear({ 0xE0, 0xF8, 0xD0 });
					_renderImg.create(160, 144, { 0xE0, 0xF8, 0xD0 });
					DrawBackground();
					DrawSprites();
					_renderTexture.loadFromImage(_renderImg);
					_renderSprite.setTexture(_renderTexture);
					_renderSprite.setPosition(sf::Vector2f(0, 0));
					_window.draw(_renderSprite);
					_window.display();
					sf::Event event;
					while (_window.pollEvent(event)) {}
				}
				
				_LY += 1;
			}

			nextTickUpdate = MODE_TIMING[currentMode];
			if (requestInterrupt > 0) Register_Interrupt::MergeRequestInterrupt(*_mainBus, requestInterrupt);
		}
		else
			nextTickUpdate -= 1;
	}
}