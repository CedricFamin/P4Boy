#include "Cartridge.h"

#include <fstream>
#include <iterator>
#include <vector>

#include "AddressRange.h"

namespace P4Boy
{
	class AddressAction_Cartridge : public AddressAction
	{
	public:
		AddressAction_Cartridge(Cartridge::ptr::shared cartridge, Cartridge::ptr::shared bootRom)
			: _cartridge(cartridge)
			, _bootRom(bootRom)
		{

		}

		Cartridge* SelectCartridge(Address addr) const
		{
			if (_bootRom->IsBootRomEnabled() && addr <= 0xFF)
			{
				return _bootRom.get();
			}
			return _cartridge.get();
		}
		virtual uint8_t Get(Address addr) const
		{
			return SelectCartridge(addr)->Read(addr);
		}
		virtual void Set(Address addr, uint8_t) { }
	private:
		Cartridge::ptr::shared _cartridge;
		Cartridge::ptr::shared _bootRom;
	};

	void Cartridge::LoadRom(char const* romPath)
	{
		_romPath = romPath;
		std::ifstream input(romPath, std::ios::binary);
		_romCode = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});
	}

	void Cartridge::ConnectAddressRange(Cartridge::ptr::shared& bootRom, MainBus& mainBus)
	{
		auto bootRomEnabler = new AddressAction_SingleAction([this, bootRom](Address addr, uint8_t value)
		{
			this->SetBootRomEnabled(!(value != 0));
			bootRom->SetBootRomEnabled(!(value != 0));
		});
		auto readRom = new AddressAction_Cartridge(Cartridge::ptr::shared(this), bootRom);

		mainBus.AddRange(AddressSingle(0xFF50, bootRomEnabler));
		mainBus.AddRange(new AddressRange(0x0000, 0x7FFF, readRom));
	}

	uint8_t Cartridge::Read(Address addr) const
	{
		return _romCode[addr];
	}
}