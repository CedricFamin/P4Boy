#pragma once

#include "Register.h"
#include "MainBus.h"
#include "CPUIntructionManager.h"

namespace P4Boy
{
	class Register_AF : public Register_16b
	{
	public:
		Register_AF() : A(*this), F(*this), z(*this, 7), n(*this, 6), h(*this, 5), c(*this, 4) {}

		inline Register_AF& operator=(uint16_t value) { _value = value; return *this; }

		SubRegister_8b_High A;
		SubRegister_8b_Low F;
		SubRegister_1b<Register_16b> z;
		SubRegister_1b<Register_16b> n;
		SubRegister_1b<Register_16b> h;
		SubRegister_1b<Register_16b> c;
	};

	class Register_BC : public Register_16b
	{
	public:
		Register_BC() : C(*this), B(*this) {}

		inline Register_BC& operator=(uint16_t value) { _value = value; return *this; }

		SubRegister_8b_Low C;
		SubRegister_8b_High B;
	};

	class Register_DE : public Register_16b
	{
	public:
		Register_DE() : D(*this), E(*this) {}

		inline Register_DE& operator=(uint16_t value) { _value = value; return *this; }

		SubRegister_8b_Low E;
		SubRegister_8b_High D;
	};

	class Register_HL : public Register_16b
	{
	public:
		Register_HL() : H(*this), L(*this) {}

		inline Register_HL& operator=(uint16_t value) { _value = value; return *this; }

		SubRegister_8b_Low L;
		SubRegister_8b_High H;
	};

	typedef Register_16b Register_SP;
	typedef Register_16b Register_PC;

	class Register_Interrupt : public Register_8b
	{
		/*
		* Bit 0: VBlank   Interrupt Enable(INT $40)  (1 = Enable)
		* Bit 1 : LCD STAT Interrupt Enable(INT $48)  (1 = Enable)
		* Bit 2 : Timer    Interrupt Enable(INT $50)  (1 = Enable)
		* Bit 3 : Serial   Interrupt Enable(INT $58)  (1 = Enable)
		* Bit 4 : Joypad   Interrupt Enable(INT $60)  (1 = Enable)
		*/

		/*
		* Bit 0 : VBlank   Interrupt Request(INT $40)  (1 = Request)
		* Bit 1 : LCD STAT Interrupt Request(INT $48)  (1 = Request)
		* Bit 2 : Timer    Interrupt Request(INT $50)  (1 = Request)
		* Bit 3 : Serial   Interrupt Request(INT $58)  (1 = Request)
		* Bit 4 : Joypad   Interrupt Request(INT $60)  (1 = Request)
		*/
	public:
		static constexpr Address const REGISTER_REQUEST_INTERRUPT = 0xFF0F;
		static constexpr Address const REGISTER_ENABLE_INTERRUPT = 0xFFFF;

		static void MergeEnableInterrupt(MainBus& mainBus, Register_Interrupt const & r);
		static void MergeRequestInterrupt(MainBus& mainBus, Register_Interrupt const& r);
	public:
		Register_Interrupt() : VBlank(*this, 0), LCDStat(*this, 1), Timer(*this, 2), Serial(*this, 3), Joypad(*this, 4) {}
		Register_Interrupt(uint8_t v) : VBlank(*this, 0), LCDStat(*this, 1), Timer(*this, 2), Serial(*this, 3), Joypad(*this, 4) { _value = v;}

		inline Register_Interrupt& operator=(uint8_t value) { _value = value; return *this; }

		SubRegister_1b<Register_8b>  VBlank;
		SubRegister_1b<Register_8b>  LCDStat;
		SubRegister_1b<Register_8b>  Timer;
		SubRegister_1b<Register_8b>  Serial;
		SubRegister_1b<Register_8b>  Joypad;
	};

	class CPU
	{
	public:
		typedef Ptr<CPU> ptr;
		typedef std::shared_ptr<CPU> shared_ptr;
	public:
		CPU();
		void Tick();
		void ExecuteNextOpcode();
		bool ExecuteInterruptIFN();
		bool ExecuteInterrupt(Address addr);

		Register_AF AF;
		Register_BC BC;
		Register_DE DE;
		Register_HL HL;
		Register_SP SP;
		Register_PC PC;

		bool InterruptMasterEnable = false;
		Register_Interrupt InterruptEnableRegister;
		Register_Interrupt InterruptRequestRegister;
		bool halted = false;

		void ConnectAddressRange(MainBus& mainBus);
		inline MainBus& Memory() { return *_mainBus; }

		std::atomic_bool debugShowInstructions;
	protected:
	private:
		bool _interruptQueued = false;
		MainBus* _mainBus = nullptr;
		CPUIntructionManager _instructionManager;
		uint8_t _nextInstructionCycle = 0;
	};
}


