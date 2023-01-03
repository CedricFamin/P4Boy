#include "MainBus.h"

namespace P4Boy
{
	AddressRange* MainBus::GetRange(Address addr) { return const_cast<AddressRange*>(const_cast<const MainBus*>(this)->GetRange(addr)); }
	AddressRange const* MainBus::GetRange(Address addr) const
	{
		for (auto const & range : _allRanges)
		{
			if (range->IsIn(addr))
				return range.get();
		}
		return nullptr;
	}

	void MainBus::AddRange(AddressRange* range, uint8_t priority)
	{
		range->SetPriority(priority);
		_allRanges.push_back(std::shared_ptr<AddressRange>(range));
		std::sort(_allRanges.begin(), _allRanges.end(),
			[](const std::shared_ptr<AddressRange>& r1, const std::shared_ptr<AddressRange>& r2) -> bool
			{
				if (r1->Priority() != r2->Priority())
					return r1->Priority() < r2->Priority();
				return r1->From() < r2->From();
			});

	}

	void MainBus::AddRange(Address from, Address to, AddressAction* action, std::string const & name, uint8_t priority)
	{
		AddRange(new AddressRange(from, to, action, name), priority);
	}

	void MainBus::AddSingle(Address addr, AddressAction* action, std::string const & name, uint8_t priority)
	{
		AddRange(addr, addr, action, name, priority);
	}
}