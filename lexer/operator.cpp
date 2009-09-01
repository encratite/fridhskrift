#include <frith/lexer.hpp>
#include <algorithm>
#include <ail/string.hpp>
#include <ail/array.hpp>

namespace frith
{
	bool tables_initialised = false;

	operator_lexeme operators[] =
	{
		operator_lexeme(lexeme_type_addition, "+"),
		operator_lexeme(lexeme_type_subtraction, "-"),
		operator_lexeme(lexeme_type_multiplication, "*"),
		operator_lexeme(lexeme_type_division, "/"),
		operator_lexeme(lexeme_type_modulo, "%"),
		operator_lexeme(lexeme_type_exponentiation, "**"),

		operator_lexeme(lexeme_type_increment, "++"),
		operator_lexeme(lexeme_type_decrement, "--"),

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

		operator_lexeme(lexeme_type_bracket_start, "["),
		operator_lexeme(lexeme_type_bracket_end, "]"),

		operator_lexeme(lexeme_type_array_start, "{"),
		operator_lexeme(lexeme_type_array_end, "}"),

		operator_lexeme(lexeme_type_scope_start, "("),
		operator_lexeme(lexeme_type_scope_end, ")"),

		operator_lexeme(lexeme_type_iteration, "\\"),
		operator_lexeme(lexeme_type_iterator, "#"),
		operator_lexeme(lexeme_type_function_declaration, "@"),
		operator_lexeme(lexeme_type_anonymous_function_declaration, "@@"),
		operator_lexeme(lexeme_type_class_operator, "$"),
		operator_lexeme(lexeme_type_dot, "."),
	};

	std::vector<operator_lexeme> operator_lexeme_data;

	void initialise_tables()
	{
		boost::mutex::scoped_lock scoped_lock(table_mutex);

		if(tables_initialised)
			return;

		tables_initialised = true;

		std::sort(operators, operators + ail::countof(operators));

		for(std::size_t i = 0; i < ail::countof(operators); i++)
			operator_lexeme_data.push_back(operators[i]);
	}
}
