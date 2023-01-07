#include "RegisterInstruction.h"

namespace P4Boy
{
	namespace {
		uint8_t jump_if(CPU& cpu, CPUInstruction const & instr, uint16_t v, bool if_condition)
		{
			if (if_condition)
			{
				cpu.PC = v;
				return instr.cycles[0];
			}
			return instr.cycles[1];
		}

		uint8_t relative_jump_if(CPU& cpu, CPUInstruction const & instr, uint16_t v, bool if_condition)
		{
			if (if_condition)
			{
				int8_t signed_v = (v ^ 0x80) - 0x80;
				cpu.PC = cpu.PC + signed_v;
				return instr.cycles[0];
			}
			return instr.cycles[1];
		}

		uint8_t call_if(CPU& cpu, CPUInstruction const & instr, uint16_t v, bool if_condition)
		{
			if (if_condition)
			{
				cpu.Memory().Set_16b(cpu.SP - 2, cpu.PC);
				cpu.SP = cpu.SP - 2;
				cpu.PC = v;
				return instr.cycles[0];
			}
			return instr.cycles[1];
		}

		uint8_t ret_if(CPU& cpu, CPUInstruction const & instr, uint16_t v, bool if_condition)
		{
			if (if_condition)
			{
				cpu.PC = cpu.Memory().Get_16b(cpu.SP);
				cpu.SP = cpu.SP + 2;
				return instr.cycles[0];
			}
			return instr.cycles[1];
		}

		uint8_t push(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint16_t v1 = GetValue_16b(cpu, instr.operands.front(), v);
			cpu.Memory().Set_16b(cpu.SP - 2, v1);
			cpu.SP = cpu.SP - 2;
			return instr.cycles[0];
		}

		uint8_t pop(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			v = cpu.Memory().Get_16b(cpu.SP);
			cpu.SP = cpu.SP + 2;
			SetValue_16b(cpu, instr.operands.front(), v);
			return instr.cycles[0];
		}

		uint8_t rst(CPU& cpu, CPUInstruction const& instr, uint16_t address)
		{
			cpu.Memory().Set_16b(cpu.SP - 2, cpu.PC);
			cpu.SP = cpu.SP - 2;
			cpu.PC = address;
			return instr.cycles[0];
		}

	}
	bool RegisterControlFlowFunction(CPUIntructionManager& manager)
	{
		// NOP
		manager.RegisterExecute({ 0x00 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return instr.cycles[0]; });

		// JP NZ, A16
		manager.RegisterExecute({ 0xC2 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return jump_if(cpu, instr, v, cpu.AF.z == 0); });

		// JP 16
		manager.RegisterExecute({ 0xC3 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return jump_if(cpu, instr, v, true); });

		// JP Z, a16
		manager.RegisterExecute({ 0xCA }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return jump_if(cpu, instr, v, cpu.AF.z == 1); });

		// JP NC, a16
		manager.RegisterExecute({ 0xD2 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return jump_if(cpu, instr, v, cpu.AF.c == 0); });

		// JP C, a16
		manager.RegisterExecute({ 0xDA }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return jump_if(cpu, instr, v, cpu.AF.c == 1); });

		// JP HL
		manager.RegisterExecute({ 0xE9 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				return jump_if(cpu, instr, cpu.HL, true); 
			});

		// JR s8
		manager.RegisterExecute({ 0x18 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return relative_jump_if(cpu, instr, v, true); });

		// JR NZ, s8
		manager.RegisterExecute({ 0x20 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return relative_jump_if(cpu, instr, v, cpu.AF.z == 0); });

		// JR Z, s8
		manager.RegisterExecute({ 0x28 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return relative_jump_if(cpu, instr, v, cpu.AF.z == 1); });

		// JR NC, s8
		manager.RegisterExecute({ 0x30 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return relative_jump_if(cpu, instr, v, cpu.AF.c == 0); });

		// JR C, s8
		manager.RegisterExecute({ 0x38 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return relative_jump_if(cpu, instr, v, cpu.AF.c == 1); });

		// CALL a16
		manager.RegisterExecute({ 0xCD }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return call_if(cpu, instr, v, true); });

		// CALL NZ, a16
		manager.RegisterExecute({ 0xC4 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return call_if(cpu, instr, v, cpu.AF.z == 0); });

		// CALL NC, a16
		manager.RegisterExecute({ 0xD4 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return call_if(cpu, instr, v, cpu.AF.c == 0); });

		// CALL Z, a16
		manager.RegisterExecute({ 0xCC }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return call_if(cpu, instr, v, cpu.AF.z == 1); });

		// CALL C, a16
		manager.RegisterExecute({ 0xDC }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return call_if(cpu, instr, v, cpu.AF.c == 1); });

		// RET
		manager.RegisterExecute({ 0xC9 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return ret_if(cpu, instr, v, true); });

		// RET Z
		manager.RegisterExecute({ 0xC8 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return ret_if(cpu, instr, v, cpu.AF.z == 1); });

		// RET C
		manager.RegisterExecute({ 0xD8 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return ret_if(cpu, instr, v, cpu.AF.c == 1); });

		// RET NZ
		manager.RegisterExecute({ 0xC0 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return ret_if(cpu, instr, v, cpu.AF.z == 0); });

		// RET NC
		manager.RegisterExecute({ 0xD0 }, [](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return ret_if(cpu, instr, v, cpu.AF.c == 0); });

		// PUSH X
		manager.RegisterExecute({ 0xC5, 0xD5, 0xE5, 0xF5 }, push);

		// POP X
		manager.RegisterExecute({ 0xC1, 0xD1, 0xE1, 0xF1 }, pop);

		// CP X
		manager.RegisterExecute({ 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xFE },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.front(), v);

				int16_t r = int16_t(v1) - int16_t(v2);
				cpu.AF.z = r == 0;
				cpu.AF.n = 1;
				cpu.AF.c = r < 0;
				cpu.AF.h = CheckHalfCarryFlag_8b(v1, -v2);
				return instr.cycles[0]; 
			});

		// DI
		manager.RegisterExecute({ 0xF3 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.InterruptMasterEnable = false;
				return instr.cycles[0];
			});

		// EI
		manager.RegisterExecute({ 0xFB },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.InterruptMasterEnable = true;
				return instr.cycles[0];
			});

		// RETI
		manager.RegisterExecute({ 0xD9 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.InterruptMasterEnable = true;
				return ret_if(cpu, instr, v, true);
			});

		// RST 0
		manager.RegisterExecute({ 0xC7 }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x00); });

		// RST 2
		manager.RegisterExecute({ 0xD7 }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x10); });

		// RST 4
		manager.RegisterExecute({ 0xE7 }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x20); });

		// RST 6
		manager.RegisterExecute({ 0xF7 }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x30); });

		// RST 1
		manager.RegisterExecute({ 0xCF }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x08); });

		// RST 3
		manager.RegisterExecute({ 0xDF }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x18); });

		// RST 5
		manager.RegisterExecute({ 0xEF }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x28); });

		// RST 7
		manager.RegisterExecute({ 0xFF }, [](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return rst(cpu, instr, 0x38); });

		// HALT
		manager.RegisterExecute({ 0x76 },
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				cpu.halted = true;
				return instr.cycles[0];
			});
		return true;
	}
}