#pragma once

#include <memory>

#include "Motherboard.h"
#include "Clock.h"
#include "Cartridge.h"
#include "CPU.h"
#include "Joypad.h"
#include "Timer.h"
#include "Sound.h"
#include "LCD.h"
#include "MainBus.h"

namespace P4Boy
{
	class P4Boy
	{
	public:
		P4Boy();
		void Tick();
		void LoadRom(char const* romPath);

		Clock::shared_ptr		GetClock() { return _clock; }
		Motherboard::shared_ptr GetMotherboard() { return _motherboard; }
		Cartridge::shared_ptr	GetCartridge() { return _cartridge; }
		Rom::shared_ptr			GetBootRom() { return _bootRom; }
		CPU::shared_ptr			GetCPU() { return _cpu; }
		Joypad::shared_ptr		GetJoypad() { return _joypad; }
		Timer::shared_ptr		GetTimer() { return _timer; }
		Sound::shared_ptr		GetSound() { return _sound; }
		LCD::shared_ptr			GetLCD() { return _lcd; }
		MainBus&				GetMainBus() { return _motherboard->GetMainBus(); }
	protected:
	private:
		Clock::shared_ptr		_clock;
		Motherboard::shared_ptr	_motherboard;
		Cartridge::shared_ptr	_cartridge;
		Rom::shared_ptr			_bootRom;
		CPU::shared_ptr			_cpu;
		Joypad::shared_ptr		_joypad;
		Timer::shared_ptr		_timer;
		Sound::shared_ptr		_sound;
		LCD::shared_ptr			_lcd;
	};

}

