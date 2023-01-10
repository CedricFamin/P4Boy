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

		inline uint16_t Get_16b(Address addr) const { return (GetRange(addr+1)->Get_8b(addr + 1) << 8) | GetRange(addr)->Get_8b(addr); }
		inline void Set_16b(Address addr, uint16_t value) { GetRange(addr+1)->Set_8b(addr + 1, value >> 8); GetRange(addr)->Set_8b(addr, value & 0xFF); }

		void AddRange(AddressRange* range, uint8_t priority = 1);
		void AddRange(Address from, Address to, AddressAction* action, std::string const & name, uint8_t priority = 1);
		void AddSingle(Address addr, AddressAction* action, std::string const & name, uint8_t priority = 1);

		std::vector<std::shared_ptr<AddressRange>>& Ranges() { return _allRanges; }
	protected:
	private:
		std::vector<std::shared_ptr<AddressRange>> _allRanges;
	};
}
