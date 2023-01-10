#pragma once
#include "CartRidge.h"

namespace P4Boy
{
    class MBCInterface
    {
    public:
        MBCInterface(Cartridge& cartRidge, Rom& bootrom) : cartRidge(cartRidge), bootrom(bootrom) {}
        virtual ~MBCInterface() {}

        virtual uint8_t read(uint16_t address) = 0;
        virtual void write(uint16_t address, uint8_t data) = 0;

        void SetBootRomEnabled(bool enable) 
        {
            if (bootRomEnabled)
                bootRomEnabled = enable; 
        }
    protected:
        Cartridge& cartRidge;
        Rom& bootrom;

        bool bootRomEnabled = true;
    };

    class NoMBC : public MBCInterface
    {
    public:
        NoMBC(Cartridge& cartRidge, Rom& bootrom);
        ~NoMBC();

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
    };


    class MBC1 : public MBCInterface
    {
    public:
        MBC1(Cartridge& cartRidge, Rom& bootrom);
        ~MBC1();

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

    private:
        uint8_t romBank;
        uint8_t ramBank;
        uint8_t bankMode;
        bool ramEnable;
    };

}

