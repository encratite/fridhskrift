#include <frith/lexer.hpp>
#include <ail/string.hpp>

namespace frith
{	
	lexeme::lexeme()
	{
	}

	lexeme::lexeme(lexeme_type type):
		type(type)
	{
	}

	lexeme::lexeme(types::boolean boolean):
		type(lexeme_type_boolean),
		boolean(boolean)
	{
	}

	lexeme::lexeme(types::signed_integer signed_integer):
		type(lexeme_type_signed_integer),
		signed_integer(signed_integer)
	{
	}

	lexeme::lexeme(types::unsigned_integer unsigned_integer):
		type(lexeme_type_unsigned_integer),
		unsigned_integer(unsigned_integer)
	{
	}

	lexeme::lexeme(types::floating_point_value floating_point_value):
		type(lexeme_type_floating_point_value),
		floating_point_value(floating_point_value)
	{
	}

	lexeme::lexeme(lexeme_type type, std::string const & string):
		type(type),
		string(new std::string(string))
	{
	}

	std::string lexeme::to_string() const
	{
		switch(type)
		{
			case lexeme_type_name:
				return "name: " + *string;

			case lexeme_type_boolean:
				return "boolean: " + ail::bool_to_string(boolean);

			case lexeme_type_signed_integer:
				return "integer: " + ail::number_to_string(signed_integer);

			case lexeme_type_unsigned_integer:
				return "unsigned-integer: " + ail::number_to_string(unsigned_integer);

			case lexeme_type_floating_point_value:
				return "float: " + ail::number_to_string(floating_point_value);

			case lexeme_type_string:
				return "string: " + ail::replace_string(*string, "\n", "\\n");

			case lexeme_type_addition:
				return "+";

			case lexeme_type_subtraction:
				return "-";

			case lexeme_type_multiplication:
				return "*";

			case lexeme_type_division:
				return "/";

			case lexeme_type_modulo:
				return "%";

			case lexeme_type_increment:
				return "++";

			case lexeme_type_decrement:
				return "--";

			case lexeme_type_exponentiation:
				return "**";

			case lexeme_type_less_than:
				return "<";

			case lexeme_type_less_than_or_equal:
				return "<=";

			case lexeme_type_greater_than:
				return ">";

			case lexeme_type_greater_than_or_equal:
				return ">=";

			case lexeme_type_unequal:
				return "!=";

			case lexeme_type_equal:
				return "=";

			case lexeme_type_logical_not:
				return "!";

			case lexeme_type_logical_and:
				return "&";

			case lexeme_type_logical_or:
				return "|";

			case lexeme_type_shift_left:
				return "<<";

			case lexeme_type_shift_right:
				return ">>";

			case lexeme_type_binary_and:
				return "&&";

			case lexeme_type_binary_or:
				return "||";

			case lexeme_type_binary_xor:
				return "^";

			case lexeme_type_binary_not:
				return "~";

			case lexeme_type_bracket_start:
				return "bracket: start";

			case lexeme_type_bracket_end:
				return "bracket: end";

			case lexeme_type_array_start:
				return "array: start";

			case lexeme_type_array_end:
				return "array: end";

			case lexeme_type_scope_start:
				return "symbol_tree_node: start";

			case lexeme_type_scope_end:
				return "symbol_tree_node: end";

			case lexeme_type_iteration:
				return "iteration";

			case lexeme_type_iterator:
				return "iterator";

			case lexeme_type_function_declaration:
				return "function";

			case lexeme_type_anonymous_function_declaration:
				return "anonymous function";

			case lexeme_type_class_operator:
				return "class operator";

			case lexeme_type_dot:
				return ".";

			case lexeme_type_scope_operator:
				return ":";

			default:
				return "unknown";
		}
	}
}
