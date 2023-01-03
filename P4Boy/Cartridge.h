#pragma once
#include <string>
#include <vector>

#include "Utility.h"

#include "MainBus.h"

namespace P4Boy
{
	class Cartridge
	{
	public:
		typedef Ptr<Cartridge> ptr;
		typedef std::shared_ptr<Cartridge> shared_ptr;
	public:
		Cartridge() : _bootRomEnabled(true) {}
		
		void SetBootRomEnabled(bool value) { _bootRomEnabled = value; }
		bool IsBootRomEnabled() { return _bootRomEnabled; }

		void LoadRom(char const* romPath);
		void ConnectAddressRange(Cartridge::shared_ptr& bootRom, MainBus& mainBus);

		uint8_t Read(Address addr) const;
	protected:
	private:
		bool						_bootRomEnabled;
		std::string					_romPath;
		std::vector<unsigned char>	_romCode;
	};

}

