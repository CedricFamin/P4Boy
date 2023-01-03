#pragma once

#include <cassert>

#include "Utility.h"
#include "MainBus.h"
#include "AddressRange.h"

namespace P4Boy
{
	class Motherboard
	{
	public:
		typedef Ptr<Motherboard> ptr;
		typedef std::shared_ptr<Motherboard> shared_ptr;
	public:
		void Tick();

		void ConnectMainBus(AddressRange& addressRange);
		void ConnectAddressRange(MainBus& mainBus);
		MainBus& GetMainBus() { return _mainBus; }

		void TransferOAM(Address addr, uint8_t value);

		inline uint8_t Get_8b(Address addr) const { return _mainBus.Get_8b(addr); }
		inline void Set_8b(Address addr, uint8_t value) { _mainBus.Set_8b(addr, value); }

		inline uint16_t Get_16b(Address addr) const { return _mainBus.Get_16b(addr); }
		inline void Set_16b(Address addr, uint16_t value) { _mainBus.Set_16b(addr, value); }
	private:
		MainBus _mainBus;
	};
}

