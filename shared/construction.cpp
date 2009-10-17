#include <fridh/construction.hpp>

namespace fridh
{
	void construction_pattern::copy(construction_pattern const & other)
	{
		throw ail::exception("Construction pattern copy requested");
	}

	void construction_pattern::destroy()
	{
		throw ail::exception("Construction pattern destruction requested");
	}

	construction_pattern::construction_pattern()
	{
	}

	construction_pattern::construction_pattern(construction_pattern const & other)
	{
		copy(other);
	}

	construction_pattern::~construction_pattern()
	{
		destroy();
	}

	construction_pattern & construction_pattern::operator=(construction_pattern const & other)
	{
		destroy();
		copy(other);
		return *this;
	}
}
