#include <map>
#include <fridh/symbol.hpp>

namespace fridh
{

	namespace
	{
		typedef std::map<lexeme_type::type, assignment_type::type> assignment_lexeme_map_type;

		assignment_lexeme_map_type assignment_lexeme_map;
		std::set<lexeme_type> assignment_lexemes;
	}

	parse_tree_node::parse_tree_node():
		type(parse_tree_node_type::uninitialised)
	{
	}

	parse_tree_node::parse_tree_node(parse_tree_node_type::type type):
		type(type)
	{
	}

	parse_tree_node::parse_tree_node(variable * variable_pointer):
		type(parse_tree_node_type::variable),
		variable_pointer(variable_pointer)
	{
	}

	parse_tree_node::parse_tree_node(unary_operator_type::type unary_operator):
		type(parse_tree_node::unary_operator)
	{
		unary_operator_pointer = new parse_tree_unary_operator_node;
		unary_operator_pointer->type = unary_operator;
	}

	parse_tree_node::parse_tree_node(binary_operator_type::type binary_operator)
		type(parse_tree_node::binary_operator)
	{
		binary_operator_pointer = new parse_tree_binary_operator_node;
		binary_operator_pointer->type = binary_operator;
	}

	parse_tree_node::parse_tree_node(parse_tree_nodes & elements):
		type(parse_tree_node::array)
	{
		array_pointer = new parse_tree_array;
		array_pointer->elements = elements;
	}

	void parse_tree_node::is_call()
	{
		type = parse_tree_node_type::call;
		call_pointer = new parse_tree_call;
	}

	parse_tree_symbol::parse_tree_symbol():
	type(symbol_prefix::none)
	{
	}

	void initialise_map()
	{
		if(!assignment_lexeme_map.empty())
			return;

		using namespace lexeme_type::type;

		assignment_lexeme_map[assignment] = assignment_type::assignment;
		assignment_lexeme_map[addition_assignment] = assignment_type::addition;
		assignment_lexeme_map[subtraction_assignment] = assignment_type::subtraction;
		assignment_lexeme_map[multiplication_assignment] = assignment_type::multiplication;
		assignment_lexeme_map[division_assignment] = assignment_type::division;
		assignment_lexeme_map[modulo_assignment] = assignment_type::modulo;
		assignment_lexeme_map[exponentiation_assignment] = assignment_type::exponentiation;

		for(assignment_lexeme_map_type::iterator i = assignment_lexeme_map.begin(), end = assignment_lexeme_map.end(); i != end; i++)
			assignment_lexemes.insert(i->first);

		assignment_lexemes.insert(lexeme_type::decrement);
		assignment_lexemes.insert(lexeme_type::increment);
	}

	bool convert_lexeme_to_assignment_type(lexeme_type::type input, assignment_type::type & output)
	{
		initialise_map();

		assignment_lexeme_map_type::iterator iterator = assignment_lexeme_map.find(input);
		if(iterator == assignment_lexeme_map.end())
			return false;

		output = iterator->second;
		return true;
	}

	bool convert_lexeme_to_crement_type(lexeme_type::type input, crement_type::type & output)
	{
		switch(input)
		{
			case lexeme_type::decrement:
				output = crement_type::decrement;
				break;

			case lexeme_type::increment:
				output = crement_type::increment;
				break;

			default:
				return false;
		}

		return true;
	}

	bool is_assignment_lexeme(lexeme_type::type input)
	{
		return assignment_lexemes.find(input) != assignment_lexemes.end();
	}
}
