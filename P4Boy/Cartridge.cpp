#include "Cartridge.h"

#include <fstream>
#include <iterator>
#include <vector>

#include "AddressRange.h"

#include "MBC.h"

namespace P4Boy
{
	void Rom::LoadRom(char const* romPath)
	{
		_romPath = romPath;
		std::ifstream input(romPath, std::ios::binary);
		_romCode = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});
	}

	void Cartridge::LoadRom(char const* romPath)
	{
		super::LoadRom(romPath);

		_ram.resize(32 * 1024 * 1024);
	}

	uint8_t Rom::ReadRom(Address addr) const
	{
		return _romCode[addr];
	}

	void Cartridge::ConnectAddressRange(Cartridge::shared_ptr& bootRom, MainBus& mainBus)
	{
		MBCInterface* mbsc = new MBC1(*this, *bootRom.get());
		auto bootRomEnabler = new AddressAction_SingleAction(
			[mbsc](Address addr, uint8_t value) 
			{ 
				mbsc->SetBootRomEnabled(value == 0); 
			}
		);

		auto readRom = new AddressAction_SingleAction(
			[mbsc](Address addr, uint8_t value) { mbsc->write(addr, value); },
			[mbsc](Address addr) -> uint8_t { return mbsc->read(addr); }
		);

		mainBus.AddSingle(0xFF50, bootRomEnabler, "Boot Rom Enabler");
		mainBus.AddRange(0x0000, 0x7FFF, readRom, "Cartridge - ROM");
		mainBus.AddRange(0xA000, 0xBFFF, readRom, "Cartridge - RAM");
	}

	void Cartridge::SetupMBC()
	{
		auto mbcType = ReadRom(0x147);
	}

	uint8_t Cartridge::ReadRam(Address addr) const
	{
		return _ram[addr];
	}

	void Cartridge::WriteRam(Address addr, uint8_t data)
	{
		_ram[addr] = data;
	}
}