#include <frith/symbol.hpp>

namespace frith
{
#define LOGICAL_OPERATOR(name, operator) \
		bool \
			left_value, \
			right_value; \
		if(get_boolean_value(left_value) && argument.other.get_boolean_value(right_value)) \
		{ \
			argument.output.new_boolean(left_value operator right_value); \
			return true; \
		} \
		else \
		{ \
			argument.error_message = get_unary_argument_type_error(name, type); \
			return false; \
		}

	bool variable::logical_and(binary_argument & argument) const
	{
		LOGICAL_OPERATOR("Logical and", &&)
	}

	bool variable::logical_or(binary_argument & argument) const
	{
		LOGICAL_OPERATOR("Logical or", ||)
	}
}
