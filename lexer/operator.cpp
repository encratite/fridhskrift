#include <frith/lexer.hpp>
#include <algorithm>
#include <ail/string.hpp>
#include <ail/array.hpp>

namespace frith
{
	bool tables_initialised = false;

	operator_lexeme operators[] =
	{
		operator_lexeme(addition, "+"),
		operator_lexeme(subtraction, "-"),
		operator_lexeme(multiplication, "*"),
		operator_lexeme(division, "/"),
		operator_lexeme(modulo, "%"),
		operator_lexeme(exponentiation, "**"),

		operator_lexeme(increment, "++"),
		operator_lexeme(decrement, "--"),

		operator_lexeme(less_than, "<"),
		operator_lexeme(less_than_or_equal, "<="),
		operator_lexeme(greater_than, ">"),
		operator_lexeme(greater_than_or_equal, ">="),
		operator_lexeme(unequal, "!="),
		operator_lexeme(equal, "="),

		operator_lexeme(logical_not, "!"),

		operator_lexeme(logical_and, "&"),
		operator_lexeme(logical_or, "|"),

		operator_lexeme(shift_left, "<<"),
		operator_lexeme(shift_right, ">>"),

		operator_lexeme(binary_and, "&&"),
		operator_lexeme(binary_or, "||"),
		operator_lexeme(binary_xor, "^"),

		operator_lexeme(binary_not, "~"),

		operator_lexeme(bracket_start, "["),
		operator_lexeme(bracket_end, "]"),

		operator_lexeme(array_start, "{"),
		operator_lexeme(array_end, "}"),

		operator_lexeme(scope_start, "("),
		operator_lexeme(scope_end, ")"),

		operator_lexeme(iteration, "\\"),
		operator_lexeme(iterator, "#"),
		operator_lexeme(function_declaration, "@"),
		operator_lexeme(anonymous_function_declaration, "@@"),
		operator_lexeme(class_operator, "$"),
		operator_lexeme(dot, "."),
		operator_lexeme(scope_operator, ":"),
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
