#pragma once

#include <memory>

namespace P4Boy
{
	template<typename _class>
	class Ptr
	{
	public:
		typedef std::unique_ptr<_class> unique;
		typedef std::shared_ptr<_class> shared;
		typedef std::weak_ptr<_class>   weak;
	};


	typedef uint16_t Address;
}