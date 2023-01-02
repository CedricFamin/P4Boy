#include "RegisterInstruction.h"

namespace P4Boy
{
	uint8_t GetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v)
	{
		uint16_t value = v;
		if (operand.id == CPUInstruction::OperandID::A) { value = cpu.AF.A; }
		else if (operand.id == CPUInstruction::OperandID::B) { value = cpu.BC.B; }
		else if (operand.id == CPUInstruction::OperandID::C) { value = cpu.BC.C; }
		else if (operand.id == CPUInstruction::OperandID::D) { value = cpu.DE.D; }
		else if (operand.id == CPUInstruction::OperandID::E) { value = cpu.DE.E; }
		else if (operand.id == CPUInstruction::OperandID::H) { value = cpu.HL.H; }
		else if (operand.id == CPUInstruction::OperandID::L) { value = cpu.HL.L; }
		else if (operand.id == CPUInstruction::OperandID::BC) { assert(operand.immediate == false); value = cpu.Memory().Get_8b(cpu.BC); }
		else if (operand.id == CPUInstruction::OperandID::DE) { assert(operand.immediate == false); value = cpu.Memory().Get_8b(cpu.DE); }
		else if (operand.id == CPUInstruction::OperandID::HL) { assert(operand.immediate == false); value = cpu.Memory().Get_8b(cpu.HL); }
		else if (operand.id == CPUInstruction::OperandID::a8) { value = v; }
		else if (operand.id == CPUInstruction::OperandID::d8) { value = v; }
		else if (operand.id == CPUInstruction::OperandID::a16) { assert(operand.immediate == false); value = cpu.Memory().Get_8b(v); }
		else assert(false);
		return uint8_t(value);
	}

	void SetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t addr, uint8_t v)
	{
		if (operand.immediate)
		{
			if (operand.id == CPUInstruction::OperandID::A) { cpu.AF.A = v; }
			else if (operand.id == CPUInstruction::OperandID::B) { cpu.BC.B = v; }
			else if (operand.id == CPUInstruction::OperandID::C) { cpu.BC.C = v; }
			else if (operand.id == CPUInstruction::OperandID::D) { cpu.DE.D = v; }
			else if (operand.id == CPUInstruction::OperandID::E) { cpu.DE.E = v; }
			else if (operand.id == CPUInstruction::OperandID::H) { cpu.HL.H = v; }
			else if (operand.id == CPUInstruction::OperandID::L) { cpu.HL.L = v; }
			else assert(false);
		}
		else
		{
			if (operand.id == CPUInstruction::OperandID::BC) { addr = cpu.BC; }
			else if (operand.id == CPUInstruction::OperandID::DE) { addr = cpu.DE; }
			else if (operand.id == CPUInstruction::OperandID::HL) { addr = cpu.HL; }
			else if (operand.bytes == 0) assert(false);
			cpu.Memory().Set_8b(addr, v);
		}
	}

	void SetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint8_t v)
	{
		assert(operand.bytes == 0);
		SetValue_8b(cpu, operand, 0x00, v);
	}

	uint16_t GetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v)
	{
		uint16_t value = v;
		if (operand.id == CPUInstruction::OperandID::A) { value = cpu.AF.A; }
		else if (operand.id == CPUInstruction::OperandID::B) { value = cpu.BC.B; }
		else if (operand.id == CPUInstruction::OperandID::C) { value = cpu.BC.C; }
		else if (operand.id == CPUInstruction::OperandID::D) { value = cpu.DE.D; }
		else if (operand.id == CPUInstruction::OperandID::E) { value = cpu.DE.E; }
		else if (operand.id == CPUInstruction::OperandID::H) { value = cpu.HL.H; }
		else if (operand.id == CPUInstruction::OperandID::L) { value = cpu.HL.L; }
		else if (operand.id == CPUInstruction::OperandID::BC) { value = cpu.BC; }
		else if (operand.id == CPUInstruction::OperandID::DE) { value = cpu.DE; }
		else if (operand.id == CPUInstruction::OperandID::HL) { value = cpu.HL; }
		else if (operand.id == CPUInstruction::OperandID::SP) { value = cpu.SP; }
		else if (operand.id == CPUInstruction::OperandID::AF) { value = cpu.AF; }
		else if (operand.bytes == 0) assert(false);

		if (operand.immediate == false)
		{
			value = cpu.Memory().Get_16b(value);
		}
		return value;
	}

	void SetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t addr, uint16_t v)
	{
		if (operand.immediate)
		{
			if (operand.id == CPUInstruction::OperandID::AF) { cpu.AF = v & 0xFFF0; }
			else if (operand.id == CPUInstruction::OperandID::BC) { cpu.BC = v; }
			else if (operand.id == CPUInstruction::OperandID::DE) { cpu.DE = v; }
			else if (operand.id == CPUInstruction::OperandID::HL) { cpu.HL = v; }
			else if (operand.id == CPUInstruction::OperandID::SP) { cpu.SP = v; }
			else if (operand.id == CPUInstruction::OperandID::PC) { cpu.PC = v; }
			else assert(false);
		}
		else
		{

			if (operand.id == CPUInstruction::OperandID::BC) { addr = cpu.BC; }
			else if (operand.id == CPUInstruction::OperandID::DE) { addr = cpu.DE; }
			else if (operand.id == CPUInstruction::OperandID::HL) { addr = cpu.HL; }
			else if (operand.id == CPUInstruction::OperandID::SP) { addr = cpu.SP; }
			else if (operand.bytes == 0) assert(false);
			cpu.Memory().Set_16b(addr, v);
		}

	}

	void SetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v)
	{
		assert(operand.bytes == 0);
		SetValue_16b(cpu, operand, 0x00, v);
	}
}