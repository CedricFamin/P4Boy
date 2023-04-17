#include "Timer.h"

#include "AddressAction.h"
#include "CPU.h"

namespace P4Boy
{
	Timer::Timer()
	{

	}
	void Timer::ConnectRange(MainBus& mainBus)
	{
		_mainBus = &mainBus;

		mainBus.AddDirectAccess(0xFF04, _div, "Timer - DIV");
		mainBus.AddDirectAccess(0xFF05, _tima, "Timer - TIMA");
		mainBus.AddDirectAccess(0xFF06, _modulo, "Timer - Modula");
		mainBus.AddDirectAccess(0xFF07, _tac, "Timer - TAC");
	}
	void Timer::Tick()
	{
		_div = _div + 1;
		if (_tac.Enable == 0)
			return;

		_tick += 1;
		if (_tick > _tac.GetClock())
		{
			_tick = 0;
			if (_tima + 1 > 0xFF)
			{
				_tima = _modulo;
				Register_Interrupt interruptFlag = _mainBus->Get_8b(0xFF0F);
				interruptFlag.Timer = 0x1;
				_mainBus->Set_8b(0xFF0F, interruptFlag);
			}
			else
			{
				_tima = _tima + 1;
			}
		}
		
	}
}