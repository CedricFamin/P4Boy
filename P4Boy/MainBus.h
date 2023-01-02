#pragma once

#include <vector>
#include <algorithm>

#include "Utility.h"
#include "AddressRange.h"

namespace P4Boy
{
	

	class MainBus
	{
	public:

		AddressRange* GetRange(Address addr);
		AddressRange const* GetRange(Address addr) const;

		inline uint8_t Get_8b(Address addr) const { return GetRange(addr)->Get_8b(addr); }
		inline void Set_8b(Address addr, uint8_t value) { GetRange(addr)->Set_8b(addr, value); }

		inline uint16_t Get_16b(Address addr) const { return GetRange(addr)->Get_16b(addr); }
		inline void Set_16b(Address addr, uint16_t value) { GetRange(addr)->Set_16b(addr, value); }

		void AddRange(AddressRange* range, uint8_t priority = 1);
		void AddRange(Address from, Address to, AddressAction* action, uint8_t priority = 1);
		void AddSingle(Address addr, AddressAction* action, uint8_t priority = 1);
	protected:
	private:
		std::vector<std::shared_ptr<AddressRange>> _allRanges;
	};
}
