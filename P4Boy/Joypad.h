#pragma once

#include <thread>
#include <atomic>

#include "Utility.h"
#include "Register.h"

namespace P4Boy
{
	class MainBus;

	class Joypad
	{
	public:
		typedef Ptr<Joypad> ptr;

		Joypad();
		void ConnectAddressRange(MainBus& mainBus);

		void Tick();
		void UpdateState();
	public:
		std::atomic_uint8_t* _joypadState = nullptr;
		std::atomic_uint8_t _joypadStateDirection = 0xFF;
		std::atomic_uint8_t _joypadStateAction = 0xFF;
	protected:
	private:
		std::thread _joypadThread;
	};
}

