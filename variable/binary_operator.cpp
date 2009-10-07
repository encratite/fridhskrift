#include <fridh/symbol.hpp>

namespace fridh
{
#define BINARY_OPERATOR(name, operator) \
		if(is_integer_type() && argument.other.is_integer_type()) \
		{ \
			argument.output.new_unsigned_integer(unsigned_integer operator argument.other.unsigned_integer); \
			return true; \
		} \
		else \
		{ \
			argument.error_message = get_binary_argument_type_error(name, type, argument.other.type); \
			return false; \
		}

	bool variable::shift_left(binary_argument & argument) const
	{
		BINARY_OPERATOR("Shift left", <<)
	}

	bool variable::shift_right(binary_argument & argument) const
	{
		BINARY_OPERATOR("Shift right", >>)
	}

	bool variable::binary_and(binary_argument & argument) const
	{
		BINARY_OPERATOR("Binary and", &)
	}

	bool variable::binary_or(binary_argument & argument) const
	{
		BINARY_OPERATOR("Binary or", |)
	}

	bool variable::binary_xor(binary_argument & argument) const
	{
		BINARY_OPERATOR("Binary exclusive or", ^)
	}

	bool variable::binary_not(unary_argument & argument) const
	{
		if(is_integer_type())
		{
			argument.output.new_unsigned_integer(~unsigned_integer);
			return true;
		}
		else
		{
			argument.error_message = get_unary_argument_type_error("Binary not", type);
			return false;
		}
	}
}
