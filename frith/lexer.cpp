#include <algorithm>
#include <lexer.hpp>
#include <ail/array.hpp>
#include <ail/string.hpp>

lexeme::lexeme()
{
}

lexeme::lexeme(lexeme_type type):
	type(type)
{
}

explicit lexeme::lexeme(types::signed_integer signed_integer):
	type(lexeme_type_signed_integer),
	signed_integer(signed_integer)
{
}

explicit lexeme::lexeme(types::unsigned_integer unsigned_integer):
	type(lexeme_type_umsigned_integer),
	signed_integer(unsigned_integer)
{
}

explicit lexeme::lexeme(types::floating_point_value floating_point_value)
	type(lexeme_type_floating_point_value),
	signed_integer(floating_point_value)
{
}

explicit lexeme::lexeme(lexeme_type type, std::string const & string)
	type(type),
	string(new std::string(string))
{
}

operator_lexeme::operator_lexeme(lexeme_type lexeme, std::string const & string):
	lexeme(lexeme),
	string(string)
{
}

bool operator_lexeme::operator<(operator_lexeme const & other) const
{
	return string.length() > other.string.length();
}

namespace
{
	bool operators_table_initialised = false;

	operator_lexeme operators[] =
	{
		operator_lexeme(lexeme_type_addition, "+"),
		operator_lexeme(lexeme_type_subtraction, "-"),
		operator_lexeme(lexeme_type_multiplication, "*"),
		operator_lexeme(lexeme_type_division, "/"),
		operator_lexeme(lexeme_type_modulo, "%"),

		operator_lexeme(lexeme_type_less_than, "<"),
		operator_lexeme(lexeme_type_less_than_or_equal, "<="),
		operator_lexeme(lexeme_type_greater_than, ">"),
		operator_lexeme(lexeme_type_greater_than_or_equal, ">="),
		operator_lexeme(lexeme_type_unequal, "!="),
		operator_lexeme(lexeme_type_equal, "="),

		operator_lexeme(lexeme_type_logical_not, "!"),

		operator_lexeme(lexeme_type_logical_and, "&"),
		operator_lexeme(lexeme_type_logical_or, "|"),

		operator_lexeme(lexeme_type_shift_left, "<<"),
		operator_lexeme(lexeme_type_shift_right, ">>"),

		operator_lexeme(lexeme_type_binary_and, "&&"),
		operator_lexeme(lexeme_type_binary_or, "||"),
		operator_lexeme(lexeme_type_binary_xor, "^"),

		operator_lexeme(lexeme_type_binary_not, "~"),

		operator_lexeme(lexeme_type_bracket_left, "["),
		operator_lexeme(lexeme_type_bracket_right, "]"),

		operator_lexeme(lexeme_type_array_left, "{"),
		operator_lexeme(lexeme_type_array_right, "}"),
	};
}

void initialise_operators_table()
{
	if(operators_table_initialised)
		return;

	operators_table_initialised = true;

	std::sort(operators, operators + ail::countof(operators));
}

bool parse_operator(std::string const & input, std::size_t offset, lexeme & output)
{
	for(std::size_t i = 0, end = ail::countof(operators); i < end; i++)
	{
		operator_lexeme & current_lexeme = operators[i];
		std::size_t remaining_characters = end - i;
		std::size_t operator_length = current_lexeme.string.size();
		if(remaining_characters < operator_length)
			return false;

		if(input.substr(i, operator_length) == current_lexeme.string)
		{
			output = lexeme(current_lexeme.lexeme);
			return true;
		}
	}
	return false;
}

bool parse_lexemes(std::string const & input, std::vector<line_of_code> & lines, std::string & error)
{
	initialise_operators_table();

	for(std::size_t i = 0, end = input.size(); i < end; i++)
	{
		char byte = input[i];
	}
}
