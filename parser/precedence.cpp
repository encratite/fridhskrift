#include <map>
#include <fridh/parser.hpp>
#include <fridh/symbol.hpp>

namespace fridh
{
	namespace
	{
		std::set<binary_operator_type::type> binary_right_to_left_operators;

		bool initialised_data = false;
	}

	void initialise_data()
	{
		if(initialised_data)
			return;

		{
			using namespace binary_operator_type;
			for(int i = static_cast<int>(assignment); i <= static_cast<int>(exponentiation_assignment); i++)
				binary_right_to_left_operators.insert(static_cast<binary_operator_type::type>(i));
		}

		initialised_data = true;
	}

	word get_unary_operator_precedence(unary_operator_type::type input)
	{
		using namespace unary_operator_type;

		switch(input)
		{
			case negation:
			case logical_not:
			case binary_not:
				return 3;

			case increment:
			case decrement:
				return 2;
		}

		throw ail::exception("Invalid unary operator type");
	}

	word get_binary_operator_precedence(binary_operator_type::type input)
	{
		using namespace binary_operator_type;

		switch(input)
		{
			case addition:
			case subtraction:
				return 6;

			case multiplication:
			case division:
			case modulo:
				return 5;

			case exponentiation:
				//improvised, not from the C++ operators article as such
				return 4;

			case less_than:
			case less_than_or_equal:
			case greater_than:
			case greater_than_or_equal:
				return 8;

			case not_equal:
			case equal:
				return 9;

			case logical_and:
				return 13;

			case logical_or:
				return 14;

			case shift_left:
			case shift_right:
				return 7;

			case binary_and:
				return 10;

			case binary_or:
				return 12;

			case binary_xor:
				return 11;

			case selection:
				return 2;

			case assignment:
			case addition_assignment:
			case subtraction_assignment:
			case multiplication_assignment:
			case division_assignment:
			case modulo_assignment:
			case exponentiation_assignment:
				return 16;
		}

		throw ail::exception("Invalid binary operator type");
	}

	bool get_parse_tree_node_precedence(parse_tree_node & input, word & output)
	{
		switch(input.type)
		{
			case parse_tree_node_type::call:
			case parse_tree_node_type::call_operator:
				output = 2;
				break;

			case parse_tree_node_type::unary_operator_node:
				output = get_unary_operator_precedence(input.unary_operator_pointer->type);
				break;

			case parse_tree_node_type::binary_operator_node:
				output = get_binary_operator_precedence(input.binary_operator_pointer->type);
				break;

			default:
				return false;
		}

		return true;
	}

	bool is_right_to_left_operator(parse_tree_node & input)
	{
		initialise_data();

		if(input.type == parse_tree_node_type::unary_operator_node)
			return true;

		if
		(
			(
				input.type == parse_tree_node_type::binary_operator_node &&
				binary_right_to_left_operators.find(input.binary_operator_pointer->type) != binary_right_to_left_operators.end()
			)
			||
			input.is_call_node()
		)
				return true;

		return false;
	}
}
