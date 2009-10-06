#include <map>
#include <frith/intermediary.hpp>
#include <frith/symbol.hpp>

namespace frith
{
	typedef std::map<unary_operator_type::type, operator_precedence::type> unary_operator_precedence_map_type;
	typedef std::map<binary_operator_type::type, operator_precedence::type> binary_operator_precedence_map_type;

	namespace
	{
		unary_operator_precedence_map_type unary_operator_precedence_map;
		binary_operator_precedence_map_type binary_operator_precedence_map;

		bool initialised_maps = false;
	}

	void initialise_maps()
	{
		if(initialised_maps)
			return;

		unary_operator_precedence_map[unary_operator_type::negation] = operator_precedence::negation;
		unary_operator_precedence_map[unary_operator_type::logical_not] = operator_precedence::logical_not;
		unary_operator_precedence_map[unary_operator_type::binary_not] = operator_precedence::binary_not;

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

		initialised_maps = true;
	}

	word get_unary_operator_precedence(unary_operator_type::type input)
	{
		initialise_maps();

		unary_operator_precedence_map_type::iterator iterator = unary_operator_precedence_map.find(input);
		if(iterator == unary_operator_precedence_map.end())
			throw ail::exception("Invalid unary operator value specified for operator precedence lookup");
		return static_cast<word>(iterator->second);
	}

	word get_binary_operator_precedence(binary_operator_type::type input)
	{
		initialise_maps();

		binary_operator_precedence_map_type::iterator iterator = binary_operator_precedence_map.find(input);
		if(iterator == binary_operator_precedence_map.end())
			throw ail::exception("Invalid binary operator value specified for operator precedence lookup");
		return static_cast<word>(iterator->second);
	}

	bool get_parse_tree_node_precedence(parse_tree_node & input, word & output)
	{
		switch(input.type)
		{
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
}
