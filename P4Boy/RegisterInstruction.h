#pragma once

#include "CPUIntructionManager.h"
#include "CPU.h"

namespace P4Boy
{
	uint8_t GetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v);
	void SetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t addr, uint8_t v);
	void SetValue_8b(CPU& cpu, CPUInstruction::Operand const& operand, uint8_t v);

	uint16_t GetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v);
	void SetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t addr, uint16_t v);
	void SetValue_16b(CPU& cpu, CPUInstruction::Operand const& operand, uint16_t v);

	inline bool CheckHalfCarryFlag_8b(uint8_t v1, int16_t v2)
	{
		if (v2 >= 0)
			return (((int16_t(v1) & 0xF) + (v2 & 0xF)) & 0x10) == 0x10;
		return (((int16_t(v1) & 0xF) - (-v2 & 0xF)) < 0x0);
	}

	inline bool CheckHalfCarryFlag_16b(uint16_t v1, int32_t v2)
	{
		if (v2 >= 0)
			return (((v1 & 0xFFF) + (v2 & 0xFFF)) & 0x1000) == 0x1000;
		return (((uint16_t(v1) & 0xFFF) - (uint16_t(-v2) & 0xFFF)) == 0xF0);
	}

	//
	bool RegisterBinaryFunctions(CPUIntructionManager& manager);
	bool RegisterControlFlowFunction(CPUIntructionManager& manager);
	bool RegisterLoadFunctions(CPUIntructionManager& manager);
	bool RegisterMathsFunctions(CPUIntructionManager& manager);

	inline void RegisterAllFunctions(CPUIntructionManager& manager)
	{
		RegisterBinaryFunctions(manager);
		RegisterControlFlowFunction(manager);
		RegisterLoadFunctions(manager);
		RegisterMathsFunctions(manager);
	}
}