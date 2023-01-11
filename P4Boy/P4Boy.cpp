#include "P4Boy.h"

namespace P4Boy
{
	P4Boy::P4Boy()
	{
		_clock       = std::unique_ptr<Clock>(new Clock(std::chrono::nanoseconds(238)));

		_cartridge   = Cartridge::shared_ptr(new Cartridge());
		_bootRom     = Rom::shared_ptr(new Rom());
		_cpu         = CPU::shared_ptr(new CPU());
		_motherboard = Motherboard::shared_ptr(new Motherboard());
		_joypad      = Joypad::shared_ptr(new Joypad());
		_timer		 = Timer::shared_ptr(new Timer());
		_sound		 = Sound::shared_ptr(new Sound());
		_lcd		 = LCD::shared_ptr(new LCD());

		_bootRom->LoadRom("dmg_boot.bin");
		_cpu->PC = 0;

		_motherboard->ConnectAddressRange(_motherboard->GetMainBus());
		_cpu->ConnectAddressRange(_motherboard->GetMainBus());
		_lcd->ConnectAddressRange(_motherboard->GetMainBus());
		_joypad->ConnectAddressRange(_motherboard->GetMainBus());
		_timer->ConnectRange(_motherboard->GetMainBus());
	}
	
	void P4Boy::Tick()
	{
		_joypad->Tick();
		_lcd->Tick();
		_cpu->Tick();
		_motherboard->Tick();
		_timer->Tick();
		_clock->Wait();
	}

	void P4Boy::LoadRom(char const * romPath)
	{
		_cartridge->LoadRom(romPath);
		_cartridge->ConnectAddressRange(_bootRom, _motherboard->GetMainBus());
	}
}