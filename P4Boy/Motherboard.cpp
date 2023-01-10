#include "Motherboard.h"

#include <iostream>
#include  <iomanip>

namespace P4Boy
{
	AddressRange* MakeSimpleMemoryAddressRange(Address start, Address end, std::string const & name)
	{
		return new AddressRange(start, end, new AddressAction_SimpleMemory(end - start, start), name);
	}

	void Motherboard::Tick()
	{

	}

	void Motherboard::TransferOAM(Address addr, uint8_t value)
	{
		for (uint16_t i = 0; i < 0x9F; ++i)
		{
			auto byte = _mainBus.Get_8b((uint16_t(value) << 8) + i);
			_mainBus.Set_8b(0xFE00 + i, byte);
		}
	}

	void Motherboard::ConnectAddressRange(MainBus& mainBus)
	{
		mainBus.AddSingle(0xFF46, new AddressAction_SingleAction([this](Address addr, uint8_t value) { this->TransferOAM(addr, value); }), "OAM Transfer");
		mainBus.AddRange(0x8000, 0x9FFF, new AddressAction_SimpleMemory(0x9FFF - 0x8000, 0x8000), "VRAM"); // VRAM
		mainBus.AddRange(0xFF80, 0xFFFE, new AddressAction_SimpleMemory(0xFFFE - 0xFF80, 0xFF80), "High Ram");

		mainBus.AddRange(0xFF00, 0xFF7F, new AddressActionNoop(), "Invalid IO Range Ram", 2);

		// RAM
		mainBus.AddRange(MakeSimpleMemoryAddressRange(0xC000, 0xCFFF, "RAM0"));
		mainBus.AddRange(MakeSimpleMemoryAddressRange(0xD000, 0xDFFF, "RAM1"));

		mainBus.AddRange(0xE000, 0xFDFF, new AddressAction_SingleAction(
			[&mainBus](Address addr, uint8_t value)
			{
				mainBus.Set_8b(addr - (0xE000 - 0xC000), value);
			},
			[&mainBus](Address addr) -> uint8_t
			{
				return mainBus.Get_8b(addr - (0xE000 - 0xC000));
			}), "ECHO RAM");

		// OAM FE00	FE9F
		mainBus.AddRange(MakeSimpleMemoryAddressRange(0xFE00, 0xFE9F, "OAM"));

		// Not usable FEA0	FEFF
		mainBus.AddRange(0xFEA0, 0xFEFF, new AddressActionNoop(), "Not usable");

		// DEBUG 
		// AUDIO $FF10	$FF26	DMG	Audio
		mainBus.AddRange(0xFF10, 0xFF26, new AddressAction_SimpleMemory(0xFF26 - 0xFF10, 0xFF10), "Audio");
		// WAVE PATTERN $FF30	$FF3F	DMG	Wave pattern
		mainBus.AddRange(MakeSimpleMemoryAddressRange(0xFF30, 0xFF3F, "Wave Pattern, ToDo"));

		// blargg's
		static uint8_t v = 0;
		mainBus.AddSingle(0xFF01, new AddressAction_DirectValue<uint8_t>(v), "Blargg's writer");
		mainBus.AddSingle(0xFF02, new AddressAction_SingleAction(
			[mainBus](Address addr, uint8_t value)
			{
				if (value == 0x81) {
					std::cout << mainBus.Get_8b(0xFF01);
				}
			}
		), "Blargg's reader");

	}
}