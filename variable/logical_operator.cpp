#include <fridh/symbol.hpp>

namespace fridh
{
#define LOGICAL_OPERATOR(name, description, operator) \
	void variable::name(variable const & argument, variable & output) const \
	{ \
		output.new_boolean(get_boolean_value() operator argument.get_boolean_value()); \
	}

	LOGICAL_OPERATOR(logical_and, "Logical and", &&)
	LOGICAL_OPERATOR(logical_or, "Logical or", ||)
}
