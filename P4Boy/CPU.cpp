#include "CPU.h"
#include "Register.h"

#include <iostream>
#include  <iomanip>
#include <sstream>
#include <fstream>

#include "AddressAction.h"

namespace P4Boy
{
	void Register_Interrupt::MergeEnableInterrupt(MainBus& mainBus, Register_Interrupt const& r)
	{
		auto memory(mainBus.Get_8b(REGISTER_ENABLE_INTERRUPT));
		memory = memory | r.Get();
		mainBus.Set_8b(REGISTER_ENABLE_INTERRUPT, memory);
	}

	void Register_Interrupt::MergeRequestInterrupt(MainBus& mainBus, Register_Interrupt const& r)
	{
		auto memory(mainBus.Get_8b(REGISTER_REQUEST_INTERRUPT));
		memory = memory | r.Get();
		mainBus.Set_8b(REGISTER_REQUEST_INTERRUPT, memory);
	}

	CPU::CPU() : _mainBus(nullptr)
	{
		_instructionManager.ReadInstruction("Opcodes.json");
		/*
		AF.A = 1;
		AF.c = 1;
		AF.h = 1;
		AF.z = 1;

		BC.C = 0x13;
		DE.E = 0xD8;
		HL.H = 0x1;
		HL.L = 0x4D;
		SP = 0xFFFE;
		PC = 0x0100;
		*/
	}
	void CPU::ConnectAddressRange(MainBus& mainBus)
	{
		_mainBus = &mainBus;

		_mainBus->AddSingle(0xFFFF, new AddressAction_DirectValue<Register_Interrupt>(InterruptEnableRegister), "Interrupt Enable Register");
		_mainBus->AddSingle(0xFF0F, new AddressAction_DirectValue<Register_Interrupt>(InterruptRequestRegister), "Interrupt Request Register");
	}

	void CPU::ExecuteNextOpcode()
	{
		static uint16_t previousPC = -1;
		uint16_t opCode = _mainBus->Get_8b(PC);
		PC = PC + 1;
		if (opCode == 0xCB)
		{
			opCode = _mainBus->Get_8b(PC) + 0x100;
			PC = PC + 1;
		}
		CPUInstruction const& instruction = _instructionManager[opCode];
		uint16_t value = 0xFFFF;
		bool showValue = false;
		for (auto const& operand : instruction.operands)
		{
			if (operand.bytes == 1) value = _mainBus->Get_8b(PC);
			if (operand.bytes == 2) value = _mainBus->Get_16b(PC);
			showValue |= operand.bytes > 0;
			PC = PC + operand.bytes;
		}

		if (!instruction.execute) assert(false);
		if (debugShowInstructions)
		{
			std::cout << "0x" << std::setfill('0') << std::setw(5) << std::hex << PC;
			std::cout << " (0x" << std::setfill('0') << std::setw(3) << std::hex << opCode << ")";
			std::cout << std::setfill(' ') << std::setw(4) << instruction.mnemonic;
			for (auto operand : instruction.operands)
			{
				std::cout << std::setfill(' ') << std::setw(4) << operand.name;
			}
			if (showValue) std::cout << " (0x" << std::setfill('0') << std::setw(4) << std::hex << value << ")";
			std::cout << std::endl;
		}
		if (previousPC == PC)
		{
			previousPC = PC;
		}
		previousPC = PC;
		uint8_t cyles = instruction.execute(*this, instruction, value);
	}

	bool CPU::ExecuteInterrupt(Address addr)
	{
		halted = false;

		if (!InterruptMasterEnable)
			return true;

		_mainBus->Set_16b(SP - 2, PC);
		SP = SP - 2;
		PC = addr;
		InterruptMasterEnable = false;
		return true;
	}
	bool CPU::ExecuteInterruptIFN()
	{
		if (_interruptQueued)
			return false;
		if (InterruptEnableRegister.VBlank && InterruptRequestRegister.VBlank)
		{
			_interruptQueued = true;
			InterruptRequestRegister.VBlank = 0;
			return ExecuteInterrupt(0x40);
		}
		else if (InterruptEnableRegister.LCDStat && InterruptRequestRegister.LCDStat)
		{
			_interruptQueued = true;
			InterruptRequestRegister.LCDStat = 0;
			return ExecuteInterrupt(0x48);
		}
		else if (InterruptEnableRegister.Timer && InterruptRequestRegister.Timer)
		{
			_interruptQueued = true;
			InterruptRequestRegister.Timer = 0;
			return ExecuteInterrupt(0x50);
		}
		else if (InterruptEnableRegister.Serial && InterruptRequestRegister.Serial)
		{
			_interruptQueued = true;
			InterruptRequestRegister.Serial = 0;
			return ExecuteInterrupt(0x58);
		}
		else if (InterruptEnableRegister.Joypad && InterruptRequestRegister.Joypad)
		{
			_interruptQueued = true;
			InterruptRequestRegister.Joypad = 0;
			return ExecuteInterrupt(0x60);
		}
		else _interruptQueued = false;
		return false;
	}

	void CPU::Tick()
	{
		if (false)
		{
			static std::fstream ss("logs.txt", std::fstream::out);
			assert(ss.is_open());
			ss << std::uppercase << "A:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(AF.A) << " ";
			ss << std::uppercase << "F:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(AF.F) << " ";
			ss << std::uppercase << "B:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(BC.B) << " ";
			ss << std::uppercase << "C:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(BC.C) << " ";
			ss << std::uppercase << "D:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(DE.D) << " ";
			ss << std::uppercase << "E:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(DE.E) << " ";
			ss << std::uppercase << "H:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(HL.H) << " ";
			ss << std::uppercase << "L:" << std::setfill('0') << std::setw(2) << std::hex << uint32_t(HL.L) << " ";
			ss << std::uppercase << "SP:" << std::setfill('0') << std::setw(4) << std::hex << uint32_t(SP) << " ";
			ss << std::uppercase << "PC:" << std::setfill('0') << std::setw(4) << std::hex << uint32_t(PC) << " ";
			ss << std::uppercase << "PCMEM:";
			ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << uint32_t(_mainBus->Get_8b(PC)) << ",";
			ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << uint32_t(_mainBus->Get_8b(PC + 1)) << ",";
			ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << uint32_t(_mainBus->Get_8b(PC + 2)) << ",";
			ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << uint32_t(_mainBus->Get_8b(PC + 3)) << std::endl;
			ss.flush();
		}
		if (!ExecuteInterruptIFN())
		{
			if (!halted)
			{
				ExecuteNextOpcode();
				_interruptQueued = false;
			}
		}
		halted = false;
	}
}