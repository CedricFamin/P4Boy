#pragma once

#include <string>

#include "Utility.h"
#include "AddressAction.h"

namespace P4Boy
{
	class AddressRange
	{
	public:
		AddressRange(Address from, Address to, AddressAction* action, std::string const & name)
			: _memRangeFrom(from)
			, _memRangeTo(to)
			, _action(action)
			, _name(name)

		{

		}

		~AddressRange() {}

		inline Address From() const { return _memRangeFrom; }
		inline Address To() const { return _memRangeTo; }

		inline bool IsIn(Address addr) const { return _memRangeFrom <= addr && addr <= _memRangeTo; }

		inline uint8_t Get_8b(Address addr) const { return _action->Get(addr); }
		inline void Set_8b(Address addr, uint8_t value) { _action->Set(addr, value); }

		inline uint8_t Priority() const { return _priority; }
		inline void SetPriority(uint8_t v) { _priority = v; }

		inline std::string const& Name() { return _name; }
	private:
		Address _memRangeFrom;
		Address _memRangeTo;
		AddressAction::shared_ptr _action;
		uint8_t _priority = 1;
		std::string _name;
	};
}

