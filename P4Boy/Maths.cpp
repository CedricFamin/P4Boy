#include "RegisterInstruction.h"

namespace P4Boy
{
	namespace {

		uint8_t increment_generic_8b(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint8_t v1 = GetValue_8b(cpu, instr.operands.front(), v);

			cpu.AF.h = CheckHalfCarryFlag(v1, 1);
			v1 += 1;
			cpu.AF.z = v1 == 0;
			cpu.AF.n = 0;

			SetValue_8b(cpu, instr.operands.front(), v1);
			return instr.cycles[0];
		}

		uint8_t decrement_generic_8b(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint8_t v1 = GetValue_8b(cpu, instr.operands.front(), v);

			cpu.AF.h = CheckHalfCarryFlag(v1, -1);
			v1 -= 1;
			cpu.AF.z = v1 == 0;
			cpu.AF.n = 1;

			SetValue_8b(cpu, instr.operands.front(), v1);
			return instr.cycles[0];
		}

		uint8_t increment_generic_16b(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint16_t v1 = GetValue_16b(cpu, instr.operands.front(), v);
			v1 += 1;
			SetValue_16b(cpu, instr.operands.front(), v1);
			return instr.cycles[0];
		}

		uint8_t decrement_generic_16b(CPU& cpu, CPUInstruction const & instr, uint16_t v)
		{
			uint16_t v1 = GetValue_16b(cpu, instr.operands.front(), v);
			v1 -= 1;
			SetValue_16b(cpu, instr.operands.front(), v1);
			return instr.cycles[0];
		}
	}
	bool RegisterMathsFunctions(CPUIntructionManager& manager)
	{
		// INC X (8 bit)
		manager.RegisterExecute({ 0x04, 0x14, 0x24, 0x34, 0x0C, 0x1C, 0x2C, 0x3C }, increment_generic_8b);

		// DEC X (8 bit)
		manager.RegisterExecute({ 0x05, 0x15, 0x25, 0x35, 0x0D, 0x1D, 0x2D, 0x3D }, decrement_generic_8b);

		// INC X (16 bit)
		manager.RegisterExecute({ 0x03, 0x13, 0x23, 0x33 }, increment_generic_16b);

		// DEC X (16 bit)
		manager.RegisterExecute({ 0x0B, 0x1B, 0x2B, 0x3B }, decrement_generic_16b);

		// ADD X (8 bit)
		manager.RegisterExecute({ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0xC6 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.back(), v);

				int16_t r = v1 + v2;
				cpu.AF.z = (r & 0xFF) == 0;
				cpu.AF.n = 0;
				cpu.AF.h = CheckHalfCarryFlag(v1, v2);
				cpu.AF.c = (r > 0xFF);

				cpu.AF.A = r & 0xFF;

				return instr.cycles[0];
			});

		// SUB X (8 bit)
		manager.RegisterExecute({ 0xD6, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.back(), v);

				int16_t r = v1 - v2;
				cpu.AF.z = (r & 0xFF) == 0;
				cpu.AF.n = 1;
				cpu.AF.h = CheckHalfCarryFlag(v1, -v2);
				cpu.AF.c = (r < 0);

				cpu.AF.A = r & 0xFF;

				return instr.cycles[0];
			});

		// ADD X (16 bit)
		manager.RegisterExecute({ 0x09, 0x19, 0x29, 0x39 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint16_t v1 = GetValue_16b(cpu, instr.operands.front(), v);
				uint16_t v2 = GetValue_16b(cpu, instr.operands.back(), v);

				uint16_t r = v1 + v2;
				cpu.AF.n = 0;
				cpu.AF.h = CheckHalfCarryFlag(v1, v2);
				cpu.AF.c = (uint32_t(v1 + v2) > 0xFFFF);

				SetValue_16b(cpu, instr.operands.front(), r & 0xFFFF);

				return instr.cycles[0];
			});

		// ADC X
		manager.RegisterExecute({ 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0xCE },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
				uint8_t v2 = cpu.AF.c;
				uint8_t v3 = cpu.AF.A;

				uint8_t r = v1 + v2 + v3;
				cpu.AF.A = r;

				cpu.AF.z = (r & 0xFFFF) == 0;
				cpu.AF.n = 0;
				cpu.AF.h = (((v1 & 0xF) + (v2 & 0xF) + (v3 & 0xF)) > 0xF);
				cpu.AF.c = (uint32_t(v1 + v2 + v3) > 0xFF);

				return instr.cycles[0];
			});

		
		// ADD SP, s8
		manager.RegisterExecute({ 0xE8 },
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				int32_t v1 = (v ^ 0x80) - 0x80;
				int32_t v2 = cpu.SP;

				cpu.SP = v1 + cpu.SP;

				cpu.AF.h = (((v1 & 0xF) + (v2 & 0xF)) > 0xF);
				cpu.AF.c = (((v1 & 0xFF) + (v2 & 0xFF)) > 0xFF);
				cpu.AF.z = 0;
				cpu.AF.n = 0;

				return instr.cycles[0];
			});
		return true;
	}
}