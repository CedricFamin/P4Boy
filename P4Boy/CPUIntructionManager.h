#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cassert>

namespace P4Boy
{
	class CPU;

	struct CPUInstruction
	{
		typedef uint8_t (*ExecuteFunc)(CPU&, CPUInstruction const&, uint16_t);

		enum class OperandID
		{
			Unknown,

			A, B, C, D, E, H, L,
			AF, BC, DE, HL, SP, PC,

			a8, d8, s8, r8,
			d16, a16, s16
		};
		struct Operand
		{
			std::string name = "";
			OperandID id;
			bool immediate = false;
			uint8_t bytes = 0;
		};
		uint8_t opcode = 0;
		uint8_t bytes = 0;
		bool immediate = false;
		std::string mnemonic = "";
		std::vector<uint8_t> cycles;
		std::vector<Operand> operands;

		ExecuteFunc execute = nullptr;
	};

	class CPUIntructionManager
	{
	public:
		void ReadInstruction(std::string const& filePath);

		CPUInstruction const & operator[](uint16_t opcode) const { return _instructions[opcode]; }
		void RegisterExecute(std::vector<uint16_t> const&& opcodes, CPUInstruction::ExecuteFunc const& func);
		void RegisterExecute(std::vector<uint16_t> const&& opcodes, bool prefixed, CPUInstruction::ExecuteFunc const& func);
	protected:
	private:
		std::vector<CPUInstruction> _instructions;
	};
}

