#include "P4Boy.h"

namespace P4Boy
{
	P4Boy::P4Boy()
	{
		_clock       = std::unique_ptr<Clock>(new Clock(std::chrono::nanoseconds(238)));

		_cartridge   = std::shared_ptr<Cartridge>(new Cartridge());
		_bootRom     = std::shared_ptr<Cartridge>(new Cartridge());
		_cpu         = std::shared_ptr<CPU>(new CPU());
		_motherboard = std::shared_ptr<Motherboard>(new Motherboard());
		_joypad      = Joypad::ptr::shared(new Joypad());
		_timer		 = Timer::ptr::shared(new Timer());
		_sound		 = Sound::ptr::shared(new Sound());
		_lcd		 = LCD::ptr::shared(new LCD());

		_bootRom->LoadRom("dmg_boot.bin");
		_cpu->PC = 0;

		_motherboard->ConnectAddressRange(_motherboard->GetMainBus());
		_cartridge->ConnectAddressRange(_bootRom, _motherboard->GetMainBus());
		_cpu->ConnectAddressRange(_motherboard->GetMainBus());
		_lcd->ConnectAddressRange(_motherboard->GetMainBus());
		_joypad->ConnectAddressRange(_motherboard->GetMainBus());
	}
	
	void P4Boy::Tick()
	{
		_joypad->Tick();
		_lcd->Tick();
		_cpu->Tick();
		_motherboard->Tick();
		_clock->Wait();
	}

	void P4Boy::LoadRom(char const * romPath)
	{
		_cartridge->LoadRom(romPath);
	}
}