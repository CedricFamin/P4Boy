#include <cassert>
#include "MBC.h"

namespace P4Boy
{
    NoMBC::NoMBC(Cartridge& cartRidge, Rom& bootrom) :
        MBCInterface(cartRidge, bootrom)
    {
    }

    NoMBC::~NoMBC()
    {
    }

    uint8_t NoMBC::read(uint16_t address)
    {
        if (address <= 0xFF && bootRomEnabled)
        {
            return bootrom.Read(address);
        }
        return cartRidge.ReadRom(address);
    }

    void NoMBC::write(uint16_t address, uint8_t data)
    {
        // do nothing
    }

    MBC1::MBC1(Cartridge& cartridge, Rom& bootrom) :
        MBCInterface(cartridge, bootrom),
        romBank(1),
        ramBank(0),
        ramEnable(false)
    {
    }

    MBC1::~MBC1()
    {
    }

    uint8_t MBC1::read(uint16_t address)
    {
        if (address <= 0xFF && bootRomEnabled)
            return bootrom.Read(address);
        if (address <= 0x3FFF)
            return cartRidge.ReadRom(address);
        if (address >= 0x4000 && address <= 0x7FFF)
            return cartRidge.ReadRom(address - 0x4000 + romBank * 0x4000);
        if (address >= 0xA000 && address <= 0xBFFF)
            return ramEnable ? cartRidge.ReadRam(address - 0xA000 + ramBank * 0x2000) : 0xFF;

        assert(false);
        return 0x00;
    }

    void MBC1::write(uint16_t address, uint8_t data)
    {
        if (address < 0x2000)
        {
            ramEnable = (data & 0x0F) == 0x0A;
        }
        else if (address < 0x4000)
        {
            romBank = (data & 0x1F);
            if (romBank == 0x00 || romBank == 0x20 || romBank == 0x40 || romBank == 0x60)
                romBank += 1;
            romBank %= cartRidge.GetRomBankNB();
        }
        else if (address < 0x6000)
        {
            if (bankMode == BankMode::SIMPLE)
                ramBank = (data & 0x3) % cartRidge.GetRamBankNB();
            else
                romBank = ((data & 0x03) << 5) | (romBank & 0b00011111);
            
        }
        else if (address < 0x8000)
        {
            bankMode = BankMode::SIMPLE;
            if (data & 0x1)
                bankMode = BankMode::ADVANCED;
        }
        else if (address < 0xA000)
        {
            if (ramEnable) cartRidge.WriteRam((address - 0xA000) + ramBank * 0x2000, data);
        }
    }
}