#include <map>
#include <fridh/intermediary.hpp>
#include <fridh/symbol.hpp>

namespace fridh
{
	typedef std::map<unary_operator_type::type, operator_precedence::type> unary_operator_precedence_map_type;
	typedef std::map<binary_operator_type::type, operator_precedence::type> binary_operator_precedence_map_type;

	namespace
	{
		unary_operator_precedence_map_type unary_operator_precedence_map;
		binary_operator_precedence_map_type binary_operator_precedence_map;
		std::set<binary_operator_type::type> binary_right_to_left_operators;

		bool initialised_data = false;
	}

	void initialise_data()
	{
		if(initialised_data)
			return;

		unary_operator_precedence_map[unary_operator_type::negation] = operator_precedence::negation;
		unary_operator_precedence_map[unary_operator_type::logical_not] = operator_precedence::logical_not;
		unary_operator_precedence_map[unary_operator_type::binary_not] = operator_precedence::binary_not;

		unary_operator_precedence_map[unary_operator_type::increment] = operator_precedence::increment;
		unary_operator_precedence_map[unary_operator_type::decrement] = operator_precedence::decrement;

		binary_operator_precedence_map[binary_operator_type::addition] = operator_precedence::addition;
		binary_operator_precedence_map[binary_operator_type::subtraction] = operator_precedence::subtraction;
		binary_operator_precedence_map[binary_operator_type::multiplication] = operator_precedence::multiplication;
		binary_operator_precedence_map[binary_operator_type::division] = operator_precedence::division;
		binary_operator_precedence_map[binary_operator_type::modulo] = operator_precedence::modulo;
		binary_operator_precedence_map[binary_operator_type::exponentiation] = operator_precedence::exponentiation;

		binary_operator_precedence_map[binary_operator_type::less_than] = operator_precedence::less_than;
		binary_operator_precedence_map[binary_operator_type::less_than_or_equal] = operator_precedence::less_than_or_equal;
		binary_operator_precedence_map[binary_operator_type::greater_than] = operator_precedence::greater_than;
		binary_operator_precedence_map[binary_operator_type::greater_than_or_equal] = operator_precedence::greater_than_or_equal;
		binary_operator_precedence_map[binary_operator_type::unequal] = operator_precedence::unequal;
		binary_operator_precedence_map[binary_operator_type::equal] = operator_precedence::equal;

		binary_operator_precedence_map[binary_operator_type::logical_and] = operator_precedence::logical_and;
		binary_operator_precedence_map[binary_operator_type::logical_or] = operator_precedence::logical_or;

		binary_operator_precedence_map[binary_operator_type::shift_left] = operator_precedence::shift_left;
		binary_operator_precedence_map[binary_operator_type::shift_right] = operator_precedence::shift_right;

		binary_operator_precedence_map[binary_operator_type::binary_and] = operator_precedence::binary_and;
		binary_operator_precedence_map[binary_operator_type::binary_or] = operator_precedence::binary_or;
		binary_operator_precedence_map[binary_operator_type::binary_xor] = operator_precedence::binary_xor;

		binary_operator_precedence_map[binary_operator_type::selection] = operator_precedence::selection;

		{
			using namespace binary_operator_type;

			binary_right_to_left_operators.insert(assignment);
			binary_right_to_left_operators.insert(addition_assignment);
			binary_right_to_left_operators.insert(subtraction_assignment);
			binary_right_to_left_operators.insert(multiplication_assignment);
			binary_right_to_left_operators.insert(division_assignment);
			binary_right_to_left_operators.insert(modulo_assignment);
			binary_right_to_left_operators.insert(exponentiation_assignment);
		}

		initialised_data = true;
	}

	word get_unary_operator_precedence(unary_operator_type::type input)
	{
		initialise_data();

		unary_operator_precedence_map_type::iterator iterator = unary_operator_precedence_map.find(input);
		if(iterator == unary_operator_precedence_map.end())
			throw ail::exception("Invalid unary operator value specified for operator precedence lookup");
		return static_cast<word>(iterator->second);
	}

	word get_binary_operator_precedence(binary_operator_type::type input)
	{
		initialise_data();

		binary_operator_precedence_map_type::iterator iterator = binary_operator_precedence_map.find(input);
		if(iterator == binary_operator_precedence_map.end())
			throw ail::exception("Invalid binary operator value specified for operator precedence lookup");
		return static_cast<word>(iterator->second);
	}

	bool get_parse_tree_node_precedence(parse_tree_node & input, word & output)
	{
		switch(input.type)
		{
			case parse_tree_node_type::call:
			case parse_tree_node_type::call_operator:
				output = static_cast<word>(operator_precedence::call);
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
			input.type == parse_tree_node_type::binary_operator_node) &&
			binary_right_to_left_operators.find(input.binary_operator_pointer->type) != binary_right_to_left_operators.end()
		)
				return true;

		return false;
	}
}
