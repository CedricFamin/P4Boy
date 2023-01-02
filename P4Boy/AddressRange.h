#pragma once
#include "Utility.h"
#include "AddressAction.h"

namespace P4Boy
{
	class AddressRange
	{
	public:
		AddressRange(Address from, Address to, AddressAction* action)
			: _memRangeFrom(from)
			, _memRangeTo(to)
			, _action(action)

		{

		}

		~AddressRange() {}

		inline Address From() const { return _memRangeFrom; }
		inline Address To() const { return _memRangeTo; }

		inline bool IsIn(Address addr) const { return _memRangeFrom <= addr && addr <= _memRangeTo; }

		inline uint8_t Get_8b(Address addr) const { return _action->Get(addr); }
		inline void Set_8b(Address addr, uint8_t value) { _action->Set(addr, value); }

		inline uint16_t Get_16b(Address addr) const { return (_action->Get(addr + 1) << 8) | _action->Get(addr); }
		inline void Set_16b(Address addr, uint16_t value) { _action->Set(addr + 1, value >> 8); _action->Set(addr, value & 0xFF); }

		inline uint8_t Priority() const { return _priority; }
		inline void SetPriority(uint8_t v) { _priority = v; }
	private:
		Address _memRangeFrom;
		Address _memRangeTo;
		AddressAction::ptr::shared _action;
		uint8_t _priority = 1;
	};

	inline AddressRange* AddressSingle(Address addr, AddressAction* action) {
		return new AddressRange(addr, addr, action);
	}
}

