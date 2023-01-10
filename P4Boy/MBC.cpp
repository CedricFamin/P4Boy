#include "MBC.h"

namespace P4Boy
{
    NoMBC::NoMBC(Cartridge& cartRidge, Cartridge& bootrom) :
        MBCInterface(cartRidge, bootrom)
    {
    }

    NoMBC::~NoMBC()
    {
    }

    uint8_t NoMBC::read(uint16_t address)
    {
        return cartRidge.ReadRom(address);
    }

    void NoMBC::write(uint16_t address, uint8_t data)
    {
        // do nothing
    }

    MBC1::MBC1(Cartridge& cartridge, Cartridge& bootrom) :
        MBCInterface(cartridge, bootrom),
        romBank(1),
        ramBank(0),
        bankMode(0),
        ramEnable(false)
    {
    }

    MBC1::~MBC1()
    {
    }

    uint8_t MBC1::read(uint16_t address)
    {
        if (address < 0x4000)
        {
            if (address <= 0xFF && bootRomEnabled)
            {
                return bootrom.ReadRom(address);
            }
            return cartRidge.ReadRom(address);
        }
        else if (address < 0x8000)
        {
            return cartRidge.ReadRom((address - 0x4000) + (romBank - 1) * 0x4000);
        }
        else if (address < 0xA000)
        {
            return 0xFF;
        }
        else if (address < 0xC000)
        {
            if (ramEnable)
            {
                return cartRidge.ReadRam((address - 0xA000) + ramBank * 0x2000);
            }
            else
            {
                return 0xFF;
            }
        }
        else
        {
            return 0xFF;
        }
    }

    void MBC1::write(uint16_t address, uint8_t data)
    {
        if (address < 0x2000)
        {
            ramEnable = (data & 0x0F) == 0x0A;
        }
        else if (address < 0x4000)
        {
            romBank = (data & 0x1F) | (romBank & 0x60);
        }
        else if (address < 0x6000)
        {
            if (bankMode)
            romBank = (data & 0x03) << 5 | (romBank & 0x1F);
        }
        else if (address < 0x8000)
        {
            bankMode = data & 0x1;
        }
        else if (address < 0xA000)
        {
            if (ramEnable)
            {
                cartRidge.WriteRam((address - 0xA000) + ramBank * 0x2000, data);
            }
        }
    }
}