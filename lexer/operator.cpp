#include <fridh/lexer.hpp>
#include <algorithm>
#include <ail/string.hpp>
#include <ail/array.hpp>

namespace fridh
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

		operator_lexeme(assignment, "="),
		operator_lexeme(addition_assignment, "+="),
		operator_lexeme(subtraction_assignment, "-="),
		operator_lexeme(multiplication_assignment, "*="),
		operator_lexeme(division_assignment, "/="),
		operator_lexeme(modulo_assignment, "%="),
		operator_lexeme(exponentiation_assignment, "**="),

		operator_lexeme(increment, "++"),
		operator_lexeme(decrement, "--"),

		operator_lexeme(less_than, "<"),
		operator_lexeme(less_than_or_equal, "<="),
		operator_lexeme(greater_than, ">"),
		operator_lexeme(greater_than_or_equal, ">="),
		operator_lexeme(unequal, "!="),
		operator_lexeme(equal, "=="),

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

		operator_lexeme(while_operator, "\\\\"),

		operator_lexeme(function_declaration, "@"),
		operator_lexeme(anonymous_function_declaration, "@@"),
		operator_lexeme(class_operator, "$"),
		operator_lexeme(selection_operator, "."),
		operator_lexeme(scope_operator, ":"),

		operator_lexeme(call_operator, ","),
		operator_lexeme(spaced_call_operator, ", "),
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
