#pragma once

#include "Utility.h"
#include "Register.h"

#include "SFML/Graphics.hpp"

namespace P4Boy
{
	class MainBus;

	class LCDC : public Register_8b
	{
		/*
		* 7	LCD and PPU enable	0=Off, 1=On
		* 6	Window tile map area	0=9800-9BFF, 1=9C00-9FFF
		* 5	Window enable	0=Off, 1=On
		* 4	BG and Window tile data area	0=8800-97FF, 1=8000-8FFF
		* 3	BG tile map area	0=9800-9BFF, 1=9C00-9FFF
		* 2	OBJ size	0=8x8, 1=8x16
		* 1	OBJ enable	0=Off, 1=On
		* 0	BG and Window enable/priority	0=Off, 1=On
		*/
	public:
		LCDC() : 
			Enable(*this, 7)
			, WindowTileMapArea(*this, 6)
			, WindowEnable(*this, 5)
			, BGWTileDataArea(*this, 4)
			, BGWTileMapArea(*this, 3)
			, OBJSize(*this, 2)
			, OBJEnable(*this, 1)
			, BGWEnable_Priority(*this, 0)
		{

		}

		SubRegister_1b<Register_8b> Enable;
		SubRegister_1b<Register_8b> WindowTileMapArea;
		SubRegister_1b<Register_8b> WindowEnable;
		SubRegister_1b<Register_8b> BGWTileDataArea;
		SubRegister_1b<Register_8b> BGWTileMapArea;
		SubRegister_1b<Register_8b> OBJSize;
		SubRegister_1b<Register_8b> OBJEnable;
		SubRegister_1b<Register_8b> BGWEnable_Priority;

		inline LCDC& operator=(uint8_t value) { _value = value; return *this; }
	};

	class LCDS : public Register_8b
	{
		/*
		* Bit 6 - LYC=LY STAT Interrupt source         (1=Enable) (Read/Write)
		* Bit 5 - Mode 2 OAM STAT Interrupt source     (1=Enable) (Read/Write)
		* Bit 4 - Mode 1 VBlank STAT Interrupt source  (1=Enable) (Read/Write)
		* Bit 3 - Mode 0 HBlank STAT Interrupt source  (1=Enable) (Read/Write)
		* Bit 2 - LYC=LY Flag                          (0=Different, 1=Equal) (Read Only)
		* Bit 1-0 - Mode Flag                          (Mode 0-3, see below) (Read Only)
		*           0: HBlank
		*           1: VBlank
		*           2: Searching OAM
		*           3: Transferring Data to LCD Controller
		*/
	public:
		LCDS() :
			LYCCheckInterruptSource(*this, 6)
			, OAMInterruptSource(*this, 5)
			, VblankInterruptSource(*this, 4)
			, HblankInterruptSource(*this, 3)
			, LYCCheckFlag(*this, 2)
			, Mode(*this, 0)
		{

		}
		SubRegister_1b<Register_8b> LYCCheckInterruptSource;
		SubRegister_1b<Register_8b> OAMInterruptSource;
		SubRegister_1b<Register_8b> VblankInterruptSource;
		SubRegister_1b<Register_8b> HblankInterruptSource;
		SubRegister_1b<Register_8b> LYCCheckFlag;
		SubRegister_2b<Register_8b> Mode;

		inline LCDS& operator=(uint8_t value) { _value = value; return *this; }
	};
	class BGP : public Register_8b
	{
		/*
		* Bit 7 - 6 - Color for index 3
		* Bit 5 - 4 - Color for index 2
		* Bit 3 - 2 - Color for index 1
		* Bit 1 - 0 - Color for index 0
		*/
	public:
		BGP() : ColorIdx0(*this, 0), ColorIdx1(*this, 2), ColorIdx2(*this, 4), ColorIdx3(*this, 6) { }

		SubRegister_2b<Register_8b> ColorIdx3;
		SubRegister_2b<Register_8b> ColorIdx2;
		SubRegister_2b<Register_8b> ColorIdx1;
		SubRegister_2b<Register_8b> ColorIdx0;

		inline BGP& operator=(uint8_t value) { _value = value; return *this; }
	};

	class OBP : public Register_8b
	{
		/*
		* Bit 7 - 6 - Color for index 3
		* Bit 5 - 4 - Color for index 2
		* Bit 3 - 2 - Color for index 1
		* Bit 1 - 0 - Not Used
		*/
	public:
		OBP() : ColorIdx1(*this, 2), ColorIdx2(*this, 4), ColorIdx3(*this, 6) {}

		SubRegister_1b<Register_8b> ColorIdx3;
		SubRegister_1b<Register_8b> ColorIdx2;
		SubRegister_1b<Register_8b> ColorIdx1;

		inline OBP& operator=(uint8_t value) { _value = value; return *this; }
	};

	class LCD
	{
	public:
		typedef Ptr<LCD> ptr;
		typedef std::shared_ptr<LCD> shared_ptr;

		enum LCDMode
		{
			OAM_Scan = 2
			, Drawing_Pixels = 3
			, Horizontal_Blank = 0
			, Vertical_Blank = 1
		};

		LCD();
	public:

		void Tick();
		void ConnectAddressRange(MainBus& mainBus);


	protected:
	private:
		MainBus* _mainBus;
		void DrawBackground();
		void DrawSprites();
		sf::RenderWindow _window;
		sf::Texture		 _renderTexture;
		sf::Image		 _renderImg;
		sf::Sprite		 _renderSprite;
		sf::Clock _deltaClock;

		LCDC	_LCDC;
		LCDS	_LCDS;
		uint8_t _SCY = 0;
		uint8_t _SCX = 0;
		uint8_t _LY = 0;
		uint8_t _LYC = 0;
		uint8_t _WY = 0;
		uint8_t _WX = 0;
		BGP     _BGP;
		OBP     _OBP0;
		OBP     _OBP1;
	};
}


