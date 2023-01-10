#pragma once
#include <string>
#include <vector>

#include "Utility.h"

#include "MainBus.h"

namespace P4Boy
{
	class Rom
	{
	public:
		typedef Ptr<Rom> ptr;
		typedef std::shared_ptr<Rom> shared_ptr;
	public:
		void LoadRom(char const* romPath);
		uint8_t ReadRom(Address addr) const;
	private:
		std::string				_romPath = "";
		std::vector<uint8_t>	_romCode;

	};

	class Cartridge : public Rom
	{
	public:
		typedef Ptr<Cartridge> ptr;
		typedef std::shared_ptr<Cartridge> shared_ptr;
		typedef Rom super;
	public:
		Cartridge() : super() {}

		void LoadRom(char const* romPath);
		void ConnectAddressRange(Cartridge::shared_ptr& bootRom, MainBus& mainBus);
		void SetupMBC();

		uint8_t ReadRam(Address addr) const;
		void WriteRam(Address addr, uint8_t data);
	protected:
	private:
		std::vector<unsigned char>	_ram;
	};
}

