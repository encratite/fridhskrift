#include <fridh/symbol.hpp>

namespace fridh
{
#define BINARY_OPERATOR(name, description, operator) \
	void variable::name(variable const & argument, variable & output) const \
	{ \
		if(is_integer_type() && argument.is_integer_type()) \
			output.new_unsigned_integer(unsigned_integer operator argument.unsigned_integer); \
		else \
			binary_argument_type_error(description, type, argument.type); \
	}

	BINARY_OPERATOR(shift_left, "Shift left", <<)
	BINARY_OPERATOR(shift_right, "Shift right", >>)
	BINARY_OPERATOR(binary_and, "Binary and", &)
	BINARY_OPERATOR(binary_or, "Binary or", |)
	BINARY_OPERATOR(binary_xor, "Binary exclusive or", ^)

	void variable::binary_not(variable & output) const
	{
		if(is_integer_type())
			output.new_unsigned_integer(~unsigned_integer);
		else
			unary_argument_type_error("Binary not", type);
	}
}
