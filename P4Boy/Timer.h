#pragma once
#include "Utility.h"
#include "Register.h"
#include "MainBus.h"

namespace P4Boy
{
	typedef Register_8b Register_Div;
	typedef Register_8b Register_Tima;
	typedef Register_8b Register_Modulo;

	class Register_TAC : public Register_8b
	{
		/*
		* Bit  2   - Timer Enable
		* Bits 1-0 - Input Clock Select
		*            00: CPU Clock / 1024 (DMG, SGB2, CGB Single Speed Mode:   4096 Hz, SGB1:   ~4194 Hz, CGB Double Speed Mode:   8192 Hz)
		*            01: CPU Clock / 16   (DMG, SGB2, CGB Single Speed Mode: 262144 Hz, SGB1: ~268400 Hz, CGB Double Speed Mode: 524288 Hz)
		*            10: CPU Clock / 64   (DMG, SGB2, CGB Single Speed Mode:  65536 Hz, SGB1:  ~67110 Hz, CGB Double Speed Mode: 131072 Hz)
		*            11: CPU Clock / 256  (DMG, SGB2, CGB Single Speed Mode:  16384 Hz, SGB1:  ~16780 Hz, CGB Double Speed Mode:  32768 Hz)
		* 
		* The “Timer Enable” bit only affects the timer (TIMA). The divider (DIV) is always counting.
		*/
	public:

		Register_TAC() :Enable(*this, 2), InputClock(*this, 0) {}


		inline Register_TAC& operator=(uint8_t value) { _value = value; return *this; }

		SubRegister_1b<Register_TAC> Enable;
		SubRegister_2b<Register_TAC> InputClock;
	};

	class Timer
	{
	public:
		typedef Ptr<Timer> ptr;

		typedef std::shared_ptr<Timer> shared_ptr;

	public:
		Timer();
		void ConnectRange(MainBus& mainBus);
		void Tick();
	protected:
	private:
		MainBus*		_mainBus = nullptr;;
		Register_Div	_div;
		Register_Modulo _modulo;
		Register_Tima	_tima;
		Register_TAC	_tac;
	};
}


