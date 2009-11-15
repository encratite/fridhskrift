#include <fridh/lexer.hpp>

namespace fridh
{
	bool get_lexeme_group(lexeme_type::type input, lexeme_group::type & output)
	{
		using namespace lexeme_type;

		switch(input)
		{
			case name:

			case boolean:
			case signed_integer:
			case unsigned_integer:
			case floating_point_value:
			case string:

			case nil:
				output = lexeme_group::argument;
				return true;

			case addition:
			case subtraction:
			case multiplication:
			case division:
			case modulo:
			case exponentiation:

			case assignment:
			case addition_assignment:
			case subtraction_assignment:
			case multiplication_assignment:
			case division_assignment:
			case modulo_assignment:
			case exponentiation_assignment:

			case less_than:
			case less_than_or_equal:
			case greater_than:
			case greater_than_or_equal:
			case not_equal:
			case equal:

			case logical_and:
			case logical_or:

			case shift_left:
			case shift_right:

			case binary_and:
			case binary_or:
			case binary_xor:

				output = lexeme_group::binary_operator;
				return true;

			case logical_not:

			case binary_not:

				output = lexeme_group::unary_operator;
				return true;

			case increment:
			case decrement:

				output = lexeme_group::post_fix_operator;
				return true;

			default:
				return false;
		}
	}
}
