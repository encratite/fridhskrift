#include <fridh/lexer.hpp>
#include <algorithm>
#include <ail/string.hpp>
#include <ail/array.hpp>

namespace fridh
{
	bool tables_initialised = false;

	operator_lexeme operators[] =
	{
		operator_lexeme(lexeme_type::addition, "+"),
		operator_lexeme(lexeme_type::subtraction, "-"),
		operator_lexeme(lexeme_type::multiplication, "*"),
		operator_lexeme(lexeme_type::division, "/"),
		operator_lexeme(lexeme_type::modulo, "%"),
		operator_lexeme(lexeme_type::exponentiation, "**"),

		operator_lexeme(lexeme_type::assignment, "="),
		operator_lexeme(lexeme_type::addition_assignment, "+="),
		operator_lexeme(lexeme_type::subtraction_assignment, "-="),
		operator_lexeme(lexeme_type::multiplication_assignment, "*="),
		operator_lexeme(lexeme_type::division_assignment, "/="),
		operator_lexeme(lexeme_type::modulo_assignment, "%="),
		operator_lexeme(lexeme_type::exponentiation_assignment, "**="),

		operator_lexeme(lexeme_type::increment, "++"),
		operator_lexeme(lexeme_type::decrement, "--"),

		operator_lexeme(lexeme_type::less_than, "<"),
		operator_lexeme(lexeme_type::less_than_or_equal, "<="),
		operator_lexeme(lexeme_type::greater_than, ">"),
		operator_lexeme(lexeme_type::greater_than_or_equal, ">="),
		operator_lexeme(lexeme_type::not_equal, "!="),
		operator_lexeme(lexeme_type::equal, "=="),

		operator_lexeme(lexeme_type::logical_not, "!"),

		operator_lexeme(lexeme_type::logical_and, "&"),
		operator_lexeme(lexeme_type::logical_or, "|"),

		operator_lexeme(lexeme_type::shift_left, "<<"),
		operator_lexeme(lexeme_type::shift_right, ">>"),

		operator_lexeme(lexeme_type::binary_and, "&&"),
		operator_lexeme(lexeme_type::binary_or, "||"),
		operator_lexeme(lexeme_type::binary_xor, "^"),

		operator_lexeme(lexeme_type::binary_not, "~"),

		operator_lexeme(lexeme_type::bracket_start, "["),
		operator_lexeme(lexeme_type::bracket_end, "]"),

		operator_lexeme(lexeme_type::array_start, "{"),
		operator_lexeme(lexeme_type::array_end, "}"),

		operator_lexeme(lexeme_type::scope_start, "("),
		operator_lexeme(lexeme_type::scope_end, ")"),

		operator_lexeme(lexeme_type::iteration, "\\"),
		operator_lexeme(lexeme_type::iterator, "#"),

		operator_lexeme(lexeme_type::while_operator, "\\\\"),

		operator_lexeme(lexeme_type::function_declaration, "@"),
		operator_lexeme(lexeme_type::anonymous_function_declaration, "@@"),
		operator_lexeme(lexeme_type::class_operator, "$"),
		operator_lexeme(lexeme_type::dot, "."),
		operator_lexeme(lexeme_type::scope_operator, ":"),

		operator_lexeme(lexeme_type::call_operator, ","),
		operator_lexeme(lexeme_type::spaced_call_operator, ", "),
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
