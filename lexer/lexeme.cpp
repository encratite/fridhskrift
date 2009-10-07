#include <fridh/lexer.hpp>
#include <ail/string.hpp>

namespace fridh
{	
	lexeme_type::lexeme()
	{
	}

	lexeme_type::lexeme(lexeme_type::type type):
		type(type)
	{
	}

	lexeme_type::lexeme(types::boolean boolean):
		type(boolean),
		boolean(boolean)
	{
	}

	lexeme_type::lexeme(types::signed_integer signed_integer):
		type(signed_integer),
		signed_integer(signed_integer)
	{
	}

	lexeme_type::lexeme(types::unsigned_integer unsigned_integer):
		type(unsigned_integer),
		unsigned_integer(unsigned_integer)
	{
	}

	lexeme_type::lexeme(types::floating_point_value floating_point_value):
		type(floating_point_value),
		floating_point_value(floating_point_value)
	{
	}

	lexeme_type::lexeme(lexeme_type::type type, std::string const & string):
		type(type),
		string(new std::string(string))
	{
	}

	std::string lexeme_type::to_string() const
	{
		using namespace lexeme_type;

		switch(type)
		{
			case name:
				return "name: " + *string;

			case boolean:
				return "boolean: " + ail::bool_to_string(boolean);

			case signed_integer:
				return "integer: " + ail::number_to_string(signed_integer);

			case unsigned_integer:
				return "unsigned-integer: " + ail::number_to_string(unsigned_integer);

			case floating_point_value:
				return "float: " + ail::number_to_string(floating_point_value);

			case string:
				return "string: " + ail::replace_string(*string, "\n", "\\n");

			case addition:
				return "+";

			case subtraction:
				return "-";

			case multiplication:
				return "*";

			case division:
				return "/";

			case modulo:
				return "%";

			case assignment:
				return "=";

			case addition_assignment:
				return "+=";

			case subtraction_assignment:
				return "-=";

			case multiplication_assignment:
				return "*=";

			case division_assignment:
				return "/=";

			case modulo_assignment:
				return "%=";

			case increment:
				return "++";

			case decrement:
				return "--";

			case exponentiation:
				return "**";

			case less_than:
				return "<";

			case less_than_or_equal:
				return "<=";

			case greater_than:
				return ">";

			case greater_than_or_equal:
				return ">=";

			case unequal:
				return "!=";

			case equal:
				return "==";

			case logical_not:
				return "!";

			case logical_and:
				return "&";

			case logical_or:
				return "|";

			case shift_left:
				return "<<";

			case shift_right:
				return ">>";

			case binary_and:
				return "&&";

			case binary_or:
				return "||";

			case binary_xor:
				return "^";

			case binary_not:
				return "~";

			case bracket_start:
				return "bracket: start";

			case bracket_end:
				return "bracket: end";

			case array_start:
				return "array: start";

			case array_end:
				return "array: end";

			case scope_start:
				return "symbol_tree_node: start";

			case scope_end:
				return "symbol_tree_node: end";

			case iteration:
				return "iteration";

			case iterator:
				return "iterator";

			case function_declaration:
				return "function";

			case anonymous_function_declaration:
				return "anonymous function";

			case class_operator:
				return "class operator";

			case selection_operator:
				return ".";

			case call_operator:
				return ",";

			case scope_operator:
				return ":";

			default:
				return "unknown";
		}
	}
}
