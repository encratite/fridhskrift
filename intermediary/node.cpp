#include <frith/symbol.hpp>

namespace frith
{
	void lexeme_to_argument_node(lexeme & input, parse_tree_node & output)
	{
		using namespace lexeme_type;

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
					throw ail::exeption("Unknown lexeme type encountered while trying to produce a parse tree node for a variable");
			}

			output.type = parse_tree_node_type::variable;
			output.variable_pointer = new_variable;
		}
	}
}
