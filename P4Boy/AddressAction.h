#pragma once

#include <functional>
#include <vector>

#include "Utility.h"

namespace P4Boy
{
	class AddressAction
	{
	public:
		typedef Ptr<AddressAction> ptr;
	public:
		virtual ~AddressAction() {}
		virtual uint8_t Get(Address addr) { return const_cast<const AddressAction*>(this)->Get(addr); }
		virtual uint8_t Get(Address addr) const = 0;
		virtual void Set(Address addr, uint8_t) = 0;
	};

	class AddressActionNoop : public AddressAction
	{
	public:
		virtual uint8_t Get(Address addr) const { return 0xFF; }
		virtual void Set(Address addr, uint8_t) { }
	};

	class AddressAction_SingleAction : public AddressAction
	{
	public:
		typedef std::function<void(Address, uint8_t)> SetFunction;
		typedef std::function<uint8_t(Address)> GetFunction;

		AddressAction_SingleAction(SetFunction set) : _setFunc(set) {}
		AddressAction_SingleAction(GetFunction get) : _getFunc(get) {}
		AddressAction_SingleAction(SetFunction set, GetFunction get) : _setFunc(set), _getFunc(get) {}
		virtual uint8_t Get(Address addr) const { return _getFunc(addr); }
		virtual void Set(Address addr, uint8_t value) { _setFunc(addr, value); }
	private:
		SetFunction _setFunc;
		GetFunction _getFunc;
	};

	template<typename valueType>
	class AddressAction_DirectValue : public AddressAction
	{
	public:
		AddressAction_DirectValue(valueType& value) : _value(value) {}
		virtual uint8_t Get(Address addr) const { return _value; }
		virtual void Set(Address addr, uint8_t value) { _value = value; }
	private:
		valueType& _value;
	};

	class AddressAction_SimpleMemory : public AddressAction
	{
	public:
		AddressAction_SimpleMemory(size_t size, Address start) : _memory(size + 1), _start(start) {}
		virtual uint8_t Get(Address addr) const { return _memory[addr - _start]; }
		virtual void Set(Address addr, uint8_t value) 
		{
			_memory[addr - _start] = value; 
		}
	private:
		Address _start;
		std::vector<uint8_t> _memory;
	};
}

