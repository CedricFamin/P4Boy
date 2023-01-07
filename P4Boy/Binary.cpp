#include "RegisterInstruction.h"

namespace P4Boy
{
	namespace {

		uint8_t bit_generic(CPU& cpu, CPUInstruction const & instr, uint16_t v, uint8_t bit)
		{
			uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
			uint8_t r = ((v1 >> bit) ^ 0x1) & 0x1;

			cpu.AF.z = r;
			cpu.AF.n = 0;
			cpu.AF.h = 1;
			return instr.cycles[0];
		}

		uint8_t res_generic(CPU& cpu, CPUInstruction const& instr, uint16_t v, uint8_t bit)
		{
			uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
			uint8_t mask = ~(1 << bit);
			SetValue_8b(cpu, instr.operands.back(), v, v1 & mask);
			return instr.cycles[0];
		}

		uint8_t set_generic(CPU& cpu, CPUInstruction const& instr, uint16_t v, uint8_t bit)
		{
			uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
			v1 = v1 | (1 << bit);
			SetValue_8b(cpu, instr.operands.back(), v, v1);
			return instr.cycles[0];
		}

	}
	bool RegisterBinaryFunctions(CPUIntructionManager& manager)
	{
		// XOR X
		manager.RegisterExecute({ 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xEE }, 
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.back(), v);

				uint8_t r = v1 ^ v2;
				cpu.AF.A = r;

				cpu.AF.z = r == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = 0;
				return instr.cycles[0];
			});

		// OR X
		manager.RegisterExecute({ 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xF6 },
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.back(), v);

				uint8_t r = v1 | v2;
				cpu.AF.A = r;

				cpu.AF.z = r == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = 0;
				return instr.cycles[0];
			});

		// AND X
		manager.RegisterExecute({ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xE6 },
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t v2 = GetValue_8b(cpu, instr.operands.back(), v);

				uint8_t r = v1 & v2;
				cpu.AF.A = r;

				cpu.AF.z = r == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 1;
				cpu.AF.c = 0;
				return instr.cycles[0];
			});

		// SWAP X
		manager.RegisterExecute({ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = GetValue_8b(cpu, instr.operands.front(), v);

				uint8_t r = ((v1 & 0xF0) >> 4) | ((v1 & 0x0F) << 4);
				SetValue_8b(cpu, instr.operands.front(), r);

				cpu.AF.z = r == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = 0;
				return instr.cycles[0];
			});


		// RLA
		manager.RegisterExecute({ 0x17 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t c = (v1 >> 7) & 0x1;
				v1 = (v1 << 1) | cpu.AF.c;
				cpu.AF.A = v1;

				cpu.AF.z = 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = c;
				return instr.cycles[0];
			});

		// RLCA
		manager.RegisterExecute({ 0x07 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t c = (v1 >> 7) & 0x1;
				v1 = (v1 << 1) | c;
				cpu.AF.A = v1;

				cpu.AF.z = 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = c;
				return instr.cycles[0];
			});

		// RRA
		manager.RegisterExecute({ 0x1F },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t c = v1 & 0x1;
				v1 = (v1 >> 1) | (cpu.AF.c << 7);
				cpu.AF.A = v1;

				cpu.AF.z = 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = c;
				return instr.cycles[0];
			});

		// RRCA
		manager.RegisterExecute({ 0x0F },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t v1 = cpu.AF.A;
				uint8_t c = v1 & 0x1;
				v1 = (v1 >> 1) | (c << 7);
				cpu.AF.A = v1;

				cpu.AF.z = 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = c;
				return instr.cycles[0];
			});

		// CPL
		manager.RegisterExecute({ 0x2F },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.AF.A = ~cpu.AF.A & 0xFF;
				cpu.AF.n = 1;
				cpu.AF.h = 1;
				return instr.cycles[0];
			});

		// CCF
		manager.RegisterExecute({ 0x3F },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = !cpu.AF.c;
				return instr.cycles[0];
			});

		// SCF
		manager.RegisterExecute({ 0x37 },
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				cpu.AF.c = 1;
				return instr.cycles[0];
			});

		// BIT 0, X
		manager.RegisterExecute({ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47 }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 0); });

		// BIT 1, X
		manager.RegisterExecute({ 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 1); });

		// BIT 2, X
		manager.RegisterExecute({ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57 }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 2); });

		// BIT 3, X
		manager.RegisterExecute({ 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 3); });

		// BIT 4, X
		manager.RegisterExecute({ 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67 }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 4); });

		// BIT 5, X
		manager.RegisterExecute({ 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 5); });

		// BIT 6, X
		manager.RegisterExecute({ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77 }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 6); });

		// BIT 7, X
		manager.RegisterExecute({ 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t { return bit_generic(cpu, instr, v, 7); });

		// RL X
		manager.RegisterExecute({ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x017 }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t t = GetValue_8b(cpu, instr.operands.front(), v);
				uint8_t c = (t & 0b10000000) >> 7;
				t = (t << 1) | cpu.AF.c;
				cpu.AF.c = c;
				cpu.AF.z = (t & 0xFF) == 0;
				cpu.AF.h = 0;
				cpu.AF.n = 0;
				SetValue_8b(cpu, instr.operands.front(), t & 0xFF);
				return instr.cycles[0];
			});

		// RLC X
		manager.RegisterExecute({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				uint8_t t = GetValue_8b(cpu, instr.operands.front(), v);
				uint8_t c = (t >> 7) & 0x1;
				t = (t << 1) | c;
				cpu.AF.c = c;
				cpu.AF.z = (t & 0xFF) == 0;
				cpu.AF.h = 0;
				cpu.AF.n = 0;
				SetValue_8b(cpu, instr.operands.front(), t & 0xFF);
				return instr.cycles[0];
			});

		// RRC X
		manager.RegisterExecute({ 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t t = GetValue_8b(cpu, instr.operands.front(), v);
				uint8_t c = t & 0x1;
				t = (t >> 1) | (c << 7);
				cpu.AF.c = c;
				cpu.AF.z = (t & 0xFF) == 0;
				cpu.AF.h = 0;
				cpu.AF.n = 0;
				SetValue_8b(cpu, instr.operands.front(), t & 0xFF);
				return instr.cycles[0];
			});

		// RR X
		manager.RegisterExecute({ 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t t = GetValue_8b(cpu, instr.operands.front(), v);
				uint8_t c = t & 0x1;
				t = (t >> 1) | (cpu.AF.c << 7);
				cpu.AF.c = c;
				cpu.AF.z = (t & 0xFF) == 0;
				cpu.AF.h = 0;
				cpu.AF.n = 0;
				SetValue_8b(cpu, instr.operands.front(), t & 0xFF);
				return instr.cycles[0];
			});

		// SRL X
		manager.RegisterExecute({ 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F }, true,
			[](CPU& cpu, CPUInstruction const & instr, uint16_t v) -> uint8_t
			{
				uint8_t t = GetValue_8b(cpu, instr.operands.front(), v);
				uint8_t c = t & 0x1;
				t = (t >> 1) & 0b01111111;
				cpu.AF.c = c;
				cpu.AF.z = (t & 0xFF) == 0;
				cpu.AF.h = 0;
				cpu.AF.n = 0;
				SetValue_8b(cpu, instr.operands.front(), t & 0xFF);
				return instr.cycles[0];
			});

		// DAA
		manager.RegisterExecute({ 0x27 },
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t
			{
				if (!cpu.AF.n)
				{
					if (cpu.AF.c || cpu.AF.A > 0x99) {
						cpu.AF.A = cpu.AF.A + 0x60;
						cpu.AF.c = 1;
					}
					if (cpu.AF.h || ((cpu.AF.A & 0x0F) > 0x09)) { cpu.AF.A = cpu.AF.A + 0x6; }
				}
				else
				{
					if (cpu.AF.c) cpu.AF.A = cpu.AF.A - 0x60;
					if (cpu.AF.h) cpu.AF.A = cpu.AF.A - 0x6;
				}
				cpu.AF.z = cpu.AF.A == 0;
				cpu.AF.h = 0;
				return instr.cycles[0];
			});

		// RES B, X
		manager.RegisterExecute({ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 0); });
		manager.RegisterExecute({ 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 1); });
		manager.RegisterExecute({ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 2); });
		manager.RegisterExecute({ 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 3); });
		manager.RegisterExecute({ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 4); });
		manager.RegisterExecute({ 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 5); });
		manager.RegisterExecute({ 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 6); });
		manager.RegisterExecute({ 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return res_generic(cpu, instr, v, 7); });

		// SRA X
		manager.RegisterExecute({ 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t {
				uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
				cpu.AF.c = v1 & 0x1;
				v1 = (v1 >> 1) | (v1 & 0x1 << 7);
				cpu.AF.z = v1 == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				SetValue_8b(cpu, instr.operands.back(), v, v1);
				return instr.cycles[0];
			});

		// SLA X
		manager.RegisterExecute({ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t {
				uint8_t v1 = GetValue_8b(cpu, instr.operands.back(), v);
				cpu.AF.c = (v1 & (0x1 << 7)) > 0;
				v1 <<= 1;
				cpu.AF.z = v1 == 0;
				cpu.AF.n = 0;
				cpu.AF.h = 0;
				SetValue_8b(cpu, instr.operands.back(), v, v1);
				return instr.cycles[0];
			});

		// SET B, X
		manager.RegisterExecute({ 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 0); });
		manager.RegisterExecute({ 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 1); });
		manager.RegisterExecute({ 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 2); });
		manager.RegisterExecute({ 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 3); });
		manager.RegisterExecute({ 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 4); });
		manager.RegisterExecute({ 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 5); });
		manager.RegisterExecute({ 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7 }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 6); });
		manager.RegisterExecute({ 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF }, true,
			[](CPU& cpu, CPUInstruction const& instr, uint16_t v) -> uint8_t { return set_generic(cpu, instr, v, 7); });
		return true;
	}
}