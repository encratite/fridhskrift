#pragma once

#include <ail/exception.hpp>

namespace fridh
{
	struct construction_pattern
	{
		virtual void copy(construction_pattern const & other);
		virtual void destroy();

		construction_pattern();
		construction_pattern(construction_pattern const & other);
		~construction_pattern();

		construction_pattern & operator=(construction_pattern const & other);
	};
}
