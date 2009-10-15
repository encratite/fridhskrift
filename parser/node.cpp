#include <map>
#include <fridh/symbol.hpp>
#include <fridh/lexer.hpp>

namespace fridh
{
	typedef std::map<lexeme_type::type, unary_operator_type::type> unary_lexeme_map_type;
	typedef std::map<lexeme_type::type, binary_operator_type::type> binary_lexeme_map_type;

	using namespace lexeme_type;

	namespace
	{
		bool maps_are_initialised = false;

		unary_lexeme_map_type unary_lexeme_map;
		binary_lexeme_map_type binary_lexeme_map;
	}

	void initialise_maps()
	{
		if(maps_are_initialised)
			return;

		unary_lexeme_map[logical_not] = unary_operator_type::logical_not;
		unary_lexeme_map[binary_not] = unary_operator_type::binary_not;

		unary_lexeme_map[increment] = unary_operator_type::increment;
		unary_lexeme_map[decrement] = unary_operator_type::decrement;

		binary_lexeme_map[addition] = binary_operator_type::addition;
		binary_lexeme_map[subtraction] = binary_operator_type::subtraction;
		binary_lexeme_map[multiplication] = binary_operator_type::multiplication;
		binary_lexeme_map[division] = binary_operator_type::division;
		binary_lexeme_map[modulo] = binary_operator_type::modulo;
		binary_lexeme_map[exponentiation] = binary_operator_type::exponentiation;

		binary_lexeme_map[less_than] = binary_operator_type::less_than;
		binary_lexeme_map[less_than_or_equal] = binary_operator_type::less_than_or_equal;
		binary_lexeme_map[greater_than] = binary_operator_type::greater_than;
		binary_lexeme_map[greater_than_or_equal] = binary_operator_type::greater_than_or_equal;
		binary_lexeme_map[not_equal] = binary_operator_type::not_equal;
		binary_lexeme_map[equal] = binary_operator_type::equal;

		binary_lexeme_map[logical_and] = binary_operator_type::logical_and;
		binary_lexeme_map[logical_or] = binary_operator_type::logical_or;

		binary_lexeme_map[shift_left] = binary_operator_type::shift_left;
		binary_lexeme_map[shift_right] = binary_operator_type::shift_right;

		binary_lexeme_map[binary_and] = binary_operator_type::binary_and;
		binary_lexeme_map[binary_or] = binary_operator_type::binary_or;
		binary_lexeme_map[binary_xor] = binary_operator_type::binary_xor;

		binary_lexeme_map[selection_operator] = binary_operator_type::selection;

		binary_lexeme_map[assignment] = binary_operator_type::assignment;
		binary_lexeme_map[addition_assignment] = binary_operator_type::addition_assignment;
		binary_lexeme_map[subtraction_assignment] = binary_operator_type::subtraction_assignment;
		binary_lexeme_map[multiplication_assignment] = binary_operator_type::multiplication_assignment;
		binary_lexeme_map[division_assignment] = binary_operator_type::division_assignment;
		binary_lexeme_map[modulo_assignment] = binary_operator_type::modulo_assignment;
		binary_lexeme_map[exponentiation_assignment] = binary_operator_type::exponentiation_assignment;

		maps_are_initialised = true;
	}

	void lexeme_to_argument_node(lexeme & input, parse_tree_node & output)
	{
		if(input.type == name)
		{
			output.type = parse_tree_node_type::symbol;
			parse_tree_symbol * & symbol_pointer = output.symbol_pointer;
			symbol_pointer = new parse_tree_symbol;
			symbol_pointer->name = *input.string;
		}
		else
		{
			variable * & new_variable = output.variable_pointer;
			new_variable = new variable;

			switch(input.type)
			{
				case nil:
					new_variable->nil();
					break;

				case boolean:
					new_variable->new_boolean(input.boolean);
					break;

				case signed_integer:
					new_variable->new_signed_integer(input.signed_integer);
					break;

				case unsigned_integer:
					new_variable->new_unsigned_integer(input.unsigned_integer);
					break;

				case floating_point_value:
					new_variable->new_floating_point_value(input.floating_point_value);
					break;

				case string:
					new_variable->new_string(*input.string);
					break;

				default:
					delete new_variable;
					throw ail::exception("Unknown lexeme type encountered while trying to produce a parse tree node for a variable");
			}

			output.type = parse_tree_node_type::variable;
			output.variable_pointer = new_variable;
		}
	}

	void lexeme_to_unary_operator_node(lexeme & input, parse_tree_node & output)
	{
		initialise_maps();

		unary_lexeme_map_type::iterator iterator = unary_lexeme_map.find(input.type);
		if(iterator == unary_lexeme_map.end())
			throw ail::exception("Unknown lexeme type encountered while trying to match it to a corresponding unary operator parse tree node type");

		output.type = parse_tree_node_type::unary_operator_node;
		parse_tree_unary_operator_node * & unary_operator_pointer = output.unary_operator_pointer;
		unary_operator_pointer = new parse_tree_unary_operator_node;
		unary_operator_pointer->type = iterator->second;
	}

	void lexeme_to_binary_operator_node(lexeme & input, parse_tree_node & output)
	{
		initialise_maps();

		binary_lexeme_map_type::iterator iterator = binary_lexeme_map.find(input.type);
		if(iterator == binary_lexeme_map.end())
			throw ail::exception("Unknown lexeme type encountered while trying to match it to a corresponding binary operator parse tree node type");

		output.type = parse_tree_node_type::binary_operator_node;
		parse_tree_binary_operator_node * & binary_operator_pointer = output.binary_operator_pointer;
		binary_operator_pointer = new parse_tree_binary_operator_node;
		binary_operator_pointer->type = iterator->second;
	}
}
