#include <fridh/symbol.hpp>

namespace fridh
{
#define LOGICAL_OPERATOR(name, description, operator) \
	void variable::name(variable const & argument, variable & output) \
	{ \
		bool \
			left_value, \
			right_value; \
		if(get_boolean_value(left_value) && argument.get_boolean_value(right_value)) \
			output.new_boolean(left_value operator right_value); \
		else \
			unary_argument_type_error(description, type); \
	}

	LOGICAL_OPERATOR(logical_and, "Logical and", &&)
	LOGICAL_OPERATOR(logical_or, "Logical or", ||)
}
