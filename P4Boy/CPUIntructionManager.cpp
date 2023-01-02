#include "CPUIntructionManager.h"

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "RegisterInstruction.h"

using json = nlohmann::json;

namespace P4Boy
{
	namespace {
		CPUInstruction::Operand DecodeOperand(json const& json)
		{
			CPUInstruction::Operand operand;
			operand.immediate = json["immediate"].get<bool>();
			operand.name = json["name"].get<std::string>();
			operand.bytes = json.contains("bytes") ? json["bytes"].get<uint8_t>() : 0;

			operand.id = CPUInstruction::OperandID::Unknown;

			if (operand.name == std::string("A")) { operand.id = CPUInstruction::OperandID::A; }
			else if (operand.name == std::string("B")) { operand.id = CPUInstruction::OperandID::B; }
			else if (operand.name == std::string("C")) { operand.id = CPUInstruction::OperandID::C; }
			else if (operand.name == std::string("D")) { operand.id = CPUInstruction::OperandID::D; }
			else if (operand.name == std::string("E")) { operand.id = CPUInstruction::OperandID::E; }
			else if (operand.name == std::string("H")) { operand.id = CPUInstruction::OperandID::H; }
			else if (operand.name == std::string("L")) { operand.id = CPUInstruction::OperandID::L; }
			else if (operand.name == std::string("AF")) { operand.id = CPUInstruction::OperandID::AF; }
			else if (operand.name == std::string("BC")) { operand.id = CPUInstruction::OperandID::BC; }
			else if (operand.name == std::string("DE")) { operand.id = CPUInstruction::OperandID::DE; }
			else if (operand.name == std::string("HL")) { operand.id = CPUInstruction::OperandID::HL; }
			else if (operand.name == std::string("SP")) { operand.id = CPUInstruction::OperandID::SP; }
			else if (operand.name == std::string("PC")) { operand.id = CPUInstruction::OperandID::PC; }
			else if (operand.name == std::string("a8")) { operand.id = CPUInstruction::OperandID::a8; }
			else if (operand.name == std::string("a16")) { operand.id = CPUInstruction::OperandID::a16; }
			else if (operand.name == std::string("d8")) { operand.id = CPUInstruction::OperandID::d8; }
			else if (operand.name == std::string("d16")) { operand.id = CPUInstruction::OperandID::d16; }
			else if (operand.name == std::string("s8")) { operand.id = CPUInstruction::OperandID::s8; }
			else if (operand.name == std::string("r8")) { operand.id = CPUInstruction::OperandID::r8; }
			return operand;
		}

		CPUInstruction DecodeInstruction(uint8_t opcode, json const & json)
		{
			CPUInstruction instruction;

			instruction.opcode = opcode;
			instruction.bytes = json["bytes"].get<uint8_t>();
			instruction.immediate = json["immediate"].get<bool>();
			instruction.mnemonic = json["mnemonic"].get<std::string>();
			instruction.cycles = json["cycles"].get<std::vector<uint8_t>>();

			for (auto operand : json["operands"])
			{
				instruction.operands.push_back(DecodeOperand(operand));
			}

			return instruction;
		}
	}

	void CPUIntructionManager::ReadInstruction(std::string const& filePath)
	{
		_instructions.resize(0x200);

		std::ifstream ifs(filePath);
		json instructionsDef = json::parse(ifs);

		for (auto instruction_kv : instructionsDef["unprefixed"].items())
		{
			auto opcode = stoi(instruction_kv.key(), nullptr, 16);
			auto instructionDef = instruction_kv.value();

			CPUInstruction instruction = DecodeInstruction(opcode, instructionDef);
			_instructions[opcode] = instruction;
			
		}

		for (auto instruction_kv : instructionsDef["cbprefixed"].items())
		{
			auto opcode = stoi(instruction_kv.key(), nullptr, 16);
			auto instructionDef = instruction_kv.value();

			CPUInstruction instruction = DecodeInstruction(opcode + 0x100, instructionDef);
			_instructions[opcode + 0x100] = instruction;
		}

		RegisterAllFunctions(*this);
	}

	void CPUIntructionManager::RegisterExecute(std::vector<uint16_t> const&& opcodes, bool prefixed, CPUInstruction::ExecuteFunc const& func)
	{
		for (auto opcode : opcodes)
		{
			if (prefixed) 
				opcode += 0x100;

			assert(!_instructions[opcode].execute); // Already registered opcode
			_instructions[opcode].execute = func;
		}
	}

	void CPUIntructionManager::RegisterExecute(std::vector<uint16_t> const&& opcodes, CPUInstruction::ExecuteFunc const& func)
	{
		RegisterExecute(std::move(opcodes), false, func);
	}
}