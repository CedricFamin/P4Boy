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

		mainBus.AddSingle(0xFF04, new AddressAction_DirectValue<Register_Div>(_div), "Timer - DIV");
		mainBus.AddSingle(0xFF05, new AddressAction_DirectValue<Register_Tima>(_tima), "Timer - TIMA");
		mainBus.AddSingle(0xFF06, new AddressAction_DirectValue<Register_Modulo>(_modulo), "Timer - Modula");
		mainBus.AddSingle(0xFF07, new AddressAction_DirectValue<Register_TAC>(_tac), "Timer - TAC");
	}
	void Timer::Tick()
	{
		_div = _div + 1;
		if (_tac.Enable == 0)
			return;

		Register_Tima previousTima = _tima;
		_tima = _tima + 1;
		if (previousTima > _tima)
		{
			_tima = _modulo;
			Register_Interrupt interruptFlag = _mainBus->Get_8b(0xFF0F);
			interruptFlag.Timer = 0x1;
			_mainBus->Set_8b(0xFF0F, interruptFlag);
		}
	}
}