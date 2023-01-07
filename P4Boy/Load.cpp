#include "RegisterInstruction.h"
#include <iostream>
#include <iomanip>

namespace P4Boy
{
	namespace {

		uint8_t load_8b_generic(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint8_t value = GetValue_8b(cpu, instr.operands.back(), v);
			SetValue_8b(cpu, instr.operands.front(), v, value);
			return instr.cycles[0];
		}

		uint8_t load_8b_hl_incr_decr(CPU& cpu, CPUInstruction const & instr, uint16_t v, int8_t hlupdate)
		{
			auto cycles = load_8b_generic(cpu, instr, v);
			cpu.HL = cpu.HL + hlupdate;
			return cycles;
		}

		uint8_t load_16b_generic(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint16_t value = GetValue_16b(cpu, instr.operands.back(), v);
			SetValue_16b(cpu, instr.operands.front(), v, value);
			return instr.cycles[0];
		}

		uint8_t load_from_8b_around_0xFF00(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			Address address = GetValue_8b(cpu, instr.operands.back(), v);
			address += 0xFF00;
			cpu.AF.A = cpu.Memory().Get_8b(address);
			return instr.cycles[0];
		}

		uint8_t load_to_8b_around_0xFF00(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			Address address = GetValue_8b(cpu, instr.operands.front(), v);
			address += 0xFF00;
			cpu.Memory().Set_8b(address, cpu.AF.A);
			return instr.cycles[0];
		}
	}
	bool RegisterLoadFunctions(CPUIntructionManager& manager)
	{
		// LD x, x (8 bit)
		manager.RegisterExecute({ 0x02, 0x06, 0x0A, 0x0E, 0x12, 0x16, 0x1A, 0x1E, 0x26, 0x2E, 0x36, 0x3E, 0x40, 0x41, 0x42
			, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52
			, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62
			, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72
			, 0x73, 0x74, 0x75, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0xEA, 0xFA }
		, load_8b_generic);

		// LD (HL+), A, LD A (HL+)
		manager.RegisterExecute({ 0x22, 0x2A },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return load_8b_hl_incr_decr(cpu, instr, v, +1); });

		// LD (HL-), A, LD A (HL-)
		manager.RegisterExecute({ 0x32, 0x3A },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return load_8b_hl_incr_decr(cpu, instr, v, -1); });

		// LD x, x (16 bit)
		manager.RegisterExecute({ 0x01, 0x11, 0x21, 0x31, 0x08, 0xF9 }, load_16b_generic);

		// LD A, x (8 bit), address + 0xFF
		manager.RegisterExecute({ 0xF0, 0xF2 }, load_from_8b_around_0xFF00);

		// LD (X), A (8 bit), address + 0xFF
		manager.RegisterExecute({ 0xE0, 0xE2 }, load_to_8b_around_0xFF00);

		// Add the 8 - bit signed operand s8(values - 128 to + 127) to the stack pointer SP, and store the result in register pair HL.
		manager.RegisterExecute({ 0xF8 }, 
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				int8_t sv = (uint8_t(v) ^ 0x80) - 0x80;
				uint16_t r = cpu.SP + sv;
				//std::cout << "0xF8 " << std::hex << cpu.SP << " " << int32_t(sv) << std::endl;
				//std::cout << "h:" << CheckHalfCarryFlag_16b(cpu.SP, sv) << " c:" << ((r > 0xFFFF) | (r < 0)) << std::endl;
				cpu.HL = r;
				
				cpu.AF.z = 0;
				cpu.AF.n = 0;
				cpu.AF.h = (r & 0xF) < (cpu.SP & 0xF);
				cpu.AF.c = (r & 0xFF) < (cpu.SP & 0xFF);
				return instr.cycles[0];
			});

		return true;
	}
}