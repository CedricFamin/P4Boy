#include "Cartridge.h"

#include <fstream>
#include <iterator>
#include <vector>
#include <cassert>
#include <array>

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

	namespace {
		std::string ReadString(uint16_t addrStart, uint16_t addrEnd, Rom const & rom)
		{
			std::string value;
			for (int i = addrStart; i < addrEnd; ++i)
			{
				value += rom.Read(i);
			}
			return value;
		}

		uint8_t RamSizeToNbRamBanks(uint8_t ramSize)
		{
			const std::array<int, 6> ramSizes = { 0, 1, 1, 4, 16, 8 };

			if (ramSize >= ramSizes.size())
			{
				assert(false);
				return 0;
			}
			return ramSizes[ramSize];
		}

		int RomSizeToNbRomBanks(uint8_t romSize)
		{
			const std::array<int, 14> romSizes = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 72, 80, 90 };

			if (romSize >= romSizes.size())
			{
				assert(false);
				return 2;
			}

			return romSizes[romSize];
		}

		MBCInterface* MBCBuilder(uint8_t mbcNumber, Cartridge& cartridge, Rom& bootRom)
		{
			switch (mbcNumber) {
			case 0: //No MBC
				return new NoMBC(cartridge, bootRom);
			case 1: //MBC1
			case 2: //MBC1+RAM
			case 3: //MBC1+RAM+BATTERY
				return new MBC1(cartridge, bootRom);
			case 5: //MBC2
			case 6: //MBC2+BATTERY
				break;
			case 8: //ROM+RAM
			case 9: //ROM+RAM+BATTERY
				break;
			case 0xB: //MMM01
			case 0xC: //MMM01+RAM
			case 0xD: //MMM01+RAM+BATTERY
				break;
			case 0xF: //MBC3+TIMER+BATTERY
			case 0x10: //MBC3+TIMER+RAM+BATTERY
			case 0x11: //MBC3
			case 0x12: //MBC3+RAM
			case 0x13: //MBC3+RAM+BATTERY
				break;
			case 0x15: //MBC4
			case 0x16: //MBC4+RAM
			case 0x17: //MBC4+RAM+BATTERY
				break;
			case 0x19: //MBC5
			case 0x1A: //MBC5+RAM
			case 0x1B: //MBC5+RAM+BATTERY
			case 0x1C: //MBC5+RUMBLE
			case 0x1D: //MBC5+RUMBLE+RAM
			case 0x1E: //MBC5+RUMBLE+RAM+BATTERY
				break;
			case 0xFC: //POCKET CAMERA
				break;
			case 0xFD: //BANDAI TAMA5
				break;
			case 0xFE: //HuC3
				break;
			case 0xFF: //HuC1+RAM+BATTERY
				break;
			default:
				break;
			}
			assert(false);
			return nullptr;
		}
	}
	void Cartridge::LoadRom(char const* romPath)
	{
		_rom.LoadRom(romPath);

		for (int i = ADDR_NINTENDO_LOGO_START; i <= ADDR_NINTENDO_LOGO_END; ++i)
		{
			_nintendoLogo.push_back(_rom.Read(i));
		}

		_title					= ReadString(ADDR_TITLE_START, ADDR_TITLE_END, _rom);
		_manufacturerCode		= ReadString(ADDR_MANUFACTURER_CODE_START, ADDR_MANUFACTURER_CODE_END, _rom);
		_newLicenseeCode		= ReadString(ADDR_NEW_LICENSEE_CODE_START, ADDR_NEW_LICENSEE_CODE_END, _rom);
		_GCBFlag				= _rom.Read(ADDR_GCB_FLAG);
		_SGBFlag				= _rom.Read(ADDR_SGB_FLAG);
		_cartridgeType			= _rom.Read(ADDR_CARTRIDGE_TYPE);
		_romSize				= _rom.Read(ADDR_ROM_SIZE);
		_ramSize				= _rom.Read(ADDR_RAM_SIZE);
		_destinationCode		= _rom.Read(ADDR_DESTINATION_CODE);
		_oldLicenseeCode		= _rom.Read(ADDR_OLD_LICENSEE_CODE);
		_maskRomVersionNumber	= _rom.Read(ADDR_MASK_ROM_VERSION_NUMBER);
		_headerChecksum			= _rom.Read(ADDR_HEADER_CHECKSUM);
		_globalCheckSum			= (_rom.Read(ADDR_GLOBAL_CHECKSUM_START) << 8) | _rom.Read(ADDR_GLOBAL_CHECKSUM_END);

		_romBankNb = RomSizeToNbRomBanks(_romSize);
		_ramBankNb = RamSizeToNbRamBanks(_ramSize);

		if (_ramBankNb >= 1) _ram.resize(_ramBankNb * BANK_RAM_SIZE);
	}

	void Cartridge::CreateMBC(Rom::shared_ptr& bootRom)
	{
		_mbc = MBCBuilder(_cartridgeType, *this, *bootRom.get());
	}

	void Cartridge::ConnectAddressRange(MainBus& mainBus)
	{
		auto bootRomEnabler = new AddressAction_SingleAction(
			[this](Address addr, uint8_t value) 
			{ 
				this->_mbc->SetBootRomEnabled(value == 0);
			}
		);

		auto readRom = new AddressAction_SingleAction(
			[this](Address addr, uint8_t value) { this->_mbc->write(addr, value); },
			[this](Address addr) -> uint8_t { return this->_mbc->read(addr); }
		);

		mainBus.AddSingle(0xFF50, bootRomEnabler, "Boot Rom Enabler");
		mainBus.AddRange(0x0000, 0x7FFF, readRom, "Cartridge - ROM");
		mainBus.AddRange(0xA000, 0xBFFF, readRom, "Cartridge - RAM");
	}

	uint8_t Cartridge::ReadRam(Address addr) const
	{
		return _ram[addr];
	}

	void Cartridge::WriteRam(Address addr, uint8_t data)
	{
		_ram[addr] = data;
	}

	void Cartridge::Reset()
	{
		_mbc->Reset();
	}
}