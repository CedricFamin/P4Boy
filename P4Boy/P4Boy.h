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

namespace P4Boy
{
	class P4Boy
	{
	public:
		P4Boy();
		void Tick();
		void LoadRom(char const* romPath);
	protected:
	private:
		std::shared_ptr<Clock>			_clock;
		std::shared_ptr<Motherboard>	_motherboard;
		std::shared_ptr<Cartridge>		_cartridge;
		std::shared_ptr<Cartridge>		_bootRom;
		std::shared_ptr<CPU>			_cpu;
		Joypad::ptr::shared				_joypad;
		Timer::ptr::shared				_timer;
		Sound::ptr::shared				_sound;
		LCD::ptr::shared				_lcd;
	};

}

