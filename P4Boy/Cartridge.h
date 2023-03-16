#pragma once
#include <string>
#include <vector>

#include "Utility.h"

#include "MainBus.h"

namespace P4Boy
{
	class MBCInterface;
	class Rom
	{
	public:
		typedef Ptr<Rom> ptr;
		typedef std::shared_ptr<Rom> shared_ptr;
	public:
		void LoadRom(char const* romPath);
		uint8_t Read(Address addr) const { return _romCode[addr]; }
	private:
		std::string				_romPath = "";
		std::vector<uint8_t>	_romCode;

	};

	class Cartridge
	{
	public:
		typedef Ptr<Cartridge> ptr;
		typedef std::shared_ptr<Cartridge> shared_ptr;

		static const uint16_t BANK_RAM_SIZE =  8 * 1024;
		static const uint16_t BANK_ROM_SIZE = 32 * 1024;

		static const uint16_t ADDR_NINTENDO_LOGO_START = 0x104;
		static const uint16_t ADDR_NINTENDO_LOGO_END = 0x134;
		static const uint16_t ADDR_TITLE_START = 0x134;
		static const uint16_t ADDR_TITLE_END = 0x143;
		static const uint16_t ADDR_MANUFACTURER_CODE_START = 0x13F;
		static const uint16_t ADDR_MANUFACTURER_CODE_END = 0x142;
		static const uint16_t ADDR_NEW_LICENSEE_CODE_START = 0x144;
		static const uint16_t ADDR_NEW_LICENSEE_CODE_END = 0x145;
		static const uint16_t ADDR_GCB_FLAG = 0x143;
		static const uint16_t ADDR_SGB_FLAG = 0x146;
		static const uint16_t ADDR_CARTRIDGE_TYPE = 0x147;
		static const uint16_t ADDR_ROM_SIZE = 0x148;
		static const uint16_t ADDR_RAM_SIZE = 0x149;
		static const uint16_t ADDR_DESTINATION_CODE = 0x14A;
		static const uint16_t ADDR_OLD_LICENSEE_CODE = 0x14B;
		static const uint16_t ADDR_MASK_ROM_VERSION_NUMBER = 0x14C;
		static const uint16_t ADDR_HEADER_CHECKSUM = 0x14D;
		static const uint16_t ADDR_GLOBAL_CHECKSUM_START = 0x14E;
		static const uint16_t ADDR_GLOBAL_CHECKSUM_END = 0x14F;
	public:
		Cartridge() {}

		void LoadRom(char const* romPath);
		void ConnectAddressRange(Rom::shared_ptr& bootRom, MainBus& mainBus);

		uint8_t ReadRom(Address addr) const { return _rom.Read(addr); }
		uint8_t ReadRam(Address addr) const;
		void WriteRam(Address addr, uint8_t data);

		uint8_t GetRomBankNB() const { return _romBankNb; }
		uint8_t GetRamBankNB() const { return _ramBankNb; }

		const std::vector<uint8_t>& GetNintendoLogo() const { return _nintendoLogo; }
		const std::string& GetTitle() const { return _title; }
		const std::string& GetManufacturerCode() const { return _manufacturerCode; }
		uint8_t GetGCBFlag() const { return _GCBFlag; }
		const std::string& GetNewLicenseeCode() const { return _newLicenseeCode; }
		uint8_t GetSGBFlag() const { return _SGBFlag; }
		uint8_t GetCartridgeType() const { return _cartridgeType; }
		uint8_t GetRomSize() const { return _romSize; }
		uint8_t GetRamSize() const { return _ramSize; }
		uint8_t GetDestinationCode() const { return _destinationCode; }
		uint8_t GetOldLicenseeCode() const { return _oldLicenseeCode; }
		uint8_t GetMaskRomVersionNumber() const { return _maskRomVersionNumber; }
		uint8_t GetHeaderChecksum() const { return _headerChecksum; }
		uint32_t GetGlobalChecksum() const { return _globalCheckSum; }

		MBCInterface const* GetMbcInterface() const { return _mbc; }
	protected:
	private:
		uint8_t					_romBankNb = 0;
		Rom						_rom;
		uint8_t					_ramBankNb = 0;
		std::vector<uint8_t>	_ram;

		// Cartridge header
		std::vector<uint8_t>	_nintendoLogo;
		std::string				_title = "";
		std::string				_manufacturerCode = "";
		uint8_t					_GCBFlag = 0;
		std::string				_newLicenseeCode = "";
		uint8_t					_SGBFlag = 0;
		uint8_t					_cartridgeType = 0;
		uint8_t					_romSize = 0;
		uint8_t					_ramSize = 0;
		uint8_t					_destinationCode = 0;
		uint8_t					_oldLicenseeCode = 0;
		uint8_t					_maskRomVersionNumber = 0;
		uint8_t					_headerChecksum = 0;
		uint32_t				_globalCheckSum = 0;

		MBCInterface* _mbc = nullptr;
	};
}

